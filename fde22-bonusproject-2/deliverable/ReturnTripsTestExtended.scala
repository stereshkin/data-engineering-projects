import org.apache.spark.sql.SparkSession
import org.apache.spark.SparkContext
import org.apache.spark.SparkContext._
import org.apache.spark.SparkConf
import org.apache.spark.sql.types._
import org.apache.spark.sql.functions._
import org.apache.spark.sql.Column
import org.apache.spark.sql.Dataset
import org.apache.spark.sql.Row

// (c) 2021 Thomas Neumann, Timo Kersten, Alexander Beischl, Maximilian Reif

/**
 * Usage: TaxiTest [taxidataset]
 */
object ReturnTripTestExtended {

  def time[T](proc: => T): T = {
    val start=System.nanoTime()
    val res = proc // call the code
    val end = System.nanoTime()
    println("Time elapsed: " + (end-start)/1000000 + " ms")
    res
  }

  def main(args: Array[String]) {
    val spark = SparkSession
      .builder
      .appName("ReturnTripFinder Test")
      .getOrCreate()
    import spark.implicits._

    val taxifile = args(0)
    val schema = StructType(Array(
        StructField("VendorID", DataTypes.StringType,false),
        StructField("tpep_pickup_datetime", DataTypes.TimestampType,false),
        StructField("tpep_dropoff_datetime", DataTypes.TimestampType,false),
        StructField("passenger_count", DataTypes.IntegerType,false),
        StructField("trip_distance", DataTypes.DoubleType,false),
        StructField("pickup_longitude", DataTypes.DoubleType,false),
        StructField("pickup_latitude", DataTypes.DoubleType,false),
        StructField("RatecodeID", DataTypes.IntegerType,false),
        StructField("store_and_fwd_flag", DataTypes.StringType,false),
        StructField("dropoff_longitude", DataTypes.DoubleType,false),
        StructField("dropoff_latitude", DataTypes.DoubleType,false),
        StructField("payment_type", DataTypes.IntegerType,false),
        StructField("fare_amount", DataTypes.DoubleType,false),
        StructField("extra", DataTypes.DoubleType,false),
        StructField("mta_tax", DataTypes.DoubleType,false),
        StructField("tip_amount", DataTypes.DoubleType,false),
        StructField("tolls_amount", DataTypes.DoubleType,false),
        StructField("improvement_surcharge", DataTypes.DoubleType,false),
        StructField("total_amount", DataTypes.DoubleType, false)
    ))

    val tripsDF = spark.read.schema(schema).option("header", true).csv(taxifile)
    val trips = tripsDF.where($"pickup_longitude" =!= 0 && $"pickup_latitude" =!= 0 && $"dropoff_longitude" =!= 0 && $"dropoff_latitude" =!= 0).cache()

    var dist = 20 // get dist in meters from driver
    var result = time{ReturnTrips.compute(trips, dist, spark).agg(count("*")).first.getLong(0)}
    println("Run first test: dist = 20")
    println("Your Result: ")
    println(result)
    if(result != 261634){
      println("Wrong result for dist = 20. Correct result is 261634.")
      System.exit(1);
    } else {
      println("Correct result.")
    }

    dist = 90 // get dist in meters from driver
    result = time{ReturnTrips.compute(trips, dist, spark).agg(count("*")).first.getLong(0)}
    println("Run first test: dist = 90")
    println("Your Result: ")
    println(result)
    if(result != 4851810){ // 4851810
      println("Wrong result for dist = 90. Correct result is 4851810.")
      System.exit(1);
    } else {
      println("Correct result.")
    }

    dist = 150 // get dist in meters from driver
    result = time{ReturnTrips.compute(trips, dist, spark).agg(count("*")).first.getLong(0)}
    println("Run first test: dist = 150")
    println("Result: ")
    println(result)
    if(result != 21154948){
      println("Wrong result for dist = 150. Correct result is 21154948.")
      System.exit(1);
    } else {
      println("Correct result.")
    }

    spark.stop()
  }
}