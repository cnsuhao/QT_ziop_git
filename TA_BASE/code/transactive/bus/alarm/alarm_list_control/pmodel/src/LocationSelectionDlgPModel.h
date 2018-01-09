#pragma once


#include "bus/generic_gui_pmod/src/TransActiveViewModel.h"


namespace TA_Base_Bus
{
	class LocationSelectionDlgPModel
		: public AbstractPModel
	{
	public:
		struct LocationInfo
		{
			unsigned long key;
			std::string name;
		};

		typedef std::map<unsigned long, LocationInfo> LocationInfoMap;

		LocationSelectionDlgPModel(TA_Base_Bus::IApplicationController* appController, const char * pmodelName);
		~LocationSelectionDlgPModel();

		/**
		* Sets the initial dialog selection
		*
		* @param initialLocation    The location to initially select in the drop down
		*/
		void setInitialSelection(unsigned long initialLocation);


		/**
		* This will get the location selected by the user.
		*
		* @return the selected location key
		*/
		unsigned long getSelectedLocation();

		void setSelectedLocation(unsigned long selLocation);

		LocationInfoMap getAllLocationsInfo()
		{
			return m_locationInfoMap;
		}

	private:
		/**
		* The initial location to select in the dialog
		*/
		unsigned long m_initialLocation;


		/**
		* The location selected in the dialog
		*/
		unsigned long m_selectedLocation;


		/**
		* The map of locations
		*/
		LocationInfoMap m_locationInfoMap;
	};

}