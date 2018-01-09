#pragma once

#include "bus/generic_gui_pmod/src/TransActiveViewModel.h"

class TestCommonDlgModel :
    public TA_Base_Bus::AbstractPModel
{
public:
    TestCommonDlgModel(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName);
    virtual ~TestCommonDlgModel(void);
};
