name := "Return Trip Test"

version := "1.0"

scalaVersion := "2.12.2"

libraryDependencies ++= Seq(
  "org.apache.spark" %% "spark-core" % "3.0.3" % "provided",
  "org.apache.spark" %% "spark-sql" % "3.0.3" % "provided"
)
