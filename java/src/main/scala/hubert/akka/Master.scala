package hubert.akka

import akka.actor.{Actor, ActorLogging, ActorRef, ActorSystem, Props}

//remove if not needed
import scala.collection.JavaConverters._

import scala.collection.mutable.Map
import hubert.akka.ReadGraph._
object Master {

  def props(filename: String): Props = Props(new Master(filename))

  final case class RequestASP(val source: Int)

  final case class UpdateEstimate(distance: Double)

  case object NotFinished

  case object GatherResults

}

class Master(filename: String) extends Actor with ActorLogging {
  import Master._
  import hubert.akka.Node.{Neighbours, DistanceEstimate}

  class NodeInfo(var id: Int) {
    var finished: Boolean = false
    var distance: Double = _
  }

  var all_nodes: Map[ActorRef, NodeInfo] = _

  var graph: Map[Int, Array[Int]] =
    ReadGraph
      .readGraph(filename, false)
      .asScala
      .map({ case (id, neigh) => id.toInt -> neigh.map(_.toInt) })

  import scala.runtime.ScalaRunTime._
  // log.warning("Graph: \n" + stringOf(graph))

  var idToNode: scala.collection.Map[Int, ActorRef] = _

  var finishedCount: Int = _
  var sourceID: Int = -1

  def onBuildGraph(): Unit = {
    log.warning("In BuildGraph")
    this.idToNode = this.graph.map {
      case (key, v) =>
        key -> context.actorOf(Node.props(sourceID), "node-" + key)
    }
    this.all_nodes = this.graph.map {
      case (id, ne) => idToNode(id) -> new NodeInfo(id)
    }
    this.graph.foreach {
      case (id, ne) => idToNode(id) ! Neighbours(ne.map(idToNode(_)))
    }
  }

  def onRequestASP(source: Int): Unit = {
    this.sourceID = source
    this.idToNode(sourceID) ! DistanceEstimate(0)
  }

  def onUpdateEstimate(dist: Double): Unit = {
    val nodeInfo: NodeInfo = this.all_nodes(sender())
    nodeInfo.distance = dist
    if (!nodeInfo.finished) finishedCount += 1
    nodeInfo.finished = true
    if (finishedCount == all_nodes.size)
      self ! GatherResults
  }

  private def onNotFinished(): Unit = {
    if (this.all_nodes(sender()).finished) {
      this.finishedCount -= 1
      this.all_nodes(sender()).finished = false
    }
  }

  var times_ful = 0
  private def onGatherResults(): Unit = {
    if (this.finishedCount == all_nodes.size)
      log.warning(
        times_ful + "th ssp: " + all_nodes.foldLeft(0.0)(_ + _._2.distance))
    times_ful += 1

  }

  def receive = {
    case RequestASP(src) => {
      if (this.sourceID < 0) {
        this.sourceID = src; onBuildGraph();
      }
      onRequestASP(src)
    }
    case UpdateEstimate(est) => onUpdateEstimate(est)
    case GatherResults       => onGatherResults()
  }
}

class BulkMaster(filename: String) extends Master{
  log
}
