package hubert.akka

import akka.actor.{Actor, ActorLogging, ActorRef, ActorSystem, Props}
import hubert.akka.Node.{Neighbours, DistanceEstimate, NewSource}
import hubert.akka.GraphBuilder.{NodesCreated}
import hubert.akka.Master.{CorrectBy, NotFinished}

import scala.concurrent.duration._
import scala.concurrent.{ExecutionContext, Future}
import scala.util.{Success, Failure}
import ExecutionContext.Implicits.global
import akka.util.Timeout
import akka.pattern.ask

object Supervisor {
  def props(nodes: Array[Int]): Props = Props(new Supervisor(nodes))
}

class Supervisor(nodes: Array[Int])
    extends Actor
    with AnswerCollecting
    with Gathering
    with SendingCorrections
    with ActorLogging {
  import Supervisor._

  var children: Map[ActorRef, NodeInfo] = nodes.map { id =>
    context.actorOf(Node.props, "n" + id) -> new NodeInfo(id)
  }.toMap

  context.parent ! NodesCreated(children.map {
    case (ref, info) => info.id -> ref
  }.toMap)

  var previousSource: ActorRef = _

  def onNewSource(src: ActorRef) {
    if (source == null)
      source = src

    implicit val timeout = Timeout(2 seconds)
    val futures = children.keys.map(_ ? Node.NewSource(source))
    val replyTarget = Iterable(Future(sender))
    Future.sequence(replyTarget ++ futures).onComplete {
      case Success(list) => {
        val target = list.head.asInstanceOf[ActorRef]
        previousSource = source
        source = src
        cleanInfo; resetCorrections
        target ! true
      }
      case Failure(e) =>
        log.error("Failed to establish new source: " + e.toString)
    }
  }

  def onCorrectBy(diff: Double, source: ActorRef) {
    if (source == this.source) {
      correctBy(diff, sender)
      // log.info("Node {} corrected by {}", sender.path.name, diff)
    } else {
      log.warning("Correction from a wrong source")
      context.parent ! CorrectBy(diff, source) // bubble this up immediately for an ammendment on master
    }
    if (allFinished) {
      setGather(self)
    }
  }

  def onNotFinished(): Unit = {
    val was_finished = allFinished
    notFinished(sender)
    if (was_finished && !allFinished) {
      context.parent ! NotFinished
    }
  }

  def onLastGather(): Unit = {
    if (allFinished && shouldSendCorrection(sumResults)) {
      log.info("Corrected parrent with {}", sumResults)
      context.parent ! CorrectBy(getCorrection(sumResults), source)
    } // on LastGather()
  }

  // not used
  def onUpdateEstimate(dist: Double, source: ActorRef): Unit = {
    if (source != this.source) {
      if (source == previousSource)
        log.warning("Estimate on previous source")
      else
        log.warning("Estimate for {}; {} expected.",
                    source.path.name,
                    this.source.path.name)
      return;
    }
    updateEstimate(dist, sender)
    if (allFinished) {
      self ! GatherResults
    }
  }

  def receive = {
    case CorrectBy(diff, src) => onCorrectBy(diff, src)
    case NotFinished          => onNotFinished
    case GatherResults        => onGather(() => onLastGather())
    case Node.NewSource(src)  => onNewSource(src)
  }
}
