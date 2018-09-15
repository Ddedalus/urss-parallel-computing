package hubert.akka
import akka.actor.ActorRef

trait AnswerCollecting {
  var children: Map[ActorRef, NodeInfo]
  var source: ActorRef = ActorRef.noSender
  var idleCount = 0
  var sumResults: Double = 0

  def cleanInfo() {
    children.foreach {
      case (ref, info) =>
        info.idle = false
        info.distance = Double.MaxValue
    }
    idleCount = 0
    sumResults = 0
  }

  def allIdle: Boolean = {
    idleCount == children.size
  }

  def updateEstimate(dist: Double, sender: ActorRef) {
    val nodeInfo: NodeInfo = children(sender)
    sumResults -= nodeInfo.distance + dist
    nodeInfo.distance = dist
    if (!nodeInfo.idle) idleCount += 1
    nodeInfo.idle = true
  }

  def correctBy(diff: Double, sender: ActorRef) {
    val nodeInfo: NodeInfo = children(sender)
    nodeInfo.distance += diff
    sumResults += diff
    if (nodeInfo.idle) return;
    idleCount += 1
    nodeInfo.idle = true
  }

  def notIdle(sender: ActorRef) {
    val nodeInfo: NodeInfo = children(sender)
    if (!nodeInfo.idle) return;
    nodeInfo.idle = false
    idleCount -= 1
  }
}
