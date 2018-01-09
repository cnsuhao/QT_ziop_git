#include "SystemControllerDlgModel.h"

#include "core/data_access_interface/system_controller/src/ISystemControllerData.h"
#include "core/data_access_interface/system_controller/src/SystemControllerAccessFactory.h"

#include "core/utilities/src/TA_MinMax.h"
#include "core/utilities/src/RunParams.h"
#include "core/utilities/src/DebugUtil.h"

#include "core/exceptions/src/CannotUseSysControllerException.h"
#include "core/exceptions/src/DataException.h"
#include "core/exceptions/src/DatabaseException.h"

namespace TA_Base_App
{

    SystemControllerDlgModel::SystemControllerDlgModel(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName)
        :TA_Base_Bus::AbstractPModel(appController, pmodelName)
    {
    }

    SystemControllerDlgModel::~SystemControllerDlgModel(void)
    {
    }

	void SystemControllerDlgModel::getSystemControlHostNames(std::vector<std::string> & vectorOfHostnames)
	{
		vectorOfHostnames.clear();
		try
		{
			std::vector<TA_Base_Core::ISystemControllerData*> systemControllers = TA_Base_Core::SystemControllerAccessFactory::getInstance().getAllSystemControllers(true);
			std::vector<TA_Base_Core::ISystemControllerData*>::iterator it = systemControllers.begin();
			for(;it!=systemControllers.end(); it++)
			{
				vectorOfHostnames.push_back((*it)->getName());
			}

		}
		catch ( const TA_Base_Core::DataException& )
		{
			LOG( SourceInfo, TA_Base_Core::DebugUtil::ExceptionCatch, "DataException", "Could not retrieve system controller data.  Leaving hostname field blank");
			TA_THROW( TA_Base_Core::CannotUseSysControllerException("Could not retrieve system controller data") );
		}
		catch ( const TA_Base_Core::DatabaseException& )
		{
			LOG( SourceInfo, TA_Base_Core::DebugUtil::ExceptionCatch, "DatabaseException", "Could not retrieve system controller data.  Leaving hostname field blank");
			TA_THROW( TA_Base_Core::DatabaseException("Could not retrieve system controller data") );
		}
	}

	void SystemControllerDlgModel::computeComboboxSize( int itemCount, SIZE &size, RECT &rect, int height )
	{
		// If maxlen = -1, dropdown list is limited in size to that of the screen.
		// Otherwise there is no limit on the size of the dropdown.
		const int maxlen = -1;

		// If size of drop down is not limited to the screen limit then
		// limit it to the maximum number of items 
		if(maxlen > 0)
		{
			itemCount = ta_max(maxlen, 2);
		}


		size.cx = rect.right - rect.left;
		size.cy = height * (itemCount + 2);

		// If dropdown size is limited to screen size then calculate
		// the dropdown size
		if(maxlen < 0)
		{ 
			// screen limit

			if(rect.top - size.cy < 0 || rect.bottom + size.cy > ::GetSystemMetrics(SM_CYSCREEN))
			{
				// invoke limit

				// Compute the largest distance the dropdown can appear, 
				// relative to the screen (not the window!)		
				int factor = ta_max( (rect.top / height), 
					(::GetSystemMetrics(SM_CYSCREEN) - rect.bottom) / height);

				// Compute new space. Note that we don't really fill the screen.
				// We only have to use this size if it is smaller than the max size
				// actually required
				int height2 = height * factor;
				size.cy = min(height2, size.cy);
			} 
		}	
	}

}