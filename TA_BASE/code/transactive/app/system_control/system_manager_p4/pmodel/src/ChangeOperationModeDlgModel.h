#pragma once

#include "bus/generic_gui_pmod/src/TransActiveViewModel.h"

namespace TA_Base_App
{
    class ChangeOperationModeDlgModel :
        public TA_Base_Bus::AbstractPModel
    {
    public:
        ChangeOperationModeDlgModel(TA_Base_Bus::IApplicationController* appController, const char * pmodelName);
        virtual ~ChangeOperationModeDlgModel(void);
    public:
        void setProcesses(const std::vector<std::string> & processes)
        {
            m_processes.assign(processes.begin(), processes.end());
        }
		void changeProcessOperationMode(const std::string& processId,
			TA_Base_Core::EOperationMode opMode);
		void sendAuditMessage( const std::string &process, const unsigned long entityKey, const std::string & sessionId );
		void changeOperationMode();
	public:
        /**
          * The entity names of the processes that we'll be changing the
          * the operating mode of.
          */
        std::vector<std::string> m_processes;

        /**
          * The operation mode we're changing to.
          */
        TA_Base_Core::EOperationMode m_operationMode;

    };

}