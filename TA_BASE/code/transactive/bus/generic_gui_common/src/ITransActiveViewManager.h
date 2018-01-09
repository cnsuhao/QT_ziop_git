#ifndef Generic_GUI_Common_ITransActiveViewManager_H_
#define Generic_GUI_Common_ITransActiveViewManager_H_


#include "core/synchronisation/src/NonReEntrantThreadLockable.h"
#include "bus/generic_gui_common/src/ITransActiveView.h"
#include "bus/generic_gui_common/src/IViewFactory.h"
#include <string>
#include <map>
#include <set>

namespace TA_Base_Bus
{
	 
	/** Interface for View Manager
	 *	
	 */
	class ITransActiveViewManager
	{
	public:
		virtual ~ITransActiveViewManager() {};


		/** get the view based on specific name
		 * 
		 *
		 * @param viewType
		 * @param viewID
		 * @return ITransActiveView *
		 */
		virtual ITransActiveView * getView(const char * viewType, const char * viewID,  bool createIfNew=true) = 0;



		/** add item to the repository. If duplicate item name exists, it will return false
		 * 
		 *
		 * @param item
		 * @return bool
		 */
		virtual bool addView(ITransActiveView * item) = 0;


				
        /** remove Item from the repository. It will delete the item
		 * 
		 *
		 * @param itemName
		 * @return void
		 */
		virtual bool removeView(ITransActiveView * item) = 0;


		/** add item to the repository. If duplicate item name exists, it will return false
		* 
		*
		* @param item
		* @return bool
		*/
		virtual void setViewFactory(IViewFactory* viewFactory) = 0;
	};

}
#endif