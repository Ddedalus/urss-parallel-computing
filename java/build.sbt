name := "akka-asp"

scalacOptions += "-deprecation"
version := "1.0"

scalaVersion := "2.12.6"

lazy val akkaVersion = "2.5.14"

// using ~compile will clean the screen first and display only top 5 errors
maxErrors := 5
triggeredMessage := Watched.clearWhenTriggered

cancelable in Global := true

compileOrder := CompileOrder.JavaThenScala

libraryDependencies ++= Seq(
  "com.typesafe.akka" %% "akka-actor" % akkaVersion,
  "ch.qos.logback" % "logback-classic" % "1.2.3"
)
libraryDependencies ++= Seq(
 "io.kamon" %% "kamon-core" % "1.0.1",
 "io.kamon" %% "kamon-akka-2.5" % "1.0.1",
 "io.kamon" %% "kamon-prometheus" % "1.0.0")

fork in run := true