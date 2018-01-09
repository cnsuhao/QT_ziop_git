#pragma once

#include "bus/generic_gui_pmod/src/TransActiveViewModel.h"

namespace TA_Base_App
{

    class SystemControllerDlgModel :
        public TA_Base_Bus::AbstractPModel
    {
    public:
        SystemControllerDlgModel(TA_Base_Bus::IApplicationController* appController, const char * pmodelName);
        virtual ~SystemControllerDlgModel(void);
		void getSystemControlHostNames(std::vector<std::string> & vectorOfHostnames);
		void computeComboboxSize( int itemCount, SIZE &size, RECT &rect, int height );
	};

}