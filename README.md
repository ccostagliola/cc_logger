# CC Logger

I created this project to showcase my knowledge of some modern C++ (C++11) features. It is a Logger singleton, multithread-safe, and integrated with `std::ostream`. It also provides log filtering based on the severity of the messages. It can output variables of any type supported by `std::ostream` `<<` operator. It can output variables of any user data type as long as it provides an overload of operator `<<`:
```c++
class UserData {
public:
    ...
    friend std::ostream &operator<<(std::ostream&, const UserData&);
    ...
};

std::ostream &operator<<(std::ostream& lhs, const UserData& rhs) {

    return lhs  <<  "(m_field_1: " << rhs.m_field_1
                << ", m_field_2: " << rhs.m_field_2
                ...
                << ", m_field_n: " << rhs.m_field_n
                << "\")";
}
```

Unit tests (based on Google Test) are provided.

## Example of use

The following example shows the typical use of the Logger singleton, outputting an `int`, an `std::string`, and a user data type. The user data type is composed of another user data type.

### user_data_test.hh

```c++
#ifndef __USER_DATA_TEST_H__
#define __USER_DATA_TEST_H__

#include <ostream>
#include <string>

namespace testing {

using namespace std;

class UserFieldTest {
public:
    UserFieldTest();
    UserFieldTest(int datum1, const string &datum2);

    friend ostream &operator<<(ostream&, const UserFieldTest&);

private:
    int m_datum1;
    string m_datum2;
};

class UserDataTest {
public:
    UserDataTest();
    UserDataTest(const UserFieldTest&, const string&);

    friend ostream &operator<<(ostream&, const UserDataTest&);

private:
    UserFieldTest m_user_datum1;
    string m_datum2;
};

}//namespace testing

#endif //__USER_DATA_TEST_H__
```

### user_data_test.cc

```c++
#include "user_data_test.hh"

namespace testing {

using namespace std;

UserFieldTest::UserFieldTest():
    m_datum1{0},
    m_datum2{}
{}

UserFieldTest::UserFieldTest(int datum1, const string &datum2):
    m_datum1{datum1},
    m_datum2{datum2}
{}

ostream &operator<<(ostream& lhs, const UserFieldTest& rhs) {

    return lhs << "(datum1: " << rhs.m_datum1 << ", datum2: \"" << rhs.m_datum2 << "\")";
}

UserDataTest::UserDataTest():
    m_user_datum1{},
    m_datum2{}
{}

UserDataTest::UserDataTest(const UserFieldTest& user_datum1, const string& datum2):
    m_user_datum1{user_datum1},
    m_datum2{datum2}
{}

ostream &operator<<(ostream& lhs, const UserDataTest& rhs) {

    return lhs << "(user_datum1: " << rhs.m_user_datum1 << ", datum2: \"" << rhs.m_datum2 << "\")";
}

}//namespace testing
```

### main.cc

```c++
#include <iostream>

#include "logger.hh"
#include "user_data_test.hh"

int main()
{
  cc::SingletonLogger::instance(std::clog, cc::LogSeverity::DEBUG);
  UserDataTest user_data_test{UserFieldTest{100, "UserFieldTest"}, "UserDataTest"};
  cc::SingletonLogger::instance().log(LogSeverity::DEBUG) << "user_data_test: " << user_data_test;

  return 0;
}
```

## Getting Started

To get the code and run the unit tests, follow these steps:
```
git clone https://github.com/ccostagliola/cc_logger.git
cd cc_logger
mkdir build
cd build
cmake ..
cmake --build .
ctest
```

There are no external dependencies for running the code. For building:
* Any C++ 11 compiler.
* `CMake` version `3.20` or newer.

## Usage

Include `logger.hh` wherever you want to use the class. The instance is obtained by calling `cc::SingletonLogger::instance()`. However, the first call to `instance` must configure the class with the `std::ostream` to be used and the log severity for filtering logs:
```c++
cc::SingletonLogger::instance(&std::clog, cc::LogSeverity::INFO);
```

Any successive calls to `instance`, with parameters or not, don´t change the Logger configuration. The above example configures the Logger for outputting to the standard console log, only the `INFO` messages and more severe than `INFO`. You can choose among any of these severities:
1. `TRACE`
2. `DEBUG`
3. `INFO`
4. `WARN`
5. `ERROR`
6. `FATAL`

For emitting a single line of log message use the following:
```c++
cc::SingletonLogger::instance().log(cc::LogSeverity::ERROR) << "Logging message number: " << 1;
```

## License

[BSD 3-Clause License](https://opensource.org/license/bsd-3-clause/)
