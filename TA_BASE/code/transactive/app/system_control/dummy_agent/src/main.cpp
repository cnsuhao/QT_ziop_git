#include "StdAfx.h"
#include "DummyAgent.h"
#include "core/utilities/src/DebugUtil.h"

using namespace TA_Base_Core;

int main(int argc, char* argv[])
{
    try
    {
        DummyAgent agent(argc, argv);
        agent.run();
    }
    catch (...)
    {
        LOG(SourceInfo, DebugUtil::ExceptionCatch, "exception of unknown type caught");
    }

    return 0;
}
