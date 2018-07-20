name := "asp-scala"

version := "1.0"

scalaVersion := "2.12.6"

lazy val akkaVersion = "2.5.13"

compileOrder := CompileOrder.JavaThenScala

libraryDependencies ++= Seq(
  "com.typesafe.akka" %% "akka-actor" % akkaVersion,
  "com.typesafe.akka" %% "akka-testkit" % akkaVersion,
  "org.scalatest" %% "scalatest" % "3.0.5" % "test"
)