/*********************************************************************
Copyright (c) 2023, Claudio Costagliola Fiedler
All rights reserved.

This source code is licensed under the BSD-style license found in the
LICENSE file in the root directory of this source tree.
**********************************************************************/
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include <thread>
#include <vector>

#include "logger.hh"
#include "user_data_test.hh"

using namespace testing;
using namespace cc;
using namespace std;

int count_substr(const string &str, const string &substr)
{
  if ((str.length() == 0) || (substr.length() == 0)) {
    return 0;
  }

  string::size_type pos = 0;
  int n = 0;
  while ((pos = str.find(substr, pos)) != string::npos) {
    ++n;
    pos += substr.length();
  }
  return n;
}

void log_task(int i, Logger &logger)
{
  if ((i % 2) == 0) {
    logger.log(LogSeverity::DEBUG) << "TEST:"
      << " " << "1" << 1 << "2" << 2 << "3" << 3 << "4" << 4 << "5" << 5;
    this_thread::sleep_for(std::chrono::milliseconds(50));
    logger.log(LogSeverity::DEBUG) << "TEST:"
      << " " << "1" << 1 << "2" << 2 << "3" << 3 << "4" << 4 << "5" << 5;
    this_thread::sleep_for(std::chrono::milliseconds(50));
    logger.log(LogSeverity::DEBUG) << "TEST:"
      << " " << "1" << 1 << "2" << 2 << "3" << 3 << "4" << 4 << "5" << 5;
    this_thread::sleep_for(std::chrono::milliseconds(50));
  } else {
    logger.log(LogSeverity::DEBUG) << "TEST:"
      << " " << "a" << 1 << "c" << 2 << "e" << 3 << "g" << 4 << "i" << 5 << "k" << 6 << "m" << 7 << "ñ" << 8 << "p" << 9 << "r" << 1 << "t" << 2 << "v" << 3 << "x" << 4 << "z";
    this_thread::sleep_for(std::chrono::milliseconds(50));
    logger.log(LogSeverity::DEBUG) << "TEST:"
      << " " << "a" << 1 << "c" << 2 << "e" << 3 << "g" << 4 << "i" << 5 << "k" << 6 << "m" << 7 << "ñ" << 8 << "p" << 9 << "r" << 1 << "t" << 2 << "v" << 3 << "x" << 4 << "z";
    this_thread::sleep_for(std::chrono::milliseconds(50));
    logger.log(LogSeverity::DEBUG) << "TEST:"
      << " " << "a" << 1 << "c" << 2 << "e" << 3 << "g" << 4 << "i" << 5 << "k" << 6 << "m" << 7 << "ñ" << 8 << "p" << 9 << "r" << 1 << "t" << 2 << "v" << 3 << "x" << 4 << "z";
    this_thread::sleep_for(std::chrono::milliseconds(50));
  }
}

TEST(AuxFunctions, count_substr)
{
  ASSERT_EQ(count_substr("", ""), 0);
  ASSERT_EQ(count_substr("X", ""), 0);
  ASSERT_EQ(count_substr("", "X"), 0);
  ASSERT_EQ(count_substr("X", "X"), 1);
  ASSERT_EQ(count_substr("XX", "X"), 2);
  ASSERT_EQ(count_substr("X", "XX"), 0);
  ASSERT_EQ(count_substr("ABCD", "AB"), 1);
  ASSERT_EQ(count_substr("ABCD", "BC"), 1);
  ASSERT_EQ(count_substr("ABCD", "CD"), 1);
  ASSERT_EQ(count_substr("ABCDABCD", "AB"), 2);
}

TEST(Logging, Multithreading)
{
  stringstream ss;
  Logger logger{ss, LogSeverity::DEBUG};
  
  vector<thread> threads;
  for (int i = 0; i < 10; ++i) {
    threads.emplace_back(log_task, i, std::ref(logger));
  }

  for (auto &thread: threads)
    thread.join();

  ASSERT_EQ(count_substr(ss.str(), "[DEBUG] TEST: a1c2e3g4i5k6m7ñ8p9r1t2v3x4z"), 15);
  ASSERT_EQ(count_substr(ss.str(), "[DEBUG] TEST: 1122334455"), 15);
}

TEST(Logging, UserDataTypes)
{
  stringstream ss;
  Logger logger{ss, LogSeverity::DEBUG};

  UserDataTest user_data_test{UserFieldTest{100, "UserFieldTest"}, "UserDataTest"};

  logger.log(LogSeverity::DEBUG) << "user_data_test:" << user_data_test;

  ASSERT_THAT(ss.str(), HasSubstr("100"));
  ASSERT_THAT(ss.str(), HasSubstr("UserFieldTest"));
  ASSERT_THAT(ss.str(), HasSubstr("UserDataTest"));
}

TEST(Filtering, TRACE)
{
  stringstream ss;
  Logger logger{ss, LogSeverity::TRACE};

  logger.log(LogSeverity::TRACE) << " ";
  logger.log(LogSeverity::DEBUG) << " ";
  logger.log(LogSeverity::INFO) << " ";
  logger.log(LogSeverity::WARN) << " ";
  logger.log(LogSeverity::ERROR) << " ";
  logger.log(LogSeverity::FATAL) << " ";

  ASSERT_THAT(ss.str(), HasSubstr("TRACE"));
  ASSERT_THAT(ss.str(), HasSubstr("DEBUG"));
  ASSERT_THAT(ss.str(), HasSubstr("INFO"));
  ASSERT_THAT(ss.str(), HasSubstr("WARN"));
  ASSERT_THAT(ss.str(), HasSubstr("ERROR"));
  ASSERT_THAT(ss.str(), HasSubstr("FATAL"));
}

TEST(Filtering, DEBUG)
{
  stringstream ss;
  Logger logger{ss, LogSeverity::DEBUG};

  logger.log(LogSeverity::TRACE) << " ";
  logger.log(LogSeverity::DEBUG) << " ";
  logger.log(LogSeverity::INFO) << " ";
  logger.log(LogSeverity::WARN) << " ";
  logger.log(LogSeverity::ERROR) << " ";
  logger.log(LogSeverity::FATAL) << " ";

  ASSERT_THAT(ss.str(), Not(HasSubstr("TRACE")));
  ASSERT_THAT(ss.str(), HasSubstr("DEBUG"));
  ASSERT_THAT(ss.str(), HasSubstr("INFO"));
  ASSERT_THAT(ss.str(), HasSubstr("WARN"));
  ASSERT_THAT(ss.str(), HasSubstr("ERROR"));
  ASSERT_THAT(ss.str(), HasSubstr("FATAL"));
}

TEST(Filtering, INFO)
{
  stringstream ss;
  Logger logger{ss, LogSeverity::INFO};

  logger.log(LogSeverity::TRACE) << " ";
  logger.log(LogSeverity::DEBUG) << " ";
  logger.log(LogSeverity::INFO) << " ";
  logger.log(LogSeverity::WARN) << " ";
  logger.log(LogSeverity::ERROR) << " ";
  logger.log(LogSeverity::FATAL) << " ";

  ASSERT_THAT(ss.str(), Not(HasSubstr("TRACE")));
  ASSERT_THAT(ss.str(), Not(HasSubstr("DEBUG")));
  ASSERT_THAT(ss.str(), HasSubstr("INFO"));
  ASSERT_THAT(ss.str(), HasSubstr("WARN"));
  ASSERT_THAT(ss.str(), HasSubstr("ERROR"));
  ASSERT_THAT(ss.str(), HasSubstr("FATAL"));
}

TEST(Filtering, WARN)
{
  stringstream ss;
  Logger logger{ss, LogSeverity::WARN};

  logger.log(LogSeverity::TRACE) << " ";
  logger.log(LogSeverity::DEBUG) << " ";
  logger.log(LogSeverity::INFO) << " ";
  logger.log(LogSeverity::WARN) << " ";
  logger.log(LogSeverity::ERROR) << " ";
  logger.log(LogSeverity::FATAL) << " ";

  ASSERT_THAT(ss.str(), Not(HasSubstr("TRACE")));
  ASSERT_THAT(ss.str(), Not(HasSubstr("DEBUG")));
  ASSERT_THAT(ss.str(), Not(HasSubstr("INFO")));
  ASSERT_THAT(ss.str(), HasSubstr("WARN"));
  ASSERT_THAT(ss.str(), HasSubstr("ERROR"));
  ASSERT_THAT(ss.str(), HasSubstr("FATAL"));
}

TEST(Filtering, ERROR)
{
  stringstream ss;
  Logger logger{ss, LogSeverity::ERROR};

  logger.log(LogSeverity::TRACE) << " ";
  logger.log(LogSeverity::DEBUG) << " ";
  logger.log(LogSeverity::INFO) << " ";
  logger.log(LogSeverity::WARN) << " ";
  logger.log(LogSeverity::ERROR) << " ";
  logger.log(LogSeverity::FATAL) << " ";

  ASSERT_THAT(ss.str(), Not(HasSubstr("TRACE")));
  ASSERT_THAT(ss.str(), Not(HasSubstr("DEBUG")));
  ASSERT_THAT(ss.str(), Not(HasSubstr("INFO")));
  ASSERT_THAT(ss.str(), Not(HasSubstr("WARN")));
  ASSERT_THAT(ss.str(), HasSubstr("ERROR"));
  ASSERT_THAT(ss.str(), HasSubstr("FATAL"));
}

TEST(Filtering, FATAL)
{
  stringstream ss;
  Logger logger {ss, LogSeverity::FATAL};

  logger.log(LogSeverity::TRACE) << " ";
  logger.log(LogSeverity::DEBUG) << " ";
  logger.log(LogSeverity::INFO) << " ";
  logger.log(LogSeverity::WARN) << " ";
  logger.log(LogSeverity::ERROR) << " ";
  logger.log(LogSeverity::FATAL) << " ";

  ASSERT_THAT(ss.str(), Not(HasSubstr("TRACE")));
  ASSERT_THAT(ss.str(), Not(HasSubstr("DEBUG")));
  ASSERT_THAT(ss.str(), Not(HasSubstr("INFO")));
  ASSERT_THAT(ss.str(), Not(HasSubstr("WARN")));
  ASSERT_THAT(ss.str(), Not(HasSubstr("ERROR")));
  ASSERT_THAT(ss.str(), HasSubstr("FATAL"));
}

TEST(Logging, File)
{
  {
    ofstream ofs{"log_output.log"};
    Logger logger{ofs, LogSeverity::DEBUG};

    logger.log(LogSeverity::TRACE) << "Test";
    logger.log(LogSeverity::DEBUG) << "File";
    logger.log(LogSeverity::INFO) << "Output";
    logger.log(LogSeverity::WARN) << "With";
    logger.log(LogSeverity::ERROR) << "Log";
    logger.log(LogSeverity::FATAL) << "Class";
  }

  ifstream ifs("log_output.log");
  string file_content((istreambuf_iterator<char>(ifs)), (istreambuf_iterator<char>()));

  ASSERT_THAT(file_content, Not(HasSubstr("TRACE")));
  ASSERT_THAT(file_content, HasSubstr("DEBUG"));
  ASSERT_THAT(file_content, HasSubstr("INFO"));
  ASSERT_THAT(file_content, HasSubstr("WARN"));
  ASSERT_THAT(file_content, HasSubstr("ERROR"));
  ASSERT_THAT(file_content, HasSubstr("FATAL"));

  ASSERT_THAT(file_content, HasSubstr("File"));
  ASSERT_THAT(file_content, HasSubstr("Output"));
  ASSERT_THAT(file_content, HasSubstr("With"));
  ASSERT_THAT(file_content, HasSubstr("Log"));
  ASSERT_THAT(file_content, HasSubstr("Class"));
}

TEST(SingletonLogger, Instance) {

  Logger *log_instance = &SingletonLogger::instance(&std::cout);

  ASSERT_EQ(log_instance, &SingletonLogger::instance());
  ASSERT_EQ(log_instance, &SingletonLogger::instance(&std::cout));
  ASSERT_EQ(log_instance, &SingletonLogger::instance(&std::clog, LogSeverity::INFO));
}

TEST(SingletonLoggerDeathTest, Instance) {

  ASSERT_DEATH(SingletonLogger::instance(), "Logger has not been configured!");
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
