package hubert.akka

import akka.actor.{Actor, ActorRef}

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
  import scala.concurrent.duration._
  val QueueCheckingPeriod = 50.millis
  val SourceInactivityRequired: Millis = 300
  val DelayBetweenInitSources = 20.millis
  val PropagationTimeout = 1.second
  val ActiveMaxSize = 10
}

trait SupervisionStrategy extends Actor{
  import scala.concurrent.duration._
  import akka.actor.AllForOneStrategy
  import akka.actor.SupervisorStrategy._
  override val supervisorStrategy =
    AllForOneStrategy(maxNrOfRetries = 0, withinTimeRange = 1.milli) {
      case e: Exception => Stop
    }
}