package hubert.akka

import java.util.ArrayList

import akka.actor.{Actor, ActorLogging, ActorRef, ActorSystem, Props}

//remove if not needed
import scala.collection.JavaConversions._

object Node {

  def props: Props = Props(new Node())

  final case class Neighbours(neighbours: Array[ActorRef])

  final case class DistanceEstimate(distance: Double, src: ActorRef)

  final case class NewSource(sourceId: ActorRef)

  final case class UpdateParent(source: ActorRef)

  case object Propagate
}

class Node() extends Actor with SendingCorrections with ActorLogging {
  import hubert.akka.Master.{NotFinished, CorrectBy}
  import Node._

  private var neigh: Array[ActorRef] = _
  private var dist: Double = 0
  private var propagated = true
  var newMessages = false
  var sentReady = true
  var source = ActorRef.noSender

  def onNewSource(src: ActorRef) {
    this.source = src
    sender ! true

    dist = Double.MaxValue
    propagated = true;
    newMessages = true; sentReady = false
    resetCorrections
  }

  def onDistanceEstimate(distance: Double): Unit = {
    if (dist == Double.MaxValue) { // first message received
      self ! UpdateParent(source)
      self ! Propagate
      propagated = false
    }
    newMessages = true
    if (sentReady) {
      context.parent ! NotFinished
      sentReady = false
    }
    if (distance < dist) {
      dist = distance
      if (propagated) {
        self ! Propagate
        propagated = false
      }
    }

  }

  def onPropagate(): Unit = {
    for (n <- neigh) {
      n ! DistanceEstimate(dist + 1, source)
    }
    propagated = true
  }

  def onUpdateParent(src: ActorRef) {
    if (!newMessages) {
      context.parent ! CorrectBy(getCorrection(dist), source)
      log.info("Updated {}", dist)
      sentReady = true
    } else {
      newMessages = false
      self ! UpdateParent(source)
    }
  }

  def receive = {
    case Neighbours(nb) => neigh = nb
    case NewSource(src) => {
      if (newMessages || !sentReady) {
        log.info("Forwarded")
        self.forward(NewSource(src), sender)
      } else
        onNewSource(src)
    }
    case DistanceEstimate(dist, src) => {
      if (src != this.source) onNewSource(src)
      onDistanceEstimate(dist)
    }
    case Propagate         => onPropagate
    case UpdateParent(src) => onUpdateParent(src)

  }
}
