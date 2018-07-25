package hubert.akka

import akka.actor.{Actor, ActorLogging, ActorRef, ActorSystem, Props}

object Supervisor {
  def props(nodes: Array[Int]): Props = Props(new Supervisor(nodes))
}

class Supervisor(nodes: Array[Int]) extends Actor with ActorLogging {
  import Supervisor._
  import hubert.akka.Node.{Neighbours, DistanceEstimate, NewSource}
  import hubert.akka.GraphBuilder.{NodesCreated}

  var children: Map[ActorRef, NodeInfo] = nodes.map{
    id => context.actorOf(Node.props, "n" + id) -> new NodeInfo(id)
  }.toMap

  val tmp = children.map{
    case (ref, info) => info.id -> ref
  }.toMap
  context.parent ! NodesCreated(tmp)

  // def onNotFinished(): Unit = {
  //   if (this.all_nodes(sender()).finished) {
  //     this.finishedCount -= 1
  //     this.all_nodes(sender()).finished = false
  //   }
  // }

  // def onGatherResults(): Double = {
  //   if (this.finishedCount == all_nodes.size) {
  //     val sum = all_nodes.foldLeft(0.0)(_ + _._2.distance)
  //     log.info("SSP for " + sourceID.path + ": " + sum)
  //     return sum;
  //   } else {
  //     log.warning("Updates arrived after all reported finish!");
  //     return -1;
  //   }

  // }

  def receive = {
    case Some(o) => log.info("Unrecognised" + o.toString)
  }
}

