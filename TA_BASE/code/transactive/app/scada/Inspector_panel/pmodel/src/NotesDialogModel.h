#pragma  once
#include "bus\generic_gui_pmod\src\TransActiveViewModel.h"
#include "InspectorPanelConsts.h"
//#include "EquipmentProxy.h"
#include "AbstractEquipmentPModel.h"


namespace TA_Base_App
{
	class NotesDialogModel: public AbstractEquipmentPModel
	{
	public:
		NotesDialogModel(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName);
		~NotesDialogModel(void);
	public:
		bool			setNotes(const std::string& notes);
		std::string		getNotes();
		bool			isNotesPermitted();
	protected:
		virtual void	onDataNodeProxyUpdate(unsigned long key, TA_Base_Bus::ScadaEntityUpdateType updateType);
	private:
	};
}