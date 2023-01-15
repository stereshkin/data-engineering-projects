import org.apache.spark.sql.SparkSession
import org.apache.spark.SparkContext
import org.apache.spark.SparkContext._
import org.apache.spark.SparkConf
import org.apache.spark.sql.types._
import org.apache.spark.sql.functions._
import org.apache.spark.sql.Column
import org.apache.spark.sql.Dataset
import org.apache.spark.sql.Row
//import org.apache.spark.sql.execution.command.AnalyzeColumnCommand
import scala.math

// (c) 2021 Thomas Neumann, Timo Kersten, Alexander Beischl, Maximilian Reif

object ReturnTrips {
  val earthRadius = 6371000; 

  def compute(trips : Dataset[Row], dist : Double, spark : SparkSession) : Dataset[Row] = {
    import spark.implicits._


    spark.conf.set("spark.sql.shuffle.partitions", 3000)
    spark.conf.set("spark.databricks.io.cache.enabled", true)




    def makeDistanceExpr(lon1: Column, lat1: Column, lon2: Column, lat2: Column): Column = {
    val expr = pow(sin(abs(lat1 - lat2) / 2), 2) + cos(lat1) * cos(lat2) * pow(sin(abs(lon1 - lon2) / 2), 2)
    asin(sqrt(expr)) * lit(2 * earthRadius)
  }
    
    val tableA = trips
      .withColumn("pickup_dt", unix_timestamp($"tpep_pickup_datetime"))
      .withColumn("dropoff_dt", unix_timestamp($"tpep_dropoff_datetime"))
      .withColumn("pickup_lon", toRadians("pickup_longitude"))
      .withColumn("pickup_lat", toRadians("pickup_latitude"))
      .withColumn("dropoff_lon", toRadians("dropoff_longitude"))
      .withColumn("dropoff_lat", toRadians("dropoff_latitude")).select(
        "pickup_dt",
        "dropoff_dt",
        "pickup_lon",
        "pickup_lat",
        "dropoff_lon",
        "dropoff_lat"
    ).withColumn("pickup_time_bucket", floor($"pickup_dt" / 28800))
      .withColumn("dropoff_time_bucket", floor($"dropoff_dt" / 28800))
      .withColumn("pickup_lat_bucket", floor($"pickup_lat" / (1.5 * dist / earthRadius)))
      .withColumn("dropoff_lat_bucket", floor($"dropoff_lat" / (1.5 * dist / earthRadius)))


    val tableANeighbors = tableA
      .withColumn("pickup_lat_bucket", explode(array($"pickup_lat_bucket" - 1, $"pickup_lat_bucket", $"pickup_lat_bucket" + 1)))
      .withColumn("dropoff_lat_bucket", explode(array($"dropoff_lat_bucket" - 1, $"dropoff_lat_bucket", $"dropoff_lat_bucket" + 1)))
      .withColumn("pickup_time_bucket", explode(array($"pickup_time_bucket" - 1, $"pickup_time_bucket")))



    val result = tableANeighbors.as("b")
      .join(tableA.as("a"),
        ($"a.dropoff_time_bucket" === $"b.pickup_time_bucket") &&
        ($"a.pickup_lat_bucket" === $"b.dropoff_lat_bucket") &&
        ($"b.pickup_lat_bucket" === $"a.dropoff_lat_bucket") &&
        ($"a.dropoff_dt" < $"b.pickup_dt") &&
        ($"b.pickup_dt" - $"a.dropoff_dt" < 28800) &&
        (makeDistanceExpr($"a.pickup_lon", $"a.pickup_lat", $"b.dropoff_lon", $"b.dropoff_lat") < dist) &&
        (makeDistanceExpr($"a.dropoff_lon", $"a.dropoff_lat", $"b.pickup_lon", $"b.pickup_lat") < dist)
      )
    result
  }

}
