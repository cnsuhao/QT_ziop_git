#include "StyleDlgModelService.h"
#include <boost/filesystem.hpp>
#include "core/utilities/src/RunParams.h"

using namespace TA_Base_Core;

namespace TA_Base_App
{
    std::vector<std::string> StyleDlgModelService::getStyleSheets()
    {
        std::vector<std::string> qss;
        std::vector<boost::filesystem::path> search = {"./style", "../style" };

        if (RunParams::getInstance().isSet("StylePath"))
        {
            search.push_back(RunParams::getInstance().get("StylePath"));
        }

        boost::filesystem::recursive_directory_iterator end;

        for (boost::filesystem::path& p : search)
        {
            if (boost::filesystem::exists(p))
            {
                for (boost::filesystem::recursive_directory_iterator it(p); it != end; ++it)
                {
                    if (!is_directory(it->status()) && it->path().extension() == ".qss")
                    {
                        qss.push_back(it->path().stem().string());
                    }
                }
            }
        }

        return qss;
    }

    std::vector<std::wstring> StyleDlgModelService::getLanguages()
    {
        const wchar_t* languages[] = { L"English", L"简体中文" };
        return std::vector<std::wstring>(languages, languages + sizeof(languages) / sizeof(wchar_t*));
    }
}
