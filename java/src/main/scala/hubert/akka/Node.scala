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
  private var source = ActorRef.noSender

  def onNewSource(src: ActorRef) {
    source = src
    sender ! true
    dist = Double.MaxValue
    propagated = true; newMessages = true; sentReady=false
    resetCorrections
  }

  def onDistanceEstimate(distance: Double): Unit = {
    newMessages = true
    if (distance >= dist) return
    dist = distance
    if (propagated) {
      self ! Propagate
      propagated = false
    }
    if (sentReady) {
      context.parent ! NotFinished
      sentReady = false
    }
    // send the messages just for the first time

  }

  def onPropagate(): Unit = {
    for (n <- neigh) {
      n ! DistanceEstimate(dist + 1, source)
    }
    propagated = true
    self ! UpdateParent(source)
    newMessages = false
  }

  def onUpdateParent(src: ActorRef) {
    if (!newMessages) {
      context.parent ! CorrectBy(getCorrection(dist), source)
      sentReady = true
    } else {
      newMessages = false
      self ! UpdateParent(source)
    }
  }

  def receive = {
    case Neighbours(nb) => neigh = nb
    case ns: NewSource => {
      if (newMessages || !sentReady)
        self.forward(ns)
      else
        onNewSource(ns.sourceId)
    }
    case DistanceEstimate(dist, src) => {
      if (src == source) onDistanceEstimate(dist)
      else log.warning("Wrong source {}", src.path.name)
    }
    case Propagate         => onPropagate
    case UpdateParent(src) => onUpdateParent(src)

  }
}
