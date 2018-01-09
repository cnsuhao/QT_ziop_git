#include "StdAfx.h"
#include "RunParamsAny.h"
#include <boost/test/unit_test.hpp>

using namespace TA_Base_Core;

BOOST_AUTO_TEST_CASE(TestRunParamsAnyn_1)
{
    RunParamsAny::getInstance().set("hello1", "world");
    std::string str = RunParamsAny::getInstance().get<const char*>("hello1");
    std::cout << str << std::endl;

    const int x = 0;
    RunParamsAny::getInstance().set("hello2", x);
    std::cout << RunParamsAny::getInstance().get<int>("hello2") << std::endl;

    RunParamsAny::getInstance().clear();
}

BOOST_AUTO_TEST_CASE(TestRunParamsAnyn_2)
{
    boost::async([]
    {
        boost::this_thread::sleep_for(boost::chrono::milliseconds(100));
        RunParamsAny::getInstance().set("hello3", "world");
        RunParamsAny::getInstance().set("hello4", "world2");
    });

    {
        const char* str = RunParamsAny::getInstance().promiseGet<const char*>("hello3", 10);

        if (str)
        {
            std::cout << str << std::endl;
        }
    }

    {
        std::string str = RunParamsAny::getInstance().promiseGet<const char*>("hello4");
        std::cout << str << std::endl;
    }

    RunParamsAny::getInstance().clear();
}
