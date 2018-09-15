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
  final case class RequestASP(source: Int)
  case object TimedOut
  case object RequestBulk
}

class Master(filename: String)
    extends GraphBuilder(filename)
    with Gathering
    with Timers {
  import Master._
  import Messages._
  import hubert.akka.Supervisor._
  timers.startSingleTimer(TimedOut, TimedOut, 20.seconds)

  var idIter: Iterator[Source] = _
  var grandTotal: Double = _
  var active: Map[Source, SourceStatus] = _

  def onCorrectBy(diff: Double, source: Source) {
    if (active.contains(source)) {
      var status = active(source)
      status.putDiff(source, diff)
      if (status.allIdle)
        setGather(self, source)
    } else {
      grandTotal += diff
      log.warning("Applied old source correction")
    }
  }

  def onNotIdle(source: Source) {
    var status = active(source)
    status.setNotIdle(sender)
  }

  def onLastGather(source: Source): Unit = {
    var status = active(source)
    if (status.allIdle) {
      log.info("Sum: {}", status.getSum)
      grandTotal += status.getSum
      active -= source
      supervisors.foreach { _ ! RemoveSource(source)}
      if (idIter.hasNext)
        proclaimNewSource
      else if (active.isEmpty) {
        log.warning("Grand total: {}", grandTotal)
        self ! PoisonPill
      }
    } else {
      log.warning("Last gather when not everyone idle")
      setGather(self, source)
    }
  }

  def proclaimNewSource() {
    if (!idIter.hasNext) return
    val newSource = idIter.next

    implicit val timeout = Timeout(2 seconds)
    var futures =  supervisors.map { _ ? NewSource(newSource)}.toList
    // Wtf, Array is not traversable once?!
    Future.sequence(futures).onComplete {
      case Success(list) => {
        val newSource =
          list.asInstanceOf[Array[SourceRegistered]].head.source
        log.info("New source {}", newSource.path.name)
        newSource ! NodeAct.DistanceEstimate(0, newSource)
      }
      case Failure(e) => {
        log.warning("Propagating new source timed out")
        self ! PoisonPill
      }
    }
    active += (newSource -> new SourceStatus(supervisors))
  }

  override def postStop() {
    context.system.terminate
  }

  override def receive = super.receive orElse {
    case RequestBulk => {
      if (graph != null)
        timers.startSingleTimer(RequestBulk, RequestBulk, 500.millis)
      else {
        idIter = nodesRef.values.iterator
        proclaimNewSource
      }
    }
    case CorrectBy(diff, src) => onCorrectBy(diff, src)
    case NotIdle(src)         => onNotIdle(src)
    case GatherResults(src)   => onGather(src, src => onLastGather(src))
    case TimedOut => {
      log.info("System timed out");
      context.system.terminate
    }
  }
}
