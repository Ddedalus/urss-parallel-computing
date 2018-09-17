package hubert.akka

import akka.actor.ActorRef

// TODO: remove the internal map here, just store total distance and idle number
// this requires confidence in Node logic 

class SourceStatus(children_ : Iterable[ActorRef]) {
  var idleCount = 0
  var announced = 0.0
  var steady = false
  var children : Map[ActorRef, NodeStatus] = children_.map(ref => ref -> new NodeStatus()).toMap
                    // node -> status

  def putDiff(nodeRef : ActorRef, diff : Double){
   steady = false
    val node = children(nodeRef)
    node.distance += diff
    if(!node.idle){
      idleCount += 1
      node.idle = true
    } 
  }

  def setNotIdle(nodeRef : ActorRef){
    steady = false
    val node = children(nodeRef)
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