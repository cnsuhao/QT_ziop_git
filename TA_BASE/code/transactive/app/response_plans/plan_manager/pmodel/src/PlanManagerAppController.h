//
// The source code in this file is the property of
// Ripple Systems and is not for redistribution
// in any form.
//
// Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/response_plans/plan_manager/src/PlanManagerAppController.h $
// @author:  Bart Golab
// @version: $Revision: #2 $
//
// Last modification: $DateTime: 2012/02/06 16:15:14 $
// Last modified by:  $Author: haijun.li $
//
// <description>

#ifndef PLANMANAGERMANAGER_APP_CONTROLLER_H
#define PLANMANAGERMANAGER_APP_CONTROLLER_H


#include "bus/generic_gui_pmod/src/AbstractGUIAppController.h"


namespace TA_Base_Bus
{
    class ITransActiveApp;
}

namespace TA_Base_App
{
    /////////////////////////////////////////////////////////////////////////////
    // PlanManagerAppController

    class PlanManagerAppController : public TA_Base_Bus::AbstractGUIAppController
    {
    // Operations
    public:
        PlanManagerAppController(TA_Base_Bus::ITransActiveApp * pWinApp, const char * applicatoinName);
        ~PlanManagerAppController();

        /**
          * getApplicationType
          * 
          * This returns the application type for this GUI. GenericGUI will then
          * save this in the RunParams and will use it to pass to the Control Station. 
          * In the existing system the application type would need to be hard-coded 
          * into here. This method must be implemented by the lowest level class.
          *
          * @return unsigned long representing the application type
          *
          */
	    virtual unsigned long getApplicationType();

        virtual TA_Base_Bus::ITransActivePModel * createPModel(const std::string & pModelName);

	protected:
		void processWorkflowDuringAppInit(); //SEPTODO?
		void prepareAppControllerRun(); //SEPTODO?
		void onRunParamChange(const std::string& name, const std::string& value);

	private:
		// Copy constructor and operator= made private to prevent them being used.
		PlanManagerAppController(const PlanManagerAppController&);
		PlanManagerAppController& operator=(const PlanManagerAppController&);

        ///////////////////////////////////////////////////////////////////////////////
        // SEPTODO: Check same methods used in AbstractGUIAppController
		//// AbstractGUIApplication operations
        ////SEP? virtual void createApplicationObject();
        //// TD 11846 Fix: Changed setPosition's signature to follow AbstractGUIApplication's setPosition's signature.
		//SEP: Called from GenericGUI::setWindowPosition
        virtual void setMainViewPosition(const unsigned long posFlag, const unsigned long alignFlag, const RECT& objectDim, const RECT& boundaryDim );

        ///////////////////////////////////////////////////////////////////////////////
        //// IGUIApplication operations
        virtual void entityChanged(const std::vector<std::string>& changes); //SEP:  Called by GenericGUI::processUpdate(


    };

} // namespace TA_Base_App

#endif // PLANMANAGERMANAGER_APP_CONTROLLER_H
