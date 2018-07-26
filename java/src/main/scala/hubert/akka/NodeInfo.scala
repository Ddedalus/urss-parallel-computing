package hubert.akka

import akka.actor.ActorRef

class NodeInfo(var id: Int) {
  var finished: Boolean = false
  var distance: Double = _
}

trait AnswerCollecting {
  var children: Map[ActorRef, NodeInfo]
  var source: ActorRef = ActorRef.noSender
  var finishedCount = 0
  def cleanInfo() {
    children.foreach {
      case (ref, info) =>
        info.finished = false
        info.distance = Double.MaxValue
    }
    finishedCount = 0
  }
  def updateEstimate(dist: Double, sender: ActorRef) {
    val nodeInfo: NodeInfo = children(sender)
    nodeInfo.distance = dist
    if (!nodeInfo.finished) finishedCount += 1
    nodeInfo.finished = true
  }

  def notFinished(sender: ActorRef) {
    val nodeInfo: NodeInfo = children(sender)
    if (!nodeInfo.finished) return;
    nodeInfo.finished = false
    finishedCount -= 1
  }

  def sumResults(): Double = {
    if (this.finishedCount == children.size)
      children.foldLeft(0.0)(_ + _._2.distance)
    else
      return -1;
  }
}
