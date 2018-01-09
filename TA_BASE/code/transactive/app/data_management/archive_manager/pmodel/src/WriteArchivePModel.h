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
#ifndef WRITE_ARCHIVE_DLG__PMODEL_1234567890_H
#define WRITE_ARCHIVE_DLG__PMODEL_1234567890_H

#include "bus/generic_gui_pmod/src/transactiveviewmodel.h"
#include "app/data_management/archive_manager/pmodel/src/resource.h"
#include "app/data_management/archive_manager/pmodel/src/WriteArchiveHelper.h"


#include <map>
#include <string>
//forward declaration
namespace TA_Base_App
{
	class RetrievingDataThread;
}


namespace TA_Base_App
{
	class WriteArchivePModel : public TA_Base_Bus::AbstractPModel
	{
	public:

		WriteArchivePModel(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName);
		virtual ~WriteArchivePModel(void);

		bool getFilesDeatilsVector(WriteArchiveHelper::FileDetailsVector& fileDetails);

		int getCdsMapSize()
		{
			return m_cdsMap.size();
		}

		/**
          * cleanUp
          *
          * Deletes the archive files and export logs.  Used if the
          * operation was successful.
          */

		void cleanUpArchive();

		bool isCDsMapEmpty();

		time_t getFromDate()
		{
			return m_fromDate;
		}

		time_t getToDate()
		{
			return m_toDate;
		}

		void setSelectedDataDateRange(time_t fromDate, time_t toDate, UINT volumeSize);

		virtual void notifyViews(const char * updateType, TA_Base_Bus::EventArgs * args=NULL)
        {
			return AbstractPModel::notifyViews(updateType,args);
        }

		std::string getRetrievingDateRangeString();

		void startRetrievingThread();

		void stopRetrievingThread();

		void TerminateRetrievingThread();

		void setStateManagerIdle();

		void deleteRetrievedArchiveFiles();

		TA_ArchiveManager::FileStatusMap getRetrievingFileStatusMap();

		void setWhetherManual(bool manual)
		{
			m_isManualArchive = manual;
		}

		void getBurnFileDetails(std::string& volumeLabel, std::string& cdLabel, WriteArchiveHelper::FileDetailsVector& files);
		void deleteFirstCdMapEntry();
		 int getCdMapsSize();
		 
		 TA_Base_Core::IEntityData* getGuiEntity() 
		 { 
			 return m_guiEntity; 
		 }
		
	private:

		TA_Base_Core::IEntityData* m_guiEntity;
		//
        // The object that does all the back end work.
        //

        WriteArchiveHelper m_writeArchiveHelper;
        
        //
        // Stores the file names including the full path grouped by CD
        // referenced by the volume label.
        //
       
        WriteArchiveHelper::CdsMap m_cdsMap;

	
        // 
        // The date range is only limited when manually archiving.
        //
        bool m_isManualArchive;


		//
        // These dates will be filled with the start and end dates for
        // which data is to be retrieved (manually archived).
        //

        time_t m_fromDate;
        time_t m_toDate;

		RetrievingDataThread* m_pRetrievingThread;

	};
}


#endif //WRITE_ARCHIVE_DLG__PMODEL_1234567890_H