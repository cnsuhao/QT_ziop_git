#include "ControlStationRootDlg.h"
#include <QStyleFactory>
#include <boost/foreach.hpp>
#include <boost/preprocessor/cat.hpp>


namespace TA_Base_App
{
    ControlStationRootDlg::ControlStationRootDlg(TA_Base_Bus::ITransActiveApp* app, ControlStationRootDlgModel* model)
        : TA_Base_Bus::AbstractTransActiveDialog(app, "main", NULL),
          m_model(model)
    {
        ui.setupUi(this);
    }
}
