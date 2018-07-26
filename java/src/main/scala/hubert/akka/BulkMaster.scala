package hubert.akka

import akka.actor.{
  Actor,
  ActorLogging,
  ActorRef,
  ActorSystem,
  Props,
  PoisonPill
}
import akka.pattern.ask
import scala.concurrent.duration.{_}
import scala.concurrent._
import ExecutionContext.Implicits.global
import akka.util.Timeout
import scala.util.{Success, Failure}
import scala.concurrent.Future

object BulkMaster {
  def props(filename: String): Props = Props(new BulkMaster(filename))

  case object RequestBulk
}
class BulkMaster(filename: String) extends Master(filename) {
  import BulkMaster._
  import hubert.akka.Supervisor._

  var idIter: Iterator[ActorRef] = _;
  var grandTotal: Double = _;
  var gatherCounter: Int = 0;

  def proclaimNewSource() {
    if (idIter.hasNext) {
      this.source = idIter.next
      implicit val timeout = Timeout(5 seconds)
      val futures = children.keys.map { _ ? Node.NewSource(source) }
      Future.sequence(futures).onComplete {
        case Success(list) => source ! Node.NewSource(this.source)
        case Failure(e)    => log.warning("Propagating new source timed out")
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
      log.info("Gather counter {}", gatherCounter)
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
  }
}
