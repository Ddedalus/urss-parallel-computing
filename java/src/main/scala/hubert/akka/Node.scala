package hubert.akka

import java.util.ArrayList

import akka.actor.{Actor, ActorLogging, ActorRef, ActorSystem, Props}

//remove if not needed
import scala.collection.JavaConversions._

object Node {

  def props: Props = Props(new Node())

  final case class Neighbours(neighbours: Array[ActorRef])

  final case class DistanceEstimate(distance: Double)

  final case class NewSource(sourceId: ActorRef)

  case object AskDistance

  case object Propagate
}

class Node() extends Actor with ActorLogging{
  import hubert.akka.Master.{NotFinished, UpdateEstimate}
  import Node._

  private var neigh: Array[ActorRef] = _
  private var dist: Double = Double.MaxValue
  private var has_propagated: Boolean = true
  private var source = ActorRef.noSender

  def onDistanceEstimate(distance: Double): Unit = {
    if (distance >= dist) return
    dist = distance
    if (!has_propagated) return
    // send the messages just for the first time, clear mailbox then
    context.parent ! NotFinished
    self ! Propagate
    has_propagated = false

  }

  def onPropagate(): Unit = {
    for (n <- neigh) {
      n ! DistanceEstimate(dist + 1)
    }
    has_propagated = true
    self ! AskDistance
  }

  def onNewSource(src: ActorRef): Unit = {
    if (source == src) return;
    has_propagated = true
    source = src
    neigh.foreach(n => n ! NewSource(source))
    dist = Double.MaxValue;
    if (source == self)
      self ! DistanceEstimate(0.0)
  }

  def receive = {
    // case DistanceEstimate(dist) => onDistanceEstimate(dist);
    // case Propagate              => onPropagate();
    case AskDistance => {
      if (has_propagated)
        context.parent ! UpdateEstimate(dist, source)
    }
    case Neighbours(nb) => neigh = nb
    case NewSource(src) => onNewSource(src)
    case Propagate => onPropagate
    case DistanceEstimate(dist) => onDistanceEstimate(dist)

  }
}
