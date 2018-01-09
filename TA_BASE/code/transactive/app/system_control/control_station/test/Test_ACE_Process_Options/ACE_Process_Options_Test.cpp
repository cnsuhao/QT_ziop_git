#include "StdAfx.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(ACE_Process_Options_1)
{
    struct Local
    {
        ACE_Process_Options options;
    };

    new Local;
}
