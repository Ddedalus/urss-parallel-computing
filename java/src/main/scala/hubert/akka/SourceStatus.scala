package hubert.akka

import akka.actor.ActorRef

// TODO: define node and source type aliases

class SourceStatus(children_ : Iterable[ActorRef]) {
  var idleCount = 0
  var announced = 0.0
  var children : Map[ActorRef, NodeStatus] = children_.map(ref => ref -> new NodeStatus()).toMap
                    // source -> status

  def putDiff(src : ActorRef, diff : Double){
    val node = children(src)
    node.distance += diff
    if(!node.idle){
      idleCount += 1
      node.idle = true
    } 
  }

  def setNotIdle(src : ActorRef){
    val node = children(src)
    if(node.idle){
      node.idle = false
      idleCount -= 1
    }
  }

  def allIdle = idleCount == children.size

  def getCorrection: Double = {
    val diff = children.values.map(c => c.distance).sum - announced
    announced += diff
    return diff
  }

  def getSum: Double = {
    return children.values.map(c => c.distance).sum
  }
}

class NodeStatus{
  var idle: Boolean = false
  var distance: Double = _
}