#pragma once

#include "bus/generic_gui_pmod/src/TransActiveViewModel.h"

namespace TA_Base_App
{

    class StartProcessDlgModel :
        public TA_Base_Bus::AbstractPModel
    {
    public:
        StartProcessDlgModel(TA_Base_Bus::IApplicationController* appController, const char * pmodelName);
        virtual ~StartProcessDlgModel(void);
    public:
        void setProcesses(const std::vector<std::string> & processes)
        {
            m_processes.assign(processes.begin(), processes.end());
        }

		/**
		* startProcess
		*
		* Ask the System Controller to start a process.
		*
		* @param processId  The entity name of the process.
		*
		* @exception SystemControllerException  An error occurred communicating
		*                                       with the System Controller.
		*/
		void startProcess(const std::string& processId);
		void startProcesses();
	public:
        std::vector<std::string> m_processes;

    };

}