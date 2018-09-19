package hubert.akka

import akka.actor.{Actor, ActorLogging, ActorRef, ActorSystem, Props}
import scala.concurrent.duration._

// import kamon.Kamon
// import kamon.prometheus.PrometheusReporter

object AkkaASP extends App {
  import hubert.akka.Master._

  val DefaultParams: Map[Symbol, Any] = Map(
    'ActiveMaxSize -> 100,
    'SourceInactivity -> 300.millis,
    'SystemTimeout -> 30.second,
    'PropagationTimeout -> 1.second,
    'QueueCheckPeriod -> 50.millis,
    'DelayBetweenInitSources -> 20.millis
  )

  val usage
    : String = ("As a first argument, please specify path to a graph file\n\n" +
    "The following switches and defaults are available:\n" +
    "  switch            \t default \t description\n" +
    "--active-max-size\t-a \t %s \t Maximal number of parallel searches to be performed\n" +
    "--source-inactivity\t-i \t %s \t how long source should stay in idleQueue\n" +
    "--system-timeout\t-s \t %s \t after this time, ActorSystem will be terminated\n" +
    "--propagation-timeout\t-p \t %s \t Future timeout for notifying Supervisors about new source\n" +
    "--queue-check-period\t-s \t %s \t how often to check idleQueue\n" +
    "--delay-between-init\t-d \t %s \t To begin, several sources are spawned at once. This option introduces a delay between them.\n")
    .format(
      DefaultParams('ActiveMaxSize).toString,
      DefaultParams('SourceInactivity),
      DefaultParams('SystemTimeout).toString,
      DefaultParams('PropagationTimeout).toString,
      DefaultParams('QueueCheckPeriod).toString,
      DefaultParams('DelayBetweenInitSources).toString
    )

  if (args.length == 0) println(usage)
  val arglist = args.toList
  type OptionMap = Map[Symbol, Any]

  def argParser(map: OptionMap, list: List[String]): OptionMap = {
    def isSwitch(s: String): Boolean = (s(0) == '-')
    list match {
      case Nil => map
      case ("--system-timeout" | "-s") :: value :: tail =>
        argParser(map ++ Map('SystemTimeout -> value.toInt.seconds), tail)
      case ("--queue-check-period" | "-q") :: value :: tail =>
        argParser(map ++ Map('QueueCheckPeriod -> value.toInt.millis), tail)
      case ("--delay-between-init" | "-d") :: value :: tail =>
        argParser(map ++ Map('DelayBetweenInitSources -> value.toInt.millis),
                   tail)
      case ("--propagation-timeout" | "-p") :: value :: tail =>
        argParser(map ++ Map('PropagationTimeout -> value.toInt.millis), tail)
      case ("--active-max-size" | "-a") :: value :: tail =>
        argParser(map ++ Map('ActiveMaxSize -> value.toInt), tail)
      case ("--help" | "-h") :: tail => print(usage)
                                        sys.exit(0)
      case string :: opt2 :: tail if isSwitch(opt2) =>
        argParser(map ++ Map('GraphPath -> string), list.tail)
      case string :: Nil =>
        argParser(map ++ Map('GraphPath -> string), list.tail)
      case option :: tail =>
        println("Unknown option " + option)
        sys.exit(1)
    }
  }

  val options = argParser(DefaultParams, arglist)
  println("Options:")
  options.foreach(println)

  // Kamon.addReporter(new PrometheusReporter())
  // Kamon.addReporter(new ZipkinReporter())

  // val filename = "/home/hubert/Code/Warwick/BSP/data/sample/sample.graph.txt"
// implicit val system: ActorSystem = ...
  // val filename = "/home/hubert/Code/Warwick/BSP/data/newcastle/n3.edges"
  // implicit val system: ActorSystem = ActorSystem("asp")
  // implicit val i = inbox()
  // val master = system.actorOf(Master.props(filename), "master")
  // val reply = i.receive()
  // val transformedReply = i.select(DefaultParams.SystemTimeout) {
  //   case Master.Answer(tot) => print("Received answer: %d".format(tot))
  // }

}
