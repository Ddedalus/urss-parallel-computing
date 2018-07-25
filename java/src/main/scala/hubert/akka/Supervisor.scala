package hubert.akka

import akka.actor.{Actor, ActorLogging, ActorRef, ActorSystem, Props}

//remove if not needed
import scala.collection.JavaConverters._

// import scala.collection.mutable.Map
import hubert.akka.ReadGraph._
import huber.akka.NodeInfo

object Supervisor {

  def props(nodes: Array[Int]): Props = Props(new Supervisor(nodes))

  final case class RequestASP(val source: Int)

  final case class UpdateEstimate(distance: Double)

  case object NotFinished

  case object GatherResults

  val branching = 100

}

class Supervisor(nodes: Array[Int]) extends Actor with ActorLogging {
  import Supervisor._
  import hubert.akka.Node.{Neighbours, DistanceEstimate, NewSource}

  var children: Map[ActorRef, NodeInfo] = _
  if (nodes.length > branching) {
    // if(nodes.length < branching*branching)
    children = nodes
      .sliding(branching, branching).zipWithIndex
      .map{ case (list, i) =>
        (context.actorOf(Supervisor.props(list), "s" + i), new NodeInfo(i))
      }.toMap
  } else {
    children = nodes.map { id =>
      (context.actorOf(Node.props, "n" + id), new NodeInfo(id))
    }.toMap
  }
  var graphSlice: Map[Int, Array[Int]] = _

  var finishedCount: Int = _
  var sourceID: ActorRef = ActorRef.noSender

  def onBuildGraph(slice : Map[Int, Array[Int]]): Unit = {
    this.graphSlice = slice
    if(slice.size > branching){
      slice.sliding(branching, branching).toList.zip(children).foreach{
        case (list, sup) => sup._1 ! 
      }
    }else{
      children.foreach{
        case (ref, info) => ref ! new Neighbours(slice(info.id)) 
      }
    }
    this.graphSlice.foreach {
      case (id, ne) => idToNode(id) ! Neighbours(ne.map(idToNode(_)))
    }
    this.graph = null
    log.info("Finished initializing agents: " + all_nodes.size)
  }
  this.onBuildGraph;

  def onRequestASP(source: Int): Unit = {
    this.sourceID = idToNode(source)
    log.info("New source " + source + " with agent " + this.sourceID.path);
    this.sourceID ! NewSource(this.sourceID)
    this.sourceID ! DistanceEstimate(0)
  }

  def onUpdateEstimate(dist: Double): Unit = {
    val nodeInfo: NodeInfo = this.all_nodes(sender())
    nodeInfo.distance = dist
    if (!nodeInfo.finished) finishedCount += 1
    nodeInfo.finished = true
    if (finishedCount == all_nodes.size)
      self ! GatherResults
  }

  def onNotFinished(): Unit = {
    if (this.all_nodes(sender()).finished) {
      this.finishedCount -= 1
      this.all_nodes(sender()).finished = false
    }
  }

  def onGatherResults(): Double = {
    if (this.finishedCount == all_nodes.size) {
      val sum = all_nodes.foldLeft(0.0)(_ + _._2.distance)
      log.info("SSP for " + sourceID.path + ": " + sum)
      return sum;
    } else {
      log.warning("Updates arrived after all reported finish!");
      return -1;
    }

  }

  def receive = {
    case RequestASP(src)     => onRequestASP(src)
    case UpdateEstimate(est) => onUpdateEstimate(est)
    case GatherResults       => onGatherResults()
  }
}

object BulkMaster {
  def props(filename: String): Props = Props(new BulkMaster(filename))
  case object RequestBulk
}
class BulkMaster(filename: String) extends Master(filename) {
  import BulkMaster._
  import hubert.akka.Node.{NewSource, DistanceEstimate}
  var idIter: Iterator[ActorRef] = _;
  var grandTotal: Double = _;

  def cleanUp(): Unit = {
    all_nodes.foreach(tup => {
      tup._2.finished = false;
      tup._2.distance = Double.MaxValue
    })
    finishedCount = 0;
  }

  override def onGatherResults(): Double = {
    val sum = super.onGatherResults();
    if (sum == -1)
      return sum;

    grandTotal += sum
    this.cleanUp
    if (idIter != null && idIter.hasNext) {
      sourceID = idIter.next();
      sourceID ! NewSource(sourceID)
    } else if (idIter != null) {
      log.warning("All pairs ssp: " + grandTotal)
    }
    return sum;
  }

  override def receive = super.receive orElse {
    case RequestBulk => {
      idIter = all_nodes.keys.iterator;
      sourceID = idIter.next();
      sourceID ! DistanceEstimate(0.0);
    }
  }
}
