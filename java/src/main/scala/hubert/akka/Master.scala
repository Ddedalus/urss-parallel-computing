package hubert.akka

import akka.actor.{Timers, Actor, ActorLogging, ActorRef, Props}
import scala.concurrent.duration._

object Master {
  def props(filename: String): Props = Props(new Master(filename))
  final case class RequestASP(source: Int)
  final case class UpdateEstimate(distance: Double, source: ActorRef)
  case object NotFinished
  final case class CorrectBy(diff: Double, source: ActorRef)
  case object TimedOut
}



class Master(filename: String)
    extends GraphBuilder(filename)
    with AnswerCollecting
    with Gathering
    with Timers {
  import Master._
  import hubert.akka.Node._

  timers.startSingleTimer(TimedOut, TimedOut, 20.seconds)


  def onRequestASP(source: Int): Unit = {
    this.source = this.nodesRef(source)
    this.source ! NewSource(this.source)
  }

  def onUpdateEstimate(dist: Double, source: ActorRef): Unit = {
    if (source != this.source) {
      log.warning("Estimate on a wrong source: {}, instead of {}",
                  source.path.name,
                  this.source.path.name)
      return
    }
    updateEstimate(dist, sender)

    if (allFinished)
      setGather(self)
  }

  def onCorrectBy(diff: Double, source: ActorRef) {
    if (source == this.source)
      correctBy(diff, sender)
    else
      log.warning("Correction from a wrong source")
    if (allFinished) {
      setGather(self)
    }
  }

  def onLastGather(): Unit = if (allFinished) {
    log.info("SSP of {} = {}", source.path.name, sumResults)
  }
  
  override def postStop() {
    context.system.terminate
  }

  override def receive = super.receive orElse {
    case msg: RequestASP => {
      if (graph != null) {
        timers.startSingleTimer(NotFinished, msg, 500.millis)
        log.warning("Not ready to accept Request yet...")
      } else
        onRequestASP(msg.source)
    }
    case UpdateEstimate(dist, src) => onUpdateEstimate(dist, src)
    case CorrectBy(diff, src)      => onCorrectBy(diff, src)
    case NotFinished               => notFinished(sender)
    case GatherResults             => onGather(() => onLastGather())
    case TimedOut                => {
      log.info("System timed out");
      context.system.terminate}
  }
}
