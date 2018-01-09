//
// The source code in this file is the property of
// Ripple Systems and is not for redistribution
// in any form.
//
// Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/response_plans/plan_manager/src/PlanManagerFrameModel.h $
// @author:  Bart Golab
// @version: $Revision: #5 $
//
// Last modification: $DateTime: 2012/10/08 17:44:33 $
// Last modified by:  $Author: qi.huang $
//
#pragma once

#include <map>
#include <string>

#include "bus/generic_gui_pmod/src/TransActiveViewModel.h"

namespace TA_Base_App
{
    class PlanManagerCorbaDefImpl;

    /////////////////////////////////////////////////////////////////////////////
    // The main frame of the application

    class PlanManagerFrameModel 
		: public TA_Base_Bus::AbstractPModel
    {
    public:
        PlanManagerFrameModel(TA_Base_Bus::IApplicationController* appController, const char * pmodelName);
		virtual ~PlanManagerFrameModel();
	public:
        /**
          * notifyUpdate
          * 
          * Calls the notifyUpdate() function to retrieve the 
          * up-to-date process data.
          */
        virtual void notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args=NULL);
		//virtual void sessionIdChanged();

        //void initCorbaObj();
		std::string checkLocations();
		int activatePManagerServant();
		std::string registerStationAgent(int iIndex);
		bool isStepServantActive();
		void registerRunParam();
		std::string getRunParam(const char* name);
		void setRunRaram(const char* name, const char* value);
		std::string getDefaultPlanAgentLocationName();
		void setBaseTitle(std::string strTitle);
		std::string getTitle();
		std::string updateTitle(std::string strTitle);
		void cleanUp();
		void setTest();

		// Error Message Handler
		void sendErrorMsgToUser(const unsigned int msgId, const unsigned int msgType, const std::string param1, const std::string param2);
		bool canEditCategories();
		bool canEditPlans();
		bool canApprovePlans();
		bool canCustomisePlans();
		bool canControlPlans();
		bool canSchedulePlans();

    // Attributes
    private:
        //TA_Base_Bus::AbstractGUIApplication* m_appGUI; //SEPTODO
        std::string m_strBaseTitle;
    };

} // namespace TA_Base_App
