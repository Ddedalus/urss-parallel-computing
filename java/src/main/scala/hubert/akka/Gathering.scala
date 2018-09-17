package hubert.akka
import akka.actor.ActorRef

trait Gathering {
  final case class GatherResults(source : ActorRef)
  var gatherCounters =Map[ActorRef, Int]()

  def setGather(me: ActorRef, source : ActorRef) {
    if(! gatherCounters.contains(source))
      gatherCounters += source -> 0
    gatherCounters += source -> (gatherCounters(source) + 1)
    me ! GatherResults(source)
  }

  def onGather(source : ActorRef, onLastGather: (ActorRef) => Unit) {
    gatherCounters += source  -> (gatherCounters(source) -1)
    if (gatherCounters(source) == 0) {
      onLastGather(source)
    }
  }
}