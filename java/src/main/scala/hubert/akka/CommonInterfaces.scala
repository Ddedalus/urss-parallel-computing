package hubert.akka

import akka.actor.ActorRef

trait CommonInterfaces {
  type Node = ActorRef
  type Source = Int
  type Millis = Long
}

object Messages extends CommonInterfaces{
  final case class NotIdle(source: Source)
  final case class CorrectBy(diff: Double, source: Source)
  
  final case class SourceRegistered(source : Source)
  final case class RemoveSource(source : Source)
  final case class NewSource(source : Source)
}

object Params extends CommonInterfaces{
  val QueueCheckingPeriod : Millis = 50
  val SourceInactivityRequired: Millis = 500
  val DelayBetweenInitSources: Millis = 20
  val ActiveMaxSize = 20
}

trait SupervisionStrategy extends Actor{
  import akka.actor.AllForOneStrategy
  import akka.actor.SupervisorStrategy._
  override val supervisorStrategy =
    AllForOneStrategy(maxNrOfRetries = 0, withinTimeRange = 1.milli) {
      case e: Exception => Stop
    }
}