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

/**
 * @brief Enum class representing the severity of a log message
 */
enum class LogSeverity {
  TRACE, /**< Used for trace messages */
  DEBUG, /**< Used for general debug messages */
  INFO, /**< Used for messages for informing the user */
  WARN, /**< Used for warnings */
  ERROR, /**< Used for runtime errors */
  FATAL /**< Used for fatal errors */
};

/**
 * @brief Class used to output the accumulated string, formed after chaining the << operators,
 * to the std::ostream used for log.
 */
class LoggerDelegate final {
public:

  /**
   * @brief Prefered constructor of the class.
   * @param os The std::ostream object address used for outputting the log messages
   * @param preamble The preamble to be inserted before the log line
   * @param empty Indicates whether this instance generates output. It doesn't generate output
   * when the log has been filtered out
   */
  LoggerDelegate(std::ostream &os, const std::string &preamble, bool empty = false);

  /**
   * @brief Class destructor. Outputs the accumulated string to the std::ostream object configured
   * as output.
   */
  ~LoggerDelegate();

  /**
   * @brief Stream insertion operator overloading
   * @tparam T The type of the object used as RHS of the operator
   * @param rhs The RHS of the operator
   * @return A reference to the std::ostream object used for acumulate the message.
   */
  template<typename T> std::ostream &operator<<(T&& rhs) {
    m_ss << std::forward<T>(rhs);
    return m_ss;
  }

  /**
   * @brief Deleted class copy constructor.
   */
  LoggerDelegate(const LoggerDelegate&) = delete;
  /**
   * @brief Move constructor. It's declared because it's needed for compilation.
   * It's defined only to issue assert if called.
   */
  LoggerDelegate(LoggerDelegate&&);
  /**
   * @brief Deleted assignment operator.
   */
  LoggerDelegate& operator=(const LoggerDelegate&) = delete;
  /**
   * @brief Deleted move assignment operator.
   */
  LoggerDelegate& operator=(LoggerDelegate&&) = delete;

private:
  std::ostream &m_os;
  std::string m_preamble;
  std::stringstream m_ss;
  const bool m_empty;
};

/**
 * @brief Main class used for logging.
 * 
 * This class is intended to be used as singleton. However,
 * the singleton pattern implementation is handled by the class \ref SingletonLogger.
 * This design was chosen to ease unit testing of the class.
 */
class Logger final {
public:
  /**
   * @brief Constructor of the class
   * @param os A reference to the std::ostream object to use for outputting the log messages.
   * Any std::ostream subclass can be used, for example: std::cout, std::clog, std::cerr,
   * std::ofstream, etc
   * @param sev The log level to filter out log messages. Only log messages with a severity
   * equal or higher will be emitted.
   */
  Logger(std::ostream& os, LogSeverity sev);
  /**
   * @brief Defaulted destructor. Note that this destructor will not destroy the std::ostream
   * object
   */
  ~Logger() = default;
  /**
   * @brief Deleted copy constructor
   */
  Logger(const Logger&) = delete;
  /**
   * @brief Deleted move constructor
   */
  Logger(Logger&&) = delete;
  /**
   * @brief Deleted assignment operator
   */
  Logger& operator=(const Logger&) = delete;
  /**
   * @brief Deleted move assignment operator
   */
  Logger& operator=(Logger&&) = delete;

  /**
   * @brief Logs a message with a severity of sev
   * 
   * This method is intended to use like this:
   * 
   * cc::SingletonLogger::instance().log(cc::LogSeverity::INFO) << "Temp: " << temp << " celcius";
   * 
   * @param sev The severity of the message. No message will be issued if is not severe enough
   * according to the severity filter configured in
   * \ref Logger(std::ostream& os, LogSeverity sev)
   */
  LoggerDelegate log(LogSeverity sev = LogSeverity::DEBUG);

private:
  std::string LogSeverityText(LogSeverity sev);

  std::stringstream m_dummy_ss;
  std::ostream &m_os;
  const LogSeverity m_sev_filter;
};

/**
 * @brief Class implementing the singleton pattern on the Logger class.
 * 
 * The logger functionality should be used using this class. The stand alone \ref Logger class
 * exists only to allow proper unit testing.
 * 
 */
class SingletonLogger {
public:
  /**
   * @brief Static method to obtain the singleton Logger object.
   * 
   * Note that this method must be called first using the parameters which will be used
   * to configure the Logger object.
   * If the first call to this method is without parameters, an assertion will terminate program
   * execution. This calling order is enforced to allow proper working of the singleton.
   * 
   * @param os A pointer to the std::ostream object to use for constructing the \ref Logger object.
   * @param sev The severity which will be used for constructing the \ref Logger object.
   */
  static Logger &instance(std::ostream *os = nullptr, LogSeverity sev = LogSeverity::DEBUG);

  /**
   * @brief Deleted copy constructor
   */
  SingletonLogger(const SingletonLogger&) = delete;
  /**
   * @brief Deleted move constructor
   */
  SingletonLogger(SingletonLogger&&) = delete;
  /**
   * @brief Deleted assignment operator
   */
  SingletonLogger& operator=(const SingletonLogger&) = delete;
  /**
   * @brief Deleted move assignment operator
   */
  SingletonLogger& operator=(SingletonLogger&&) = delete;
  /**
   * @brief Defaulted destructor
   */
  ~SingletonLogger() = default;

private:
  SingletonLogger();

  static std::atomic_bool m_is_constructed;
};

//Helper functions

/** 
 * @brief Configures the singleton Logger object. It must be called before any of the xxx_log()
 * functions
 * @param os The std::ostream object to be used for log output. The object must be valid during
 * the whole program execution. A good place is the stack of main(), for example.
 * @param sev The value used to filter the logs. Logs with a severity equal or higher than
 * sev will be issued.
 * @sa SingletonLogger::instance(std::ostream *os = nullptr, LogSeverity sev = LogSeverity::DEBUG)
 */
void configure_logger(std::ostream &os, LogSeverity sev = LogSeverity::DEBUG);
/** 
 * @brief Logs a trace message, using `<<` stream insertion operator:
 * 
 * `cc::trace_log() << "Trace log";`
 * @sa Logger::log(LogSeverity sev = LogSeverity::DEBUG)
 */
LoggerDelegate trace_log();
/** 
 * @brief Logs a debug message, using `<<` stream insertion operator:
 * 
 * `cc::debug_log() << "Debug log";`
 * @sa Logger::log(LogSeverity sev = LogSeverity::DEBUG)
 */
LoggerDelegate debug_log();
/** 
 * @brief Logs an information message, using `<<` stream insertion operator:
 * 
 * `cc::info_log() << "Info log";`
 * @sa Logger::log(LogSeverity sev = LogSeverity::DEBUG)
 */
LoggerDelegate info_log();
/** 
 * @brief Logs a warning message, using `<<` stream insertion operator:
 * 
 * `cc::warn_log() << "Warn log";`
 * @sa Logger::log(LogSeverity sev = LogSeverity::DEBUG)
 */
LoggerDelegate warn_log();
/** 
 * @brief Logs an error message, using `<<` stream insertion operator:
 * 
 * `cc::error_log() << "Error log";`
 * @sa Logger::log(LogSeverity sev = LogSeverity::DEBUG)
 */
LoggerDelegate error_log();
/** 
 * @brief Logs a fatal message, using `<<` stream insertion operator:
 * 
 * `cc::fatal_log() << "Fatal log";`
 * @sa Logger::log(LogSeverity sev = LogSeverity::DEBUG)
 */
LoggerDelegate fatal_log();

} //namespace cc

#endif //__CC_LOGGER_H__
