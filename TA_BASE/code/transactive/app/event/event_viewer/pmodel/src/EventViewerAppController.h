/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/event/event_viewer/pmodel/src/EventViewerAppController.h $
  * @author:  
  * @version: $Revision:  $
  *
  * Last modification: $DateTime:  $
  * Last modified by:  $Author: Shiva $
  *
  * This is the implementation of the 
  * GenericGUI IGUIApplicationController interfaces.
  *
  */

#ifndef EVENT_VIEWER_APP_CONTROLLER_H
#define EVENT_VIEWER_APP_CONTROLLER_H

#include "bus/generic_gui_pmod/src/AbstractGUIAppController.h"


namespace TA_Base_App
{
    class EventViewerAppController : public TA_Base_Bus::AbstractGUIAppController
    {

    public:

        /**
          * Constructor
          */
        EventViewerAppController(TA_Base_Bus::ITransActiveWinApp * pApp, const char * appName);

        /**
          * Destructor
          */
        ~EventViewerAppController();


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

		/**
		* prepareForClose
		*
		* This method is used to prepare the GUI for closing. It should save the user settings
		* and perform any other tidying up. eg Stop any processing threads, ask the user if they
		* wish to save anything etc.
		*/
		void prepareForClose();
       

      
    private:

        // Copy constructor and operator= made private to prevent them being used.
        EventViewerAppController(const EventViewerAppController&);
        EventViewerAppController& operator=(const EventViewerAppController&);

    };

} // namespace TA_Base_App

#endif // EVENT_VIEWER_APP_CONTROLLER_H
