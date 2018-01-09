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

#ifndef ARCHIEVE_MANAGER_DLG__PMODEL_1234567890_H
#define ARCHIEVE_MANAGER_DLG__PMODEL_1234567890_H

#include "app/data_management/archive_manager/pmodel/src/BasicHelper.h"
#include "bus/generic_gui_pmod/src/transactiveviewmodel.h"
#include "core/utilities/src/RunParams.h"


namespace TA_Base_App
{
	enum EArchiveOperation
	{
		RESTORE = 0,
		SCHEDULE = 1,
		MANUAL = 2
	};

	class ArchiveManagerDlgPModel : public TA_Base_Bus::AbstractPModel, public TA_Base_Core::RunParamUser
	{
	public:
		ArchiveManagerDlgPModel(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName);
		virtual ~ArchiveManagerDlgPModel(void);

		//---------- Methods Inherited from RunParamUser --------//
		/**
		* onRunParamChange
		*
		* Called whenever a RunParam is updated during run-time.
		*
		* @param name The name of the RunParam that has changed.
		* @param value The new value of the RunParam.
		*/
		void onRunParamChange(const std::string& name, const std::string& value);
		//---------- Methods Inherited from RunParamUser --------//

	public:
		void init();
		bool canRestoreArchives();
		
		/** 
          * canCreateManualArchive
          *
          * Performs the manual archive process:
          * 1. Sends the required audit messages.
          * 2. Checks the users privileges.
          *
          */

		bool canCreateManualArchive();
		 
		/** 
          * canWriteScheduledArchives
          *
          * Performs the scheduled archive process:
          * 1. Sends the required audit messages.
          * 2. Checks the users privileges.
		  *
          */
		bool canWriteScheduledArchives();

		bool canDeleteArchive();		
		int getManualDVDVolume();
		void sendAuditMsgForCancel(EArchiveOperation eOperation);
		void sendRestoreAuditMsgForOK();
		void displayErrorMsg(EArchiveOperation eOpt );
		void checkButtonEnabled();
		bool isAutomaticButtonEnabled();
		bool isManualButtonEnabled();
		bool isRestoreButtonEnabled();
		bool isDeleteButtonEnabled();	
		bool isServerUp();

	private:
		void setServerStatus( bool isServerUp );

		std::string m_strMsgForUser;
		TA_ArchiveManager::EMsgLevel m_messageLevel;

		//
        // The status of the server.
        //

        bool m_isServerUp;

		bool m_isAutomaticButtonEnabled;
		bool m_isManualButtonEnabled;
		bool m_isRestoreButtonEnabled;
		bool m_isDeleteButtonEnabled;
	};
}
#endif // ARCHIEVE_MANAGER_DLG__PMODEL_1234567890_H
