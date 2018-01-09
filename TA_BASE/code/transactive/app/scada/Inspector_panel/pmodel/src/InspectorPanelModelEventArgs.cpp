#include "inspectorpanelmodeleventargs.h"

namespace TA_Base_App
{
	InspectorPanelModelEventArgs::InspectorPanelModelEventArgs(std::vector<unsigned long>& dataPoints)
	:m_dataPointVector(dataPoints),
	m_iPoint(0xFFFFFFFF)
	{
	}

	InspectorPanelModelEventArgs::InspectorPanelModelEventArgs(unsigned int iPoint)
	:m_iPoint(iPoint)
	{

	}

	InspectorPanelModelEventArgs::~InspectorPanelModelEventArgs()
	{

	}


	std::vector<unsigned long>&		InspectorPanelModelEventArgs::getDataPointVector()
	{
		return m_dataPointVector;
	}
	
	unsigned int					InspectorPanelModelEventArgs::getDataPoint()
	{
		return m_iPoint;
	}
}