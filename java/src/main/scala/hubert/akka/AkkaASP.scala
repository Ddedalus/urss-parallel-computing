package hubert.akka

import akka.actor.{Actor, ActorLogging, ActorRef, ActorSystem, Props}

// import kamon.Kamon
// import kamon.prometheus.PrometheusReporter

object AkkaASP extends App {
  import hubert.akka.Master._
  import hubert.akka.BulkMaster._

  // Kamon.addReporter(new PrometheusReporter())
  // println(System.getenv());
  // Kamon.addReporter(new ZipkinReporter())

  val filename = "/home/hubert/Code/Warwick/BSP/data/sample/sample.graph.txt"
  val system: ActorSystem = ActorSystem("asp")

  val master = system.actorOf(BulkMaster.props(filename), "master")
  println(master)
  master ! RequestBulk
  System.in.read();
}
