/*********************************************************************
Copyright (c) 2023, Claudio Costagliola Fiedler
All rights reserved.

This source code is licensed under the BSD-style license found in the
LICENSE file in the root directory of this source tree.
**********************************************************************/
#include <iostream>

#include "logger.hh"

int main() {

    cc::SingletonLogger::instance(&std::clog, cc::LogSeverity::TRACE);

    cc::SingletonLogger::instance().log(cc::LogSeverity::TRACE) << " ";
    cc::SingletonLogger::instance().log(cc::LogSeverity::DEBUG) << " ";
    cc::SingletonLogger::instance().log(cc::LogSeverity::INFO) << " ";
    cc::SingletonLogger::instance().log(cc::LogSeverity::WARN) << " ";
    cc::SingletonLogger::instance().log(cc::LogSeverity::ERROR) << " ";
    cc::SingletonLogger::instance().log(cc::LogSeverity::FATAL) << " ";

    return 0;
}
