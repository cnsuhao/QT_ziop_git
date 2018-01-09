#pragma once
#include <set>
#include "ProcessUtilities.h"

namespace TA_Base_Core
{
    class ProcessDataSeq;
}

namespace TA_Base_App
{
    struct Factory;

    struct ProcessDataUser
    {
        ProcessDataUser();
        ProcessUtilities m_util;
        std::set<size_t> m_selection;
        TA_Base_Core::ProcessDataSeq& m_data;
    };
}
