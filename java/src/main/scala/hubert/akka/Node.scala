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

  case object AskDistance

  case object Propagate
}

class Node() extends Actor with SendingCorrections with ActorLogging {
  import hubert.akka.Master.{NotFinished, CorrectBy}
  import Node._

  private var neigh: Array[ActorRef] = _
  private var dist: Double = 0
  private var has_propagated: Boolean = true
  private var source = ActorRef.noSender

  // def onNewSource(src: ActorRef): Unit = {
  //   if (source == src) return;
  //   has_propagated = true
  //   source = src
  //   neigh.foreach(n => n ! NewSource(source))
  //   dist = Double.MaxValue;
  //   if (source == self)
  //     self ! DistanceEstimate(0.0)

  //   if (source.path.name == "n234" && self.path.name == "n79") {
  //     log.info("Current distance: " + this.dist)
  //   }
  // }

  def onNewSource(src: ActorRef) {
    source = src
    sender ! true
    dist = Double.MaxValue
    has_propagated = true
    resetCorrections
  }

  def onDistanceEstimate(distance: Double): Unit = {

    if (distance >= dist) return
    dist = distance
    if (!has_propagated) return
    // send the messages just for the first time
    context.parent ! NotFinished
    self ! Propagate
    has_propagated = false

  }

  def onPropagate(): Unit = {
    for (n <- neigh) {
      n ! DistanceEstimate(dist + 1, source)
    }
    has_propagated = true
    self ! AskDistance
  }

  def receive = {
    case Neighbours(nb) => neigh = nb
    case ns : NewSource=> {
    if(!has_propagated || shouldSendCorrection(dist))
      self.forward(ns)
    else
      onNewSource(ns.sourceId)
    }
    case DistanceEstimate(dist, src) => {
      if (src == source) onDistanceEstimate(dist)
      else log.warning("Distance estimate for a wrong source")
    }
    case Propagate => onPropagate
    case AskDistance => {
      if (has_propagated) {
        context.parent ! CorrectBy(getCorrection(dist), source)
      }
    }

  }
}
