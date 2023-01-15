#include "JoinQuery.hpp"
#include <assert.h>
#include <fstream>
#include <thread>
#include <sstream>
#include <algorithm>
#include <future>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>


//---------------------------------------------------------------------------
std::unordered_map<int, std::vector<std::string>> fillMap(const std::string& path) {
    int fd = open(path.c_str(), O_RDONLY);
    lseek(fd, 0, SEEK_END);
    auto size = lseek(fd, 0, SEEK_CUR);
    char* mapped_file = static_cast<char*>(mmap(nullptr, size, PROT_READ, MAP_SHARED, fd, 0));
    std::string current_string;
    std::unordered_map<int, std::vector<std::string>> info_rows;
    int j = 0;
    std::vector<std::string> row;
    std::string_view sv  = std::string_view(mapped_file);
    auto pos = sv.find('\n');
    auto prev_pos = 0;
    int k = 0;
    while (pos != sv.npos) {
        auto current_string = sv.substr(prev_pos, pos - prev_pos);
        std::vector<std::string> res;
        auto prev_delim_pos =  0;
        auto delim_pos = current_string.find('|');
        while (delim_pos != current_string.npos) {

            if (k == 0 or k == 1 or k ==4 or k == 6) {
                auto item = current_string.substr(prev_delim_pos, delim_pos - prev_delim_pos);
                if (item.front() == '|' or item.front() == '\n') {
                    item.remove_prefix(1);
                }
                if (item.back() == '|' or item.back() == '\n') {
                    item.remove_suffix(1);
                }

                res.emplace_back(item);
            }
            prev_delim_pos = delim_pos;
            k += 1;
            delim_pos = current_string.find('|', prev_delim_pos+1);
        }
        info_rows[j] = res;
        j += 1;
        k = 0;
        prev_pos = pos;
        pos = sv.find('\n', prev_pos+1);
    }

    munmap(mapped_file, size);
    return info_rows;
}


JoinQuery::JoinQuery(std::string lineitem, std::string order,
                     std::string customer)
{
    std::vector<std::string> data = {lineitem, order, customer};
    std::vector<std::future<std::unordered_map<int, std::vector<std::string>>>> futures;
    for (int i = 0; i < 3; i++) {
        futures.push_back(std::async(fillMap, std::ref(data[i])));
    }
    customer_info_rows = futures[2].get();
    orders_info_rows = futures[1].get();
    lineitem_info_rows = futures[0].get();

}

std::unordered_multimap<std::string, std::vector<std::string>> performJoin(
        std::unordered_map<int, std::vector<std::string>>& info, size_t index, size_t begin_chunk, size_t end_chunk,
        std::unordered_multimap<std::string, std::vector<std::string>>& prevHashMap) {

    std::unordered_multimap<std::string, std::vector<std::string>> res;
    for (size_t i = begin_chunk; i < end_chunk; i++) {
        auto search  = prevHashMap.find(info[i][index]);
        if (search != end(prevHashMap)) {
            res.insert({info[i][index], info[i]});
        }
    }
    return res;
}

std::pair<uint64_t, size_t> performLastJoin(
        std::unordered_map<int, std::vector<std::string>>& info, size_t index, size_t begin_chunk, size_t end_chunk,
        std::unordered_multimap<std::string, std::vector<std::string>>& prevHashMap) {

    uint64_t sum = 0;
    size_t j = 0;
    for (size_t i = begin_chunk; i < end_chunk; i++) {
        auto search  = prevHashMap.find(info[i][index]);
        if (search != end(prevHashMap)) {
            sum += atoi(info[i][2].c_str());
            j += 1;
        }
    }
    return std::make_pair(sum, j);
}


//---------------------------------------------------------------------------
size_t JoinQuery::avg(std::string segmentParam)
{
    std::unordered_multimap<std::string, std::vector<std::string>> HashMapCustomer, HashMapOrder;
    unsigned chunks = std::thread::hardware_concurrency();
    for (const auto& c : customer_info_rows) {
        if (c.second[3] == segmentParam) {
            HashMapCustomer.insert({c.second[0], customer_info_rows[c.first]});
        };
    }




    std::vector<std::future<std::unordered_multimap<std::string, std::vector<std::string>>>> futures_first;
    for (unsigned j = 0; j < chunks; j++) {
        auto begin_chunks = j * orders_info_rows.size() / chunks;
        auto end_chunks = (j+1) * orders_info_rows.size() / chunks;
        futures_first.push_back(std::async(performJoin, std::ref(orders_info_rows), 1, begin_chunks, end_chunks, std::ref(HashMapCustomer)));
    }

    /*for (const auto& order_row : orders_info_rows) {
        if (order_row.second.empty()) {
            continue;
        }
        auto search = HashMapCustomer.find(order_row.second[1]);
        if (search != end(HashMapCustomer)) {
            res_after_first_join.insert({order_row.second[1], order_row.second});
        }
    }*/
    for (auto & i : futures_first) {
        for (const auto& r : i.get()) {
            HashMapOrder.insert({r.second[0], r.second});
        }
    }



    std::vector<std::future<std::pair<uint64_t, size_t>>> futures;
    for (unsigned j = 0; j < chunks; j++) {
        auto begin_chunks = j * lineitem_info_rows.size() / chunks;
        auto end_chunks = (j+1) * lineitem_info_rows.size() / chunks;
        futures.push_back(std::async(performLastJoin, std::ref(lineitem_info_rows), 0, begin_chunks, end_chunks, std::ref(HashMapOrder)));
    }


    uint64_t summa = 0;
    uint64_t j = 0;
    for (auto i = 0; i < chunks; i++) {
        auto item = futures[i].get();
        summa += item.first;
        j += item.second;
    }
    /*
    for (const auto& lineitem_row : lineitem_info_rows) {
        if (lineitem_row.second.empty()) {
            continue;
        }
        auto search  = HashMapOrder.find(lineitem_row.second[0]);
        if (search != end(HashMapOrder)) {
            res_after_second_join.insert({lineitem_row.second[0], lineitem_row.second});
        }
    }*/



    /*for (const auto& t : res_after_second_join) {
        sum += atoi(t.second[2].c_str());
    }*/
    uint64_t avg = summa * 100 / j;
    return avg;
}
//---------------------------------------------------------------------------
size_t JoinQuery::lineCount(std::string rel)
{
   std::ifstream relation(rel);
   assert(relation);  // make sure the provided string references a file
   size_t n = 0;
   for (std::string line; std::getline(relation, line);) n++;
   return n;
}
//---------------------------------------------------------------------------
