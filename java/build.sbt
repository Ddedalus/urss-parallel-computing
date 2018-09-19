name := "akka-asp"
version := "1.1"

scalaVersion := "2.12.6"
lazy val akkaVersion = "2.5.14"

triggeredMessage := Watched.clearWhenTriggered
cancelable in Global := true
compileOrder := CompileOrder.JavaThenScala
maxErrors := 5
fork in run := true

// using ~compile will clean the screen first and display only top 5 errors

libraryDependencies ++= Seq(
  "com.typesafe.akka" %% "akka-actor" % akkaVersion,
  "com.typesafe.akka" %% "akka-slf4j" % "2.5.16",
  "org.slf4j" % "slf4j-api" % "1.7.25",
  "ch.qos.logback" % "logback-classic" % "1.2.3"
)
//libraryDependencies ++= Seq(
// "io.kamon" %% "kamon-core" % "1.0.1",
// "io.kamon" %% "kamon-akka-2.5" % "1.0.1",
// "io.kamon" %% "kamon-prometheus" % "1.0.0")
