package hubert.akka

import java.util.ArrayList

import akka.actor.{Actor, ActorLogging, ActorRef, ActorSystem, Props}

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

object Node {

  def props: Props = Props(new Node())

  final case class Neighbours(neighbours: Array[ActorRef])

  final case class DistanceEstimate(distance: Double, src: ActorRef)

  final case class NewSource(sourceId: ActorRef)

  final case class UpdateParent(source: ActorRef)

  final case class Propagate(soruce: ActorRef)

}

class Node() extends Actor with ActorLogging {
  import hubert.akka.Master.{NotIdle, CorrectBy}
  import Node._

  private var neigh: Array[ActorRef] = _
  private var active: Map[ActorRef, Status] = _

  def onDistanceEstimate(distance: Double, src: ActorRef): Unit = {
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

  def onPropagate(src: ActorRef): Unit = {
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

  def onUpdateParent(src: ActorRef) {
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
