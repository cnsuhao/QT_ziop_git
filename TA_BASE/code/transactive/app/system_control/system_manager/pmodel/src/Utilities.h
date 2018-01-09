#pragma once
#include <string>
#include <vector>
#include <map>

namespace TA_Base_Core
{
    class ProcessDataSeq;
}

namespace Utilities
{
    std::vector< std::vector<std::string> > convert(const TA_Base_Core::ProcessDataSeq& data);
    std::map<std::pair<size_t, size_t>, std::string> compareProcessData(const TA_Base_Core::ProcessDataSeq& old, const TA_Base_Core::ProcessDataSeq& newed);
}
