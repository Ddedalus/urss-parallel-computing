package hubert.akka

import akka.actor.{Actor, ActorLogging, ActorRef, Props}

//remove if not needed
import scala.collection.JavaConverters._

object GraphBuilder {

  def props(filename: String): Props = Props(new GraphBuilder(filename))

  final case class NodesCreated(refs: Map[Int, ActorRef])

  case object BuildGraph

  val branching: Int = 100

}

class GraphBuilder(filename: String) extends Actor with ActorLogging {
  import hubert.akka.ReadGraph._
  import GraphBuilder._
  import hubert.akka.Supervisor._
  import hubert.akka.Node.{Neighbours, DistanceEstimate, NewSource}

  var graph: Map[Int, Array[Int]] =
    ReadGraph
      .readGraph(filename, false)
      .asScala
      .map({ case (id, neigh) => id.toInt -> neigh.map(_.toInt) }).toMap

  var children: Map[ActorRef, NodeInfo] = _
  var nodesRef: Map[Int, ActorRef] = _
  var childrenCount: Int = _
  var outside: ActorRef = ActorRef.noSender

  if (graph.size < branching) {
    log.info("No supervision needed")
    children = graph.keys.map { id =>
      context.actorOf(Node.props, "n" + id) -> new NodeInfo(id)
    }.toMap
    nodesRef = children.map{
      case (ref, info) => info.id -> ref
    }
    self ! BuildGraph // ready to inform nodes
  } else {
    log.info("Supervision needed")
    children = graph.keys.sliding(branching, branching).toList.zipWithIndex.map{
      case (list, index) =>
        context.actorOf(Supervisor.props(list.toArray), "s" + index) -> new NodeInfo(0)
    }.toMap
  } // now has to collect all responses

  def onNodesCreated(refs: Map[Int, ActorRef]) {
    if(nodesRef == null)
      nodesRef = refs
    else
      nodesRef = nodesRef ++ refs;
    childrenCount += 1
    if (childrenCount == children.size) {
      self ! BuildGraph
    }
  }

  def onBuildGraph(): Unit = {
    nodesRef.foreach {
      case (id, ref) => {
        val neigh = graph(id).map { n_id =>
          nodesRef(n_id)
        }
        ref ! new Neighbours(neigh)
      }
    }
    this.graph = null

    log.info("Finished initializing agents: " + children.size)
  }

  def receive = {
    case BuildGraph         => onBuildGraph
    case NodesCreated(list) => onNodesCreated(list)
  }
}
