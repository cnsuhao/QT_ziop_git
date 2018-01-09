#ifndef Generic_GUI_Common_IViewFactory_H_
#define Generic_GUI_Common_IViewFactory_H_


#include "core/synchronisation/src/NonReEntrantThreadLockable.h"
#include "bus/generic_gui_common/src/ITransActiveView.h"
#include <string>
#include <map>
#include <set>

namespace TA_Base_Bus
{
	 
	/** Interface for View Manager
	 *	
	 */
	class IViewFactory
	{
	public:
		virtual ~IViewFactory() {};


		/** get the view based on specific name
		 * 
		 *
		 * @param viewType
		 * @param viewID
		 * @return ITransActiveView *
		 */
		virtual ITransActiveView * createView(const char * viewType) = 0;
 
	};

}
#endif