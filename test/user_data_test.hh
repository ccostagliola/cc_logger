/*********************************************************************
Copyright (c) 2023, Claudio Costagliola Fiedler
All rights reserved.

This source code is licensed under the BSD-style license found in the
LICENSE file in the root directory of this source tree.
**********************************************************************/
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
