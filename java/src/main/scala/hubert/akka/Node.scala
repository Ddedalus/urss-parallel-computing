package hubert.akka

import java.util.ArrayList
import scala.collection.mutable.Map

import akka.actor.{Actor, ActorLogging, ActorRef, ActorSystem, Props}

//remove if not needed
import scala.collection.JavaConversions._

object Node {
  def props: Props = Props(new Node())

  final case class Neighbours(neighbours: Array[ActorRef])

  final case class DistanceEstimate(distance: Double, src: ActorRef)

  final case class SwapSources(newSrc : ActorRef, oldSrc : ActorRef)

  final case class AskDistance(src : ActorRef)

  final case  class Propagate(src : ActorRef)
}

class NodeStatus() {
  var distance = Double.MaxValue;
  var announced = 0.0;
  var has_propagated = true;

  def getCorrection: Double = {
    val ret = distance - announced
    announced = distance
    return ret
  }
  def shouldSendCorrection: Boolean = {
    distance - announced != 0
  }

}

class Node() extends Actor with SendingCorrections with ActorLogging {
  import hubert.akka.Master.{NotFinished, CorrectBy}
  import Node._

  private var neigh: Array[ActorRef] = _
  private var distances: Map[ActorRef, NodeStatus] = _

  def onSwapSources(newSrc: ActorRef, odlSrc: ActorRef) {
    if (odlSrc != ActorRef.noSender && distances.contains(odlSrc)) {
      if (!distances(odlSrc).has_propagated || distances(odlSrc).distance == Double.MaxValue) {
        log.error("Removed source still in use!")
        context.system.terminate
      } else if (distances(odlSrc).shouldSendCorrection) {
        context.parent ! CorrectBy(distances(odlSrc).getCorrection, odlSrc)
      }
      distances -= odlSrc
    }
    distances.put(newSrc, new NodeStatus())
  }

  def onDistanceEstimate(distance: Double, src : ActorRef): Unit = {
    if(! distances.contains(src)){
      log.warning("Estimate for nonexistent source!")
    }else{
      val status = distances(src)
      if(distance > status.distance) return
      status.distance = distance
      if(!status.has_propagated) return

      context.parent ! NotFinished(src)
      self ! Propagate(src)
      status.has_propagated = false
    }

  }

  def onPropagate(src : ActorRef): Unit = {
    for (n <- neigh) {
      n ! DistanceEstimate(distances(src).distance + 1, src)
    }
    distances(src).has_propagated = true
    self ! AskDistance(src)
  }

  def receive = {
    case Neighbours(nb) => {neigh = nb}
    case SwapSources(newS, old) => onSwapSources(newS, old)
    case DistanceEstimate(dist, src) => {
      if (src == source) onDistanceEstimate(dist)
      else log.warning("Distance estimate for a wrong source")
    }
    case Propagate(src) => onPropagate(src)
    case AskDistance(src) => {
      if (distances(src).has_propagated) {
        context.parent ! CorrectBy(getCorrection(dist), src)
      }
    }

  }
}
