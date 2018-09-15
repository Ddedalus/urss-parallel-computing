package hubert.akka

import java.util.ArrayList

import akka.actor.{Actor, ActorLogging, ActorSystem, Props}

//remove if not needed
import scala.collection.JavaConversions._

class Status(dist: Double) { //with SendingCorrections
  var distance = dist
  var announced = 0.0
  var propagated = false
  var newMessages = false
  var sentReady = true
  def getCorrection(): Double = {
    val ret = distance - announced
    announced = distance
    return ret
  }
}

object NodeAct extends ActorRefAliases {

  def props: Props = Props(new NodeAct())

  final case class Neighbours(neighbours: Array[Node])

  final case class DistanceEstimate(distance: Double, src: Source)

  final case class NewSource(sourceId: Source)

  final case class UpdateParent(source: Source)

  final case class Propagate(soruce: Source)

}

class NodeAct() extends Actor with ActorLogging {
  import hubert.akka.BulkMaster.{NotIdle, CorrectBy}
  import NodeAct._

  private var neigh: Array[Node] = _
  private var active: Map[Source, Status] = _

  def onDistanceEstimate(distance: Double, src: Source): Unit = {
    if (! active.contains(src)) { // check this against docs
      active += (src -> new Status(distance))
      self ! UpdateParent(src)
      self ! Propagate(src)
    } else {
      var status = active(src)
      status.newMessages = true
      if (status.distance > distance) {
        if(status.sentReady)
          context.parent ! NotIdle(src)  
        if (status.propagated)
          self ! Propagate(src)

        status.propagated = false
        status.distance = distance
      }
    }
  }

  def onPropagate(src: Source): Unit = {
    if (! active.contains(src))
      log.info("Error, propagating inactive source")
    else {
      var status = active(src)
      for (n <- neigh) {
        n ! DistanceEstimate(status.distance + 1, src)
      }
      status.propagated = true
    }
  }

  def onUpdateParent(src: Source) {
    if (! active.contains(src))
      log.info("Error, updating inactive source")
    else {
      var status = active(src)
      if (!status.newMessages) {
        context.parent ! CorrectBy(status.getCorrection(), src)
        // log.info("Updated {}", dist)
        status.sentReady = true
      } else {
        status.newMessages = false
        self ! UpdateParent(src)
      }
    }
  }

  def receive = {
    case Neighbours(nb)              => neigh = nb
    case DistanceEstimate(dist, src) => onDistanceEstimate(dist, src)
    case Propagate(src)              => onPropagate(src)
    case UpdateParent(src)           => onUpdateParent(src)

  }
}
