package hubert.akka

import akka.actor.{
  Actor,
  ActorLogging,
  ActorRef,
  ActorSystem,
  Props,
  PoisonPill
}
import scala.concurrent.duration._
import scala.concurrent.{ExecutionContext, Future}
import ExecutionContext.Implicits.global
import scala.util.{Success, Failure}

import akka.util.Timeout
import akka.pattern.ask

object BulkMaster {
  def props(filename: String): Props = Props(new BulkMaster(filename))

  case object RequestBulk
}

class BulkMaster(filename: String) extends Master(filename) {
  import BulkMaster._
  import hubert.akka.Supervisor._

  var idIter: Iterator[ActorRef] = _;
  var newSource: ActorRef = _
  var grandTotal: Double = _;

  override def onCorrectBy(diff: Double, source: ActorRef) {
    if (source == this.source)
      correctBy(diff, sender)
    else {
      grandTotal += diff
      log.info("Applied old source correction")
    }
    if (allFinished){
      setGather(self)
    }
  }

  override def onLastGather(): Unit = if (allFinished) {
    grandTotal += sumResults
    cleanInfo()
    proclaimNewSource
  }

  def proclaimNewSource() {
    if (idIter.hasNext) {
      this.newSource = idIter.next
      // if(newSource.path.name == "n234" || newSource.path.name == "n1"){ 
      //   log.warning("Skipping: " + newSource.path.name)
      //   this.newSource = idIter.next}

      implicit val timeout = Timeout(5 seconds)
      val futures = children.keys.map { _ ? Node.NewSource(newSource) }
      Future.sequence(futures).onComplete {
        case Success(list) => {
          log.info("New source {}", newSource.path.name)
          self ! Node.NewSource(this.newSource)
          this.newSource ! Node.DistanceEstimate(0, newSource)

        }
        case Failure(e) => {
          log.warning("Propagating new source timed out")
          self ! PoisonPill
        }
      }
    } else {
      log.warning("Grand total: {}", grandTotal)
      self ! PoisonPill
    }
  }

  // not used
  override def onUpdateEstimate(dist: Double, source: ActorRef): Unit = {
    super.onUpdateEstimate(dist, source)
    if (allFinished) {
      gatherCounter += 1
    }
  }

  override def receive = super.receive orElse {
    case RequestBulk => {
      if (graph != null)
        timers.startSingleTimer(RequestBulk, RequestBulk, 500.millis)
      else {
        idIter = nodesRef.values.iterator
        proclaimNewSource
      }
    }
    case Node.NewSource(src) => this.source = src
  }
}
