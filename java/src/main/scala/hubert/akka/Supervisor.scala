package hubert.akka

import akka.actor.{Actor, ActorLogging, ActorRef, ActorSystem, Props}

object Supervisor {
  def props(nodes: Array[Int]): Props = Props(new Supervisor(nodes))

  case object GatherResults
}

class Supervisor(nodes: Array[Int])
    extends Actor
    with AnswerCollecting
    with ActorLogging {
  import Supervisor._
  import hubert.akka.Node.{Neighbours, DistanceEstimate, NewSource, AskDistance}
  import hubert.akka.GraphBuilder.{NodesCreated}
  import hubert.akka.Master.{UpdateEstimate, NotFinished}

  var children: Map[ActorRef, NodeInfo] = nodes.map { id =>
    context.actorOf(Node.props, "n" + id) -> new NodeInfo(id)
  }.toMap

  context.parent ! NodesCreated(children.map {
    case (ref, info) => info.id -> ref
  }.toMap)

  var previousSource : ActorRef = _

  def onUpdateEstimate(dist: Double, source: ActorRef): Unit = {
    if (source != this.source) {
      if(source == previousSource)
        log.warning("Estimate on previous source")
      else
        log.warning("Estimate for {}; {} expected.", source.path.name, this.source.path.name)
      return;
    }
    updateEstimate(dist, sender)
    if (finishedCount == children.size) {
      self ! GatherResults
    }
  }

  def onNotFinished(): Unit = {
    if (finishedCount == children.size)
      context.parent ! NotFinished
    notFinished(sender)
  }

  def onGatherResults() {
    if (this.finishedCount == children.size) {
      context.parent ! UpdateEstimate(sumResults, source)
    } else {
      // log.warning("Updates arrived after all reported finish!");
      return;
    }

  }

  def onNewSource(src: ActorRef) {
    if(source == null)
      source = src
    previousSource = source
    source = src
    sender ! true
    cleanInfo
  }

  def receive = {
    case UpdateEstimate(dist, src) => onUpdateEstimate(dist, src)
    case NotFinished               => onNotFinished
    case GatherResults             => onGatherResults
    case Node.NewSource(src)            => onNewSource(src)

  }
}
