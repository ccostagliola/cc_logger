/*********************************************************************
Copyright (c) 2023, Claudio Costagliola Fiedler
All rights reserved.

This source code is licensed under the BSD-style license found in the
LICENSE file in the root directory of this source tree.
**********************************************************************/
#ifndef __CC_LOGGER_H__
#define __CC_LOGGER_H__

#include <cassert>
#include <ostream>
#include <sstream>
#include <atomic>

namespace cc {

enum class LogSeverity: int {TRACE = 0, DEBUG, INFO, WARN, ERROR, FATAL};

class LoggerDelegate final {
public:
  LoggerDelegate(std::ostream &os, const std::string &preamble, bool empty = false);
  LoggerDelegate(const LoggerDelegate &o);
  ~LoggerDelegate();

  template<typename T> std::ostream &operator<<(T&& rhs) {
    m_ss << std::forward<T>(rhs);
    return m_ss;
  }

  LoggerDelegate(LoggerDelegate&&) = default;
  LoggerDelegate& operator=(const LoggerDelegate&) = delete;
  LoggerDelegate& operator=(LoggerDelegate&&) = delete;

private:
  std::ostream &m_os;
  std::string m_preamble;
  std::stringstream m_ss;
  const bool m_empty;
};

class Logger final {
public:
  LoggerDelegate log(LogSeverity sev = LogSeverity::DEBUG);
  std::string LogSeverityText(LogSeverity sev);

  Logger();
  Logger(std::ostream&, LogSeverity);
  ~Logger() = default;
  Logger(const Logger&) = delete;
  Logger(Logger&&) = delete;
  Logger& operator=(const Logger&) = delete;
  Logger& operator=(Logger&&) = delete;

private:
  std::stringstream m_dummy_ss;
  std::ostream &m_os;
  const LogSeverity m_sev_filter;
};

class SingletonLogger {
public:
  static Logger &instance(std::ostream *os = nullptr, LogSeverity sev = LogSeverity::DEBUG);

  SingletonLogger(const SingletonLogger&) = delete;
  SingletonLogger(SingletonLogger&&) = delete;
  SingletonLogger& operator=(const SingletonLogger&) = delete;
  SingletonLogger& operator=(SingletonLogger&&) = delete;
  ~SingletonLogger() = default;

private:
  SingletonLogger();

  static std::atomic_bool m_is_constructed;
};

//Helper functions
//void configure_logger()

} //namespace cc

#endif //__CC_LOGGER_H__
