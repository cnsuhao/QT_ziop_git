#include "StyleDlgModel.h"
#include "IOptionsDlg.h"
#include "StyleDlgModelService.h"


namespace TA_Base_App
{
    std::vector<std::wstring> StyleSheetSynData::languages;
    std::vector<std::string> StyleSheetSynData::stylesheets;


    StyleDlgModel::StyleDlgModel(StyleDlgModelService* service)
        : AbstractModel(service)
    {
        setupModel(this);
    }


    void StyleDlgModel::initSyncData()
    {
        if (ui.languages.empty())
        {
            ui.languages = pm.languages = m_service->getLanguages();
        }

        ui.language_index = pm.language_index = 1;
        ui.style_index = pm.style_index = 3;
        ui.stylesheet_index = pm.stylesheet_index = 15;
    }


    void StyleDlgModel::beforeAttachView()
    {
        pm.stylesheets = m_service->getStyleSheets();
    }
}
