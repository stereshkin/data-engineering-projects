#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <map>

//---------------------------------------------------------------------------
class JoinQuery
/// Class which provides answers to queries on lineitem orders and customer
{
   public:
   /**************************************************************************
    *  The constructor receives paths to data from the TPC-H benchmark.
    *  You can explore an example dataset from this benchmark on
    *  http://www.hyper-db.de/interface.html
    *  Furthermore, another example dataset is provided in
    *  test/data/tpch/sf0_001
    *
    *  Parameters to constructor:
    *     lineitem: path to a lineitem.tbl
    *     orders: path to a orders.tbl
    *     customer: path to a customer.tbl
    ************************************************************************/
   JoinQuery(std::string lineitem, std::string orders, std::string customer);

   /**************************************************************************
    *  Computes avg(l_quantity)*100 for the following query.
    *
    *  select avg(l_quantity)*100         (fifth column of lineitem.tbl)
    *  from lineitem, orders, customer
    *  where
    *   l_orderkey = o_orderkey and   (first column of lineitem.tbl == first_column of orders.tbl)
    *   o_custkey = c_custkey and     (second column of orders.tbl == first_column of customer.tbl)
    *   c_mktsegment = <segmentParam>   (seventh column of customers.tbl == <segmentParam>)
    *
    *  where the tables lineitem, orders and customer are those identified by
    *  the paths given in the constructor.
    *
    *  Parameters:
    *     segmentParam: string to use instead of <segmentParam> in query
    *
    *  Returns: avg(l_quantity) * 100
    *     In the data files, l_quantity is of type integer.
    *     Therefore you should return avg(l_quantity) * 100
    *     as integer (more specific C++ type: size_t) as well.
    ************************************************************************/
   size_t avg(std::string segmentParam);
   /// Returns line count of given file
   size_t lineCount(std::string rel);
private:
    std::unordered_map<int, std::vector<std::string>> lineitem_info_rows, orders_info_rows, customer_info_rows;
};
//---------------------------------------------------------------------------
