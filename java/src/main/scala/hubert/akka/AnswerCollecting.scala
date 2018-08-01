package hubert.akka
import akka.actor.ActorRef
import scala.collection.mutable.Map

class SourceStatus(children: Array[ActorRef]) {
  var readyCount = 0
  var sum = 0.0
  var status = Map(children.map(ref => ref -> false): _*)
}

trait AnswerCollecting {
  var statueses: Map[ActorRef, SourceStatus]
  val children: Array[ActorRef]

  def removeSource(src: ActorRef) {
    statueses -= src
  }

  def addSource(src: ActorRef) {
    var ss = new SourceStatus(children)
    statueses.put(src, ss)
  }

  def getSum(src : ActorRef): Double = statueses(src).sum

  def onAllFinished(src: ActorRef)

  def correctBy(diff: Double, src: ActorRef, sender: ActorRef) {
    var ss = statueses(src)
    ss.sum += diff
    if (ss.status(sender)) return;
    ss.readyCount += 1
    ss.status(sender) = true
    if(ss.readyCount == ss.status.size)
      onAllFinished(src)
  }

  def notFinished(src: ActorRef, sender: ActorRef) {
    var ss = statueses(src)
    if (ss.status(sender)) return;
    ss.status(sender) = false
    ss.readyCount -= 1
  }

  def getTotal()
}
