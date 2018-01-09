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

#include "app/data_management/archive_manager/pmodel/src/WriteArchivePModel.h"
#include "app/data_management/archive_manager/pmodel/src/GuiConstants.h"
#include "app/data_management/archive_manager/pmodel/src/RetrievingDataThread.h"
#include "app/data_management/archive_manager/pmodel/src/resource.h"
#include "app/data_management/archive_manager/pmodel/src/GuiConstants.h"
#include "app/data_management/archive_manager/pmodel/src/TransactiveUserMsgEventArgs.h"
#include "app/data_management/archive_manager/pmodel/src/PostMessageEventArgs.h"
#include "app/data_management/archive_manager/pmodel/src/GenericEventArgs.h"
#include "bus\TA_MessageResource\resource.h"
#include "core/exceptions/src/ArchiveException.h"
#include "core/message/types/ArchiveAudit_MessageTypes.h"
#include "core/utilities/src/RunParams.h"

namespace TA_Base_App
{

	WriteArchivePModel::WriteArchivePModel(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName)
		:	TA_Base_Bus::AbstractPModel(appController, pmodelName),
			m_writeArchiveHelper(appController->getEntity()),
			m_guiEntity(appController->getEntity()),
			m_isManualArchive(false)			
	{
		time( &m_fromDate );
        m_fromDate -= TA_ArchiveManager::ONE_DAY;  // Yesterday
        m_toDate = m_fromDate;
		m_pRetrievingThread = NULL;
	}

	WriteArchivePModel::~WriteArchivePModel(void)
	{
	}

	void WriteArchivePModel::setSelectedDataDateRange(time_t fromDate, time_t toDate, UINT volumeSize)
	{
		WriteArchiveHelper::setMaxDvdSize(volumeSize);
		m_fromDate = fromDate;
		m_toDate = toDate;		
	}

	std::string WriteArchivePModel::getRetrievingDateRangeString()
	{
		std::stringstream range;
        range << RetrievingData_DateRange_str;
        range << TA_ArchiveManager::convertTimeTToStringDate( m_fromDate );
        if ( m_fromDate != m_toDate )
        {
            range << to_str;
            range << TA_ArchiveManager::convertTimeTToStringDate( m_toDate );
        }
		return range.str();
	}

	void WriteArchivePModel::startRetrievingThread()
	{
		if(m_pRetrievingThread == NULL)
		{
			m_pRetrievingThread = new RetrievingDataThread(m_fromDate,m_toDate,this);
		}
		m_pRetrievingThread->start();
	}

	void WriteArchivePModel::stopRetrievingThread()
	{
		if(m_pRetrievingThread != NULL)
		{
			m_pRetrievingThread->stopThread();
		}
	}

	void WriteArchivePModel::setStateManagerIdle()
	{
		if(m_pRetrievingThread != NULL)
		{
			m_pRetrievingThread->setIdleState();
		}
	}

	void WriteArchivePModel::TerminateRetrievingThread()
	{
		if(m_pRetrievingThread != NULL)
		{
			delete m_pRetrievingThread;
			m_pRetrievingThread = NULL;
		}
	}

	void WriteArchivePModel::deleteRetrievedArchiveFiles()
	{
		if(m_pRetrievingThread != NULL)
		{
			m_pRetrievingThread->deleteArchiveFiles();
		}			
	}

	TA_ArchiveManager::FileStatusMap WriteArchivePModel::getRetrievingFileStatusMap()
	{
		return m_pRetrievingThread->getFileStatusMap(); 
	}

	bool WriteArchivePModel::getFilesDeatilsVector(WriteArchiveHelper::FileDetailsVector& fileDetails)
	{
		 // Get the dates, names and sizes of the files in the archive directory.
        std::vector<std::string> archiveDirs;
        try
        {
            // If it is a manual archive, the date range will be limited.

            archiveDirs = m_writeArchiveHelper.getArchiveDirectory( m_isManualArchive );

            // Grab the first set of file details
            if ( m_isManualArchive )
            {
                fileDetails = m_writeArchiveHelper.getAllFileDetails( archiveDirs[0], m_fromDate, m_toDate );
            }
            else
            {
                fileDetails = m_writeArchiveHelper.getAllFileDetails( archiveDirs[0] );
            }

            //  Now if there are any more loop through and append them to the vector
            for (unsigned int i =1; i < archiveDirs.size(); ++i)
            {
                WriteArchiveHelper::FileDetailsVector extraFileDetails = m_writeArchiveHelper.getAllFileDetails( archiveDirs[i] );

                fileDetails.insert( fileDetails.end(), extraFileDetails.begin(), extraFileDetails.end() );
            }
        }
        catch( TA_Base_Core::ArchiveException& e )
        {
            LOG_EXCEPTION_CATCH( SourceInfo, "ArchiveException", e.what() );
            TA_ArchiveManager::sendAuditMessageIgnoreErrors( TA_Base_Core::ArchiveAudit::CopyArchiveFailed, e.what() ); 
            
			TransactiveUserMsgEventArgs args(IDS_UE_050141,e.what());
			notifyViews(SHOW_USER_MESSAGE_NOTIFY.c_str(),&args); 

            return false;
        }
        if ( fileDetails.empty() )
        {
            // KT 10 Mar 04: PW #3136 - Changed '-' to '_' for operating system compatibility.

			std::string error;
            if ( m_isManualArchive )
            {
				std::string files;
                for(unsigned int i =0; i < archiveDirs.size(); ++i)
                {
                    files += archiveDirs[i].c_str();
                }

                std::string startTime = TA_ArchiveManager::convertTimeTToStringDate( m_fromDate );
                std::string endTime = TA_ArchiveManager::convertTimeTToStringDate( m_toDate );
                error= IGuiObserver::getInstance()->getStringResourceFormated( UW050013_NO_MAN_ARCHIVES, files, startTime, endTime);
				TransactiveUserMsgEventArgs args(IDS_UW_050013,files,startTime,endTime);
				notifyViews(SHOW_USER_MESSAGE_NOTIFY.c_str(),&args); 
            }
            else
            {
				std::string files;
                for(unsigned int i =0; i < archiveDirs.size(); ++i)
                {
                    files += archiveDirs[i].c_str();
                    
                    if (i < archiveDirs.size() - 1)
                    {
                        // For all items but the last one append a comma
                        files += ", ";
                    }
                }

                error = IGuiObserver::getInstance()->getStringResourceFormated( UW050014_NO_AUTO_ARCHIVES,  files);
				TransactiveUserMsgEventArgs args(IDS_UW_050014,files);
				notifyViews(SHOW_USER_MESSAGE_NOTIFY.c_str(),&args); 
            }
            TA_ArchiveManager::sendAuditMessageIgnoreErrors( TA_Base_Core::ArchiveAudit::CopyArchiveFailed, error ); 
            return false;
        }

		// Split the files into the CDs that they will be burnt to.
        // Then we know how many CDs it will take.

        try
        {
            m_cdsMap = m_writeArchiveHelper.splitIntoCds( fileDetails );            
        }
        catch( TA_Base_Core::ArchiveException& e )
        {
            LOG_EXCEPTION_CATCH( SourceInfo, "ArchiveException", e.what() );
            TA_ArchiveManager::sendAuditMessageIgnoreErrors( TA_Base_Core::ArchiveAudit::CopyArchiveFailed, e.what() );            
			TransactiveUserMsgEventArgs args(IDS_UE_050142,e.what());
			notifyViews(SHOW_USER_MESSAGE_NOTIFY.c_str(),&args); 
            return false;
        }
		return true;
	}

	 void WriteArchivePModel::cleanUpArchive()
    {
        // Delete all the files that are left if this is a manual archive.

        if ( m_isManualArchive )
        {
            WriteArchiveHelper::CdsMap::iterator it;
            while( !m_cdsMap.empty() )
            {
                m_writeArchiveHelper.deleteArchiveFiles( m_cdsMap.begin()->second.second );
                m_cdsMap.erase( m_cdsMap.begin() );
            }
        }
    }

	 bool WriteArchivePModel::isCDsMapEmpty()
	 {
		 // Send an audit message.		
		try
		{
            TA_ArchiveManager::sendAuditMessage( TA_Base_Core::ArchiveAudit::CopyArchiveRequest, m_cdsMap.begin()->first, m_isManualArchive );
        }
        catch( TA_Base_Core::ArchiveException& e )
        {
            LOG_EXCEPTION_CATCH( SourceInfo, "ArchiveException", e.what() );
			return true; //TODO check
        }
		
        // If there are no more CDs to write, clean up and close the dialog.

        if ( m_cdsMap.empty() )
        {
            cleanUpArchive();	    
           return true;
        }
		return false;

	 }

	 void WriteArchivePModel::getBurnFileDetails(std::string& volumeLabel, std::string& cdLabel, WriteArchiveHelper::FileDetailsVector& files)
	 {
		 volumeLabel = m_cdsMap.begin()->first;
		 cdLabel = m_cdsMap.begin()->second.first;
		 files = m_cdsMap.begin()->second.second;
	 }

	 void WriteArchivePModel::deleteFirstCdMapEntry()
	 {
		 m_writeArchiveHelper.deleteArchiveFiles( m_cdsMap.begin()->second.second );
		 for( unsigned int i = 0; i < m_cdsMap.begin()->second.second.size(); i++ )
		 {			
			 notifyViews(DELETE_WRITE_LIST_CTRL.c_str());
		 }
		 m_cdsMap.erase( m_cdsMap.begin() );
	 }

	 int WriteArchivePModel::getCdMapsSize()
	 {
		 return  m_cdsMap.size();
	 }

}