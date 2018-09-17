package hubert.akka
import akka.actor.ActorRef

trait Gathering {
  final case class GatherResults(source : Int)
  var gatherCounters =Map[Int, Int]()

  def setGather(me: ActorRef, source : Int) {
    if(! gatherCounters.contains(source))
      gatherCounters += source -> 0
    gatherCounters += source -> (gatherCounters(source) + 1)
    me ! GatherResults(source)
  }

  def onGather(source : Int, onLastGather: (Int) => Unit) {
    gatherCounters += source  -> (gatherCounters(source) -1)
    if (gatherCounters(source) == 0) {
      onLastGather(source)
    }
  }
}