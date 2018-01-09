/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/system_control/system_manager/src/AlarmListControlController.h $
  * @author:  LUO HUIRONG
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/12/09 16:15:14 $
  * Last modified by:  $Author: LUO HUIRONG $
  *
  * This is the alarm list control implementation of the 
  * GenericGUI IGUIApplicationController interfaces.
  *
  */

#ifndef AlarmListControlController_H
#define AlarmListControlController_H

#include "bus/generic_gui_pmod/src/AbstractLibraryAppController.h"
#include <string>

 
namespace TA_Base_Bus
{
    //class SessionHandler;

    class AlarmListControlController : public TA_Base_Bus::AbstractLibraryAppController
    {

    public:

        /**
          * Constructor
          */
        AlarmListControlController(TA_Base_Bus::ITransActiveApp * pApp, const char * appName);

        /**
          * Destructor
          */
        ~AlarmListControlController();


        

        virtual TA_Base_Bus::ITransActivePModel * createPModel(const std::string & pModelName);

    protected:
        void cleanup();

        void allocAppResource();
       
     
		void prepareAppControllerRun();
    private:
         
    private:

        // Copy constructor and operator= made private to prevent them being used.
        AlarmListControlController(const AlarmListControlController&);
        AlarmListControlController& operator=(const AlarmListControlController&);

    private:
        /**
          * The instance of the session handler we use to connect
          * to the system controller and/or database
          */
        //SessionHandler* m_sessionHandler;  
       

		 

    };

} // namespace TA_Base_App

#endif // AlarmListControlController_H
