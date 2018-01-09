#include "StdAfx.h"
#include "RunParams.h"
#include <boost/test/unit_test.hpp>

using namespace TA_Base_Core;

BOOST_AUTO_TEST_CASE(RunParams_1)
{
    std::string cmd = " hello \t world ";
    boost::trim(cmd);
    std::vector<std::string> vs;
    boost::split(vs, cmd, boost::is_space(), boost::token_compress_on);
    //std::remove_if()

    for (const std::string& s : vs)
    {
        std::cout << "<" <<  s << ">" << std::endl;
    }
}
