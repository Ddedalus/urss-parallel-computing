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
  var newSource : ActorRef = _
  var grandTotal: Double = _;
  var gatherCounter: Int = 0;

  def proclaimNewSource() {
    if (idIter.hasNext) {
      this.newSource = idIter.next
      implicit val timeout = Timeout(5 seconds)
      val futures = children.keys.map { _ ? Node.NewSource(newSource) }
      Future.sequence(futures).onComplete {
        case Success(list) => {
          log.info("New source {}", newSource.path.name)
          self ! Node.NewSource(this.newSource)
          this.newSource ! Node.NewSource(this.newSource)
          
        }
        case Failure(e)    => {
          log.warning("Propagating new source timed out")
          self ! PoisonPill
        }
      }
    } else {
      log.warning("Grand total: {}", grandTotal)
      self ! PoisonPill
    }
  }

  override def onUpdateEstimate(dist: Double, source: ActorRef): Unit = {
    super.onUpdateEstimate(dist, source)
    if (finishedCount == children.size) {
      gatherCounter += 1
    }
  }

  override def onGatherResults(): Double = {
    if (gatherCounter > 1) {
      // log.info("Gather counter {}", gatherCounter)
      gatherCounter -= 1
      return -1
    } else {
      val sum = sumResults;
      if (sum > 0) {
        grandTotal += sum
        cleanInfo()
        proclaimNewSource
      }
      gatherCounter = 0
      return sum;
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
