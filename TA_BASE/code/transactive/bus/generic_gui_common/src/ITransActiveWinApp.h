#ifndef ITransActiveWinApp_H_
#define ITransActiveWinApp_H_

#include "bus/generic_gui_common/src/UserInteraction.h"
#include "bus/generic_gui_common/src/ITransActiveViewManager.h"
#include "bus/generic_gui_common/src/IGUIApplicationController.h"
#include "bus/generic_gui_common/src/ITransActiveApp.h"
#include "core/synchronisation/src/NonReEntrantThreadLockable.h"

#include "core/process_management/IDL/src/ProcessManagementDataDefinitionsCorbaDef.h"
#include <string>
#include <map>
#include <set>

namespace TA_Base_Bus
{
	class ITransActivePModel;
	class IGUIApplicationController;
	class ITransActiveView;
	/** Interface for TransActiveWinApp
	 *	
	 */
	class ITransActiveWinApp : public IUserInteractiveReply
		                     , public IUserNotificationServiceCenter
							 , public ITransActiveApp
    {
    public:
        virtual ~ITransActiveWinApp() {};

	 
   
        /** Close all transactive message box. Normally used when exit the application
         * 
         *
         * @return void
         */
        virtual void closeAllTransActiveMessages() = 0;

        /** display about dialog of GUI application
         * 
         *   
         * @return void
         */
        virtual void displayAboutBox() = 0;


	    /** Change application focus. Normally called when launched GUI Application,
		*
		* The Control Station needs to alert the GUI applications when they should regain
		* the focus on the screen. This is the most common focus request by the Control
		* Station but it may also need to tell the applications to minimize or maximize.
		* This is done through this method. This takes an enumeration indicating how the
		* GUI should change it's focus.
		*
		* @param  focus - This indicates how the window should change it's focus.
		*/
        virtual void changeAppFocus(const TA_Base_Core::EFocusType focus) = 0;

        /** get the pointer to main view
         * 
         *
         * @return ITransActiveView *
         */
        virtual ITransActiveView * getMainView() = 0;

		/**
		 *  move main view according to specific position.
		 *
		 * @param viewPositionInfo
		 * @param isFirstCalculatePosition
		 * @return void
		 */
		virtual void moveMainView(const WindowPositionInfo & viewPositionInfo, bool isFirstCalculatePosition) = 0;

	};
}
#endif
