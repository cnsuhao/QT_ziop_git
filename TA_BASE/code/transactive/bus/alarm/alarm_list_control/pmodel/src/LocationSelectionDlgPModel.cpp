#include "LocationSelectionDlgPModel.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/data_access_interface/src/LocationAccessFactory.h"
#include "core/data_access_interface/src/ILocation.h"

namespace TA_Base_Bus
{
	LocationSelectionDlgPModel::LocationSelectionDlgPModel(TA_Base_Bus::IApplicationController* appController, const char * pmodelName)
		: TA_Base_Bus::AbstractPModel(appController, pmodelName)
	{
		FUNCTION_ENTRY("constructor");

		// get a list of locations,
		std::vector<TA_Base_Core::ILocation*> allLocations = TA_Base_Core::LocationAccessFactory::getInstance().getAllLocations();

		// the customer is going to expect locations sorted by order ID

		for (std::vector<TA_Base_Core::ILocation*>::iterator locationIter = allLocations.begin();
			locationIter != allLocations.end(); ++locationIter)
		{
			// filter out the list to only station locations.
			if ((0 != (*locationIter)->getKey()) &&
				((*locationIter)->getLocationType() == TA_Base_Core::ILocation::STATION))
			{
				LocationInfo newLocation;
				newLocation.key = (*locationIter)->getKey();
				newLocation.name = (*locationIter)->getName();

				m_locationInfoMap.insert(LocationInfoMap::value_type((*locationIter)->getOrderId(), newLocation));
			}

			delete *locationIter;
			*locationIter = NULL;
		}

		allLocations.clear();

		FUNCTION_EXIT;
	}

	LocationSelectionDlgPModel::~LocationSelectionDlgPModel()
	{
	}

	void LocationSelectionDlgPModel::setInitialSelection(unsigned long initialLocation)
	{
		FUNCTION_ENTRY("initialize");
		m_initialLocation = initialLocation;
		FUNCTION_EXIT;
	}

	unsigned long LocationSelectionDlgPModel::getSelectedLocation()
	{
		FUNCTION_ENTRY("initialize");
		FUNCTION_EXIT;
		return m_selectedLocation;
	}

	void LocationSelectionDlgPModel::setSelectedLocation(unsigned long selLocation)
	{
		FUNCTION_ENTRY("setSelectedLocation");
		m_selectedLocation = selLocation;
		FUNCTION_EXIT;
	}
}