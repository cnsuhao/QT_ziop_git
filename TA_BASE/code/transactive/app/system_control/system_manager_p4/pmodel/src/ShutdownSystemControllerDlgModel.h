#pragma once

#include "bus/generic_gui_pmod/src/TransActiveViewModel.h"

namespace TA_Base_App
{

    class ShutdownSystemControllerDlgModel :
        public TA_Base_Bus::AbstractPModel
    {
    public:
        ShutdownSystemControllerDlgModel(TA_Base_Bus::IApplicationController* appController, const char * pmodelName);
        virtual ~ShutdownSystemControllerDlgModel(void);
    public:
        void setHostName(const std::string & hostName)
        {
            m_hostname = hostName;
        }
		void shutdownSystemController();
	public:

        /**
          * The hostname of the System Controller we're shutting down.
          */
        std::string m_hostname;
    
    };

}