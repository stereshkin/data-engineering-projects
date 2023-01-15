#include "JoinQuery.hpp"
#include <gtest/gtest.h>
#include "Util.hpp"

//---------------------------------------------------------------------------
TEST(JoinQuery, LineCount)
/// test if line counting works
{
   auto dataDir = getDir(__FILE__) + "/data/tpch/sf0_001/";
   JoinQuery query(dataDir + "lineitem.tbl", dataDir + "orders.tbl",
                   dataDir + "customer.tbl");
   auto result =
       query.lineCount(getDir(__FILE__) + "/data/tpch/sf0_001/lineitem.tbl");
   ASSERT_EQ(result, size_t(6005));
}
//---------------------------------------------------------------------------
TEST(JoinQuery, SmallInputMACHINERY)
/// test if the correct average if returned for test data
{
auto dataDir = getDir(__FILE__) + "/data/tpch/sf0_001/";
JoinQuery query(dataDir + "lineitem.tbl", dataDir + "orders.tbl",
                dataDir + "customer.tbl");
auto result = query.avg("MACHINERY");
ASSERT_EQ(result, size_t(2539));
}
//---------------------------------------------------------------------------
TEST(JoinQuery, SmallInputFURNITURE)
/// test if the correct average if returned for test data
{
auto dataDir = getDir(__FILE__) + "/data/tpch/sf0_001/";
JoinQuery query(dataDir + "lineitem.tbl", dataDir + "orders.tbl",
                dataDir + "customer.tbl");
auto result = query.avg("FURNITURE");
ASSERT_EQ(result, size_t(2577));
}
//---------------------------------------------------------------------------
TEST(JoinQuery, SmallInputBUILDING)
/// test if the correct average if returned for test data
{
auto dataDir = getDir(__FILE__) + "/data/tpch/sf0_001/";
JoinQuery query(dataDir + "lineitem.tbl", dataDir + "orders.tbl",
                dataDir + "customer.tbl");
auto result = query.avg("BUILDING");
ASSERT_EQ(result, size_t(2497));
}
//---------------------------------------------------------------------------
TEST(JoinQuery, SmallInputAUTOMOBILE)
/// test if the correct average if returned for test data
{
auto dataDir = getDir(__FILE__) + "/data/tpch/sf0_001/";
JoinQuery query(dataDir + "lineitem.tbl", dataDir + "orders.tbl",
                dataDir + "customer.tbl");
auto result = query.avg("AUTOMOBILE");
ASSERT_EQ(result, size_t(2578));
}
//---------------------------------------------------------------------------
TEST(JoinQuery, SmallInputHOUSEHOLD)
/// test if the correct average if returned for test data
{
auto dataDir = getDir(__FILE__) + "/data/tpch/sf0_001/";
JoinQuery query(dataDir + "lineitem.tbl", dataDir + "orders.tbl",
                dataDir + "customer.tbl");
auto result = query.avg("HOUSEHOLD");
ASSERT_EQ(result, size_t(2486));
}
//---------------------------------------------------------------------------
TEST(JoinQuery, SmallInputMultipleSegments)
/// test if we handle the file correctly and can call multiple segments after each other
{
auto dataDir = getDir(__FILE__) + "/data/tpch/sf0_001/";
JoinQuery query(dataDir + "lineitem.tbl", dataDir + "orders.tbl",
                dataDir + "customer.tbl");
auto result = query.avg("AUTOMOBILE");
ASSERT_EQ(result, size_t(2578));
result = query.avg("HOUSEHOLD");
ASSERT_EQ(result, size_t(2486));
result = query.avg("BUILDING");
ASSERT_EQ(result, size_t(2497));
}
//---------------------------------------------------------------------------