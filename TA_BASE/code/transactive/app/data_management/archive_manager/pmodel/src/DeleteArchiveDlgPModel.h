/**
* The source code in this file is the property of
* Ripple Systems and is not for redistribution
* in any form.
*
* Source:   $File:  $
* @author:  
* @version: $Revision:  $
*
* Last modification: $DateTime:  $
* Last modified by:  $Author:  $
*
* 
*
*/
#ifndef DELETE_ARCHIVE_DLG___PMODEL_1234567890_H
#define DELETE_ARCHIVE_DLG___PMODEL_1234567890_H

#include "bus/generic_gui_pmod/src/transactiveviewmodel.h"
#include "app/data_management/archive_manager/pmodel/src/resource.h"
#include "app/data_management/archive_manager/src/DeleteArchiveHelper.h"
#include "app/data_management/archive_manager/pmodel/src/StateManager.h"
#include "core/synchronisation/src/ReEntrantThreadLockable.h"
#include "core/threads/src/Thread.h"

#include <map>
#include <string>

namespace TA_Base_App
{
	class DeleteArchiveDlgPModel : public TA_Base_Bus::AbstractPModel, public TA_Base_Core::Thread
	{
	public:

		DeleteArchiveDlgPModel(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName);
		virtual ~DeleteArchiveDlgPModel(void);

		/**
          * run
          * 
          * Inherited from Thread.  This performs the deletion when requested in OnRemoveData().
          * By having a separate thread do this, we can allow user interaction to continue with
          * the GUI.  If Cancel is pressed, the thread will finish removing the data for the
          * current date and then terminate.
          *
          * This class inherits from Thread instead of having a separate class for it, because
          * the deletion process has lots of user/GUI interaction throughout.
          */

        virtual void run();

        /**
          * terminate
          *
          * If the thread is still running, it tells the thread to stop after it has finished
          * removing the current date's data.
          */
        
        virtual void terminate();	      



	private:
		 
		//
        // Will be populated with the map of archives from the inventory.
        //
        
        TA_Base_Core::IArchiveInventory::ArchiveInventoryItemMap m_archives;

        //
        // The object that does all the back end work.
        //

        DeleteArchiveHelper m_deleteArchiveHelper;

        //
        // Manages the state of the GUI.
        //
        
        StateManager m_stateManager;
        
        //
        // The value (and the associated message) which records whether:
        // 1. The user has cancelled, or 
        // 2. If the process failed, or 
        // 3. If the process was successful.
        //

        int m_status;
        std::stringstream m_statusMessage;
		
	};
}


#endif //DELETE_ARCHIVE_DLG___PMODEL_1234567890_H