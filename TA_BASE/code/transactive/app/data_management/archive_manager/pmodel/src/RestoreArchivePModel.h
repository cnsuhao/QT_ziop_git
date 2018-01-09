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
#ifndef RESTORE_ARCHIVE_DLG__PMODEL_1234567890_H
#define RESTORE_ARCHIVE_DLG__PMODEL_1234567890_H

#include "bus/generic_gui_pmod/src/transactiveviewmodel.h"
#include "app/data_management/archive_manager/pmodel/src/resource.h"
#include "app/data_management/archive_manager/pmodel/src/RestoreArchiveHelper.h"
#include "app/data_management/archive_manager/pmodel/src/StateManager.h"
#include "core/synchronisation/src/ReEntrantThreadLockable.h"
#include "core/threads/src/Thread.h"

#include <map>
#include <string>

namespace TA_Base_App
{
	class RestoreArchivePModel : public TA_Base_Bus::AbstractPModel, public TA_Base_Core::Thread
	{
	public:

		RestoreArchivePModel(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName);
		virtual ~RestoreArchivePModel(void);

		/**
		* run
		* 
		* Inherited from Thread.  This performs the restoration when requested in OnRestore().
		* By having a separate thread do this, we can allow user interaction to continue with
		* the GUI.  If Cancel is pressed, the thread will finish restoring the data for the
		* current date and then terminate.
		*
		* This class inherits from Thread instead of having a separate class for it, because
		* the restoration process has lots of user/GUI interaction throughout.
		*/

		virtual void run();

		/**
		* terminate
		*
		* If the thread is still running, it tells the thread to stop after it has finished
		* importing the current date's data.
		*/

		virtual void terminate();

		/**
		* restoreFiles
		*
		* Restores the files listed in the vector.  This includes adding an entry
		* to the AR_INVENTORY table.  The archives are restored in date order.
		* If the user cancels during the operation, the rest of the files
		* for the date that is being restored will be restored and then this method
		* will return.  This prevents data corruption.
		*
		* @return     bool
		*             True if the process runs to completion.
		*             False if the user cancels midway through.
		*
		* @param      const RestoreArchiveHelper::DatesIndexMap& restoredDatesMap
		*             The dates to restore in chronological order.
		* @param      const std::string& volumeLabel
		*             The volume label to use in the AR_INVENTORY table.
		*/

		bool restoreFiles( const time_t& restoredDate, const std::string& volumeLabel );

		/**
		* processLogFiles
		*
		* If the log files from the restoration process indicate that
		* the process was successful, this method deletes the log files.
		* Otherwise it displays a Restore Complete dialog listing all
		* the files that contain problems.  The user is then
		* responsible for deleting the log files.
		*
		* Any attempts to Cancel by the user during this method will be
		* ignored (or at least a message box will be displayed, and then
		* the method will continue).
		*/

		bool processLogFiles();

		std::vector<std::string> getCDDrives();

		RestoreArchiveHelper::FileDetailsVector getAllFileDetails(const std::string& drive);

		TA_ArchiveManager::FileStatusMap getFileStatusMap()
		{
			return m_fileStatusMap;
		}

		void setStateManagerState(StateManager::EState stateId)
		{
			m_stateManager.setState( stateId );
		}

		StateManager::EState getStateManagerState()
		{
			return m_stateManager.getState() ;
		}

		bool hasInvalidArchiveFile()
		{
			return m_restoreArchiveHelper.hasInvalidArchiveFile();
		}

	private:
		//
		// Map of the archive date to the file name (including the full path).
		//

		typedef std::multimap< std::string, std::string > DateToFileMap;    

		//
		// The object that does all the back end work.
		//

		RestoreArchiveHelper m_restoreArchiveHelper;

		//
		// Will be populated with the status and log file for each
		// set of data that is restored.
		//

		TA_ArchiveManager::FileStatusMap m_fileStatusMap;

		//
		// Manages the state of the GUI.
		//

		StateManager m_stateManager;

	};
}


#endif //RESTORE_ARCHIVE_DLG__PMODEL_1234567890_H