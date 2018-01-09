#pragma  once

#include <vector>
#include "bus\generic_gui_pmod\src\CommandGateway.h"

namespace TA_Base_App
{
	class InspectorPanelModelEventArgs: public TA_Base_Bus::EventArgs
	{
	public:
		InspectorPanelModelEventArgs(std::vector<unsigned long>& dataPoints);
		InspectorPanelModelEventArgs(unsigned int iPoint);
		~InspectorPanelModelEventArgs();
	public:
		std::vector<unsigned long>&		getDataPointVector();
		unsigned int					getDataPoint();
	private:
		std::vector<unsigned long>	m_dataPointVector;
		unsigned int				m_iPoint;
	};
}