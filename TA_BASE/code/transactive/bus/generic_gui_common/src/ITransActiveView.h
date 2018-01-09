#ifndef ITransActiveView_H_
#define ITransActiveView_H_


#include "core/synchronisation/src/NonReEntrantThreadLockable.h"
#include "bus/generic_gui_common/src/IStateChangeObserver.h"
#include <string>
#include <map>
#include <set>

namespace TA_Base_Bus
{
/** Interface for TransActiveView
	 *	
     *  It is responsible to display and modify the presentation data but not for storing it
	 */
    class ITransActiveView: public IStateChangeObserver
		                  
	{
	public:
        virtual ~ITransActiveView() {};


		//************************************
		// Method:    getViewHandle
		// FullName:  TA_Base_Bus::ITransActiveView::getViewHandle
		// Access:    virtual public 
		// Returns:   HWND
		// Qualifier: presentation model will use this method to access related window property, such as window rect 
		//************************************
		
        /** Get the windows handle of view object
         *	
         */
		//TODO: replace below HWND by common type
        //virtual HWND getViewHandle() = 0;

		virtual RECT getViewPosition() = 0;

		virtual std::string getViewID()=0;

		virtual std::string getViewType()=0;

		virtual void setViewID(const char* viewID)=0;

		/** return presentation model name that view attach
         *	
         */
        virtual std::string getMyPModelName() = 0;

	};
}
#endif
