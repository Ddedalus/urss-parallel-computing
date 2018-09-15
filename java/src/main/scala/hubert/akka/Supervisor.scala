package hubert.akka

import akka.actor.{Actor, ActorLogging, ActorSystem, Props}

object Supervisor extends CommonInterfaces {
  def props(nodes: Iterable[Int]): Props = Props(new Supervisor(nodes))
}

class Supervisor(nodes: Iterable[Int])
    extends Actor
    with Gathering
    with ActorLogging {
  import Supervisor._
  import GraphBuilder.{NodesCreated}
  import NodeAct.{Neighbours}
  import Messages._

  var idMap: Map[Int, Node] = nodes.map { id =>
    id -> context.actorOf(NodeAct.props, "n" + id)
  }.toMap
  var children: Array[Node] = idMap.values.toArray

  context.parent ! new NodesCreated(idMap)
  idMap = null

  var active: Map[Node, SourceStatus] = _

  def onNewSource(source: Node) {
    active += (source -> new SourceStatus(children))
    sender ! SourceRegistered(source)
  }

  def onCorrectBy(diff: Double, source: Node) {
    if (!active.contains(source)) {
      log.warning("Correction on inactive source")
    } else {
      var status = active(source)
      status.putDiff(sender, diff)
      if (status.allIdle) {
        setGather(self, source)
      }
    }
  }

  def onNotIdle(source: Node): Unit = {
    var status = active(source)
    if (status.allIdle)
      context.parent ! NotIdle(source)
    status.setNotIdle(sender)
  }

  def onLastGather(source: Node): Unit = {
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
    case NotIdle(source)      => onNotIdle(source)
    case GatherResults(source) =>
      onGather(source, source => onLastGather(source))
    case NewSource(src) => onNewSource(src)
    case RemoveSource(src)      => active -= src
  }
}
