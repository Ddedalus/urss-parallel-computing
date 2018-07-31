package hubert.akka
import akka.actor.ActorRef

trait Gathering {
  case object GatherResults
  var gatherCounter: Int = 0;

  def setGather(me: ActorRef) {
    gatherCounter += 1
    me ! GatherResults
  }

  def onGather(onLastGather: () => Unit) {
    gatherCounter -= 1
    if (gatherCounter == 0) {
      onLastGather()
    }
  }
}