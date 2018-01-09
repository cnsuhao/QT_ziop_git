/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/event/event_viewer/pmodel/src/EventViewerAppController.cpp $
  * @author:  
  * @version: $Revision:  $
  *
  * Last modification: $DateTime: $
  * Last modified by:  $Author: Shiva $
  *
  * This is the implementation of the 
  * GenericGUI IGUIApplicationController interfaces.
  */

#include "EventViewerAppController.h"
#include  "app/event/event_viewer/pmodel/src/ConstantItems.h"
#include  "app/event/event_viewer/pmodel/src/EventViewerDlgPModel.h"
#include  "app/event/event_viewer/pmodel/src/CombinedEventFilterPModel.h"
//#include  "app/event/event_viewer/pmodel/src/EventDisplayModel.h"

#include "bus/application_types/src/apptypes.h"


using TA_Base_Core::DebugUtil;
using TA_Base_Core::RunParams;
using TA_Base_Bus::SettingsMgr;
using namespace TA_Base_Bus;

namespace TA_Base_App
{

    EventViewerAppController::EventViewerAppController(TA_Base_Bus::ITransActiveWinApp * pApp, const char * appName)
       : TA_Base_Bus::AbstractGUIAppController(pApp, appName)
    {

    }

    EventViewerAppController::~EventViewerAppController()
    {

    }



    unsigned long EventViewerAppController::getApplicationType()
    {
	    return EVENT_GUI_APPTYPE;
    }


    void EventViewerAppController::allocAppResource()
    {
		
    }

    void EventViewerAppController::cleanup()
    {
		AbstractGUIAppController::cleanup();
	}
	
	
	void EventViewerAppController::prepareForClose()
	{
		EventViewerDlgPModel * pModel = dynamic_cast<EventViewerDlgPModel*> (getPModel(EVENT_VIEWER_DLG_PMODEL));

		pModel->preparaForClose();

		AbstractGUIAppController::prepareForClose();
	}


	TA_Base_Bus::ITransActivePModel * EventViewerAppController::createPModel( const std::string & pModelName )
	{
		 ITransActivePModel * pModel = NULL;
		// if (pModelName == EVENT_DISPLAY_PMODEL)
       // {
       //     pModel = new EventDisplayModel(this, pModelName.c_str());
       // }
		// else
		 if (pModelName == EVENT_VIEWER_DLG_PMODEL)
        {
            pModel = new EventViewerDlgPModel(this, pModelName.c_str());
        }
		 else if (pModelName == COMBINED_EVENT_FILTER_PMODEL)
        {
            pModel = new CombinedEventFilterPModel(this, pModelName.c_str());
        }
		 else
        {
            std::string errorDesc = "presentation model name is not valid: " + pModelName;
            TA_ASSERT(FALSE, errorDesc.c_str());
        }
		 return pModel;
	}	

} // namespace TA_Base_App
