/**
* The source code in this file is the property of
* Ripple Systems and is not for redistribution
* in any form.
*
* Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/alarm/alarm_manager/src/AlarmFilterDlg.cpp $
* @author:  Ripple
* @version: $Revision: #8 $
*
* Last modification: $DateTime: 2016/12/06 18:37:01 $
* Last modified by:  $Author: Noel $
*
*/
// AlarmFilterDlg.cpp : implementation file
//
#ifdef _MSC_VER
#pragma warning(disable : 4786)
#endif

#include <sstream>
//#include "Afxdtctl.h"
#include "app/alarm/alarm_manager/src/AlarmFilterDlg.h"
#include "app/alarm/alarm_manager/pmodel/src/AlarmFilterDlgPModel.h"
#include "app/alarm/alarm_manager/pmodel/src/PopulateNamedFilterEventArgs.h"
#include "app/alarm/alarm_manager/pmodel/src/PopulateFilterSelEventArgs.h"
#include "app/alarm/alarm_manager/pmodel/src/FilterEventArgs.h"
#include "app/alarm/alarm_manager/pmodel/src/FilterDlgBtnStateUpdate.h"
#include "app/alarm/alarm_manager/pmodel/src/FilterChangeEventArgs.h"

#include "app/alarm/alarm_manager/src/Globals.h"
#include "bus\alarm\alarm_list_control\pmodel\src\Filter.h"
#include "bus/generic_gui_view/src/TransActiveMessage.h"

#include "core\exceptions\src\AlarmListCtrlException.h"
#include "core\exceptions\src\DatabaseException.h"
#include "core\exceptions\src\DataException.h"
#include "app\alarm\alarm_manager\pmodel\src\SubsytemDataCache.h"

#include "core\data_access_interface\src\ISubsystem.h"
#include "core\data_access_interface\src\SubsystemAccessFactory.h"
#include "core\data_access_interface\src\ILocation.h"
#include "core\data_access_interface\src\LocationAccessFactory.h"
#include "core\data_access_interface\src\IOperator.h"
#include "core\data_access_interface\src\OperatorAccessFactory.h"
#include "core\data_access_interface\src\IAlarmSeverityData.h"
#include "core\data_access_interface\src\AlarmSeverityData.h"
#include "core\data_access_interface\src\AlarmSeverityAccessFactory.h"
#include "core\data_access_interface\src\IAlarmTypeData.h"
#include "core\data_access_interface\src\AlarmTypeAccessFactory.h"
#include "core\utilities\src\DebugUtil.h"
#include "core\utilities\src\RunParams.h"

#include "core/data_access_interface/src/NamedFilterAccessFactory.h"
#include "core/exceptions/src/DataException.h"
#include "AlarmFilterDlg.h"
#include <QDateTime>
#include <QPalette>

#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


using TA_Base_Core::NamedFilter;

namespace // unnamed
{
	const time_t EARLY_TIME_LIMIT(1);
	const time_t LATEST_TIME_LIMIT(LONG_MAX);
	const std::string TIME_PICKER_FORMAT("HH:mm:ss");

	const std::string sSave("save");
	const std::string sSaved("saved");

	const std::string sLoad("load");
	const std::string sLoaded("loaded");

	const std::string sDelete("delete");
	const std::string sDeleted("deleted");

} // unnamed namespace

/////////////////////////////////////////////////////////////////////////////
// AlarmFilterDlg dialog
/////////////////////////////////////////////////////////////////////////////

namespace TA_Base_App
{
	/////////////////////////////////////////////////////////////////////////////
	// This is to get the selected item from a list of items, 
	// e.g. the operator list dlg, the location list dlg, alarm list dlg, etc.
	TAUtfString getSelectedItemsString(const TA_Base_App::ListItems& selectedItems)
	{
		TAUtfString text;
		if (selectedItems.empty())
		{
			text = QObject::tr("No Filter");
		}
		else
		{
			for (TA_Base_App::ListItems::const_iterator itr = selectedItems.begin(); itr != selectedItems.end(); ++itr)
			{
				if (itr != selectedItems.begin())
				{
					text += ", ";
				}
				text += TAUtfString(itr->str);
			}
		}
		return text;
	}

	const int AlarmFilterDlg::ALARM_FILTER_DATA_INITIALIZE_TIMER = 1; //TD17081

	/////////////////////////////////////////////////////////////////////////////
	// Ctor
	AlarmFilterDlg::AlarmFilterDlg(TA_Base_Bus::ITransActiveApp* app)
		: AbstractDialog(app, ViewType_AlarmFilter)
		, m_bInitialized(false)
	{	 
		ui.setupUi(this);
		m_timer = new QTimer(this);

		setUpSlotConnection();
		//m_timer->start(1);
	}

	/////////////////////////////////////////////////////////////////////////////
	// Dtor
	AlarmFilterDlg::~AlarmFilterDlg()
	{	
		m_pModel->detachView(this, EV_FILTER_CHANGE);
		m_pModel->detachView(this, EV_FILTERDLG_BTN_UPDATE);
		m_pModel->detachView(this, EV_POPULATE_NAMED_FILTER);
		m_pModel->detachView(this, EV_POPULATE_FILTER_SEL);
	}

	void AlarmFilterDlg::setUpSlotConnection()
	{
		connect(ui.btnOk, SIGNAL(clicked()), this, SLOT(OnOK()));
		connect(ui.btnDefault, SIGNAL(clicked()), this, SLOT(OnClearFiltersButton()));
		connect(ui.btnDelete, SIGNAL(clicked()), this, SLOT(OnDelete()));
		connect(ui.btnApply, SIGNAL(clicked()), this, SLOT(OnApply()));
		connect(ui.btnOk, SIGNAL(clicked()), this, SLOT(OnOK()));
		connect(ui.btnCancel, SIGNAL(clicked()), this, SLOT(OnCancel()));
		//TD 11033 Ensure the same functions are called accordingly regardless of single or double click
		connect(ui.btnDefault, SIGNAL(doubleClicked()), this, SLOT(OnClearFiltersButton()));
		connect(ui.btnDelete, SIGNAL(doubleClicked()), this, SLOT(OnDelete()));
		connect(ui.btnApply, SIGNAL(doubleClicked()), this, SLOT(OnApply()));
		connect(ui.btnOk, SIGNAL(doubleClicked()), this, SLOT(OnOK()));
		connect(ui.btnCancel, SIGNAL(doubleClicked()), this, SLOT(OnCancel()));

		connect(ui.txtEquipmentID, SIGNAL(textChanged(const std::string &)), this, SLOT(OnChangeEditAsset(const std::string &)));
		connect(ui.txtDescription, SIGNAL(textChanged(const std::string &)), this, SLOT(OnChangeEditDescription(const std::string &)));
		connect(ui.dateFrom, SIGNAL(dateChanged(const QDate &)), this, SLOT(OnDatetimechangeDatetimepickerFromdate(const QDate &)));
		connect(ui.timeFrom, SIGNAL(timeChanged(const QTime &)), this, SLOT(OnDatetimechangeDatetimepickerFromtime(const QTime &)));
		connect(ui.dateTo, SIGNAL(dateChanged(const QDate &)), this, SLOT(OnDatetimechangeDatetimepickerTodate(const QDate &)));
		connect(ui.timeTo, SIGNAL(timeChanged(const QTime &)), this, SLOT(OnDatetimechangeDatetimepickerTotime(const QTime &)));

		connect(ui.btnSubsystem, SIGNAL(clicked()), this, SLOT(OnSubsystemTypeButton()));
		//connect(ui.btnSubsystem, SIGNAL(clicked()), this, SLOT(OnSubsystemButton())); // We dont have subsystem
		connect(ui.btnSystem, SIGNAL(clicked()), this, SLOT(OnSystemButton()));
		connect(ui.btnLocation, SIGNAL(clicked()), this, SLOT(OnLocationButton()));
		connect(ui.btnOperator, SIGNAL(clicked()), this, SLOT(OnOperatorButton()));
		connect(ui.btnAlarmType, SIGNAL(clicked()), this, SLOT(OnAlarmTypeButton()));

		connect(ui.btnSave, SIGNAL(clicked()), this, SLOT(OnSave()));
		connect(ui.chkFrom, SIGNAL(stateChanged(int)), this, SLOT(OnFromCheckBoxClicked(int)));
		connect(ui.chkTo, SIGNAL(stateChanged(int)), this, SLOT(OnToCheckBoxClicked(int)));
		connect(ui.listSeverity, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(OnItemChange(QListWidgetItem*))); //severity

		connect(ui.cboNamedFilter, SIGNAL(currentIndexChanged(int)), this, SLOT(OnSelchangeNamedFilter(int))); // named filter
		connect(ui.cboNamedFilter, SIGNAL(editTextChanged(const std::string &)), this, SLOT(OnTextchangeNamedFilter(const std::string &))); //name Filter
		connect(m_timer, SIGNAL(timeout()), this, SLOT(OnTimer()));

		connect(ui.rdoOpen, SIGNAL(toggled(bool)), this, SLOT(OnStateRadioButton(bool)));
		connect(ui.rdoClosed, SIGNAL(toggled(bool)), this, SLOT(OnStateRadioButton(bool)));
		connect(ui.rdoStateAll, SIGNAL(toggled(bool)), this, SLOT(OnStateRadioButton(bool)));

		connect(ui.rdoAcked, SIGNAL(toggled(bool)), this, SLOT(OnAckRadioButton(bool)));
		connect(ui.rdoUnacked, SIGNAL(toggled(bool)), this, SLOT(OnAckRadioButton(bool)));
		connect(ui.rdoAckAll, SIGNAL(toggled(bool)), this, SLOT(OnAckRadioButton(bool)));

		connect(ui.rdoOperation, SIGNAL(toggled(bool)), this, SLOT(OnOMTypeButton(bool)));
		connect(ui.rdoMaintenance, SIGNAL(toggled(bool)), this, SLOT(OnOMTypeButton(bool)));
		connect(ui.rdoBoth, SIGNAL(toggled(bool)), this, SLOT(OnOMTypeButton(bool)));
		connect(ui.rdoOMAll, SIGNAL(toggled(bool)), this, SLOT(OnOMTypeButton(bool)));

		// Custom connection
		connect(this, SIGNAL(showAlarmFilter(const std::string &)), this, SLOT(OnShowFilter(const std::string &)));
		
	}

	AlarmFilterDlg::CFilterCtrlSignalBlocker::CFilterCtrlSignalBlocker(AlarmFilterDlg* parent) : m_parent(parent)
	{
		m_parent->ui.txtEquipmentID->blockSignals(true);
		m_parent->ui.txtDescription->blockSignals(true);
		m_parent->ui.dateFrom->blockSignals(true);
		m_parent->ui.timeFrom->blockSignals(true);
		m_parent->ui.dateTo->blockSignals(true);
		m_parent->ui.timeTo->blockSignals(true);

		m_parent->ui.chkFrom->blockSignals(true);
		m_parent->ui.chkTo->blockSignals(true);
		m_parent->ui.listSeverity->blockSignals(true);
		m_parent->ui.cboNamedFilter->blockSignals(true);
		m_parent->ui.rdoOpen->blockSignals(true);
		m_parent->ui.rdoClosed->blockSignals(true);
		m_parent->ui.rdoStateAll->blockSignals(true);

		m_parent->ui.rdoAcked->blockSignals(true);
		m_parent->ui.rdoUnacked->blockSignals(true);
		m_parent->ui.rdoAckAll->blockSignals(true);

		m_parent->ui.rdoOperation->blockSignals(true);
		m_parent->ui.rdoMaintenance->blockSignals(true);
		m_parent->ui.rdoBoth->blockSignals(true);
		m_parent->ui.rdoOMAll->blockSignals(true);
	}

	AlarmFilterDlg::CFilterCtrlSignalBlocker::~CFilterCtrlSignalBlocker() 
	{
		m_parent->ui.txtEquipmentID->blockSignals(false);
		m_parent->ui.txtDescription->blockSignals(false);
		m_parent->ui.dateFrom->blockSignals(false);
		m_parent->ui.timeFrom->blockSignals(false);
		m_parent->ui.dateTo->blockSignals(false);
		m_parent->ui.timeTo->blockSignals(false);

		m_parent->ui.chkFrom->blockSignals(false);
		m_parent->ui.chkTo->blockSignals(false);
		m_parent->ui.listSeverity->blockSignals(false);
		m_parent->ui.cboNamedFilter->blockSignals(false);
		m_parent->ui.rdoOpen->blockSignals(false);
		m_parent->ui.rdoClosed->blockSignals(false);
		m_parent->ui.rdoStateAll->blockSignals(false);

		m_parent->ui.rdoAcked->blockSignals(false);
		m_parent->ui.rdoUnacked->blockSignals(false);
		m_parent->ui.rdoAckAll->blockSignals(false);

		m_parent->ui.rdoOperation->blockSignals(false);
		m_parent->ui.rdoMaintenance->blockSignals(false);
		m_parent->ui.rdoBoth->blockSignals(false);
		m_parent->ui.rdoOMAll->blockSignals(false);
	}

	void AlarmFilterDlg::notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args/* = NULL*/)
	{
		if (_stricmp(updateType, EV_FILTER_CHANGE) == 0)
		{
			FilterChangeEventArgs* evArgs = dynamic_cast<FilterChangeEventArgs*>(args);
			int idx = ui.cboNamedFilter->findText(evArgs->getFiltername().c_str());
			if (idx == -1)
			{
				ui.cboNamedFilter->blockSignals(true);
				ui.cboNamedFilter->addItem(evArgs->getFiltername().c_str());
				int newIdx = ui.cboNamedFilter->findText(evArgs->getFiltername().c_str());
				ui.cboNamedFilter->setCurrentIndex(newIdx);

				ui.cboNamedFilter->blockSignals(false);
			}
			else
				ui.cboNamedFilter->setCurrentIndex(idx);
			
		}
		else if (_stricmp(updateType, EV_FILTERDLG_BTN_UPDATE) == 0)
		{	
			FilterDlgBtnStateEventArgs* evArgs = dynamic_cast<FilterDlgBtnStateEventArgs*>(args);
			T_ButtonStateMap temp = evArgs->getButtonStateMap();
			updateButtonState(temp);
		}
		else if (_stricmp(updateType, EV_POPULATE_NAMED_FILTER) == 0)
		{
			PopulateNamedFilterEventArgs* evArgs = dynamic_cast<PopulateNamedFilterEventArgs*>(args);
			populateNamedFilters(evArgs->getFiltername());
		}
		else if (_stricmp(updateType, EV_POPULATE_FILTER_SEL) == 0)
		{
			PopulateFilterSelEventArgs* evArgs = dynamic_cast<PopulateFilterSelEventArgs*>(args);
			populateFilterSelections(*(evArgs->getFilterObject()));
		}
	}

	/////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////
	// AlarmFilterDlg message handlers
	/////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////
	void AlarmFilterDlg::initUIControl()
	{
		m_timer->start(0);
		// ++TD17081

		m_fromDateMask.setParent(ui.grpDateTime);
		m_fromTimeMask.setParent(ui.grpDateTime);
		m_toDateMask.setParent(ui.grpDateTime);
		m_toTimeMask.setParent(ui.grpDateTime);
		
		m_fromDateMask.setGeometry(ui.dateFrom->geometry());
		m_fromTimeMask.setGeometry(ui.timeFrom->geometry());
		m_toDateMask.setGeometry(ui.dateTo->geometry());
		m_toTimeMask.setGeometry(ui.timeTo->geometry());

		m_fromDateMask.move(ui.dateFrom->pos());
		m_fromTimeMask.move(ui.timeFrom->pos());
		m_toDateMask.move(ui.dateTo->pos());
		m_toTimeMask.move(ui.timeTo->pos());

		m_fromDateMask.setVisible(false);
		m_fromTimeMask.setVisible(false);
		m_toDateMask.setVisible(false);
		m_toTimeMask.setVisible(false);

		// Window is being shown.
		disableButtons(); //Disable all buttons except "Cancel"
		
		// Set the time format on the date/time pickers
		ui.dateTo->setDisplayFormat(TIME_PICKER_FORMAT.c_str());
		ui.dateFrom->setDisplayFormat(TIME_PICKER_FORMAT.c_str());

		//TD15028 ++ wangyn
		OnFromCheckBoxClicked(0);
		OnToCheckBoxClicked(0);
		//++ TD15028
		//m_noApply = false;

		//changxing  always make ok button enable.
		ui.btnOk->setEnabled(true);
		ui.cboNamedFilter->setMaxCount(32);

		clearAllDisplays();
		LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "TD15027: Set limit of text");
	}

	void AlarmFilterDlg::syncUIDataToPModel()
	{
		FUNCTION_ENTRY("syncUIDataToPModel");
		// Todo: if needed
		FUNCTION_EXIT;
	}

	std::string AlarmFilterDlg::getMyPModelName(void)
	{
		return PMODEL_AlarmFilterDlg;
	}

	void AlarmFilterDlg::setupPModel(void)
	{
		m_pTheRealPModel = dynamic_cast<AlarmFilterDlgPModel*> (m_pModel);

		m_pModel->attachView(this, EV_FILTER_CHANGE);
		m_pModel->attachView(this, EV_FILTERDLG_BTN_UPDATE);
		m_pModel->attachView(this, EV_POPULATE_NAMED_FILTER);
		m_pModel->attachView(this, EV_POPULATE_FILTER_SEL);
	}

	void AlarmFilterDlg::translateUI()
	{
		ui.retranslateUi(this);
	}
	/////////////////////////////////////////////////////////////////////////////
	//TD17081
	void AlarmFilterDlg::OnTimer()
	{
		// Get the list of items for multiple selection dialogs
		m_pTheRealPModel->retrieveListItems();

		// get severity and display it
		ListSeverityItems severityList = m_pTheRealPModel->getSeverityList();
		ui.listSeverity->clear();
		QVBoxLayout *vbox = new QVBoxLayout;
		for (ListSeverityItems::iterator itr = severityList.begin(); itr != severityList.end(); itr++)
		{
			int r = (*itr).color & 0xff;
			int g = ((*itr).color >> 8) & 0xff;
			int b = ((*itr).color >> 16) & 0xff;
			QColor backgcolor(r, g, b);

			QListWidgetItem *item = new QListWidgetItem;
			item->setData(Qt::DisplayRole, (*itr).str.c_str());
			item->setData(Qt::CheckStateRole, Qt::Checked);
			item->setData(Qt::TextColorRole, backgcolor);
			item->setData(Qt::UserRole, (int)(*itr).pkey);
			ui.listSeverity->addItem(item);
			m_pTheRealPModel->addSelectedSeverity(itr->pkey); // severity is selected by default
		}

		populateNamedFilters(AlarmFilter::FILTER_DEFAULT);
		m_timer->stop();
		m_bInitialized = true;
	}

	/////////////////////////////////////////////////////////////////////////////
	void AlarmFilterDlg::OnOK()
	{
		// Apply the filter.
		m_pTheRealPModel->applyFilter();

		QDialog::accept();
	}

	/////////////////////////////////////////////////////////////////////////////
	// On clicking Default button
	//
	void AlarmFilterDlg::OnClearFiltersButton()
	{
		// Clears all filter selections.
		m_pTheRealPModel->clearFilter();

		// Clear display
		clearAllDisplays();
	
		if (m_pTheRealPModel->updateNeeded() == true)
		{
			enableButtons();
		}
	}

	/////////////////////////////////////////////////////////////////////////////
	// Clear or reset all controls and display
	//
	void AlarmFilterDlg::clearAllDisplays()
	{
		CFilterCtrlSignalBlocker objSignalBlocker(this);

		/*ui.txtSystem->setText("");
		ui.txtSubsystem->setText("");
		ui.txtLocation->setText("");
		ui.txtOperator->setText("");
		ui.txtAlarmType->setText("");*/
		ui.txtEquipmentID->setText(std::string(""));
		ui.txtDescription->setText(std::string(""));

		populateSubsystemsEditCtrl();
		populateSubsystemTypesEditCtrl();
		populateSystemsEditCtrl();
		populateLocationsEditCtrl();
		populateOperatorsEditCtrl();
		populateAlarmTypesEditCtrl();

		// Set the "All" radio buttons.
		setAckRadioButton(AckFilter::ALL_ACK);
		setStateRadioButton(StateFilter::ALL_STATE);
		setOMTypeButton(OMFilter::ALL_OM);

		// make sure the Ack/Closed radio buttons don't stay disabled if they previously were
		//static_cast<CButton*>(GetDlgItem(IDC_ACK_ACKED))->EnableWindow(true);
		//static_cast<CButton*>(GetDlgItem(IDC_STATE_CLOSED))->EnableWindow(true);

		// Unset the from/to time check boxes.
		ui.chkFrom->setChecked(false);
		ui.chkTo->setChecked(false);
		
		// Disable the date/time controls.
		fromCheckBoxUpdated(false);
		toCheckBoxUpdated(false);

		// Check the severity checkboxes.
		SetAllSeverityFilterControls(true);
	}

	/////////////////////////////////////////////////////////////////////////////
	void AlarmFilterDlg::OnApply()
	{
		m_pTheRealPModel->applyFilter();
	}

	/////////////////////////////////////////////////////////////////////////////
	void AlarmFilterDlg::OnChangeEditAsset(const std::string & text)
	{
		m_pTheRealPModel->setEquipmentID(text);
		m_pTheRealPModel->setUpdateNeeded(true);
	}

	/////////////////////////////////////////////////////////////////////////////
	void AlarmFilterDlg::OnChangeEditDescription(const std::string & text)
	{
		m_pTheRealPModel->setDescription(text);
		m_pTheRealPModel->setUpdateNeeded(true);
	}

	void AlarmFilterDlg::OnTextchangeNamedFilter(const std::string & text)
	{
		// if the text entered is the name of an existing filter (in the combo)
		TAUtfString str = text;
		TAUtfString currFiltername = m_pTheRealPModel->getFilterObject()->getFilterName().c_str();
		bool isCurFilter = str.compare(currFiltername) ? false : true;

		str.toUpper();
		if (!str.compare(TAUtfString("")) || str.contains(TAUtfString("DEFAULT")) == true
			|| str.contains(TAUtfString("USER")) == true)
		{
			ui.btnSave->setEnabled(false);
		}
		else if (isCurFilter && !m_pTheRealPModel->updateNeeded() ) // there is no change in name and settings
		{
			ui.btnSave->setEnabled(false);
		}
		else
		{
			// TD #13187
			// don't call setUpdateNeeded here, otherwise the named filter combo
			// box will reset to 'User Defined' on every keystroke
			//m_pTheRealPModel->setUpdateNeeded(true);
			ui.btnSave->setEnabled(true);

		}
	}
	/////////////////////////////////////////////////////////////////////////////
	void AlarmFilterDlg::OnDatetimechangeDatetimepickerFromdate(const QDate & date)
	{
		// See OnDatetimechangeDatetimepickerTodate for comments
		QDateTime temp;
		temp.setDate(date);
		m_pTheRealPModel->setFromDate(temp.toTime_t());
	}

	/////////////////////////////////////////////////////////////////////////////
	void AlarmFilterDlg::OnDatetimechangeDatetimepickerFromtime(const QTime & time)
	{
		// See OnDatetimechangeDatetimepickerTotime for comments
		QDateTime temp;
		temp.setTime(time);

		m_pTheRealPModel->setFromTime(temp.toTime_t());
		
		QDateTime dateFrom = QDateTime::fromTime_t(m_pTheRealPModel->getFromDateTime());
		QDateTime dateTo = QDateTime::fromTime_t(m_pTheRealPModel->getToDateTime());

		ui.timeTo->setDateTimeRange(dateFrom, dateTo);
	}

	/////////////////////////////////////////////////////////////////////////////
	void AlarmFilterDlg::OnDatetimechangeDatetimepickerTodate(const QDate & date)
	{
		QDateTime temp;
		temp.setDate(date);
		m_pTheRealPModel->setToDate(temp.toTime_t());
	}

	/////////////////////////////////////////////////////////////////////////////
	void AlarmFilterDlg::OnDatetimechangeDatetimepickerTotime(const QTime & time)
	{
		QDateTime temp;
		temp.setTime(time);
		m_pTheRealPModel->setToTime(temp.toTime_t());
	}

	/////////////////////////////////////////////////////////////////////////////
	void AlarmFilterDlg::OnSubsystemButton()
	{	
		ITransActiveView *pView = this->getTransActiveWinApp()->getViewManager()->getView(ViewType_MultiSel, SubSystem_ViewID);
		multipleSelGuiDlg* subsystemDlg = dynamic_cast<multipleSelGuiDlg*>(pView);
		subsystemDlg->setDialogTitle(tr("Select Subsystem..."));
		subsystemDlg->setItems(m_pTheRealPModel->getSubSystemList(), true);
		subsystemDlg->setSelection(m_pTheRealPModel->getSelSubSystemList());
		
		boost::shared_ptr<ListItems> selItems(new ListItems);
		subsystemDlg->setResult(selItems);
		if (subsystemDlg->doModal() == IDOK)
		{	
			m_pTheRealPModel->setSelSubSystemList(*selItems.get());
			populateSubsystemsEditCtrl(); // update edit control
		}
	}
	/////////////////////////////////////////////////////////////////////////////
	void AlarmFilterDlg::OnSubsystemTypeButton()
	{
		//m_subsystemTypes.clear();
		//m_subsystemTypes = SubsystemDataCache::getInstance().getSubsystemsFormSystems(m_selectedSystems);
		ITransActiveView *pView = this->getTransActiveWinApp()->getViewManager()->getView(ViewType_MultiSel, SubSystemType_ViewID);
		multipleSelGuiDlg* subsystemTypeDlg = dynamic_cast<multipleSelGuiDlg*>(pView);
		subsystemTypeDlg->setDialogTitle(tr("Select Subsystem..."));
		
		subsystemTypeDlg->setItems(m_pTheRealPModel->getSubSystemTypeList(), true);
		subsystemTypeDlg->setSelection(m_pTheRealPModel->getSelSubSystemTypeList());

		boost::shared_ptr<ListItems> selItems(new ListItems);
		subsystemTypeDlg->setResult(selItems);
		if (subsystemTypeDlg->doModal() == IDOK)
		{
			m_pTheRealPModel->setSelSubSystemTypeList(*selItems.get());
			populateSubsystemTypesEditCtrl(); // update edit control
		}
	}

	/////////////////////////////////////////////////////////////////////////////
	void AlarmFilterDlg::OnSystemButton()
	{
		//retrieveSystems();
		ITransActiveView *pView = this->getTransActiveWinApp()->getViewManager()->getView(ViewType_MultiSel, SystemID_ViewID);
		multipleSelGuiDlg* systemDlg = dynamic_cast<multipleSelGuiDlg*>(pView);
		systemDlg->setDialogTitle(tr("Select System..."));
		systemDlg->setItems(m_pTheRealPModel->getSystemList(), true);
		systemDlg->setSelection(m_pTheRealPModel->getSelSystemList());

		boost::shared_ptr<ListItems> selItems(new ListItems);
		systemDlg->setResult(selItems);
		if (systemDlg->doModal() == IDOK)
		{
			m_pTheRealPModel->setSelSystemList(*selItems.get());
			m_pTheRealPModel->setSelSubSystemTypeList(ListItems()); // clear selection
			//m_selectedSubsystemTypes.clear();
			populateSubsystemTypesEditCtrl();

			populateSystemsEditCtrl(); // update edit control
		}
	}

	/////////////////////////////////////////////////////////////////////////////
	void AlarmFilterDlg::OnLocationButton()
	{
		ITransActiveView *pView = this->getTransActiveWinApp()->getViewManager()->getView(ViewType_MultiSel, Location_ViewID);
		multipleSelGuiDlg* locationDlg = dynamic_cast<multipleSelGuiDlg*>(pView);
		locationDlg->setDialogTitle(tr("Select Location..."));
		locationDlg->setItems(m_pTheRealPModel->getLocationList(), true);
		locationDlg->setSelection(m_pTheRealPModel->getSelLocationList());
		locationDlg->setSortItems(false);

		boost::shared_ptr<ListItems> selItems(new ListItems);
		locationDlg->setResult(selItems);
		if (locationDlg->doModal() == IDOK)
		{
			m_pTheRealPModel->setSelLocationList(*selItems.get());
			populateLocationsEditCtrl(); // update edit control
		}
	}

	/////////////////////////////////////////////////////////////////////////////
	void AlarmFilterDlg::OnOperatorButton()
	{	
		ITransActiveView *pView = this->getTransActiveWinApp()->getViewManager()->getView(ViewType_MultiSel, Operator_ViewID);
		multipleSelGuiDlg* operatorDlg = dynamic_cast<multipleSelGuiDlg*>(pView);
		operatorDlg->setDialogTitle(tr("Select Operators..."));
		operatorDlg->setItems(m_pTheRealPModel->getOperatorList(), true);
		operatorDlg->setSelection(m_pTheRealPModel->getSelOperatorList());

		boost::shared_ptr<ListItems> selItems(new ListItems);
		operatorDlg->setResult(selItems);
		if (operatorDlg->doModal() == IDOK)
		{
			m_pTheRealPModel->setSelOperatorList(*selItems.get());
			populateOperatorsEditCtrl(); // update edit control
		}
	}

	/////////////////////////////////////////////////////////////////////////////
	void AlarmFilterDlg::OnAlarmTypeButton()
	{	
		ITransActiveView *pView = this->getTransActiveWinApp()->getViewManager()->getView(ViewType_MultiSel, AlarmType_ViewID);
		multipleSelGuiDlg* alarmTypeDlg = dynamic_cast<multipleSelGuiDlg*>(pView);
		alarmTypeDlg->setDialogTitle(tr("Select Alarm Types..."));
		alarmTypeDlg->setItems(m_pTheRealPModel->getAlarmTypeList(), true);
		alarmTypeDlg->setSelection(m_pTheRealPModel->getSelAlarmTypeList());

		boost::shared_ptr<ListItems> selItems(new ListItems);
		alarmTypeDlg->setResult(selItems);
		if (alarmTypeDlg->doModal() == IDOK)
		{
			m_pTheRealPModel->setSelAlarmTypeList(*selItems.get());
			populateAlarmTypesEditCtrl(); // update edit control
		}
	}

	/////////////////////////////////////////////////////////////////////////////
	void AlarmFilterDlg::OnCancel()
	{
		m_pTheRealPModel->cancel();
		QDialog::reject();

	}

	/////////////////////////////////////////////////////////////////////////////
	void AlarmFilterDlg::OnSave()
	{
		FUNCTION_ENTRY("OnSaveFilter");
		m_pTheRealPModel->saveFilter();

		// TD12755 Reset the save button
		ui.btnSave->setEnabled(false);
		FUNCTION_EXIT;
	}

	/////////////////////////////////////////////////////////////////////////////
	void AlarmFilterDlg::OnFromCheckBoxClicked(int state)
	{
		// Enable or disable the From date/time controls.
		bool enableWindow = ui.chkFrom->isChecked();

		QDateTime currentTime = QDateTime::currentDateTime();
		if (enableWindow)
		{
			// TD 8703: When checkboxes are enabled, set times to earliest and latest in displayed list.
			time_t earliestTime = m_pTheRealPModel->getEarliestAlarmTime();

			// get the currently set date/time of the 'To' fields
			time_t toDateTime = m_pTheRealPModel->getToDateTime();
			if (earliestTime == 0)
			{
				// If there are no alarms displayed, then set to current time
				earliestTime = currentTime.toTime_t();
			}

			time_t fromDateTime = earliestTime;
			// if the 'To' checkbox is ticked, then the maximum range needs to be limited
			if (ui.chkTo->isChecked() == true)
			{
				// set the maximum range bound to be the time that 'To' is set to,
				// so the user cannot overlap to/from times
				//m_fromDate.SetRange(&EARLY_TIME_LIMIT,&toDateTime);
				//m_fromTime.SetRange(&EARLY_TIME_LIMIT,&toDateTime);

				if (fromDateTime > toDateTime)
				{
					fromDateTime = toDateTime;
				}
			}
			// ..otherwise, the maximum range can extend to the current time
			else
			{
				//m_fromDate.SetRange(&EARLY_TIME_LIMIT,&currentTime);
				//m_fromTime.SetRange(&EARLY_TIME_LIMIT,&currentTime);
			}

			// set the initial date/time of the 'From' fields to be that of the
			// earliest alarm. if there are no alarms, this will be the current date/time
			m_pTheRealPModel->setFromDate(fromDateTime);
			m_pTheRealPModel->setFromTime(fromDateTime);

			ui.dateFrom->setDate(QDateTime::fromTime_t(fromDateTime).date()); //m_fromDate.SetTime(&fromDateTime);
			ui.timeFrom->setTime(QDateTime::fromTime_t(fromDateTime).time()); //m_fromTime.SetTime(&fromDateTime);
			
			// set the minimum range bound of 'To' to be the time that 'From' has been set to,
			// again, so the user cannot overlap to/from times		
			//m_toTime.SetRange(&fromDateTime,&currentTime);
			//m_toDate.SetRange(&fromDateTime,&currentTime);

		}
		else
		{
			//m_toTime.SetRange(&EARLY_TIME_LIMIT,&currentTime);
			//m_toDate.SetRange(&EARLY_TIME_LIMIT,&currentTime);

			//marvin++
			//TD14801
			//when to pressed must check for earliest time
			time_t earliestTime = m_pTheRealPModel->getEarliestAlarmTime();

			//TD15208++
			if (earliestTime == 0)
			{
				earliestTime = currentTime.toTime_t();
			}
			//++TD15208
			time_t fromEarliest = earliestTime;

			//TD15028++ wangyn
			ui.dateFrom->setDate(QDateTime::fromTime_t(fromEarliest).date()); //m_fromDate.SetTime(&fromEarliest);
			ui.timeFrom->setTime(QDateTime::fromTime_t(fromEarliest).time()); //m_fromTime.SetTime(&fromEarliest);
			//++TD15028
			//m_toDate.SetRange(&fromEarliest,&currentTime);
			//m_toTime.SetRange(&fromEarliest,&currentTime);
			//marvin++
			//TD14801
		}

		ui.dateFrom->setEnabled(enableWindow);
		ui.timeFrom->setEnabled(enableWindow);
		fromCheckBoxUpdated(enableWindow);	//TD15208
		m_pTheRealPModel->setUpdateNeeded(true);
	}

	/////////////////////////////////////////////////////////////////////////////
	void AlarmFilterDlg::OnToCheckBoxClicked(int state)
	{
		// Enable or disable the To date/time controls.
		bool enableWindow = ui.chkTo->isChecked();
		QDateTime currentTime = QDateTime::currentDateTime();

		if (enableWindow)
		{
			// TD 8703: When checkboxes are enabled, set times to earliest and latest in displayed list.
			time_t latestTime = m_pTheRealPModel->getLatestAlarmTime();
			time_t fromDateTime = m_pTheRealPModel->getFromDateTime();

			if (latestTime == 0)
			{
				// If there are no alarms displayed, then set to current time
				latestTime = currentTime.toTime_t();
			}

			time_t toDateTime = latestTime;
			if (ui.chkFrom->isChecked() == true)
			{
				// set the minimum range bound to be the time that 'From' is set to,
				// so the user cannot overlap to/from times
				//m_toDate.SetRange(&fromDateTime, &currentTime);
				//m_toTime.SetRange(&fromDateTime, &currentTime);
				ui.dateTo->setDateTimeRange(QDateTime::fromTime_t(fromDateTime), currentTime);
				ui.timeTo->setDateTimeRange(QDateTime::fromTime_t(fromDateTime), currentTime);
				if (toDateTime < fromDateTime)
				{
					toDateTime = fromDateTime;
				}
			}
			// ..otherwise, the minimum range can extend back to the earliest time
			else
			{

				//marvin++
				//TD14801
				//when to pressed must check for earliest time
				time_t earliestTime = m_pTheRealPModel->getEarliestAlarmTime();
				time_t fromDateTime = earliestTime;

				//m_toDate.SetRange(&fromDateTime,&currentTime);
				//m_toTime.SetRange(&fromDateTime,&currentTime);
				//marvin++
				//TD14801
			}

			// set the initial date/time of the 'To' fields to be that of the
			// latest alarm. if there are no alarms, this will be the current date/time
			ui.dateTo->setDateTime(QDateTime::fromTime_t(toDateTime));
			ui.timeTo->setDateTime(QDateTime::fromTime_t(toDateTime));
			
			// set the maximum range bound of 'From' to be the time that 'To' has been set to,
			// again, so the user cannot overlap to/from times
			// m_fromDate.SetRange(&EARLY_TIME_LIMIT,&toDateTime);
			// m_fromTime.SetRange(&EARLY_TIME_LIMIT,&toDateTime);
		}
		else
		{
			//TD15028++ wangyn
			time_t latestTime = m_pTheRealPModel->getLatestAlarmTime();

			if (latestTime == 0)
			{
				latestTime = currentTime.toTime_t();
			}

			time_t toLatestTime =latestTime;

			ui.dateTo->setDateTime(QDateTime::fromTime_t(toLatestTime));
			ui.timeTo->setDateTime(QDateTime::fromTime_t(toLatestTime));
			//++TD15208
			ui.dateFrom->setDateTimeRange(QDateTime::fromTime_t(EARLY_TIME_LIMIT), currentTime);
			ui.dateFrom->setDateTimeRange(QDateTime::fromTime_t(EARLY_TIME_LIMIT), currentTime);
		}
		ui.dateTo->setEnabled(enableWindow); 
		ui.timeTo->setEnabled(enableWindow);
		toCheckBoxUpdated(enableWindow);	//TD15208
		m_pTheRealPModel->setUpdateNeeded(true);
	}

	/////////////////////////////////////////////////////////////////////////////
	// This is normally called by the parent to inform filter dialog to display
	// the named filter passed
	//
	void AlarmFilterDlg::showFilter(std::string filterName)
	{
		FUNCTION_ENTRY("showFilter");
	
		// Note: We need to queue the call so that the timer initialization can be executed first so
		// we can retrieve all information from database and initialize the controls before we display the target filter settings.
		QMetaObject::invokeMethod(this, "showAlarmFilter", Qt::QueuedConnection, Q_ARG(const std::string&, filterName));

		FUNCTION_EXIT;
	}

	void AlarmFilterDlg::OnShowFilter(const std::string & filterName)
	{
		FUNCTION_ENTRY("OnShowFilter");
		
		CFilterCtrlSignalBlocker objBlocker(this);
		m_pTheRealPModel->setNewFilter(filterName);

		FUNCTION_EXIT;
	}

	/////////////////////////////////////////////////////////////////////////////
	void AlarmFilterDlg::OnStateRadioButton(bool checked)
	{
		// The state of the "Acked" radio button should be the opposite
		// of the "Closed" radio button.
		StateFilter alarmState;
		if (ui.rdoOpen->isChecked() == true)
			alarmState = StateFilter::OPEN;
		else if (ui.rdoClosed->isChecked() == true)
			alarmState = StateFilter::CLOSED;
		else
			alarmState = StateFilter::ALL_STATE;

		m_pTheRealPModel->setAlarmState(alarmState);
		ui.rdoAcked->setEnabled(!ui.rdoClosed->isChecked());
	}

	/////////////////////////////////////////////////////////////////////////////
	void AlarmFilterDlg::OnAckRadioButton(bool checked)
	{
		// The state of the "Closed" radio button should be the opposite
		// of the "Acked" radio button.
		AckFilter ackState;

		if (ui.rdoAcked->isChecked() == true)
			ackState = AckFilter::ACKED;
		else if (ui.rdoUnacked->isChecked() == true)
			ackState = AckFilter::UNACKED;
		else
			ackState = AckFilter::ALL_ACK;

		m_pTheRealPModel->setAckState(ackState);		
		ui.rdoClosed->setEnabled(!ui.rdoAcked->isChecked());

		m_pTheRealPModel->setUpdateNeeded(true);
	}

	void AlarmFilterDlg::OnOMTypeButton(bool checked)
	{
		// The state of the "Closed" radio button should be the opposite
		// of the "Acked" radio button.
		OMFilter omFlag;
		if (ui.rdoOperation->isChecked() == true)
			omFlag = OMFilter::OPERATION;
		else if (ui.rdoMaintenance->isChecked() == true)
			omFlag = OMFilter::MAINTENANCE;
		else if (ui.rdoBoth->isChecked() == true)
			omFlag = OMFilter::BOTH;
		else
			omFlag = OMFilter::ALL_OM;

		m_pTheRealPModel->setOperationalMode(omFlag);
	}
	
	void AlarmFilterDlg::OnItemChange(QListWidgetItem* item)
	{
		if (item->checkState() == Qt::Checked)
			m_pTheRealPModel->addSelectedSeverity(item->data(Qt::UserRole).value<int>());
		else
			m_pTheRealPModel->removeSelectedSeverity(item->data(Qt::UserRole).value<int>());
		
		m_pTheRealPModel->setUpdateNeeded(true);
	}


	/////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////
	// AlarmFilterDlg helper functions
	/////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////
	
	/////////////////////////////////////////////////////////////////////////////
	void AlarmFilterDlg::populateFilterSelections(TA_Base_Bus::Filter& filter)
	{
		CFilterCtrlSignalBlocker objBlocker(this);
		// Clear the dialog.
		clearAllDisplays();

		// Need to clear the severity filters checkboxes as they will be individually
		// set by the filters as they are loaded
		SetAllSeverityFilterControls(false);

		std::vector<TA_Base_Bus::Filter::FilterDetails> filterSettings = filter.getFilters();

		std::string stringValue;
		unsigned long ulValue;
		bool isRangeSet = false;
		// Need to track how many severity filters have been set as there is a special case
		// on dialog init where there can be zero severity filters. This means load ALL
		// alarm severities. We need to put the code to handle this case in here because the
		// cleaner option (code-wise) would be to force the filter to explicitly specify all
		// severities, but this would cause an alarm store reload whenever the dialog is
		// loaded so it is not practical.
		unsigned int numSeverityFilters = 0;

		for (unsigned int i = 0; i < filterSettings.size(); ++i)
		{
			TA_Base_Bus::Filter::FilterDetails setting = filterSettings[i];
			switch (setting.type)
			{
			case TA_Base_Bus::Filter::FILTER_ALARM_TIME:
				ulValue = setting.value.ulValue;
				if (setting.comparison == TA_Base_Bus::Filter::COMPARE_GREATER_THAN_EQUAL)
				{
					// Filter was on from time. Tick the check button.
					ui.chkFrom->setChecked(true);

					// Enable the from time controls
					ui.dateFrom->setEnabled(true);
					ui.timeFrom->setEnabled(true);
					
					// Populate the date and time
					TA_Base_Bus::Filter::SDateRange dateRange = setting.value.dateRange;
					QDateTime fromDateTime = QDateTime::fromTime_t(dateRange.startTime);
					ui.dateFrom->setDateTime(fromDateTime);
					ui.timeFrom->setDateTime(fromDateTime);
					
					// Set valid time range
					QDateTime time = QDateTime::currentDateTime();
					ui.dateFrom->setDateTimeRange(QDateTime::fromTime_t(EARLY_TIME_LIMIT), time);
					ui.timeFrom->setDateTimeRange(QDateTime::fromTime_t(EARLY_TIME_LIMIT), time);
					ui.dateTo->setDateTimeRange(fromDateTime, time);
					ui.timeTo->setDateTimeRange(fromDateTime, time);
				}
				else if (setting.comparison == TA_Base_Bus::Filter::COMPARE_LESS_THAN_EQUAL)
				{
					// Filter was on to time. Tick the check button.
					ui.chkTo->setChecked(true);
					
					// Enable the to time controls
					ui.dateTo->setEnabled(true);
					ui.timeTo->setEnabled(true);

					// Populate the date and time
					TA_Base_Bus::Filter::SDateRange dateRange = setting.value.dateRange;
					QDateTime toDateTime = QDateTime::fromTime_t(dateRange.startTime);

					ui.dateTo->setDateTime(toDateTime);
					ui.timeTo->setDateTime(toDateTime);
					ui.dateFrom->setDateTime(toDateTime);
					ui.timeFrom->setDateTime(toDateTime);
					
					// Set valid time range
					QDateTime time = QDateTime::currentDateTime();
					ui.dateFrom->setDateTimeRange(QDateTime::fromTime_t(EARLY_TIME_LIMIT), toDateTime);
					ui.timeFrom->setDateTimeRange(QDateTime::fromTime_t(EARLY_TIME_LIMIT), toDateTime);
				}

				break;

			case TA_Base_Bus::Filter::FILTER_ALARM_TIME_RANGE:
			{
				// Set the check buttons as checked.
				ui.chkFrom->setChecked(true);
				ui.chkTo->setChecked(true);

				// Enable the date/time controls
				ui.dateFrom->setEnabled(true);
				ui.timeFrom->setEnabled(true);
				ui.dateTo->setEnabled(true);
				ui.timeTo->setEnabled(true);

				TA_Base_Bus::Filter::SDateRange dateRange = setting.value.dateRange;
				QDateTime fromDateTime = QDateTime::fromTime_t(dateRange.startTime);
				ui.dateFrom->setDateTime(fromDateTime);
				ui.timeFrom->setDateTime(fromDateTime);

				QDateTime toDateTime = QDateTime::fromTime_t(dateRange.endTime);
				ui.dateTo->setDateTime(toDateTime);
				ui.timeTo->setDateTime(toDateTime);


				QDateTime time = QDateTime::currentDateTime();
				ui.dateFrom->setDateTimeRange(QDateTime::fromTime_t(EARLY_TIME_LIMIT), toDateTime);
				ui.timeFrom->setDateTimeRange(QDateTime::fromTime_t(EARLY_TIME_LIMIT), toDateTime);
				ui.dateTo->setDateTimeRange(fromDateTime, QDateTime::fromTime_t(EARLY_TIME_LIMIT));
				ui.timeTo->setDateTimeRange(fromDateTime, QDateTime::fromTime_t(EARLY_TIME_LIMIT));

				// flag that the time range has been set
				isRangeSet = true;

				// enable the controls
				fromCheckBoxUpdated(true);
				toCheckBoxUpdated(true);
			}
			break;

			case TA_Base_Bus::Filter::FILTER_ASSET_NAME:
				stringValue = *(setting.value.stringValue);
				ui.txtEquipmentID->setText(stringValue);
				break;

			case TA_Base_Bus::Filter::FILTER_DESCRIPTION:
				stringValue = *(setting.value.stringValue);
				ui.txtDescription->setText(stringValue);
				break;

			case TA_Base_Bus::Filter::FILTER_SUBSYSTEM:
			{
				m_pTheRealPModel->addSelectedSubsystem(setting.value.ulValue);
			}
			break;

			case TA_Base_Bus::Filter::FILTER_LOCATION:
			{
				ListItems::iterator it;
				m_pTheRealPModel->addSelectedLocation(setting.value.ulValue);
			}
			break;

			case TA_Base_Bus::Filter::FILTER_OPERATOR:
			{
				stringValue = *(setting.value.stringValue);
				m_pTheRealPModel->addSelectedOperator(stringValue);
			}
			break;

			case TA_Base_Bus::Filter::FILTER_ALARM_TYPE:
			{
				ulValue = setting.value.ulValue;;
				m_pTheRealPModel->addSelectedAlarmType(ulValue);
			}
			break;

			case TA_Base_Bus::Filter::FILTER_SEVERITY:
				// BJR KLUDGE: This is pretty ugly, but I can't think of anything a lot
				// cleaner that isn't a lot of work (this previously used the resource
				// directly (which was still a kludge, but was fewer lines of code) but
				// this won't work with the CColorCheck class.
				++numSeverityFilters;
				ulValue = setting.value.ulValue; // Severity pkey
				if (setting.comparison == TA_Base_Bus::Filter::COMPARE_EQUAL)
				{
					m_pTheRealPModel->addSelectedSeverity(ulValue);
					// Set filter check
					for (int idx=0; idx < ui.listSeverity->count(); idx++)
					{
						QListWidgetItem* listItem = ui.listSeverity->item(idx);
						int pkey = listItem->data(Qt::UserRole).value<int>();
						if (pkey == ulValue)
						{
							listItem->setData(Qt::CheckStateRole, 1);
							break;
						}
					}

				}
				else if (setting.comparison == TA_Base_Bus::Filter::COMPARE_LESS_THAN)
				{
					// The less than filter is used to indicate that ALL severities
					// are currently filtered out. Don't need to do anything since we
					// have already cleared all the filters
					TA_ASSERT(ulValue == 0, "Invalid filter parameter");
				}
				else
				{
					// This is not possible unless a code maintainer adds a severity filter
					// type but does not fix this code
					TA_ASSERT(false, "Invalid filter type.");
				}
				break;

			case TA_Base_Bus::Filter::FILTER_STATE:
				populateState(setting);
				break;
			case TA_Base_Bus::Filter::FILTER_OMTYPE:
				populateOMType(setting);
				break;
			case TA_Base_Bus::Filter::FILTER_SUBSYSTEMTYPE:
			{
				stringValue = *(setting.value.stringValue);
				m_pTheRealPModel->addSelectedSubsystemType(stringValue);
			}
			break;
			case TA_Base_Bus::Filter::FILTER_SYSTEMTYPE:
			{
				stringValue = *(setting.value.stringValue);
				m_pTheRealPModel->addSelectedSystem(stringValue);
			}
			break;

			default:
				TA_ASSERT(0, "AlarmFilterDlg::populateFilterSelections(): Invalid filter type received.");
				break;
			}
		}

		// Handle the special case (on init dialog) where there are no severity
		// filters. In this case we actually DO want to set all severity checkbox controls
		if (numSeverityFilters == 0)
		{
			SetAllSeverityFilterControls(true);
		}

		// Set the "To Time" pickers to have the "current" time as necessary.
		QDateTime time = QDateTime::currentDateTime();
		if (!isRangeSet)
		{
			ui.dateTo->setDateTime(time);
			ui.timeTo->setDateTime(time);

			// And set the valid time range
			ui.dateTo->setDateTimeRange(QDateTime::fromTime_t(EARLY_TIME_LIMIT), time);
			ui.timeTo->setDateTimeRange(QDateTime::fromTime_t(EARLY_TIME_LIMIT), time);
			ui.dateFrom->setDateTimeRange(QDateTime::fromTime_t(EARLY_TIME_LIMIT), time);
			ui.timeFrom->setDateTimeRange(QDateTime::fromTime_t(EARLY_TIME_LIMIT), time);
		}

		populateLocationsEditCtrl();
		populateSubsystemsEditCtrl();
		populateSubsystemTypesEditCtrl();
		populateSystemsEditCtrl();
		populateOperatorsEditCtrl();
		populateAlarmTypesEditCtrl();
	}


	/////////////////////////////////////////////////////////////////////////////
	void AlarmFilterDlg::populateLocationsEditCtrl()
	{
		ui.txtLocation->setText(getSelectedItemsString(m_pTheRealPModel->getSelLocationList()).std_str());
		//m_locationEdit.SetWindowText(getSelectedItemsString(m_selectedLocations));
	}
	/////////////////////////////////////////////////////////////////////////////
	void AlarmFilterDlg::populateSubsystemTypesEditCtrl()
	{ 
		ui.txtSubsystem->setText(getSelectedItemsString(m_pTheRealPModel->getSelSubSystemTypeList()).std_str());
		//m_subsystemTypeEdit.SetWindowText(getSelectedItemsString(m_selectedSubsystemTypes));
	}

	/////////////////////////////////////////////////////////////////////////////
	void AlarmFilterDlg::populateSubsystemsEditCtrl()
	{
		ui.txtSubsystem->setText(getSelectedItemsString(m_pTheRealPModel->getSubSystemList()).std_str());
		//m_subsystemEdit.SetWindowText(getSelectedItemsString(m_selectedSubsystems));
	}

	/////////////////////////////////////////////////////////////////////////////
	void AlarmFilterDlg::populateSystemsEditCtrl()
	{
		ui.txtSystem->setText(getSelectedItemsString(m_pTheRealPModel->getSelSystemList()).std_str());
		//m_systemEdit.SetWindowText(getSelectedItemsString(m_selectedSystems));
	}

	/////////////////////////////////////////////////////////////////////////////
	void AlarmFilterDlg::populateOperatorsEditCtrl()
	{
		ui.txtOperator->setText(getSelectedItemsString(m_pTheRealPModel->getSelOperatorList()).std_str());
		//m_operatorEdit.SetWindowText(getSelectedItemsString(m_selectedOperators));
	}

	/////////////////////////////////////////////////////////////////////////////
	void AlarmFilterDlg::populateAlarmTypesEditCtrl()
	{
		ui.txtAlarmType->setText(getSelectedItemsString(m_pTheRealPModel->getSelAlarmTypeList()).std_str());
		//m_alarmTypeEdit.SetWindowText(getSelectedItemsString(m_selectedAlarmTypes));
	}

	/////////////////////////////////////////////////////////////////////////////
	void AlarmFilterDlg::populateOMType(const TA_Base_Bus::Filter::FilterDetails& setting)
	{
		switch (setting.value.alarmOMTypeValue)
		{
		case TA_Base_Bus::Filter::OMTYPE_OPERATION:
			setOMTypeButton(OMFilter::OPERATION);
			break;
		case TA_Base_Bus::Filter::OMTYPE_MAINTENANCE:
			setOMTypeButton(OMFilter::MAINTENANCE);
			break;
		case TA_Base_Bus::Filter::OMTYPE_BOTH:
			setOMTypeButton(OMFilter::BOTH);
			break;
		case TA_Base_Bus::Filter::OMTYPE_ALL:
			setOMTypeButton(OMFilter::ALL_OM);
			break;
		default:
			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError, "Filter alarmOMTypeValue is error");
		}
	}

	/////////////////////////////////////////////////////////////////////////////
	void AlarmFilterDlg::populateState(const TA_Base_Bus::Filter::FilterDetails& setting)
	{
		switch (setting.value.alarmStateValue)
		{
		case TA_Base_Bus::Filter::STATE_ALL_OPEN:
			setAckRadioButton(AckFilter::ALL_ACK);
			setStateRadioButton(StateFilter::OPEN);
			break;

		case TA_Base_Bus::Filter::STATE_OPEN_UNACKED:
			setAckRadioButton(AckFilter::UNACKED);
			setStateRadioButton(StateFilter::OPEN);
			break;

		case TA_Base_Bus::Filter::STATE_OPEN_ACKED:
			setAckRadioButton(AckFilter::ACKED);
			setStateRadioButton(StateFilter::ALL_STATE);
			ui.rdoClosed->setEnabled(false);
			break;

		case TA_Base_Bus::Filter::STATE_CLOSED:
			setAckRadioButton(AckFilter::ALL_ACK);
			setStateRadioButton(StateFilter::CLOSED);
			ui.rdoAcked->setEnabled(false);
			break;

		case TA_Base_Bus::Filter::STATE_ALL_UNACKED:
			setAckRadioButton(AckFilter::UNACKED);
			setStateRadioButton(StateFilter::ALL_STATE);
			break;

		case TA_Base_Bus::Filter::STATE_ANY:
			setAckRadioButton(AckFilter::ALL_ACK);
			setStateRadioButton(StateFilter::ALL_STATE);
			break;
		}
	}



	/////////////////////////////////////////////////////////////////////////////
	// Set or Clear all the severity checkbox controls
	void AlarmFilterDlg::SetAllSeverityFilterControls(bool doSet)
	{
		Qt::CheckState doCheck = Qt::CheckState::Unchecked;
		if (doSet)
		{
			doCheck = Qt::CheckState::Checked;
		}

		for (int i = 0; i < ui.listSeverity->count(); ++i)
		{
			ui.listSeverity->item(i)->setCheckState(doCheck);
		}

	}
	/////////////////////////////////////////////////////////////////////////////
	void AlarmFilterDlg::setOMTypeButton(OMFilter omFlag)
	{
		TA_ASSERT(omFlag == OMFilter::OPERATION || omFlag == OMFilter::MAINTENANCE
			|| omFlag == OMFilter::BOTH || omFlag == OMFilter::ALL_OM,
			"Invalid resource ID for setAckRadioButton");

		ui.rdoOperation->setChecked(omFlag == OMFilter::OPERATION);
		ui.rdoMaintenance->setChecked(omFlag == OMFilter::MAINTENANCE);
		ui.rdoBoth->setChecked(omFlag == OMFilter::BOTH);
		ui.rdoOMAll->setChecked(omFlag == OMFilter::ALL_OM);
	}

	/////////////////////////////////////////////////////////////////////////////
	void AlarmFilterDlg::setAckRadioButton(AckFilter ackState)
	{
		TA_ASSERT(ackState == AckFilter::ACKED || ackState == AckFilter::UNACKED
			|| ackState == AckFilter::ALL_ACK,
			"Invalid resource ID for setAckRadioButton");

		ui.rdoAcked->setChecked(ackState == AckFilter::ACKED);
		ui.rdoUnacked->setChecked(ackState == AckFilter::UNACKED);
		ui.rdoAckAll->setChecked(ackState == AckFilter::ALL_ACK);
	}

	/////////////////////////////////////////////////////////////////////////////
	void AlarmFilterDlg::setStateRadioButton(StateFilter alarmState)
	{
		TA_ASSERT(alarmState == StateFilter::OPEN || alarmState == StateFilter::CLOSED
			|| alarmState == StateFilter::ALL_STATE,
			"Invalid resource ID for setStateRadioButton");

		ui.rdoOpen->setChecked(alarmState == StateFilter::OPEN);
		ui.rdoOpen->setChecked(alarmState == StateFilter::CLOSED);
		ui.rdoOpen->setChecked(alarmState == StateFilter::ALL_STATE);
	}

	void AlarmFilterDlg::selectUserFilter()
	{
		ui.cboNamedFilter->addItem(NamedFilter::UNNAMED.c_str());
		ui.btnSave->setEnabled(true);
		ui.btnDelete->setEnabled(false);
	}

	void AlarmFilterDlg::enableButtons()
	{
		// Enable buttons as required.
		ui.btnOk->setEnabled(true);
		ui.btnApply->setEnabled(true);
		ui.btnCancel->setEnabled(true);
	}

	void AlarmFilterDlg::disableButtons()
	{
		// Disable buttons as required.
		//		m_okButton.EnableWindow(false);  //changxing:need not do this.
		ui.btnApply->setEnabled(false);
		ui.btnCancel->setEnabled(true);
	}


	void AlarmFilterDlg::OnSelchangeNamedFilter(int index)
	{
		FUNCTION_ENTRY("OnSelchangeNamedFilter");

		// Get the currently selected item.
		int selIdx = index;
		if (0 <= selIdx)
		{
			// Get the name of the filter.
			 
		    std::string filterName = ui.cboNamedFilter->itemTextEx(selIdx);

			TA_Base_Bus::Filter* objFilter = m_pTheRealPModel->getFilterObject();
			bool isDefault = filterName.compare(AlarmFilter::FILTER_DEFAULT) ? false : true;
			if (isDefault)
			{
				OnClearFiltersButton();
				//return;
			}

			bool isUserDefine = filterName.compare("User Defined")? false: true;
			if (!isUserDefine)
			{ // Check whether we have this filter on the combobox
				int idx = ui.cboNamedFilter->findText("User Defined");
				if (idx != -1)
				{ // we have it then we need to remove it because this because this one is only temporary 
					ui.cboNamedFilter->blockSignals(true);
					ui.cboNamedFilter->removeItem(idx);
					ui.cboNamedFilter->blockSignals(false);
				}
			}
			m_pTheRealPModel->changeNamedFilter(filterName);
		}

		FUNCTION_EXIT;

	}

	void AlarmFilterDlg::OnDelete()
	{
		FUNCTION_ENTRY("OnDeleteFilter");
		m_pTheRealPModel->deleteFilter();

		FUNCTION_EXIT;
	}

	

	void AlarmFilterDlg::populateNamedFilters(std::string curName)
	{
		// Make sure the combo box is empty.
		CFilterCtrlSignalBlocker objSigBlocker(this);

		ui.cboNamedFilter->clear();
		
		int nCtr = 0;
		int nDefaultIdx = 0;
		std::list<std::string > filterNames = m_pTheRealPModel->getFilterNameList();
		std::list<std::string >::iterator itr = filterNames.begin();
		for (itr; itr != filterNames.end(); itr++)
		{
			if (itr->compare(AlarmFilter::FILTER_DEFAULT) == 0)
			{
				nDefaultIdx = nCtr;
			}
			ui.cboNamedFilter->insertItem(nCtr, *itr);
			nCtr++;
		}

		bool isUserDefined = curName.compare(TA_Base_Core::NamedFilter::UNNAMED) ? false : true;
		bool isDefault = curName.compare(AlarmFilter::FILTER_DEFAULT) ? false : true;

		// Check validity of curName and set the combo box to the name
		if (isDefault == false && isUserDefined == false)
		{
			// If it is the current filter then select it.
			int index = ui.cboNamedFilter->findText(curName);
			ui.cboNamedFilter->setCurrentIndex(index);
			ui.btnSave->setEnabled(true);
			ui.btnDelete->setEnabled(true);
		}

		// If the filter is custom it won't appear in the combo box, so add its text
		// manually.
		else if (isUserDefined == true)
		{
			selectUserFilter();
		}
		else if (isDefault == true) // Default filter name
		{

			//m_namedFilter.AddTempString(AlarmFilter::FILTER_DEFAULT.c_str());
			ui.cboNamedFilter->setCurrentIndex(nDefaultIdx);
			ui.btnSave->setEnabled(false);
			ui.btnDelete->setEnabled(false);
		}
	}

	//void AlarmFilterDlg::setNamedFilter(std::string filterName)
	//{
	//	bool isDefault = (filterName == AlarmFilterDlg::FILTER_DEFAULT) ? true : false;
	//	//bool isCurFilter = (filterName == m_filter.getFilterName()) ? true : false;
	//	//if (isCurFilter) return; // No need to reapply current filter

	//	if (filterName != "" && isDefault == false && checkInNamedFilterDatabase(filterName) == true) // This filter can be found in database
	//	{
	//		// Load the named filter from database	
	//		std::auto_ptr<NamedFilter> loadedFilter;

	//		try
	//		{
	//			// Load the filter.
	//			loadedFilter = TA_Base_Core::NamedFilterAccessFactory::getInstance().getNamedFilter(filterName);
	//		}
	//		catch (TA_Base_Core::DataException& ex)
	//		{
	//			LOG_EXCEPTION_CATCH(SourceInfo, "DataException", ex.getWhichData());

	//			// Display the load filter error message.
	//			TA_Base_Bus::TransActiveMessage userMsg;
	//			userMsg << sLoad << sLoaded;
	//			UINT selectedButton = userMsg.showMsgBox(IDS_UE_040031);

	//			return;
	//		}

	//		// Copy the new filter to our internal filter.
	//		m_nmFilter.clear();
	//		m_nmFilter = *(loadedFilter.get());

	//		// Convert the nmFilter to Filter class
	//		std::vector<std::string> stringList = m_nmFilter.getStringFilterList();
	//		std::vector<std::string> numericList = m_nmFilter.getNumericFilterList();

	//		if (stringList.size() > 0 || numericList.size() > 0) {
	//			// Clear the current filter and set it the to named filter
	//			m_filter.removeAll();
	//			m_filter = convertNamedFilterToFilter(m_nmFilter);
	//		}
	//		else
	//		{
	//			//Warn user of null named filter
	//			TA_Base_Bus::TransActiveMessage userMsg;
	//			const std::string retEr("retrieve ");
	//			const std::string fileEr("load using " + filterName);
	//			userMsg << retEr;
	//			userMsg << fileEr;
	//			userMsg.showMsgBox(IDS_UE_040031);

	//			return;
	//		}

	//	}
	//	else if (isDefault == true) // This is for default setting
	//	{
	//		m_filter.removeAll();
	//		m_filter.setFilterName(AlarmFilterDlg::FILTER_DEFAULT);
	//	}
	//	else // The filter name is undefined or not present
	//	{
	//		// Display a error message to notify an invalid filter name
	//		TA_Base_Bus::TransActiveMessage userMsg;
	//		const std::string retEr("load ");

	//		if (filterName == "")
	//		{
	//			const std::string fileEr("load using empty string. Please provide a valid name");
	//			userMsg << retEr;
	//			userMsg << fileEr;
	//			userMsg.showMsgBox(IDS_UE_040031);
	//		}
	//		else
	//		{
	//			const std::string fileEr("load using " + filterName + " as it is not found in database");
	//			userMsg << retEr;
	//			userMsg << fileEr;
	//			userMsg.showMsgBox(IDS_UE_040031);
	//		}

	//		return;
	//	}

	//	// set the alarm control list to this filter
	//	try
	//	{
	//		//clear all the fitlers in the alarm list
	//		m_alarmList.setFilter(m_filter, isDefault);
	//	}
	//	catch (TA_Base_Core::AlarmListCtrlException)
	//	{
	//		TA_Base_Bus::TransActiveMessage userMsg;
	//		UINT selectedButton = userMsg.showMsgBox(IDS_UI_040022);
	//	}
	//}


	void AlarmFilterDlg::fromCheckBoxUpdated(bool isTicked)
	{
		// TD 8703: Requirement is that the date and time fields are blanked out if the checkboxes
		// are unchecked. As date time controls don't allow empty strings, replacing these fields 
		// with a combo box and edit control gives the desired affect.
		return; //Todo: Noel

		bool windowState = true;
		if (isTicked)
		{
			windowState = false;
		}

		//TD15028++ wangyn
		QDate fromDate = ui.dateFrom->date();
		QTime fromTime = ui.timeFrom->time();

		m_fromDateMask.setText(fromDate.toString());
		m_fromTimeMask.setText(fromTime.toString());

		////++TD15208
		m_fromDateMask.setVisible(windowState);
		m_fromDateMask.setEnabled(false);

		m_fromTimeMask.setVisible(windowState);
		m_fromTimeMask.setEnabled(false);

		if (windowState == true)
		{
			windowState = false;
		}
		else
		{
			windowState = true;
		}

		ui.dateFrom->setVisible(windowState);
		ui.timeFrom->setVisible(windowState);
	}


	void AlarmFilterDlg::toCheckBoxUpdated(bool isTicked)
	{
		// TD 8703: Requirement is that the date and time fields are blanked out if the checkboxes
		// are unchecked. As date time controls don't allow empty strings, replacing these fields 
		// with a combo box and edit control gives the desired affect.
		return; //Todo: Noel

		int windowState = true;
		if (isTicked)
		{
			windowState = false;
		}
		//TD15028++ wangyn
		QDate toDate = ui.dateTo->date();
		QTime toTime = ui.timeTo->time();

		m_toDateMask.setText(toDate.toString());
		m_toTimeMask.setText(toTime.toString());

		//++TD15208
		m_toDateMask.setVisible(windowState);
		m_toDateMask.setEnabled(false);

		m_toTimeMask.setVisible(windowState);
		m_toTimeMask.setEnabled(false);

		if (windowState == true)
		{
			windowState = false;
		}
		else
		{
			windowState = true;
		}

		ui.dateFrom->setVisible(windowState);
		ui.timeFrom->setVisible(windowState);
	}

	//TD15028++ wangyn
	void AlarmFilterDlg::setFromDateTime(time_t dateTime)
	{
		QDateTime currentTime = QDateTime::currentDateTime();
		time_t earliestTime = dateTime;

		if (earliestTime == 0)
		{
			earliestTime = currentTime.toTime_t();
		}

		QDateTime fromEarliest = QDateTime::fromTime_t(earliestTime);

		ui.dateFrom->setDate(fromEarliest.date());
		ui.timeFrom->setTime(fromEarliest.time());
	}
	//++TD15208


	//TD15208++
	void AlarmFilterDlg::setToDateTime(time_t dateTime)
	{
		QDateTime currentTime = QDateTime::currentDateTime();
		time_t latestTime = dateTime;

		if (latestTime == 0)
		{
			latestTime = currentTime.toTime_t();
		}

		QDateTime toLatestTime = QDateTime::fromTime_t(latestTime);

		ui.dateTo->setDate(toLatestTime.date());
		ui.timeTo->setTime(toLatestTime.time());
	}
	//++TD15208

	void AlarmFilterDlg::updateButtonState(T_ButtonStateMap& evBtnUpdate)
	{
		// Update button state
		T_ButtonStateMap::iterator itr = evBtnUpdate.begin();
		for (itr; itr != evBtnUpdate.end(); itr++)
		{
			switch (itr->first)
			{
			case BTN_CLEAR:
				ui.btnCancel->setEnabled(itr->second);
				break;
			case BTN_SAVE:
				ui.btnSave->setEnabled(itr->second);
				break;
			case BTN_DELETE:
				ui.btnDelete->setEnabled(itr->second);
				break;
			case BTN_OK:
				ui.btnOk->setEnabled(itr->second);
				break;
			case BTN_CANCEL:
				ui.btnCancel->setEnabled(itr->second);
				break;
			case BTN_APPLY:
				ui.btnApply->setEnabled(itr->second);
				break;
			default: // Unknown button
				break;
			}
		}
	}
} // TA_Base_App
//Note: When auto generating windows message handlers, you will have to move it above the 
//      TA_Base_App scope.


