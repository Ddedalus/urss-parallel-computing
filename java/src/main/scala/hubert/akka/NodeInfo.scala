package hubert.akka

import akka.actor.ActorRef

class NodeInfo(var id: Int) {
  var finished: Boolean = false
  var distance: Double = _
}