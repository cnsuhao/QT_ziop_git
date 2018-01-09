#include "StdAfx.h"
#include "RunParamSeqWrap.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(RunParamSeqWrapTest)

BOOST_AUTO_TEST_CASE(push_back)
{
    RunParamSeqWrap seq;
    seq.push_back();
    std::cout << seq << std::endl;
}

BOOST_AUTO_TEST_CASE(push_back_2)
{
    RunParamSeqWrap seq;
    seq.push_back("good", "morning")("hello", "world");
    std::cout << seq << std::endl;
}

BOOST_AUTO_TEST_CASE(push_pair)
{
    RunParamSeqWrap seq;
    seq.push_pair(std::pair<std::string, int>("hello", 1));
    std::cout << seq << std::endl;
}

BOOST_AUTO_TEST_CASE(push_pair_list)
{
    RunParamSeqWrap seq;
    std::vector<std::pair<std::string, int>> v = { {"hello", 1}, {"world", 2} };
    seq.push_pair_list(v);
    std::cout << seq << std::endl;
}

BOOST_AUTO_TEST_CASE(push_name_value)
{
    RunParamSeqWrap seq;
    struct NameValue { std::string name; int value; };
    seq.push_name_value(NameValue{"hello", 1});
    std::cout << seq << std::endl;
}

BOOST_AUTO_TEST_CASE(push_name_value_list)
{
    RunParamSeqWrap seq;
    struct NameValue { std::string name; int value; };
    std::list<NameValue> l = { { "hello", 1 }, { "world", 2 } };
    seq.push_name_value_list(l);
    std::cout << seq << std::endl;
}

BOOST_AUTO_TEST_CASE(_return)
{
    RunParamSeqWrap seq;
    seq.push_back("hello", "world")("good", 1)(2, "morning");
    TA_Base_Core::RunParamSeq_var seq2(seq._return());
    std::cout << seq << std::endl;
    static_cast<TA_Base_Core::RunParamSeq&>(seq) = static_cast<TA_Base_Core::RunParamSeq&>(seq2);
    std::cout << seq << std::endl;
}

BOOST_AUTO_TEST_CASE(str)
{
    RunParamSeqWrap seq;
    seq.push_back("hello", "world")("good", 1)(2, "morning");
    std::cout << seq.str(1) << std::endl;
    std::cout << seq.str(2) << std::endl;
    std::cout << seq.last_str() << std::endl;
}

BOOST_AUTO_TEST_SUITE_END()
