package hubert.akka

import akka.actor.{Actor, ActorLogging, ActorRef, ActorSystem, Props}

// import kamon.Kamon
// import kamon.prometheus.PrometheusReporter

object AkkaASP extends App {
  import hubert.akka.Master._

  // Kamon.addReporter(new PrometheusReporter())
  // Kamon.addReporter(new ZipkinReporter())

  // val filename = "/home/hubert/Code/Warwick/BSP/data/sample/sample.graph.txt"
  val filename = "/home/hubert/Code/Warwick/BSP/data/newcastle/n3.edges"
  val system: ActorSystem = ActorSystem("asp")  
  val master = system.actorOf(Master.props(filename), "master")
  
}
