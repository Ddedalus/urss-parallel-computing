package hubert.akka

import akka.actor.{Actor, ActorLogging, ActorRef, ActorSystem, Props}

// import kamon.Kamon
// import kamon.prometheus.PrometheusReporter

object AkkaASP extends App {
  import hubert.akka.Master._

  // Kamon.addReporter(new PrometheusReporter())
  // println(System.getenv());
  // Kamon.addReporter(new ZipkinReporter())

  val filename = "/home/hubert/Code/Warwick/BSP/data/newcastle/n5.edges"
  val system: ActorSystem = ActorSystem("asp")

  val master = system.actorOf(Master.props(filename), "master")
  println(master)
  master ! RequestASP(2)
  System.in.read();
}
