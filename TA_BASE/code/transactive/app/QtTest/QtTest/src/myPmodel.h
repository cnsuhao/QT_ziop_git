#if !defined(myPmodel_H)
#define myPmodel_H


#include "bus/generic_gui_pmod/src/TransActiveViewModel.h"


namespace TA_Base_App
{

    class myPmodel : public TA_Base_Bus::AbstractPModel
    {

    public:

        myPmodel(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName);
        virtual ~myPmodel();

    };

} // namespace TA_Base_App

#endif
