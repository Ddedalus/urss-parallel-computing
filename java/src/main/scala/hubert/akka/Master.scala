package hubert.akka

import akka.actor.{
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
  def props(opts: Map[Symbol, Any]): Props = Props(new Master(opts))
  final case class ProclaimNewSource(a: Int)
  case object CheckQueue
  case object TimedOut
}

class Master(opts: Map[Symbol, Any])
    extends GraphBuilder(opts('GraphPath).asInstanceOf[String])
    with Timers
    with SupervisionStrategy {
  import Master._
  import Messages._

  // TODO remove for production!!!
  timers.startSingleTimer(TimedOut, TimedOut, opts('SystemTimeout).asInstanceOf[FiniteDuration])

  var idIter: Iterator[Source] = _
  var grandTotal: Double = _
  var active = scala.collection.concurrent.TrieMap[Source, SourceStatus]()
  var idleQueue = scala.collection.mutable.LinkedHashMap[Source, Millis]()

  val QueueDelay = opts('SourceInactivity).asInstanceOf[FiniteDuration].toMillis

  def onQueueCheck() {
    var now = System.currentTimeMillis()
    while (!idleQueue.isEmpty) {
      var (s, t) = idleQueue.head
      if (t + QueueDelay > now) return
      var status = active(s)
      if (!status.allIdle) {
        log.error("Source {} not idle after delay {}ms!", s, now - t)
        self ! PoisonPill
        return
      }
      grandTotal += status.getSum
      supervisors.foreach { _ ! RemoveSource(s) }

      active -= s
      idleQueue -= s
      // log.info("ans s{}: {}", s, status.getSum)
    }
  }

  def addSources() {
    if (active.size >= opts('ActiveMaxSize).asInstanceOf[Int]) return

    while (idIter.hasNext && active.size < opts('ActiveMaxSize).asInstanceOf[Int]) {
      var source = idIter.next
      active += (source -> new SourceStatus(supervisors))
      proclaimNewSource(source)
    }

    if (active.isEmpty && idleQueue.isEmpty && grandTotal > 0) {
      log.warning(" \n \n \tGrand total: {} \n \n ", grandTotal)
      context.system.terminate
    }
  }

  def onCorrectBy(diff: Double, source: Source) {
    if (active.contains(source)) {
      var status = active(source)
      status.putDiff(sender, diff)
      if (status.allIdle)
        idleQueue += source -> System.currentTimeMillis()
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
    idleQueue -= source
    status.setNotIdle(sender)
  }

  def proclaimNewSource(inputSrc: Source = -1): Source = {
    var source = inputSrc

    if (!idIter.hasNext && source == -1)
      return -1
    else if (source == -1)
      source = idIter.next

    implicit val timeout = Timeout(opts('PropagationTimeout).asInstanceOf[FiniteDuration])
    var futures = supervisors.map { _ ? NewSource(source) }.toList
    // Wtf, Array is not traversable once?!
    Future.sequence(futures).onComplete {
      case Success(list) => {
        var newSource =
          list.asInstanceOf[List[SourceRegistered]].head.source
        var sourceRef = nodesRef(newSource)
        sourceRef ! NodeAct.DistanceEstimate(0, newSource)
      }
      case Failure(e) => {
        log.error("Propagating new source timed out")
        self ! PoisonPill
      }
    }
    return source
  }

  override def preRestart(reason: Throwable, message: Option[Any]) {
    log.error("Attempted master restart")
    context.system.terminate
  }

  override def receive = super.receive orElse {
    case CheckQueue => onQueueCheck; addSources
    case GraphBuilder.GraphReady => {
        idIter = nodesRef.keys.iterator
        timers.startPeriodicTimer(CheckQueue, CheckQueue, opts('QueueCheckPeriod).asInstanceOf[FiniteDuration])
      }
    case CorrectBy(diff, src) => onCorrectBy(diff, src)
    case NotIdle(src)         => onNotIdle(src)
    case ProclaimNewSource(a) => proclaimNewSource()
    case TimedOut => {
      log.info("System timed out");
      context.system.terminate
    }
  }
}
