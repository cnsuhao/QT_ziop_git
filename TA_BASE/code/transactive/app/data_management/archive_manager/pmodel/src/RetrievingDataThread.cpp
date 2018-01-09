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
  */

#include "app/data_management/archive_manager/pmodel/src/RetrievingDataThread.h"
#include "app/data_management/archive_manager/pmodel/src/GenericEventArgs.h"
#include "app/data_management/archive_manager/pmodel/src/TransactiveUserMsgEventArgs.h"
#include "app/data_management/archive_manager/pmodel/src/PostMessageEventArgs.h"
#include "app/data_management/archive_manager/pmodel/src/GuiConstants.h"
#include "app/data_management/archive_manager/pmodel/src/resource.h"
#include "bus\TA_MessageResource\resource.h"
#include "core/exceptions/src/ArchiveException.h"
#include "core/message/types/ArchiveAudit_MessageTypes.h"


namespace TA_Base_App
{
	RetrievingDataThread::RetrievingDataThread(time_t fromDate, time_t toDate, WriteArchivePModel * pModel)
		:m_fromDate(fromDate),m_toDate(toDate),m_PModel(pModel),m_writeArchiveHelper(pModel->getGuiEntity())
	{
		
	}

	RetrievingDataThread::~RetrievingDataThread()
	{
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

	void RetrievingDataThread::run()
    {
        // Make sure no exceptions escape from the run() method.

        try
        {
            m_stateManager.setState( StateManager::PROCESSING );

            // Determine all the dates in the range fromDate to toDate.

            time_t currentTime( m_fromDate );
            do
            {
                if ( StateManager::STOPPING == m_stateManager.getState() )
                {
                    m_toDate = currentTime - TA_ArchiveManager::ONE_DAY;
                    break;
                }
            
                // KT 9 Mar 04: PW #3161 Convert date to local format.

	              // m_date.SetWindowText( .c_str() ); TODO
				GenericEventArgs args;
				std::string dateStr = TA_ArchiveManager::convertTimeTToStringDate( currentTime );
				args.Param1 = &dateStr;
				m_PModel->notifyViews(SET_RETRIEVING_DATA_DATE_CTRL.c_str(),&args);

                try
                {
                    m_writeArchiveHelper.retrieveDataForDate( currentTime, m_fileStatusMap, m_archiveFiles );            
                }
                catch( TA_Base_Core::ArchiveException& e )
                {
					TransactiveUserMsgEventArgs args(IDS_UE_050139,e.what());		  
					m_PModel->notifyViews(SHOW_USER_MESSAGE_RETRIEVINGDATADLG.c_str(),&args); 

                    try
                    {
                        TA_ArchiveManager::deleteLogFiles( m_fileStatusMap );
                    }
                    catch( TA_Base_Core::ArchiveException& e )
                    {
                        // The error message contains a list of the files that couldn't be deleted and the reasons.

                        LOG_EXCEPTION_CATCH( SourceInfo, "ArchiveException", e.what() );
                        TA_ArchiveManager::sendAuditMessageIgnoreErrors( TA_Base_Core::ArchiveAudit::CopyArchiveFailed, e.what() ); 
						TransactiveUserMsgEventArgs args(IDS_UE_050138,e.what());		  
						m_PModel->notifyViews(SHOW_USER_MESSAGE_RETRIEVINGDATADLG.c_str(),&args); 
                    }
                    //PostMessage( WM_CLOSE_DLG, 0, IDABORT );
					PostMessageEventArgs postArgs(WM_CLOSE_DLG,IDABORT);
					m_PModel->notifyViews(POST_MESSAGE_TO_RETRIEVINGDATADLG.c_str(),&postArgs);
                    return;
                }
                currentTime += TA_ArchiveManager::ONE_DAY;

            } while( currentTime <= m_toDate );

            // This next bit HAS to be done after retrieveDataForDate has been called, so disable
            // the stop button, so the operator can't stop during this process.
            // (Even if they do somehow, a message box will be displayed, then the stop
            // will be ignored until this finishes).
            
            //PostMessage( WM_DISABLE_DLG, 0, 0 );
			PostMessageEventArgs args(WM_DISABLE_DLG);
			m_PModel->notifyViews(POST_MESSAGE_TO_RETRIEVINGDATADLG.c_str(),&args);	           
            
            // This will delete all the log files if every export was successful.
            // It will display the Restore Complete dialog if there were errors/warnings during the restore.
            // Either way, this method will set the state to IDLE and close the dialog.
            
            processLogFiles();
        }
        catch( TA_Base_Core::TransactiveException& e )
        {
            LOG_EXCEPTION_CATCH( SourceInfo, "TransactiveException", e.what() ); 
        }
        catch( ... )
        {
            LOG_EXCEPTION_CATCH( SourceInfo, "Unknown Exception", "Unknown exception caught in RestoreArchiveDlg::run()" ); 
        }
    }


    void RetrievingDataThread::terminate()
    {
        if ( TA_Base_Core::Thread::THREAD_STATE_RUNNING == getCurrentState() )
        {
	        // If we are in the middle of retrieving, we have to wait for the
            // day that is currently being restored to be fully retrieved, before
            // we stop.

            if ( StateManager::PROCESSING == m_stateManager.getState() )
            {
				TransactiveUserMsgEventArgs args(IDS_UI_050053); 
				m_PModel->notifyViews(SHOW_USER_MESSAGE_RETRIEVINGDATADLG.c_str(),&args); 
                m_stateManager.setState( StateManager::STOPPING );
            }
            else if ( StateManager::STOPPING == m_stateManager.getState() )
            {
                // We're still waiting for the current day to finish retrieving.
				TransactiveUserMsgEventArgs args(IDS_UI_050053); 
				m_PModel->notifyViews(SHOW_USER_MESSAGE_RETRIEVINGDATADLG.c_str(),&args); 
            }
        }

        // Wait until it has completed.
        // We could put a count in here so we don't get stuck, but how long should we count for?
        // Retrieval of a single day could take an hour or more... we don't know.

        while( StateManager::IDLE != m_stateManager.getState() )
        {
            sleep( 1000 );  // 1 sec
        }
    }

	void RetrievingDataThread::processLogFiles()
    {
        if ( m_fileStatusMap.empty() )
        {
            return;
        }

        // We don't check if the status goes to STOPPING in here, because we want to complete
        // this whether the user has cancelled or not.

        // If there were no failures found in the log files, delete them.
        // Otherwise display the Retrieve Complete to show which files contained warnings/errors/non-successes.
    
        std::pair< TA_ArchiveManager::FileStatusMap::iterator, 
            TA_ArchiveManager::FileStatusMap::iterator > range(  
            m_fileStatusMap.equal_range( TA_ArchiveManager::EX_SUCC ) );           

        // Check if the exports were all successful.

        if( ( m_fileStatusMap.begin() == range.first ) && ( m_fileStatusMap.end() == range.second ) )
        {
            // If so, delete all the log files to stop them from accumulating.

            try
            {
                TA_ArchiveManager::deleteLogFiles( m_fileStatusMap );
            }
            catch( TA_Base_Core::ArchiveException& e )
            {
                // The error message contains a list of the files that couldn't be deleted and the reasons.

                LOG_EXCEPTION_CATCH( SourceInfo, "ArchiveException", e.what() );
				TransactiveUserMsgEventArgs args(IDS_UE_050138,e.what()); 
				m_PModel->notifyViews(SHOW_USER_MESSAGE_RETRIEVINGDATADLG.c_str(),&args); 
            }
            //PostMessage( WM_CLOSE_DLG, 0, IDOK );
			PostMessageEventArgs args(WM_CLOSE_DLG,IDOK);
			m_PModel->notifyViews(POST_MESSAGE_TO_RETRIEVINGDATADLG.c_str(),&args);	
        }
        else
        {
            // They don't all contain successes, so show the dialog.
            // Do this by sending a message, so the windows thread handles the dialog.
            // If you don't do this, the thread shows the dialog and then aborts and
            // it debug asserts.

            //PostMessage( WM_COMPLETE_DLG, 0, 0 );
			
			PostMessageEventArgs args(WM_COMPLETE_DLG);
			m_PModel->notifyViews(POST_MESSAGE_TO_RETRIEVINGDATADLG.c_str(),&args);	
        }
    }

	void RetrievingDataThread::stopThread()
	{
		// If we are in the middle of retrieving, we have to wait for the
        // day that is currently being restored to be fully retrieved, before
        // we stop.

        if ( StateManager::PROCESSING == m_stateManager.getState() )
        {
			TransactiveUserMsgEventArgs args(IDS_UI_050053); 
			m_PModel->notifyViews(SHOW_USER_MESSAGE_RETRIEVINGDATADLG.c_str(),&args); 
            m_stateManager.setState( StateManager::STOPPING );
			PostMessageEventArgs postArgs(WM_DISABLE_DLG);
			m_PModel->notifyViews(POST_MESSAGE_TO_RETRIEVINGDATADLG.c_str(),&postArgs);	
        }
	}
	
	void RetrievingDataThread::deleteArchiveFiles()
	{
		try
		{
			m_writeArchiveHelper.deleteArchiveFiles( m_archiveFiles );
		}
		catch( TA_Base_Core::ArchiveException& e )
		{
			LOG_EXCEPTION_CATCH( SourceInfo, "ArchiveException", e.what() );  
			TA_ArchiveManager::sendAuditMessageIgnoreErrors( TA_Base_Core::ArchiveAudit::CopyArchiveFailed, e.what() ); 
			TransactiveUserMsgEventArgs args(IDS_UE_050138,e.what()); 
			m_PModel->notifyViews(SHOW_USER_MESSAGE_RETRIEVINGDATADLG.c_str(),&args); 
		}
	}	
}