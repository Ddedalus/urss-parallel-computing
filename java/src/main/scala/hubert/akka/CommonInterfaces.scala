package hubert.akka

import akka.actor.ActorRef

trait CommonInterfaces {
  type Node = ActorRef
  type Source = Int
}

object Messages extends CommonInterfaces{
  final case class NotIdle(source: Source)
  final case class CorrectBy(diff: Double, source: Source)
  
  final case class SourceRegistered(source : Source)
  final case class RemoveSource(source : Source)
  final case class NewSource(source : Source)
}