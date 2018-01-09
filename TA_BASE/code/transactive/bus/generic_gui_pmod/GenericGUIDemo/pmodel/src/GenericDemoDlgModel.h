#pragma once

#include "bus/generic_gui_pmod/src/TransActiveViewModel.h"

class GenericDemoDlgModel :
    public TA_Base_Bus::AbstractPModel
{
public:
    GenericDemoDlgModel(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName);
    virtual ~GenericDemoDlgModel(void);
};
