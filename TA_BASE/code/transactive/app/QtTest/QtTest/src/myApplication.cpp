#include "myApplication.h"
#include "myController.h"
#include "myViewFactory.h"
#include "bus/generic_gui_view/src/HelpLauncher.h"
#include "bus/generic_gui_view/src/TransActiveMessage.h"
#include "bus/generic_gui_view/src/TransActiveWinApp.h"
#include "bus/generic_gui_pmod/src/TransActiveViewManager.h"

#include "core/exceptions/src/GenericGUIException.h"
#include "core/exceptions/src/ProcessAlreadyRunningException.h"
#include "core/exceptions/src/CannotUseSysControllerException.h"
#include "core/exceptions/src/UserCancelException.h"

#include "core/threads/src/Thread.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/RunParams.h"
#include "core/utilities/src/TAAssert.h"
#include "core/message/src/MessagePublicationManager.h"
#include "core/message/src/AuditMessageSender.h"
#include "core/message/src/NameValuePair.h"
#include "core/message/types/ApplicationStatusAudit_MessageTypes.h"


#include "ace/ACE.h"


using TA_Base_Core::GenericGUIException;
using TA_Base_Core::DebugUtil;
using TA_Base_Core::RunParams;
using TA_Base_Bus::TransActiveWinApp;

namespace TA_Base_App
{

    myApplication::myApplication(int argc, char *argv[]) :TA_Base_Bus::TransActiveWinApp(argc,argv,TA_Base_Bus::MVT_DIALOG, true)
    {
        myViewFactory* vf=new myViewFactory(this);
        this->getViewManager()->setViewFactory(vf);
    }

    TA_Base_Bus::IGUIApplicationController * myApplication::createApplicationController()
    {
        return new myController(this, "myTestApp");
    }

    TA_Base_Bus::ITransActiveView* myApplication::createMainView()
    {
        return this->getViewManager()->getView("A","main");
    }

    TA_Base_Bus::ITransActiveView* myApplication::getMainView()
    {
          return this->getViewManager()->getMainView();
    }


    bool myApplication::runWithoutControlStation()
    {
        return true;
    }

} // namespace TA_Base_App
