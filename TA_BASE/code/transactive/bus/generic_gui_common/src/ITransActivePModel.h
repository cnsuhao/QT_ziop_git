#ifndef ITransActivePModel_H_
#define ITransActivePModel_H_


#include "core/synchronisation/src/NonReEntrantThreadLockable.h"
#include "bus/generic_gui_common/src/IStateChangeObserver.h"
#include "bus/generic_gui_common/src/ITransActiveView.h"
#include <string>
#include <map>
#include <set>

namespace TA_Base_Bus
{ 
	/** Interface for TransActiveViewModel
	 *	
	 */
	class ITransActivePModel: public IStateChangeObserver
	{
	public:

		virtual ~ITransActivePModel() {};

	
        /** Allocate resource related with corresponding view
         * 
         *
         *  @return void
         */
        virtual void allocViewResource() = 0;

        /** Cleanup resource related with corresponding view
         * 
         *
         *  @return void
         */
        virtual void cleanup() = 0;

        /** attach view to this presentation model object
         *	
         */
        virtual void attachView(ITransActiveView * pView, const char * updateType = NULL) = 0;

        /** detach view to this presentation model object
         *	
         */
        virtual void detachView(ITransActiveView * pView, const char * updateType = NULL) = 0;

        /** return presentation model name
         *	
         */
        virtual std::string getName() = 0;
    protected:
        /** notify views when received notification 
         *	
         */
        virtual void notifyViews(const char * updateType, TA_Base_Bus::EventArgs * args=NULL) = 0;
	};


}
#endif