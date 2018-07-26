package hubert.akka

import akka.actor.{Timers, Actor, ActorLogging, ActorRef, Props, PoisonPill}
import scala.concurrent.duration._

object Master {
  def props(filename: String): Props = Props(new Master(filename))
  final case class RequestASP(source: Int)
  final case class UpdateEstimate(distance: Double, source: ActorRef)
  case object GatherResults
  case object NotFinished
}

class Master(filename: String)
    extends GraphBuilder(filename)
    with AnswerCollecting
    with Timers {
  import Master._
  import hubert.akka.Node._

  timers.startSingleTimer(PoisonPill, PoisonPill, 20.seconds)

  def onRequestASP(source: Int): Unit = {
    this.source = this.nodesRef(source)
    this.source ! NewSource(this.source)
  }

  def onUpdateEstimate(dist: Double, source: ActorRef): Unit = {
    if (source != this.source) {
      log.warning("Estimate on a wrong source: {}, instead of {}",
                  this.source.path)
      return
    }
    updateEstimate(dist, sender)

    if (finishedCount == children.size)
      self ! GatherResults
  }

  def onGatherResults() : Double = {
    val sum = sumResults
    if (sum == -1.0)
      log.warning("Updates arrived after all reported finish!")
    else
      log.info("SSP of {} = {}", source.path, sum)
    return sum
  }

  override def postStop(){
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
    case NotFinished               => notFinished(sender)
    case GatherResults => onGatherResults
  }
}
