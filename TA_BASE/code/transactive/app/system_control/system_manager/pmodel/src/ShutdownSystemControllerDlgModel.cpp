#include "ShutdownSystemControllerDlgModel.h"
#include "IShutdownSystemControllerDlg.h"
#include "ShutdownSystemControllerDlgModelService.h"


namespace TA_Base_App
{

    ShutdownSystemControllerDlgModel::ShutdownSystemControllerDlgModel(ShutdownSystemControllerDlgModelService* service)
        : AbstractModel(service)
    {
        setupModel(this);
    }


    void ShutdownSystemControllerDlgModel::beforeAttachView()
    {
        pm.hostname = m_service->getHostname();
    }


    void ShutdownSystemControllerDlgModel::onAccept()
    {
        AbstractModel::onAccept();
        m_service->onShutdownSystemController(pm.hostname);
    }
}
