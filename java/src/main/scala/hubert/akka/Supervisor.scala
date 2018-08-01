package hubert.akka

import akka.actor.{Actor, ActorLogging, ActorRef, ActorSystem, Props}
import hubert.akka.Node.{Neighbours, DistanceEstimate, AskDistance}
import hubert.akka.GraphBuilder.{NodesCreated}
import hubert.akka.Master.{CorrectBy, NotFinished}

import scala.concurrent.duration._
import scala.concurrent.{ExecutionContext, Future}
import scala.util.{Success, Failure}
import ExecutionContext.Implicits.global
import akka.util.Timeout
import akka.pattern.ask

object Supervisor {
  def props(nodes: Array[Int]): Props = Props(new Supervisor(nodes))
}

class Supervisor(nodes: Array[Int])
    extends Actor
    with AnswerCollecting
    with Gathering
    with SendingCorrections
    with ActorLogging {
  import Supervisor._

  var children: Array[ActorRef] = nodes.map(
    id => context.actorOf(Node.props, "n" + id))

  context.parent ! NodesCreated(nodes.zip(children).toMap)

  def onCorrectBy(diff: Double, source: ActorRef) {
    if (statusses.contains(source)) {
      correctBy(diff, source,  sender)
      // log.info("Node {} corrected by {}", sender.path.name, diff)
    } else {
      log.warning("Correction from a wrong source")
      context.parent ! CorrectBy(diff, source) // bubble this up immediately for an ammendment on master
    }
  }

  override def onAllFinished(src : ActorRef){
    super.onAllFinished(src)
    setGather(self)
  }

  override def onFinishRevoked(src : ActorRef){
    super.onFinishRevoked(src)
    context.parent ! NotFinished(src)
  }

  def onLastGather(): Unit = {
    if () {
      log.info("Corrected parrent with {}", sumResults)
      context.parent ! CorrectBy(getCorrection(sumResults), source)
    } // on LastGather()
  }

  // not used
  def onUpdateEstimate(dist: Double, source: ActorRef): Unit = {
    if (source != this.source) {
      if (source == previousSource)
        log.warning("Estimate on previous source")
      else
        log.warning("Estimate for {}; {} expected.",
                    source.path.name,
                    this.source.path.name)
      return;
    }
    updateEstimate(dist, sender)
    if (allFinished) {
      self ! GatherResults
    }
  }

  def receive = {
    case CorrectBy(diff, src) => onCorrectBy(diff, src)
    case NotFinished          => onNotFinished
    case GatherResults        => onGather(() => onLastGather())
    case Node.NewSource(src)  => onNewSource(src)
  }
}
