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
    'PropagationTimeout -> 1000.milli,
    'QueueCheckPeriod -> 50.millis,
    'DelayBetweenInitSources -> 20.millis,
    'LogPath -> "./sample_aspLog.log"
  )

  val usage
    : String = ("As a first argument, please specify path to a graph file\n\n" +
    "The following switches and defaults are available:\n" +
    "  switch                 default\t  description\n" +
    "--active-max-size     -a\t%s\t\tMaximal number of parallel searches to be performed\n" +
    "--source-inactivity   -i\t%s\thow long source should stay in idleQueue\n" +
    "--system-timeout      -s\t%s\tafter this time, ActorSystem will be terminated\n" +
    "--propagation-timeout -p\t%s\tFuture timeout for notifying Supervisors about new source\n" +
    "--queue-check-period  -s\t%s\thow often to check idleQueue\n" +
    "--delay-between-init  -d\t%s\tTo begin, several sources are spawned at once. This option introduces a delay between them.\n" +
    "--log-path            -l\t%s\tFile to append with result, option and timing data\n" +
    "--help                -h\t    \t Display this message and exit")
    .format(
      DefaultParams('ActiveMaxSize),
      DefaultParams('SourceInactivity),
      DefaultParams('SystemTimeout),
      DefaultParams('PropagationTimeout),
      DefaultParams('QueueCheckPeriod),
      DefaultParams('DelayBetweenInitSources),
      DefaultParams('LogPath)
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
      case ("--log-path" | "-l") :: value :: tail =>
        argParser(map ++ Map('ActiveMaxSize -> value.toString), tail)
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

  if(! options.contains('GraphPath)){
    print("Please specify a path to graph file!")
    sys.exit(2)
  }

  // Kamon.addReporter(new PrometheusReporter())
  // Kamon.addReporter(new ZipkinReporter())

  // val filename = "/home/hubert/Code/Warwick/BSP/data/newcastle/n3.edges"
  val system: ActorSystem = ActorSystem("asp")
  val master = system.actorOf(Master.props(options), "master")
  // val reply = i.receive()
  // val transformedReply = i.select(DefaultParams.SystemTimeout) {
  //   case Master.Answer(tot) => print("Received answer: %d".format(tot))
  // }

}
