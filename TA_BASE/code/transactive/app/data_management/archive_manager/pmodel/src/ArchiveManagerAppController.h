/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: $
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

#ifndef ARCHIVE_MANAGER_APP_CONTROLLER_H
#define ARCHIVE_MANAGER_APP_CONTROLLER_H

#include "bus/generic_gui_pmod/src/AbstractGUIAppController.h"


namespace TA_Base_App
{
    class ArchiveManagerAppController : public TA_Base_Bus::AbstractGUIAppController
    {

    public:

        /**
          * Constructor
          */
        ArchiveManagerAppController(TA_Base_Bus::ITransActiveWinApp * pApp, const char * appName);

        /**
          * Destructor
          */
        ~ArchiveManagerAppController();


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

		virtual  void checkEntity(TA_Base_Core::IEntityData* guiEntity);     
		virtual void serverIsDown() ;
		virtual void serverIsUp() ;
		
		/** save user settings to db
		 * 
		 *
		 *  @return void
		 */
		virtual void saveAllUserSettings();

		/** load user settings from db
		 * 
		 *
		 *  @return void
		 */
		virtual void loadAllUserSettings();

      
    private:

        // Copy constructor and operator= made private to prevent them being used.
        ArchiveManagerAppController(const ArchiveManagerAppController&);
        ArchiveManagerAppController& operator=(const ArchiveManagerAppController&);

    };

} // namespace TA_Base_App

#endif // ARCHIVE_MANAGER_APP_CONTROLLER_H
