# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/sergeytereshkin/Desktop/fde-22-bonusproject-1/cmake-build-debug/vendor/benchmark/src/benchmark_src"
  "/Users/sergeytereshkin/Desktop/fde-22-bonusproject-1/cmake-build-debug/vendor/benchmark/src/benchmark_src-build"
  "/Users/sergeytereshkin/Desktop/fde-22-bonusproject-1/cmake-build-debug/vendor/benchmark"
  "/Users/sergeytereshkin/Desktop/fde-22-bonusproject-1/cmake-build-debug/vendor/benchmark/tmp"
  "/Users/sergeytereshkin/Desktop/fde-22-bonusproject-1/cmake-build-debug/vendor/benchmark/src/benchmark_src-stamp"
  "/Users/sergeytereshkin/Desktop/fde-22-bonusproject-1/cmake-build-debug/vendor/benchmark/src"
  "/Users/sergeytereshkin/Desktop/fde-22-bonusproject-1/cmake-build-debug/vendor/benchmark/src/benchmark_src-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/sergeytereshkin/Desktop/fde-22-bonusproject-1/cmake-build-debug/vendor/benchmark/src/benchmark_src-stamp/${subDir}")
endforeach()
