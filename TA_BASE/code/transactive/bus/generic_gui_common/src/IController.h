#ifndef Generic_Gui_Common_IApplicationController_H_C
#define Generic_Gui_Common_IApplicationController_H_C

 
#include "bus/generic_gui_common/src/CommandGateway.h"
#include "bus/generic_gui_common/src/CommonDataType.h"
#include "bus/generic_gui_common/src/ITransActivePModel.h"
#include "bus/generic_gui_common/src/ITransActiveView.h"
#include "bus/generic_gui_common/src/ITransActiveViewManager.h"
 
#include <string>
#include <map>
#include <set>

namespace TA_Base_Bus
{

	/** use to store parameter name and value's pointer
	*	
	*/

	/** Interface for  controller.
	*   class AbstractGUI of older GenericGUI framework, used to control and coordinate presentation model behaviors
	*   It needs to allocate and cleanup application level resource, most of the business object should be maintained by this class object
	*   This class is a central class of GenericGUI framework: 
	*
	*   -  Most of application level data will be kept by it, such as entity data, session info, etc.  
	*       Moreover, you can initialize other application-level business object from this class
	*   -	It will monitor server state change, such as GroupOnline and GroupOffline
	*   -	It will monitor runtime parameter change, session id change and duty change
	*   -	It should keep some necessary service object reference, such as AuditMessageSender, which used to send AuditMessage 
	*	
	*/
	class IController: public ICommandGateway
	{

	public:
		virtual ~IGUIApplicationController() {};

		/** Cleanup the application level resource.
		* 
		* 
		*  @return void
		*/
		virtual void cleanup() = 0;

		/** Process method request based on method name. Used to control and coordinate interaction of different presentation model 
		* 
		*
		* @param methodName
		* @param params
		* @return void
		*/
		virtual void invoke(const char * methodName, const NameValueParams & params) = 0;

	
		 
		/** get presentation model based on name
		*	
		*/
		virtual ITransActivePModel * getPModel(const std::string & pModelName) = 0;

	};
}

#endif