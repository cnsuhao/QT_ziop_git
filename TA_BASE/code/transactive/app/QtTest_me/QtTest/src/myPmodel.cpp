#include "StdAfx.h"
#include "myPmodel.h"


namespace TA_Base_App
{
     myPmodel::myPmodel(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName)
         : TA_Base_Bus::AbstractPModel(appController,pmodelName)
    {
    }

    myPmodel::~myPmodel()
    {
    }

} // namespace TA_Base_App
