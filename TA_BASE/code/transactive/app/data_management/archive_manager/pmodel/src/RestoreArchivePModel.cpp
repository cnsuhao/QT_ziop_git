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

#include "app/data_management/archive_manager/pmodel/src/RestoreArchivePModel.h"
#include "app/data_management/archive_manager/pmodel/src/TransactiveUserMsgEventArgs.h"
#include "app/data_management/archive_manager/pmodel/src/GenericEventArgs.h"
#include "app/data_management/archive_manager/pmodel/src/GuiConstants.h"
#include "core/exceptions/src/ArchiveException.h"
#include "core/exceptions/src/TransactiveException.h"
#include "core/message/types/ArchiveAudit_MessageTypes.h"

namespace TA_Base_App
{

	RestoreArchivePModel::RestoreArchivePModel(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName)
		: TA_Base_Bus::AbstractPModel(appController, pmodelName),
		m_restoreArchiveHelper(appController->getEntity())
	{

	}

	RestoreArchivePModel::~RestoreArchivePModel(void)
	{//TODO check when called....
		try
        {
            terminateAndWait();
        }
        catch( TA_Base_Core::TransactiveException& e )
        {
            LOG_EXCEPTION_CATCH( SourceInfo, "TransactiveException", e.what() );
        }
        catch( ... )
        {
            LOG_EXCEPTION_CATCH( SourceInfo, "Unknown Exception", "Unknown exception caught in destructor." );
        }
	}

	void RestoreArchivePModel::run()
    {
		notifyViews(THREAD_FUNCTION_INVOKE.c_str());
    }

    
    void RestoreArchivePModel::terminate()
    {
        if ( TA_Base_Core::Thread::THREAD_STATE_RUNNING == getCurrentState() )
        {
            // If we are in the middle of restoring, we have to wait for the
            // day that is currently being restored to be fully restored, before
            // we terminate.

            if ( StateManager::PROCESSING == m_stateManager.getState() )
            {
				TransactiveUserMsgEventArgs args(IDS_UI_050052);
				notifyViews(SHOW_USER_MESSAGE_NOTIFY.c_str(),&args); 

                m_stateManager.setState( StateManager::STOPPING );
            }
            else if ( StateManager::STOPPING == m_stateManager.getState() )
            {
                // We're still waiting for the current day to finish restoring.
				TransactiveUserMsgEventArgs args(IDS_UI_050052);
				notifyViews(SHOW_USER_MESSAGE_NOTIFY.c_str(),&args); 
            }
        }

        // Wait until it has completed.
        // We could put a count in here so we don't get stuck, but how long should we count for?
        // Restoration of a single day could take an hour or more... we don't know.

        while( StateManager::IDLE != m_stateManager.getState() )
        {
            sleep( 1000 );  // 1 sec
        }
    }

	


    bool RestoreArchivePModel::restoreFiles( const time_t& restoredDate, 
        const std::string& volumeLabel )
    {
		// KT 27 Feb 04: PW #3133: This section has been changed so that users can remove
		// an individual days worth of restored data from the database.
		// KT 9 Mar 04: PW #3161 Converted to use time_t to remove dependence on date formatting.
		m_restoreArchiveHelper.restoreFilesForDate(restoredDate , volumeLabel, m_fileStatusMap );
		return true;        
    }
        

    bool RestoreArchivePModel::processLogFiles()
    {
        if ( m_fileStatusMap.empty() )
        {
            m_stateManager.setState( StateManager::IDLE );
			return true;
        }

        // We don't check if the status goes to STOPPING in here, because we want to complete
        // this whether the user has cancelled or not.

        // If there were no failures found in the log files, delete them.
        // Otherwise display the Restore Complete to show which files contained warnings/errors/non-successes.
    
        std::pair< TA_ArchiveManager::FileStatusMap::iterator, 
            TA_ArchiveManager::FileStatusMap::iterator > range(  
            m_fileStatusMap.equal_range( TA_ArchiveManager::EX_SUCC ) );           

        // Check if the imports were all successful.

        if( ( m_fileStatusMap.begin() == range.first ) && ( m_fileStatusMap.end() == range.second ) )
        {
            // If so, delete all the log files to stop them from accumulating.
			m_stateManager.setState( StateManager::IDLE );
			TA_ArchiveManager::deleteLogFiles( m_fileStatusMap );        
			return true;
        }
        else
        {
			return false;
        }
    }

	std::vector<std::string> RestoreArchivePModel::getCDDrives()
	{
		// Populate selected drive combo box
		std::vector<std::string> cdDrives;
        std::vector<std::string> drives = m_restoreArchiveHelper.getMediaLocations();
        for (std::vector<std::string>::iterator iter = drives.begin(); iter != drives.end(); iter++)
        {
			// marvin++
			// TD13391 
			//must be from cd
			if( GetDriveType((*iter).c_str()) == DRIVE_CDROM)
			{
				cdDrives.push_back(*iter);
			}
			// ++marvin
			// TD13391 
        }
		return cdDrives;
	}

	RestoreArchiveHelper::FileDetailsVector RestoreArchivePModel::getAllFileDetails(const std::string& drive)
	{
		 RestoreArchiveHelper::FileDetailsVector fileDeatils = m_restoreArchiveHelper.getAllFileDetails( drive );
		 if(m_restoreArchiveHelper.hasInvalidArchiveFile())
		 {		
			 TransactiveUserMsgEventArgs args(IDS_UW_050001);
			 notifyViews(SHOW_USER_MESSAGE_NOTIFY.c_str(),&args); 
		 }
		 return fileDeatils;
	}

}