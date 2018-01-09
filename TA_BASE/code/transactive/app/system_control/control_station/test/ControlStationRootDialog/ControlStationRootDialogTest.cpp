#include "StdAfx.h"
//#include "ControlStationRootDialog.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(ControlStationRootDialog_Test)

BOOST_AUTO_TEST_CASE(placement_new)
{
    struct Test
    {
        Test(std::string str)
            : m_str(str)
        {
        }

        void foo()
        {
            std::cout << m_str << std::endl;
        }

        std::string m_str;
    };

    char* p = new char[sizeof(Test)];
    Test* pTest = new (p) Test("hello, world");
    pTest->foo();
}

BOOST_AUTO_TEST_SUITE_END()
