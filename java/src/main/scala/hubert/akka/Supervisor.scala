package hubert.akka

import akka.actor.{Actor, ActorLogging, ActorRef, ActorSystem, Props}
import hubert.akka.Node.{Neighbours, DistanceEstimate, NewSource}
import hubert.akka.GraphBuilder.{NodesCreated}
import hubert.akka.Master.{CorrectBy, NotFinished}

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

  var children: Map[ActorRef, NodeInfo] = nodes.map { id =>
    context.actorOf(Node.props, "n" + id) -> new NodeInfo(id)
  }.toMap

  context.parent ! NodesCreated(children.map {
    case (ref, info) => info.id -> ref
  }.toMap)

  var previousSource: ActorRef = _
  var tempSender : ActorRef = _

  def onNewSource(src: ActorRef) {
    previousSource = source
    source = src
    cleanInfo; resetCorrections
    sender ! true
  }

  def onCorrectBy(diff: Double, source: ActorRef) {
    if (source == this.source) {
      correctBy(diff, sender)
      // log.info("Node {} corrected by {}", sender.path.name, diff)
    } else {
      log.warning("Correction from a wrong source")
      context.parent ! CorrectBy(diff, source) // bubble this up immediately for an ammendment on master
    }
    if (allFinished) {
      setGather(self)
    }
  }

  def onNotFinished(): Unit = {
    if (allFinished)
      context.parent ! NotFinished
    notFinished(sender)
  }

  def onLastGather(): Unit = {
    if (allFinished) {
      log.info("Corrected parrent with {}", sumResults)
      context.parent ! CorrectBy(getCorrection(sumResults), source)
    }else{
      log.info("Last gather when not everyone finished...")
      setGather(self)
    } 
  }

  def receive = {
    case CorrectBy(diff, src) => onCorrectBy(diff, src)
    case NotFinished          => onNotFinished
    case GatherResults        => onGather(() => onLastGather())
    case Node.NewSource(src)  => onNewSource(src)
  }
}
