#include "StdAfx.h"
#include "Singleton.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(Singleton_1)
{
    struct TestSingleton : Singleton<TestSingleton>
    {
        virtual void initialize()
        {
            std::cout << __FUNCSIG__ << std::endl;
        }

        void foo()
        {
            std::cout << __FUNCSIG__ << std::endl;
        }
    };

    TestSingleton::getInstance().foo();
}

BOOST_AUTO_TEST_CASE(Singleton_2)
{
    struct TestSingleton : Singleton<TestSingleton>
    {
        void foobar(const std::string& str, int x)
        {
            m_x = x;
            m_str = str;
        }

        void foo()
        {
            std::cout << m_str << m_x << std::endl;
        }

        std::string m_str;
        int m_x;
    };

    TestSingleton::setInitializer(&TestSingleton::foobar, "hello, world ", 123);
    TestSingleton::getInstance().foo();
}
