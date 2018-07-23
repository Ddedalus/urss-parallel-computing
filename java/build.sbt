name := "akka-asp"

version := "1.0"

scalaVersion := "2.12.6"

lazy val akkaVersion = "2.5.8"

cancelable in Global := true

compileOrder := CompileOrder.JavaThenScala

libraryDependencies ++= Seq(
  "com.typesafe.akka" %% "akka-actor" % akkaVersion,
)
libraryDependencies ++= Seq(
  "io.kamon" %% "kamon-core" % "1.0.1",
  "io.kamon" %% "kamon-akka-2.5" % "1.0.1",
  "io.kamon" %% "kamon-prometheus" % "1.0.0")

fork in run := true