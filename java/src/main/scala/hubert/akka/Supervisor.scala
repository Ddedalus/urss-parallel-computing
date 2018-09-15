package hubert.akka

import akka.actor.{Actor, ActorLogging, ActorRef, ActorSystem, Props}
import hubert.akka.Node.{Neighbours, DistanceEstimate, NewSource}
import hubert.akka.GraphBuilder.{NodesCreated}
import hubert.akka.BulkMaster.{CorrectBy, NotIdle}

object Supervisor {
  def props(nodes: Array[Int]): Props = Props(new Supervisor(nodes))
}

class Supervisor(nodes: Array[Int])
    extends Actor
    with Gathering
    with SendingCorrections
    with ActorLogging {
  import Supervisor._

  var children: Array[ActorRef] = nodes.map { id =>
    context.actorOf(Node.props, "n" + id)}

  context.parent ! NodesCreated

  var active : Map[ActorRef, SourceStatus] = _

  def onNewSource(src: ActorRef) {
    active += (src -> new SourceStatus(children))
    sender ! true
  }

  def onCorrectBy(diff: Double, source: ActorRef) {
    if (! active.contains(source)){
      log.warning("Correction on inactive source")
    } else {
      var status = active(source)
      status.putDiff(sender, diff)
      if (status.allIdle) {
        setGather(self, source)
      }
    }
  }

  def onNotIdle(source: ActorRef): Unit = {
    var status = active(source)
    if (status.allIdle)
      context.parent ! NotIdle(source)
    status.setNotIdle(sender)
  }

  def onLastGather(source : ActorRef): Unit = {
    var status = active(source)
    if (status.allIdle) {
      log.info("Corrected parrent with {}", status.getSum)
      context.parent ! CorrectBy(status.getCorrection, source)
    } else {
      log.info("Last gather when not everyone idle...")
      setGather(self, source)
    }
  }

  def receive = {
    case CorrectBy(diff, src) => onCorrectBy(diff, src)
    case NotIdle(source)          => onNotIdle(source)
    case GatherResults(source)        => onGather(source, source => onLastGather(source))
    case Node.NewSource(src)  => onNewSource(src)
    case RemoveSource(src) => active -= src
  }
}
