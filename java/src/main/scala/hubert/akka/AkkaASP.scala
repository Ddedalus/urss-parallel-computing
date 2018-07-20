package hubert.akka

import java.io.IOException

import akka.actor.{Actor, ActorLogging, ActorRef, ActorSystem, Props}

object AkkaASP extends App {
  import hubert.akka.Master._

  val filename = "/home/hubert/Code/Warwick/BSP/data/newcastle/n2.edges"
  val system: ActorSystem = ActorSystem("asp")

  val master = system.actorOf(Master.props(filename), "master")
  println(master)
  master ! BuildGraph
  master ! RequestASP(2)
  println("Press any key to continue...")
  System.in.read()
}
