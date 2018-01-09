#ifndef SYSTEM_MANAGER_APP_INCLUDED
#define SYSTEM_MANAGER_APP_INCLUDED

#include "bus/generic_gui_view/src/TransActiveWinApp.h"


namespace TA_Base_App
{
    class SystemManagerApp : public TA_Base_Bus::TransActiveWinApp
    {
    public:

        SystemManagerApp( int argc, char *argv[] );
        TA_Base_Bus::IGUIApplicationController * createApplicationController();
        virtual TA_Base_Bus::ITransActiveView* getMainView();
        virtual TA_Base_Bus::ITransActiveView* createMainView();

    private:

        bool runWithoutControlStation();
    };
}


#endif
