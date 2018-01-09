#include "SystemManagerApp.h"
#include "SystemManagerDlg.h"
#include "pmodel\src\SystemManagerAppController.h"
#include "ace\ACE.h"
#include "core\utilities\src\DebugUtil.h"


namespace TA_Base_App
{

    SystemManagerApp::SystemManagerApp(int argc, char *argv[])
        :TA_Base_Bus::TransActiveWinApp( argc, argv, TA_Base_Bus::MVT_DIALOG, true )
    {			 
        ACE::init();
    }


    TA_Base_Bus::IGUIApplicationController * SystemManagerApp::createApplicationController()
    {
        return new SystemManagerAppController( this, "System Manager" );
    }


    TA_Base_Bus::ITransActiveView* SystemManagerApp::createMainView()
    {
        return new SystemManagerDlg( this );
    }


    TA_Base_Bus::ITransActiveView* SystemManagerApp::getMainView()
    {
        return NULL;
    }


    bool SystemManagerApp::runWithoutControlStation()
    {
        return false;
    }

}
