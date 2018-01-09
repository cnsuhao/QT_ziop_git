/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/system_control/system_manager/src/SystemManagerAppController.h $
  * @author:  Justin Ebedes
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2012/02/06 16:15:14 $
  * Last modified by:  $Author: haijun.li $
  *
  * This is the System Manager implementation of the 
  * GenericGUI IApplicationController interfaces.
  *
  */

#ifndef SYSTEMMANAGER_APP_CONTROLLER_H
#define SYSTEMMANAGER_APP_CONTROLLER_H

#include "bus/generic_gui_pmod/src/AbstractGUIAppController.h"
#include <string>

namespace TA_Base_Bus
{
    class ISecuritySubsystemAccessor;
	class ISystemControllerAccessor;
}

namespace TA_Base_App
{
    //class SessionHandler;

    class SystemManagerAppController : public TA_Base_Bus::AbstractGUIAppController
    {

    public:

        /**
          * Constructor
          */
        SystemManagerAppController(TA_Base_Bus::ITransActiveWinApp * pApp, const char * appName);

        /**
          * Destructor
          */
        ~SystemManagerAppController();


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
        void cleanup();

        void allocAppResource();
       
        void processWorkflowDuringAppInit();
		void prepareAppControllerRun();
    private:
        void getRunParamFromSystemController();

		bool getRunParamFromSystemController( TA_Base_Core::RunParams::ParamVector & vtRunParam );
		void doLoginOperation();
		std::string checkConsoleName();
    private:

        // Copy constructor and operator= made private to prevent them being used.
        SystemManagerAppController(const SystemManagerAppController&);
        SystemManagerAppController& operator=(const SystemManagerAppController&);

    private:
        /**
          * The instance of the session handler we use to connect
          * to the system controller and/or database
          */
        //SessionHandler* m_sessionHandler;  
       

		TA_Base_Bus::ISystemControllerAccessor * m_pSystemControllerAccessor;

    };

} // namespace TA_Base_App

#endif // SYSTEMMANAGERGUI_H
