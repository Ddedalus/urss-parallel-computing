package hubert.akka

import akka.actor.{
  Actor,
  ActorLogging,
  ActorRef,
  ActorSystem,
  Props,
  PoisonPill,
  Timers
}
import akka.util.Timeout
import akka.pattern.ask

import scala.concurrent.duration._
import scala.concurrent.{ExecutionContext, Future}
import ExecutionContext.Implicits.global
import scala.util.{Success, Failure}

object Master extends CommonInterfaces {
  def props(filename: String): Props = Props(new Master(filename))
  final case class GatherResults(source: Source)
  case object TimedOut
  case object RequestBulk
}

class Master(filename: String) extends GraphBuilder(filename) with Timers {
  import Master._
  import Messages._
  // TODO remove for production!!!
  timers.startSingleTimer(TimedOut, TimedOut, 10.seconds)

  var idIter: Iterator[Source] = _
  var grandTotal: Double = _
  var active = Map[Source, SourceStatus]()

  def onCorrectBy(diff: Double, source: Source) {
    if (active.contains(source)) {
      var status = active(source)
      status.putDiff(sender, diff)
      if (status.allIdle)
        self ! GatherResults(source)
    } else {
      grandTotal += diff
      log.warning("Applied old source correction")
    }
  }

  def onNotIdle(source: Source) {
    var status = active(source)
    status.setNotIdle(sender)
  }

  def onDelayedGather(source: Source) {
    var status = active(source)
    if (!status.allIdle)
      return

    if (!status.steady) {
      status.steady = true
      timers.startSingleTimer(source , GatherResults(source), 10.millis)
    } else {
      grandTotal += status.getSum
      active -= source
      log.info("s{}: {}; active: {}", source, status.getSum, active.keys)
      supervisors.foreach { _ ! RemoveSource(source) }
      proclaimNewSource
    }
  }

  def proclaimNewSource() {
    if (active.isEmpty && grandTotal > 0) {
      log.warning("Grand total: {}", grandTotal)
      self ! PoisonPill
    }

    if (!idIter.hasNext)
      return

    val newSource = idIter.next

    implicit val timeout = Timeout(1 seconds)
    var futures = supervisors.map { _ ? NewSource(newSource) }.toList
    // Wtf, Array is not traversable once?!
    Future.sequence(futures).onComplete {
      case Success(list) => {
        val newSource =
          list.asInstanceOf[List[SourceRegistered]].head.source
        val sourceRef = nodesRef(newSource)
        log.info("New source {}", sourceRef.path.name)
        sourceRef ! NodeAct.DistanceEstimate(0, newSource)
      }
      case Failure(e) => {
        log.warning("Propagating new source timed out")
        self ! PoisonPill
      }
    }
    active += (newSource -> new SourceStatus(supervisors))
  }

  override def receive = super.receive orElse {
    case RequestBulk => {
      if (graph != null)
        timers.startSingleTimer(RequestBulk, RequestBulk, 500.millis)
      else {
        idIter = nodesRef.keys.iterator
        for(i <- 1 to 5) proclaimNewSource
      }
    }
    case CorrectBy(diff, src) => onCorrectBy(diff, src)
    case NotIdle(src)         => onNotIdle(src)
    case GatherResults(src)   => onDelayedGather(src)
    case TimedOut => {
      for((id, s) <- active){
        log.info("\nactive: {}\n"+
                  "idle count: {}/{}\n"+
                  "not idle: {}\n",
                  id, s.idleCount, s.children.size,
                  s.children.filter(ns => ns._2.idle == false).keys.map(ar => ar.path.name).mkString(","))
      }
      log.info("System timed out");
      context.system.terminate
    }
  }
}
