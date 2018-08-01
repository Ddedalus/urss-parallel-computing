package hubert.akka
import akka.actor.ActorRef
import scala.collection.mutable.Map

class SourceStatus(children: Array[ActorRef]) {
  var readyCount = 0
  var sum = 0.0
  var announced = 0.0
  var wasFinished = false
  var status = Map(children.map(ref => ref -> false): _*)

  def getCorrection: Double = {
    val ret = sum - announced
    announced = sum
    return ret
  }

  def shouldSendCorrection: Boolean = {
    sum - announced != 0
  }

} 

trait AnswerCollecting{
  var statusses: Map[ActorRef, SourceStatus]
  val children: Array[ActorRef]

  def removeSource(src: ActorRef) {
    statusses -= src
  }

  def addSource(src: ActorRef) {
    var ss = new SourceStatus(children)
    statusses.put(src, ss)
  }

  def getSum(src: ActorRef): Double = statusses(src).sum

  def onAllFinished(src: ActorRef) {
    statusses(src).wasFinished = true
  }

  def onFinishRevoked(src: ActorRef) {
    statusses(src).wasFinished = true
  }

  def correctBy(diff: Double, src: ActorRef, sender: ActorRef) {
    var ss = statusses(src)
    ss.sum += diff
    if (ss.status(sender)) return;
    ss.readyCount += 1
    ss.status(sender) = true
    if (ss.readyCount == ss.status.size)
      onAllFinished(src)
  }

  def notFinished(src: ActorRef, sender: ActorRef) {
    var ss = statusses(src)
    if (ss.status(sender)) return;
    ss.status(sender) = false
    ss.readyCount -= 1
    if(ss.wasFinished)
      onFinishRevoked(src)
  }

  def getTotal()
}
