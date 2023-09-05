/*********************************************************************
Copyright (c) 2023, Claudio Costagliola Fiedler
All rights reserved.

This source code is licensed under the BSD-style license found in the
LICENSE file in the root directory of this source tree.
**********************************************************************/
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
