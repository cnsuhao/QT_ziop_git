#ifndef AbstractTransActiveFormView_H_
#define AbstractTransActiveFormView_H_

#ifdef WIN32
#include <windows.h>
#endif


#include "bus/generic_gui_common/src/AbstractTransActiveView.h"
#include "core/utilities/src/TAAssert.h"

namespace TA_Base_Bus
{

	 
	/** Abstract FormView class under GenericGUI framework 
	 *	
	 */
	class AbstractTransActiveFormView: public AbstractTransActiveView
    {
	public:
		/** Constructor function
		 *	
		 */
		AbstractTransActiveFormView(ITransActiveWinApp * pApp)
			: AbstractTransActiveView(pApp)
		{

		}
    protected:
        /** update view after init view and presentation model
         * 
         *
         *  @return void
         */
        virtual void updateView() {};

        /** do extra UI initialization during view creation
         * 
         *
         *  @return void
         */
        virtual void initUIControl() {};

        /** synchronize presentation data between view layer and presentation model layer
         * 
         *
         *  @return void
         */
        virtual void syncUIDataToPModel() {};

    };

}

#endif
