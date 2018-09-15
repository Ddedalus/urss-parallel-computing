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
  import hubert.akka.Messages._
  import GraphBuilder._
  import hubert.akka.Supervisor._
  import hubert.akka.NodeAct.{Neighbours}

  var graph: Map[Int, Array[Int]] =
    ReadGraph
      .readGraph(filename, false)
      .asScala
      .map({ case (id, neigh) => id.toInt -> neigh.map(_.toInt) }).toMap

  var nodesRef: Map[Int, ActorRef] = _
  var outside: ActorRef = ActorRef.noSender

  var supervisors: Array[ActorRef] =
  graph.keys.sliding(branching, branching).toList.zipWithIndex.map{
    case (list, index) =>
      context.actorOf(Supervisor.props(list.toArray), "s" + index)
  }.toArray

  var supervisorsCount: Int = _
  def onNodesCreated(refs: Map[Int, Node]) {
    nodesRef = nodesRef ++ refs;
    supervisorsCount += 1
    if (supervisorsCount == supervisors.size) {
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

    log.info("Idle initializing agents: " + supervisors.size)
  }

  def receive = {
    case BuildGraph         => onBuildGraph
    case NodesCreated(list) => onNodesCreated(list)
  }
}
