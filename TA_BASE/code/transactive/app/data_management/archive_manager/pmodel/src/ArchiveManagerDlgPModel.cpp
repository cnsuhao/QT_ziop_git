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
#include "app/data_management/archive_manager/pmodel/src/ArchiveManagerDlgPModel.h"
#include "app/data_management/archive_manager/pmodel/src/resource.h"
#include "bus\TA_MessageResource\resource.h"
#include "app/data_management/archive_manager/pmodel/src/GuiConstants.h"
#include "app/data_management/archive_manager/pmodel/src/TransactiveUserMsgEventArgs.h"
#include "app/data_management/archive_manager/pmodel/src/PostMessageEventArgs.h"
#include "bus/security/access_control/actions/src/AccessControlledActions.h"
#include "core/data_access_interface/entity_access/src/ArchiveManagerEntityData.h"
#include "core/exceptions/src/ArchiveException.h"
#include "core/message/types/ArchiveAudit_MessageTypes.h"

namespace TA_Base_App
{
	ArchiveManagerDlgPModel::ArchiveManagerDlgPModel(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName)
		: TA_Base_Bus::AbstractPModel(appController, pmodelName),
		m_isServerUp( true ),
		m_isAutomaticButtonEnabled(false),
		m_isManualButtonEnabled(false),
		m_isRestoreButtonEnabled(false),
		m_isDeleteButtonEnabled(false)
	{

	}

	ArchiveManagerDlgPModel::~ArchiveManagerDlgPModel(void)
	{

	}

	void ArchiveManagerDlgPModel::init()
	{
		TA_Base_Core::RunParams::getInstance().registerRunParamUser(this, RPARAM_SESSIONID);
	}

	void ArchiveManagerDlgPModel::onRunParamChange(const std::string& name, const std::string& value)
	{
		if ( 0 == name.compare(RPARAM_SESSIONID) )
		{
			//PostMessage( WM_SESSIONID_CHANGE, 0, 0 );     
			PostMessageEventArgs args(WM_SESSIONID_CHANGE);
			notifyViews(POST_MESSAGE_INVOKE.c_str(),&args);	
		}
	} 

	 void ArchiveManagerDlgPModel::setServerStatus( bool isServerUp )
    {
        m_isServerUp = isServerUp;
       // PostMessage( WM_SERVER_STATUS_CHANGE, 0, 0 );
		PostMessageEventArgs args(WM_SERVER_STATUS_CHANGE);
		notifyViews(POST_MESSAGE_INVOKE.c_str(),&args);	
    }

	bool ArchiveManagerDlgPModel::canRestoreArchives()
	{
		m_messageLevel = TA_ArchiveManager::ERR;
		m_strMsgForUser = "";
		// Send an audit message.
		try
		{
			if( !TA_ArchiveManager::isActionPermitted( TA_Base_Bus::aca_ARCHIVE_RESTORE ) )
			{
				// UE-050034
				std::string error;
				try
				{						
					error = IGuiObserver::getInstance()->getStringResourceFormated( UE050034_INSUF_PRIV_RESTORE,
						TA_ArchiveManager::getOperatorNameFromSessionId(),
						TA_ArchiveManager::getWorkstationNameFromSessionId());
				}
				catch( TA_Base_Core::ArchiveException& e )
				{
					LOG_EXCEPTION_CATCH( SourceInfo, "ArchiveException", e.what() );
					error= IGuiObserver::getInstance()->getStringResource(  UE050122_INSUF_PRIV_RESTORE );
				}
				TA_ArchiveManager::sendAuditMessageIgnoreErrors( TA_Base_Core::ArchiveAudit::RestoreArchiveFailed, std::string( error ) ); 
				m_strMsgForUser =  error;
				return false;
			}
		}
		catch( TA_Base_Core::ArchiveException& e )
		{
			LOG_EXCEPTION_CATCH( SourceInfo, "ArchiveException", e.what() );
			TA_ArchiveManager::sendAuditMessageIgnoreErrors( TA_Base_Core::ArchiveAudit::RestoreArchiveFailed, e.what() ); 
			m_strMsgForUser = e.what();
			return false;
		}
		return true;
	}

	bool ArchiveManagerDlgPModel::canWriteScheduledArchives()
	{
		m_strMsgForUser = "";
		m_messageLevel = TA_ArchiveManager::ERR;
		// Check that the user has permission.
		try
		{
			if( !TA_ArchiveManager::isActionPermitted( TA_Base_Bus::aca_ARCHIVE_TRANSFER ) )
			{
				std::string error;
				try
				{
					error = IGuiObserver::getInstance()->getStringResourceFormated( UE050079_INSUF_PRIV_TRANSFER,
						TA_ArchiveManager::getOperatorNameFromSessionId(),
						TA_ArchiveManager::getWorkstationNameFromSessionId());
				}
				catch( TA_Base_Core::ArchiveException& e )
				{
					LOG_EXCEPTION_CATCH( SourceInfo, "ArchiveException", e.what() );
					error = IGuiObserver::getInstance()->getStringResource( UE050123_INSUF_PRIV_TRANSFER );
				}
				TA_ArchiveManager::sendAuditMessageIgnoreErrors( TA_Base_Core::ArchiveAudit::CopyArchiveFailed, std::string( error ) ); 
				m_strMsgForUser = error;
				return false;
			}
		}
		catch( TA_Base_Core::ArchiveException& e )
		{
			LOG_EXCEPTION_CATCH( SourceInfo, "ArchiveException", e.what() );
			TA_ArchiveManager::sendAuditMessageIgnoreErrors( TA_Base_Core::ArchiveAudit::CopyArchiveFailed, e.what() ); 
			m_strMsgForUser = e.what();
			return false;
		}
		return true;
	}

	bool ArchiveManagerDlgPModel::canCreateManualArchive()
	{
		m_strMsgForUser = "";
		m_messageLevel = TA_ArchiveManager::ERR;

		/*
		// Send an audit message.

		try
		{
		// True => isManualArchive
		TA_ArchiveManager::sendAuditMessage( TA_Base_Core::ArchiveAudit::CopyArchiveRequest, "", true ); 
		}
		catch( TA_Base_Core::ArchiveException& e )
		{
		LOG_EXCEPTION_CATCH( SourceInfo, "ArchiveException", e.what() );
		messageForUser = e.what();
		return;
		}
		*/

		// Check that the user has permission.

		try
		{
			if( !TA_ArchiveManager::isActionPermitted( TA_Base_Bus::aca_ARCHIVE_MANUAL ) )
			{
				std::string error;
				try
				{
					error = IGuiObserver::getInstance()->getStringResourceFormated( UE050021_INSUF_PRIV_MAN_ARCHIVE,
						TA_ArchiveManager::getOperatorNameFromSessionId(),
						TA_ArchiveManager::getWorkstationNameFromSessionId());
				}
				catch( TA_Base_Core::ArchiveException& e )
				{
					LOG_EXCEPTION_CATCH( SourceInfo, "ArchiveException", e.what() );
					error= IGuiObserver::getInstance()->getStringResource( UE050124_INSUF_PRIV_MAN_ARCHIVE );
				}
				TA_ArchiveManager::sendAuditMessageIgnoreErrors( TA_Base_Core::ArchiveAudit::CopyArchiveFailed, std::string( error ) ); 
				m_strMsgForUser = error;
				return false;
			}
		}
		catch( TA_Base_Core::ArchiveException& e )
		{
			LOG_EXCEPTION_CATCH( SourceInfo, "ArchiveException", e.what() );
			m_strMsgForUser = e.what();
			return false;
		}
		return true;
	}


	int ArchiveManagerDlgPModel::getManualDVDVolume()
	{
		// yangguang++ TD16713
		TA_Base_Core::IEntityData* data = m_appController->getEntity();
		if ( 0 == data )
		{
			std::string error = IGuiObserver::getInstance()->getStringResourceFormated( UE050104_PATH_ERROR,UE050104_REASON_ENTITY_DATA);
			TA_THROW( TA_Base_Core::ArchiveException( error ) );
		}
		TA_Base_Core::ArchiveManagerEntityData* amData = dynamic_cast< TA_Base_Core::ArchiveManagerEntityData* >( data );
		if ( 0 == amData )
		{
			std::string error = IGuiObserver::getInstance()->getStringResourceFormated( UE050104_PATH_ERROR,UE050104_REASON_CAST_FAILED);
			TA_THROW( TA_Base_Core::ArchiveException( error ) );
		}

		//16713++
		int dvdvolume = atoi(amData->getVolumeofDVD().c_str());		
		LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "[XF] dvd volume is : %i", dvdvolume);
		//++16713
		return dvdvolume;
	}

	void ArchiveManagerDlgPModel::sendAuditMsgForCancel(EArchiveOperation eOperation)
	{
		UINT messageID,loadStringId;
		switch(eOperation)
		{
		case EArchiveOperation::RESTORE:
			messageID = UI050045_RESTORE_ABORTED;
			loadStringId = UI050062_RESTORE_ABORTED;
			break;
		case EArchiveOperation::SCHEDULE:
		case EArchiveOperation::MANUAL:
			messageID = UI050030_WRITE_ABORTED;
			loadStringId = UI050060_TRANSFER_ABORTED;
			break;
		}
		std::string  message;
		try
		{
			message = IGuiObserver::getInstance()->getStringResourceFormated(messageID ,
				TA_ArchiveManager::getOperatorNameFromSessionId(),
				TA_ArchiveManager::getWorkstationNameFromSessionId() );
		}
		catch( TA_Base_Core::ArchiveException& e )
		{
			LOG_EXCEPTION_CATCH( SourceInfo, "ArchiveException", e.what() );
			message = IGuiObserver::getInstance()->getStringResource(loadStringId );
		}
		if(eOperation == EArchiveOperation::SCHEDULE)
		{
			// Send an audit message.
			try
			{
				TA_ArchiveManager::sendAuditMessage( TA_Base_Core::ArchiveAudit::CopyArchiveCancelled );
			}
			catch( TA_Base_Core::ArchiveException& e )
			{
				LOG_EXCEPTION_CATCH( SourceInfo, "ArchiveException", e.what() );
			}
		}

		m_strMsgForUser = std::string( message );
		m_messageLevel = TA_ArchiveManager::INFO;
		return;
	}

	void ArchiveManagerDlgPModel::sendRestoreAuditMsgForOK()
	{
		// Send an audit message.
		try
		{
			TA_ArchiveManager::sendAuditMessage( TA_Base_Core::ArchiveAudit::ArchiveRestored ); 
		}
		catch( TA_Base_Core::ArchiveException& e )
		{
			LOG_EXCEPTION_CATCH( SourceInfo, "ArchiveException", e.what() );
			m_strMsgForUser = e.what();
			m_messageLevel = TA_ArchiveManager::WARN;
			return;
		}
	}

	void ArchiveManagerDlgPModel::displayErrorMsg(EArchiveOperation eOpt )
	{
		UINT messageID;
		switch(eOpt)
		{
		case EArchiveOperation::RESTORE:
			messageID = IDS_UE_050128;
			break;
		case EArchiveOperation::SCHEDULE:
			messageID = IDS_UE_050126;
			break;
		case EArchiveOperation::MANUAL:
			messageID = IDS_UE_050127;
			break;
		}

		if( !m_strMsgForUser.empty() )
		{

			TransactiveUserMsgEventArgs args;		
			args.setArgs1(m_strMsgForUser);

			//TA_Base_Bus::TransActiveMessage userMsg;
			// userMsg << messageForUser.c_str();TODO

			if (m_messageLevel == TA_ArchiveManager::ERR)
			{
				args.setUID(messageID);
				notifyViews(SHOW_USER_MESSAGE_NOTIFY.c_str(),&args); 
				//UINT selectedButton = userMsg.showMsgBox(messageId);
			}
			else if (m_messageLevel == TA_ArchiveManager::WARN)
			{
				args.setUID(IDS_UW_050018);
				notifyViews(SHOW_USER_MESSAGE_NOTIFY.c_str(),&args); 
				//UINT selectedButton = userMsg.showMsgBox(IDS_UW_050018);
			}
			else if (m_messageLevel == TA_ArchiveManager::INFO)
			{
				//UINT selectedButton = userMsg.showMsgBox(IDS_UI_050063);
			}
		}
	}

	bool ArchiveManagerDlgPModel::canDeleteArchive()
	{
		if( !TA_ArchiveManager::isActionPermitted( TA_Base_Bus::aca_ARCHIVE_DELETE ) )
		{
			try
			{
				std::string operatorName = TA_ArchiveManager::getOperatorNameFromSessionId();
				std::string workstationName = TA_ArchiveManager::getWorkstationNameFromSessionId();
				TransactiveUserMsgEventArgs args(IDS_UE_050109,operatorName,workstationName);		
				notifyViews(SHOW_USER_MESSAGE_NOTIFY.c_str(),&args);
			}
			catch( TA_Base_Core::ArchiveException& e )
			{
				LOG_EXCEPTION_CATCH( SourceInfo, "ArchiveException", e.what() );
				TransactiveUserMsgEventArgs args(IDS_UE_050129,e.what());		  
				notifyViews(SHOW_USER_MESSAGE_NOTIFY.c_str(),&args); 
			}
			return false;
		}
		return true;
	}

	void ArchiveManagerDlgPModel::checkButtonEnabled()
	{
		// Disable all the buttons if the server is down.
                
        if ( !m_isServerUp )
        {
            m_isAutomaticButtonEnabled = false;
			m_isManualButtonEnabled = false;
			m_isRestoreButtonEnabled = false;
			m_isDeleteButtonEnabled = false;
            return;
        }

        // Only enable the buttons if the operator has sufficient privileges.

        try
        {
            if( TA_ArchiveManager::isActionPermitted( TA_Base_Bus::aca_ARCHIVE_TRANSFER ) )
            {
              m_isAutomaticButtonEnabled = true;
            }
            else
            {
                m_isAutomaticButtonEnabled = false;
            }

            if( TA_ArchiveManager::isActionPermitted( TA_Base_Bus::aca_ARCHIVE_MANUAL ) )
            {
               m_isManualButtonEnabled = true;
            }
            else
            {
                m_isManualButtonEnabled = false;
            }

            if( TA_ArchiveManager::isActionPermitted( TA_Base_Bus::aca_ARCHIVE_RESTORE ) )
            {
               m_isRestoreButtonEnabled = true;
            }
            else
            {
                m_isRestoreButtonEnabled = false;
            }

            if( TA_ArchiveManager::isActionPermitted( TA_Base_Bus::aca_ARCHIVE_DELETE ) )
            {
               m_isDeleteButtonEnabled = true;
            }
            else
            {
                m_isDeleteButtonEnabled = false;
            }
        }
        catch( TA_Base_Core::ArchiveException& e )
        {
            LOG_EXCEPTION_CATCH( SourceInfo, "ArchiveException", e.what() );            
			TransactiveUserMsgEventArgs args(IDS_UE_050129,e.what());		          
            notifyViews(SHOW_USER_MESSAGE_NOTIFY.c_str(),&args); 
        }
	}
	
	bool ArchiveManagerDlgPModel::isAutomaticButtonEnabled()
	{
		return m_isAutomaticButtonEnabled;
	}
	bool ArchiveManagerDlgPModel::isManualButtonEnabled()
	{
		return m_isManualButtonEnabled;
	}
	bool ArchiveManagerDlgPModel::isRestoreButtonEnabled()
	{
		return m_isRestoreButtonEnabled;	
	}
	bool ArchiveManagerDlgPModel::isDeleteButtonEnabled()
	{
		return m_isDeleteButtonEnabled;
	}

	bool ArchiveManagerDlgPModel::isServerUp()
	{
		return m_isServerUp;
	}


}