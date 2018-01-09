#pragma  once

#include "bus\generic_gui_pmod\src\TransActiveViewModel.h"
//#include "equipmentproxy.h"
#include "InspectorpanelAppController.h"
#include "AbstractEquipmentPModel.h"

namespace TA_Base_App
{
	class DirectControlDialogModel: public AbstractEquipmentPModel
	{
	public:
		DirectControlDialogModel(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName);
		~DirectControlDialogModel(void);

		std::string		getControlDescription();
		std::string		getControlValue();
		bool			isActionAllowd();
		std::string		getEquipmentLabel();
		bool			isConfigValid();
		void			directControl();
		void			init();
	protected:
		
		virtual void	onPertmissionUpdate();
		virtual	void	onInitStatusUpdate (EquipmentInitStatus status);
	private:
		unsigned int	m_iPoint;
		std::string		m_controlValue;
		bool			m_actionAllowed;
		bool			m_isConfigValid;
	};
}