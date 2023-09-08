/*********************************************************************
Copyright (c) 2023, Claudio Costagliola Fiedler
All rights reserved.

This source code is licensed under the BSD-style license found in the
LICENSE file in the root directory of this source tree.
**********************************************************************/
#include <iostream>
#include <iomanip>

#include "logger.hh"

int main() {

    cc::configure_logger(std::clog, cc::LogSeverity::TRACE);

    int n_int{300};
    float n_float{345.837465834};
    double n_double{29287449.197389950734954837};
    std::string s_string{"STRING EXAMPLE"};

    cc::trace_log() << "Example trace message number " << std::hex << 100;
    cc::debug_log() << "Example debug message number #" << std::setw(10) << 200 << "#";
    cc::info_log() << "Example information message number " << n_int;
    cc::warn_log() << "Example warning message number " << n_float;
    cc::error_log() << "Example error message number " << std::fixed << std::setprecision(8) << n_double;
    cc::fatal_log() << "Example fatal message \"" << s_string << "\"";

    return 0;
}
