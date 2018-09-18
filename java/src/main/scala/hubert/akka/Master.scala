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

import scala.collection.concurrent.TrieMap

import scala.concurrent.duration._
import scala.concurrent.{ExecutionContext, Future}
import ExecutionContext.Implicits.global
import scala.util.{Success, Failure}

import akka.event.LogSource

object Master extends CommonInterfaces {
  def props(filename: String): Props = Props(new Master(filename))
  final case class GatherResults(source: Source)
  final case class ProclaimNewSource(a: Int)
  case object TimedOut
  case object RequestBulk
}

class Master(filename: String)
    extends GraphBuilder(filename)
    with Timers
    with SupervisionStrategy {
  import Master._
  import Messages._
  import Params._ 

  // TODO remove for production!!!
  timers.startSingleTimer(TimedOut, TimedOut, 10.seconds)

  var idIter: Iterator[Source] = _
  var grandTotal: Double = _
  var active = scala.collection.concurrent.TrieMap[Source, SourceStatus]()

  def onCorrectBy(diff: Double, source: Source) {
    if (active.contains(source)) {
      var status = active(source)
      status.putDiff(sender, diff)
      if (status.allIdle)
        self ! GatherResults(source)
    } else {
      log.error("Unknown correction source: {} from {}\n active:\t{}",
                source,
                sender.path.name,
                active.keys)
      self ! PoisonPill
    }
  }

  def onNotIdle(source: Source) {
    if (!active.contains(source)) {
      log.error("NI Missing: {},\nactive:\t{}", source, active.keys)
      self ! PoisonPill
      return
    }
    var status = active(source)
    status.setNotIdle(sender)
  }

  def onDelayedGather(source: Source) {
    if (!active.contains(source)) {
      log.error("DG Missing: {},\nactive:\t{}", source, active.keys)
      self ! PoisonPill
      return
    }
    var status = active(source)
    if (!status.allIdle)
      return

    if (!status.steady) {
      status.steady = true
      timers.startSingleTimer(source, GatherResults(source), 300.millis)
    } else {
      grandTotal += status.getSum
      active -= source
      log.info("ans s{}: {};\nactive:\t{}", source, status.getSum, active.keys)
      supervisors.foreach { _ ! RemoveSource(source) }
      proclaimNewSource
    }
  }

  def proclaimNewSource() {
    if (active.isEmpty && grandTotal > 0 && !idIter.hasNext) {
      log.warning("Grand total: {}", grandTotal)
      self ! PoisonPill
    }

    if (!idIter.hasNext)
      return

    var source = idIter.next

    implicit val timeout = Timeout(1 seconds)
    var futures = supervisors.map { _ ? NewSource(source) }.toList
    // Wtf, Array is not traversable once?!
    Future.sequence(futures).onComplete {
      case Success(list) => {
        var newSource =
          list.asInstanceOf[List[SourceRegistered]].head.source
        var sourceRef = nodesRef(newSource)
        this.active += (newSource -> new SourceStatus(supervisors))
        if (!active.contains(newSource)) {
          log.error("Source {} not added to active!", newSource)
          self ! PoisonPill
        }
        log.info("New source: {}\nactive:\t{}", newSource, active.keys)
        sourceRef ! NodeAct.DistanceEstimate(0, newSource)
      }
      case Failure(e) => {
        log.warning("Propagating new source timed out")
        self ! PoisonPill
      }
    }
  }
  override def preRestart(reason: Throwable, message: Option[Any]) {
    log.error("Attempted master restart")
    context.system.terminate
  }

  override def receive = super.receive orElse {
    case RequestBulk => {
      if (graph != null)
        timers.startSingleTimer(RequestBulk, RequestBulk, 200.millis)
      else {
        idIter = nodesRef.keys.iterator
        for (i <- 0 to ActiveMaxSize -1) {
          timers.startSingleTimer(ProclaimNewSource(i),
                                  ProclaimNewSource(i),
                                  (i * 50).millis)
        }
      }
    }
    case CorrectBy(diff, src) => onCorrectBy(diff, src)
    case NotIdle(src)         => onNotIdle(src)
    case GatherResults(src)   => onDelayedGather(src)
    case ProclaimNewSource(a) => proclaimNewSource
    case TimedOut => {
      for ((id, s) <- active) {
        log.info(
          "\nactive: {}\n" +
            "idle count: {}/{}\n" +
            "not idle: {}\n",
          id,
          s.idleCount,
          s.children.size,
          s.children
            .filter(ns => ns._2.idle == false)
            .keys
            .map(ar => ar.path.name)
            .mkString(",")
        )
      }
      log.info("System timed out");
      context.system.terminate
    }
  }
}
