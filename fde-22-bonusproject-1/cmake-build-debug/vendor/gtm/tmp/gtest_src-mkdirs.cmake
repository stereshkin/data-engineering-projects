# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/sergeytereshkin/Desktop/fde-22-bonusproject-1/cmake-build-debug/vendor/gtm/src/googletest/googletest"
  "/Users/sergeytereshkin/Desktop/fde-22-bonusproject-1/cmake-build-debug/vendor/gtm/src/gtest_src-build"
  "/Users/sergeytereshkin/Desktop/fde-22-bonusproject-1/cmake-build-debug/vendor/gtm/gtest"
  "/Users/sergeytereshkin/Desktop/fde-22-bonusproject-1/cmake-build-debug/vendor/gtm/tmp"
  "/Users/sergeytereshkin/Desktop/fde-22-bonusproject-1/cmake-build-debug/vendor/gtm/src/gtest_src-stamp"
  "/Users/sergeytereshkin/Desktop/fde-22-bonusproject-1/cmake-build-debug/vendor/gtm/src"
  "/Users/sergeytereshkin/Desktop/fde-22-bonusproject-1/cmake-build-debug/vendor/gtm/src/gtest_src-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/sergeytereshkin/Desktop/fde-22-bonusproject-1/cmake-build-debug/vendor/gtm/src/gtest_src-stamp/${subDir}")
endforeach()
