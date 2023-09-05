include(FetchContent)
include(GoogleTest)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        7da4a4114ad2bf5ae61460e0c02ba35fc323fc9d #release-1.12.1
)
FetchContent_MakeAvailable(googletest)

add_executable (cc_logger_test
  logger_test.cc
  user_data_test.cc
  ${CMAKE_SOURCE_DIR}/src/logger.cc
)

target_include_directories(cc_logger_test
  PRIVATE
  ${CMAKE_SOURCE_DIR}/src
  ${CMAKE_SOURCE_DIR}/test
)

target_link_libraries(cc_logger_test
  PRIVATE
  GTest::gtest_main
  GTest::gmock
)

gtest_discover_tests(cc_logger_test)
