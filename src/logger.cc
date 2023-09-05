/*********************************************************************
Copyright (c) 2023, Claudio Costagliola Fiedler
All rights reserved.

This source code is licensed under the BSD-style license found in the
LICENSE file in the root directory of this source tree.
**********************************************************************/
#include <cassert>
#include <mutex>

#include "logger.hh"

namespace cc {

std::mutex mut;

//SingletonLogger
std::atomic_bool SingletonLogger::m_is_constructed{false};

Logger &SingletonLogger::instance(std::ostream *os, LogSeverity sev)
{
    static Logger _instance(*os, sev);

    assert((m_is_constructed || (os != nullptr)) && "Logger has not been configured!");
    m_is_constructed = true;

    return _instance;
}

//LoggerDelegate
LoggerDelegate::LoggerDelegate(std::ostream &os, const std::string &preamble, bool empty):
    m_os{os},
    m_preamble{preamble},
    m_ss{},
    m_empty{empty}
{}

LoggerDelegate::LoggerDelegate(const LoggerDelegate &o):
    m_os{o.m_os},
    m_preamble{o.m_preamble},
    m_ss{},
    m_empty{false}
{
    assert(false && "LoggerDelegate's copy constructor shouldn't be called!");
}

LoggerDelegate::~LoggerDelegate()
{
    if (m_empty) {
        return;
    }

    {
        const std::lock_guard<std::mutex> lock(mut);
        m_os << m_preamble << m_ss.str() << std::endl;
    }
}

// Logger
Logger::Logger():
    m_dummy_ss{},
    m_os{m_dummy_ss},
    m_sev_filter{LogSeverity::DEBUG}
{
    assert(false && "Logger's default constructor shouldn't be called!");
}

Logger::Logger(std::ostream &os, LogSeverity sev):
    m_dummy_ss{},
    m_os{os},
    m_sev_filter{sev}
{}

std::string Logger::LogSeverityText(LogSeverity sev)
{
    switch (sev) {
        case LogSeverity::DEBUG: return "DEBUG";
        case LogSeverity::TRACE: return "TRACE";
        case LogSeverity::INFO:  return "INFO ";
        case LogSeverity::WARN:  return "WARN ";
        case LogSeverity::ERROR: return "ERROR";
        case LogSeverity::FATAL: return "FATAL";
    default:
        return "";
    }
}

LoggerDelegate Logger::log(LogSeverity sev)
{
    if (sev >= m_sev_filter) {
        return LoggerDelegate{m_os, std::string("[") + LogSeverityText(sev).substr(0, 5) + "] "};
    }

    return LoggerDelegate{m_os, "", true};
}

} //namespace cc
