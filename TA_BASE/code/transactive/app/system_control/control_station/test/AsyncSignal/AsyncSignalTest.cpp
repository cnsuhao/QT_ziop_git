#include "StdAfx.h"
#include "AsyncSignal.h"
#include "core/utilities/src/RunParams.h"
#include <boost/test/unit_test.hpp>
#include <boost/current_function.hpp>

BOOST_AUTO_TEST_SUITE(AsyncSignal_Test)

BOOST_AUTO_TEST_CASE(AsyncSignal_1)
{
    struct Local
    {
        static void foo()
        {
            boost::this_thread::sleep_for(boost::chrono::milliseconds(100));
            std::cout << BOOST_CURRENT_FUNCTION << std::endl;
        }

        static void bar()
        {
            boost::this_thread::sleep_for(boost::chrono::milliseconds(200));
            std::cout << BOOST_CURRENT_FUNCTION << std::endl;
        }

        static void foobar(int& x)
        {
            x += 100;
            std::cout << BOOST_CURRENT_FUNCTION << std::endl;
        }
    };

    for (size_t i : {0, 1})
    {
        std::cout << "iteration " << i << std::endl;
        AsyncSignal<void(), AsyncSignalWait::WaitForAll> sig;
        sig.connect(&Local::foo);
        sig.connect(&Local::bar);
        sig();
        std::cout << BOOST_CURRENT_FUNCTION << std::endl;
    }
}

BOOST_AUTO_TEST_SUITE_END()
