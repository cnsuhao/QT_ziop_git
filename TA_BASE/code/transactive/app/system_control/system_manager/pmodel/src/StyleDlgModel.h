#pragma once
#include "AbstractModel.h"
#include <string>
#include <vector>

namespace TA_Base_App
{
    struct IStyleDlg;
    struct StyleDlgModelService;

    struct StyleSheetSynData
    {
        size_t language_index;
        size_t style_index;
        size_t stylesheet_index;
        static std::vector<std::wstring> languages;
        static std::vector<std::string> stylesheets;
    };

    struct StyleDlgModel : AbstractModel<IStyleDlg, StyleSheetSynData, StyleDlgModelService>
    {
    public:

        StyleDlgModel(StyleDlgModelService* service);
        virtual void beforeAttachView();
        virtual void initSyncData();
    };
}
