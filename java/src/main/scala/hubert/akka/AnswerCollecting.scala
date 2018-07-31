package hubert.akka
import akka.actor.ActorRef

trait AnswerCollecting {
  var children: Map[ActorRef, NodeInfo]
  var source: ActorRef = ActorRef.noSender
  var finishedCount = 0
  var sumResults: Double = 0

  def cleanInfo() {
    children.foreach {
      case (ref, info) =>
        info.finished = false
        info.distance = Double.MaxValue
    }
    finishedCount = 0
    sumResults = 0
  }

  def allFinished: Boolean = {
    finishedCount == children.size
  }

  def updateEstimate(dist: Double, sender: ActorRef) {
    val nodeInfo: NodeInfo = children(sender)
    sumResults -= nodeInfo.distance + dist
    nodeInfo.distance = dist
    if (!nodeInfo.finished) finishedCount += 1
    nodeInfo.finished = true
  }

  def correctBy(diff: Double, sender: ActorRef) {
    val nodeInfo: NodeInfo = children(sender)
    nodeInfo.distance += diff
    sumResults += diff
    if (nodeInfo.finished) return;
    finishedCount += 1
    nodeInfo.finished = true
  }

  def notFinished(sender: ActorRef) {
    val nodeInfo: NodeInfo = children(sender)
    if (!nodeInfo.finished) return;
    nodeInfo.finished = false
    finishedCount -= 1
  }
}
