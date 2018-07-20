package hubert.akka

import java.util.ArrayList

import akka.actor.{ Actor, ActorLogging, ActorRef, ActorSystem, Props }

//remove if not needed
import scala.collection.JavaConversions._

object Node {

	def props(): Props = Props(new Node())

	final case class Neighbours(neighbours: Array[ActorRef])

	final case class DistanceEstimate(distance: Double)

	case object AskDistance

	case object Propagate
}

class Node extends Actor {
	import hubert.akka.Master.{NotFinished, UpdateEstimate}
	import Node._

	private var neigh: Array[ActorRef] = _
	private var dist: Double = Double.MaxValue
	private var has_propagated: Boolean = true
	private var supervisor: ActorRef = _

	def onDistanceEstimate(distance: Double): Unit = {
		if (distance >= this.dist) return
		this.dist = distance
		if (!this.has_propagated) return
		// send the messages just for the first time, clear mailbox then
		context.parent ! NotFinished
		self ! Node.Propagate
		this.has_propagated = false
	}

	def onPropagate(): Unit = {
		for (n <- this.neigh) {
			n ! DistanceEstimate(this.dist + 1)
		}
		this.has_propagated = true
		self ! AskDistance
	}

	def receive = {
		case DistanceEstimate(dist) => onDistanceEstimate(dist);
		case Propagate => onPropagate();
		case AskDistance => {
			if (has_propagated)
				context.parent ! UpdateEstimate(this.dist)
		}
		case Neighbours(nb) => this.neigh = nb;		
	}
}