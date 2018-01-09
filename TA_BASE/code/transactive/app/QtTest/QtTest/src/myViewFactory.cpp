
#include "myViewFactory.h"
#include "OptionsDlg.h"

namespace TA_Base_App
{

    myViewFactory::myViewFactory(TA_Base_Bus::ITransActiveWinApp* winApp): m_winApp(winApp)
    {
    }

    TA_Base_Bus::ITransActiveView * myViewFactory::createView(const char * viewType)
    {
        TA_Base_Bus::ITransActiveView * view= new OptionsDlg(m_winApp);
        view->setMainView(true);
        return view;
    }

}
