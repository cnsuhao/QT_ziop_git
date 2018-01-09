/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/alarm/alarm_manager/Pmodel/src/AlarmManagerAppController.h $
  * @author:  Justin Ebedes
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2012/02/06 16:15:14 $
  * Last modified by:  $Author: haijun.li $
  *
  * This is the alarm Manager implementation of the 
  * GenericGUI IGUIApplicationController interfaces.
  *
  */

#ifndef AlarmManagerAppController_H
#define AlarmManagerAppController_H

#include "bus/generic_gui_pmod/src/AbstractGUIAppController.h"
#include <string>

 

namespace TA_Base_App
{
    

    class AlarmManagerAppController : public TA_Base_Bus::AbstractGUIAppController
    {

    public:

        /**
          * Constructor
          */
        AlarmManagerAppController(TA_Base_Bus::ITransActiveApp * pApp, const char * appName);

        /**
          * Destructor
          */
        ~AlarmManagerAppController();


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
		void prepareAppControllerRun();

    private:
       
    private:

        // Copy constructor and operator= made private to prevent them being used.
        AlarmManagerAppController(const AlarmManagerAppController&);
        AlarmManagerAppController& operator=(const AlarmManagerAppController&);

    private:
        
    };

} // namespace TA_Base_App

#endif // AlarmManagerAppController_H
