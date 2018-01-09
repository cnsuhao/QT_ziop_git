#ifndef IStateChangeObserver_H_
#define IStateChangeObserver_H_

#include "bus/generic_gui_common/src/PModelDataType.h"
#include <string>

namespace TA_Base_Bus
{
	/** Interface for the observer which monitor domain data change
	 *	
	 */
	class IStateChangeObserver
	{
	public:
        virtual ~IStateChangeObserver() {};

		/** notify presentation model when domain data has changed
		 * 
         *  Notify domain data change, from business logic layer to presentation logic layer, 
         *  if brings status data change by pContent, then business object should be responsible to free the memory
         *  @param updateType
         *  @param args
         *  @return void
		 */
		virtual void notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args=NULL) = 0;

	};

}
#endif
