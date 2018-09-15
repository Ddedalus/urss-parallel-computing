package hubert.akka

import java.util.ArrayList

import akka.actor.{Actor, ActorLogging, ActorSystem, Props}

//remove if not needed
import scala.collection.JavaConversions._

class Status(dist: Double) { 
  var distance = dist
  var announced = 0.0
  var propagated = false
  var newMessages = false
  var sentIdle = false
  def getCorrection(): Double = {
    val ret = distance - announced
    announced = distance
    return ret
  }
}

object NodeAct extends CommonInterfaces {

  def props: Props = Props(new NodeAct())

  final case class Neighbours(neighbours: Array[Node])
  final case class DistanceEstimate(distance: Double, src: Source)
  final case class Propagate(soruce: Source)
  final case class UpdateParent(source: Source)
}

class NodeAct() extends Actor with ActorLogging {
  import NodeAct._
  import Messages._

  private var neigh: Array[Node] = _
  private var active: Map[Source, Status] = _

  def onDistanceEstimate(distance: Double, source: Source): Unit = {
    if (! active.contains(source)) { 
      active += (source -> new Status(distance))
      self ! Propagate(source)
      self ! UpdateParent(source) // exactly one such message is always in the mailbox
    } else {
      var status = active(source)
      status.newMessages = true
      if (status.distance > distance) {
        if(status.sentIdle)
          context.parent ! NotIdle(source)  
        if (status.propagated)
          self ! Propagate(source)

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

  def onUpdateParent(source: Source) {
    if (! active.contains(source))
      log.info("Error, updating inactive source")
    else {
      var status = active(source)
      if (!status.newMessages) {
        context.parent ! CorrectBy(status.getCorrection(), source)
        // log.info("Updated {}", dist)
        status.sentIdle = true
      } else {
        status.newMessages = false
        self ! UpdateParent(source)
      }
    }
  }

  def receive = {
    case Neighbours(nb)              => neigh = nb
    case DistanceEstimate(dist, src) => onDistanceEstimate(dist, src)
    case Propagate(src)              => onPropagate(src)
    case UpdateParent(src)           => onUpdateParent(src)
    case RemoveSource(src) => active -= src
    // TODO: add some checks here
  }
}
