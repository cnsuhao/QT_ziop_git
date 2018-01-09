#include "locationselectiondialog.h"
#include "ConstDefine.h"
#include <QString>

#include "bus/alarm/alarm_list_control/pmodel/src/LocationSelectionDlgPModel.h"
#include "bus/alarm/alarm_list_control/pmodel/src/AlarmListModelConsts.h"

namespace TA_Base_Bus
{
	LocationSelectionDialog::LocationSelectionDialog(TA_Base_Bus::ITransActiveApp* app)
		: AbstractDialog(app, ViewType_LocationSelDlg)
		, m_pTheRealPModel(0)
	{
		ui.setupUi(this);

		this->m_pModel = new LocationSelectionDlgPModel(app->getApplicationController(), PMODEL_LOCATION_SEL_DLG);
		Qt::WindowFlags flags = windowFlags();
		flags = flags | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint;
		this->setWindowFlags(flags);

		connect(ui.comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onSelectionChangeLocationList(int)));
		connect(ui.btnOk, SIGNAL(clicked()), this, SLOT(onOK()));
		connect(ui.btnCancel, SIGNAL(clicked()), this, SLOT(onCancel()));
	}

	LocationSelectionDialog::~LocationSelectionDialog()
	{
	}

	void LocationSelectionDialog::initUIControl()
	{
		LocationSelectionDlgPModel::LocationInfoMap objLocMap = m_pTheRealPModel->getAllLocationsInfo();
		LocationSelectionDlgPModel::LocationInfoMap::iterator itr = objLocMap.begin();
		for (itr; itr != objLocMap.end(); itr++)
		{
			ui.comboBox->addItem(itr->second.name.c_str(), qint32(itr->second.key));
		}
	}

	void LocationSelectionDialog::setInitialSelection(unsigned long initialLocation)
	{
		m_pTheRealPModel->setInitialSelection(initialLocation);
	}

	unsigned long LocationSelectionDialog::getSelectedLocation()
	{
		return m_pTheRealPModel->getSelectedLocation();
	}

	//ITransActiveView interface
	std::string  LocationSelectionDialog::getMyPModelName(void)
	{ 
		return ""; 
	};

	void LocationSelectionDialog::setupPModel(void)
	{
		m_pTheRealPModel = dynamic_cast<LocationSelectionDlgPModel*>(m_pModel);
	}

	// For dynamic language switching
	void LocationSelectionDialog::translateUI()
	{
		ui.retranslateUi(this);
	}

	void LocationSelectionDialog::onSelectionChangeLocationList(int index)
	{
		unsigned long selData = (qint32)ui.comboBox->userData(index);
		m_pTheRealPModel->setSelectedLocation(selData);
	}

	void LocationSelectionDialog::onOK()
	{
		QDialog::accept();
		this->closeView();
	}

	void LocationSelectionDialog::onCancel()
	{
		QDialog::reject();
		this->closeView();
	}
}
