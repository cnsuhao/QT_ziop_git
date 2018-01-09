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

#if !defined(RETRIEVINGDATA_THREAD__HELPER_CLASS__1468A462_430F_4418_BC92_334001E5F340__INCLUDED_)
#define RETRIEVINGDATA_THREAD__HELPER_CLASS__1468A462_430F_4418_BC92_334001E5F340__INCLUDED_

#include "app/data_management/archive_manager/pmodel/src/WriteArchiveHelper.h"
#include "app/data_management/archive_manager/pmodel/src/resource.h"
#include "app/data_management/archive_manager/pmodel/src/StateManager.h"
#include "app/data_management/archive_manager/pmodel/src/WriteArchivePModel.h"
#include "core/threads/src/Thread.h"


namespace TA_Base_App
{
    class RetrievingDataThread : public TA_Base_Core::Thread
    {
    public:
			RetrievingDataThread(time_t fromDate, time_t toDate, WriteArchivePModel * pModel);
			virtual ~RetrievingDataThread();
       
         /**
          * run
          * 
          * Inherited from Thread.  This calls the initialisation and write methods.
          * By having a separate thread do this, we can allow user interaction to continue with
          * the GUI.  If Cancel is pressed, the thread will wait until Nero has decided 
          * that it is safe to cancel, then the process will be cancelled.
          *
          * This class inherits from Thread instead of having a separate class for it, because
          * the write process has lots of GUI interaction throughout.
          */

        virtual void run();

        /**
          * terminate
          *
          * If the thread is still running, it tells the thread to stop when Nero decides
          * it is safe to do so.
          */
        
        virtual void terminate();

		void stopThread();

		void deleteArchiveFiles();
		
		TA_ArchiveManager::FileStatusMap getFileStatusMap()
		{
			return m_fileStatusMap;
		}

		void setIdleState()
		{
			 m_stateManager.setState( StateManager::IDLE );
		}

    private:
		
		/**
          * processLogFiles
          *
          * If the log files from the retrieval process indicate that
          * the process was successful, this method deletes the log files.
          * Otherwise it displays a Retrieve Complete dialog listing all
          * the files that contain problems.  The user is then
          * responsible for deleting the log files.
          *
          * Any attempts to Cancel by the user during this method will be
          * ignored (or at least a message box will be displayed, and then
          * the method will continue).
          */

        void processLogFiles();

        
        //
        // Will be populated with the status and log file for each
        // set of data that is retrieved.
        //

        TA_ArchiveManager::FileStatusMap m_fileStatusMap;

       
        //
        // Manages the state of the GUI.
        //
        
        StateManager m_stateManager;

        //
        // The list of archive files (including the full path) that are generated.  
        // These are stored so that if the user cancels, we can delete the files.
        //

        WriteArchiveHelper::FilePathsVector m_archiveFiles;
		
		//
        // The object that does all the back end work.
        //

        WriteArchiveHelper m_writeArchiveHelper;

        //
        // The start and end dates of the data to retrieve.
        //

        time_t m_fromDate;
        time_t m_toDate;

		WriteArchivePModel * m_PModel;



	};
    
} // TA_Base_App


#endif // !defined(RETRIEVINGDATA_THREAD__HELPER_CLASS__1468A462_430F_4418_BC92_334001E5F340__INCLUDED_)
