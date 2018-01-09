#pragma once

#include "bus/generic_gui_pmod/src/TransActiveViewModel.h"

namespace TA_Base_App
{

    class StopProcessDlgModel :
        public TA_Base_Bus::AbstractPModel
    {
    public:
        StopProcessDlgModel(TA_Base_Bus::IApplicationController* appController, const char * pmodelName);
        virtual ~StopProcessDlgModel(void);
    public:
        void setProcesses(const std::vector<std::string> & processes)
        {
            m_processes.assign(processes.begin(), processes.end());
        }

		/**
		* stopProcess
		*
		* Ask the System Controller to stop a running process.
		*
		* @param  processId  The entity name of the process.
		*
		* @exception SystemControllerException  An error occurred communicating
		*                                       with the System Controller.
		*/
		void stopProcess(const std::string& processId);
		void StopProcessDlgModel::stopProcesses();
	public:
        std::vector<std::string> m_processes;

    };

}