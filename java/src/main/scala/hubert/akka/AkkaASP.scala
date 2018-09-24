package hubert.akka

import akka.actor.{Actor, ActorLogging, ActorRef, ActorSystem, Props}
import scala.concurrent.duration._

import java.io.File
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
  var graph_path = "."

  val usage
    : String = ("Usage:\n \nAs a first argument, please specify path to a graph parent directory\n \n" +
    "The following switches and defaults are available:\n" +
    "  switch                 default\t  description\n" +
    "--cores               -c\t  \t\tNumber of cores available to the program. *compulsory*\n" +
    "--active-max-size     -a\t%s\t\tMaximal number of parallel searches to be performed\n" +
    "--source-inactivity   -i\t%s\thow long source should stay in idleQueue\n" +
    "--system-timeout      -s\t%s\tafter this time, ActorSystem will be terminated\n" +
    "--propagation-timeout -p\t%s\tFuture timeout for notifying Supervisors about new source\n" +
    "--queue-check-period  -s\t%s\thow often to check idleQueue\n" +
    "--delay-between-init  -d\t%s\tTo begin, several sources are spawned at once. This option introduces a delay between them.\n" +
    "--log-path            -l\t%s\tFile to append with result, option and timing data\n" +
    "--help                -h\t    \t Display this message and exit\n Exiting")
    .format(
      DefaultParams('ActiveMaxSize),
      DefaultParams('SourceInactivity),
      DefaultParams('SystemTimeout),
      DefaultParams('PropagationTimeout),
      DefaultParams('QueueCheckPeriod),
      DefaultParams('DelayBetweenInitSources),
      DefaultParams('LogPath)
    )

  
  type OptionMap = Map[Symbol, Any]
  def argParser(map: OptionMap, list: List[String]): OptionMap = {
    def isSwitch(s: String): Boolean = (s(0) == '-')
    list match {
      case Nil => map
      case ("--cores" | "-c") :: value :: tail =>
        argParser(map ++ Map('Cores -> value.toInt), tail)
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
        argParser(map ++ Map('ActiveMaxSize -> value.toInt), tail)
      case ("--help" | "-h") :: tail => print(usage)
                                        sys.exit(0)
      case string :: opt2 :: tail if isSwitch(opt2) =>
        graph_path = string
        argParser(map, list.tail)
      case string :: Nil =>
        graph_path = string
        argParser(map, list.tail)
      case option :: tail =>
        println("Unknown option " + option)
        sys.exit(1)
    }
  }

  def listGraphFiles(dir: String):List[File] = {
    val d = new File(dir)
    if (d.exists && d.isDirectory) {
        d.listFiles.filter(_.isFile).filter(_.getName.endsWith(".edges")).sorted.toList
    } else {
        List[File]()
    }
  }

  if (args.length == 0) {println(usage); sys.exit(0)}
  val options = argParser(DefaultParams, args.toList)
  if(! options.contains('Cores)) {println("Must specify number of cores available: -c <int>"); sys.exit(1)}

  println("Current settings:")
  options.foreach(println)
  val graph_files = listGraphFiles(graph_path)

  // Kamon.addReporter(new PrometheusReporter())
  // Kamon.addReporter(new ZipkinReporter())

  val iter = graph_files.iterator
  def runActorSystem(iter : Iterator[File]){
    var f = iter.next
    print("Info: starting execution on graph file: " + f.getPath)
    var system: ActorSystem = ActorSystem("asp")
    // this fires the execution. Master appends the log on successful run
    var master = system.actorOf(Master.props(options ++ Map('GraphPath -> f.getPath)), "master")

    system.registerOnTermination {
      if(iter.hasNext)
        runActorSystem(iter)
    }
  }
  if(! iter.hasNext)
    print("Warning: No graph files found in " + graph_path)
  else{
    runActorSystem(iter)
  }

}
