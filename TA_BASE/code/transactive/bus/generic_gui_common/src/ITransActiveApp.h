#ifndef _Generic_GUI_Common_ITransActiveApp_H_C
#define _Generic_GUI_Common_ITransActiveApp_H_C

#include "bus/generic_gui_common/src/CommonDataType.h"

#include "bus/generic_gui_common/src/ITransActiveViewManager.h"
#include "bus/generic_gui_common/src/IGUIApplicationController.h"
#include "core/synchronisation/src/NonReEntrantThreadLockable.h"

 
#include <string>
#include <map>
#include <set>

namespace TA_Base_Bus
{
 
     /** Interface for ITransActiveApp
	 *	
	 */

	class ITransActivePModel;
	class IGUIApplicationController;
	class ITransActiveView;

	class ITransActiveApp  
    {
    public:
        virtual ~ITransActiveApp() {};

		virtual void setViewManager(ITransActiveViewManager * pViewManager) = 0;

		virtual ITransActiveViewManager * getViewManager() = 0;

        /** Get the pointer to application controller. 
         * 
         *
         * @return IGUIApplicationController *
         */
        virtual IGUIApplicationController * getApplicationController() = 0;

        /** process method request dynamically based on method name and name value parameters. Normally called by IApplicationController class
         * 
         *
         * @param methodName
         * @param params
         * @return void
         */
        virtual void invoke(const char * methodName, const NameValueParams & params) = 0;

		/** Get specific view configuration from TransActiveWinApp. Those configuration could be gotten from user setting in db
		 * 
		 *
		 * @param parameterName
		 * @return std::string
		 */
		virtual std::string getViewConfig(const char * parameterName) = 0;             
  
	};
}
#endif
