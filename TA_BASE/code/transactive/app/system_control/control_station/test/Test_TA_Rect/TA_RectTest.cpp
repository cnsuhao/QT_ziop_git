#include "StdAfx.h"
#include <boost/test/unit_test.hpp>
#include "bus/generic_gui_view/src/AnyRect.h"

using namespace TA_Base_Bus;

BOOST_AUTO_TEST_SUITE(TA_Rect_Test)

BOOST_AUTO_TEST_CASE(test_1)
{
    struct Local
    {
        static void foo(const RECT& r)
        {
            std::cout << BOOST_CURRENT_FUNCTION << std::endl;
        }

        static void bar(const QRect& r)
        {
            std::cout << BOOST_CURRENT_FUNCTION << std::endl;
        }

        static void foobar(const TA_Rect& r)
        {
            std::cout << BOOST_CURRENT_FUNCTION << std::endl;
        }
    };

    AnyRect r;
    RECT r1 = r;
    QRect r2 = r;
    r = r;
    r = r1;
    r = r2;
    Local::foo(r);
    Local::bar(r);
    Local::foobar(r);
}

BOOST_AUTO_TEST_CASE(test_compare)
{
    AnyRect r1, r2;
    QRect r3;
    RECT r4;
    r1 == r2; //r1 != r2;
    r1 == r3; //r3 == r1; // r1 != r3;
    r1 == r4; // r1 != r4;
    r4 == r1;
}

BOOST_AUTO_TEST_CASE(test_less_than)
{
    AnyRect r1, r2;
    r1 < r2;
    r1 <= r2;
    r1 > r2;
    r1 >= r2;
}

BOOST_AUTO_TEST_SUITE_END()
