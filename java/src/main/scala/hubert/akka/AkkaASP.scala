package hubert.akka

import akka.actor.{Actor, ActorLogging, ActorRef, ActorSystem, Props}

import kamon.Kamon
import kamon.prometheus.PrometheusReporter
import kamon.zipkin.ZipkinReporter

object AkkaASP extends App {
  import hubert.akka.Master._

  Kamon.addReporter(new PrometheusReporter())
  Kamon.addReporter(new ZipkinReporter())

  val filename = "/home/hubert/Code/Warwick/BSP/data/newcastle/n5.edges"
  val system: ActorSystem = ActorSystem("asp")

  val master = system.actorOf(Master.props(filename), "master")
  println(master)
  master ! BuildGraph
  master ! RequestASP(2)
  System.in.read();
}
