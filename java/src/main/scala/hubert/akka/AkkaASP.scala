package hubert.akka

import akka.actor.{Actor, ActorLogging, ActorRef, ActorSystem, Props}

// import org.slf4j.Logger
// import org.slf4j.LoggerFactory

import kamon.Kamon
import kamon.prometheus.PrometheusReporter

case object PingMessage
case object PongMessage
case object StartMessage
case object StopMessage

class Ping(pong: ActorRef) extends Actor {
  var count = 0
  def incrementAndPrint { count += 1}
  def receive = {
    case StartMessage =>
        incrementAndPrint
        pong ! PingMessage
    case PongMessage => 
        incrementAndPrint
        if (count > 100000000) {
          sender ! StopMessage
          println("ping stopped")
          context.stop(self)
        } else {
          sender ! PingMessage
        }
  }
}

class Pong extends Actor {
  def receive = {
    case PingMessage =>
        // println("  pong")
        sender ! PongMessage
    case StopMessage =>
        println("pong stopped")
        context.stop(self)
  }
}


object AkkaASP extends App {
  import hubert.akka.Master._

  Kamon.addReporter(new PrometheusReporter())
  // println(System.getenv());
  // Kamon.addReporter(new ZipkinReporter())

  val system = ActorSystem("PingPongSystem")
  val pong = system.actorOf(Props[Pong], name = "pong")
  println(pong + " - actor pong")
  val ping = system.actorOf(Props(new Ping(pong)), name = "ping")
  // start them going
  ping ! StartMessage

  Thread.sleep(100000)
  system.terminate

  // val filename = "/home/hubert/Code/Warwick/BSP/data/newcastle/n5.edges"
  // val system: ActorSystem = ActorSystem("asp")

  // val master = system.actorOf(Master.props(filename), "master")
  // println(master)
  // master ! BuildGraph
  // master ! RequestASP(2)
  // System.in.read();
  // Kamon.stop()
}
