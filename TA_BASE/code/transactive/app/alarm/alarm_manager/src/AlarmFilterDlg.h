/**
* The source code in this file is the property of
* Ripple Systems and is not for redistribution
* in any form.
*
* Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/alarm/alarm_manager/src/AlarmFilterDlg.h $
* @author:  Ripple
* @version: $Revision: #7 $
*
* Last modification: $DateTime: 2016/12/06 18:37:01 $
* Last modified by:  $Author: Noel $
*
*/
#if !defined(AFX_ALARMFILTERDLG_H__CCB479D0_5367_4905_B748_6C2429D62472__INCLUDED_)
#define AFX_ALARMFILTERDLG_H__CCB479D0_5367_4905_B748_6C2429D62472__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AlarmFilterDlg.h : header file
//

//#include "Afxdtctl.h"
#include "app\alarm\alarm_manager\GeneratedFiles\ui_alarmfilterguidlg.h"

#include "bus/alarm/alarm_list_control/pmodel/src/Filter.h"
#include "bus/alarm/alarm_list_control/src/AlarmListCtrl.h"
//#include "bus/mfc_extensions/src/combo_temporary_item/ComboTempItem.h"
#include "core/data_access_interface/src/NamedFilter.h"

#include "bus/generic_gui_pmod/src/PModelInterface.h"
#include "bus/generic_gui_view/src/TransActiveDialog.h"
#include "app\alarm\alarm_manager\src\multipleselguidlg.h"
#include "app\alarm\alarm_manager\src\alarmfilterdlg.h"

#include "app\alarm\alarm_manager\pmodel\src\alarmmanagerconsts.h"

#include <string>
#include <vector>
#include <map>
#include <QWidget>
#include <QTimer>

/////////////////////////////////////////////////////////////////////////////
// AlarmFilterDlg dialog


namespace TA_Base_App
{
	// Forward declaration
	class AlarmFilterDlgPModel;
	class FilterDlgBtnStateEventArgs;

	class AlarmFilterDlg : public TA_Base_Bus::AbstractDialog
	{
		Q_OBJECT

	public:
		
		// Construction
		AlarmFilterDlg(TA_Base_Bus::ITransActiveApp* app);
		// Destruction
		~AlarmFilterDlg();
		 
		/**
		* getAllFilterSelections
		*
		* This method retrieves the vector of alarm filters.
		*/
		// TA_Base_App::AlarmFilter getAllFilterSelections();

		//TA_Base_Bus::AlarmListCtrl& m_alarmList;

		/**
		* showFilter
		*
		* This is to display the filter parameters to the named filter passed in
		*
		* @param: A name string of the filter for display
		*/
		void showFilter(std::string filterName);

		/**
		* getCurrentFilterString
		*
		* This is to return string of the named filter parameters for display on caption, etc
		*
		* @return: A param string of the named filter for display
		*/
		std::string getCurrentFilterString();

		//TD15028++ wangyn
		void setFromDateTime(time_t dateTime);
		void setToDateTime(time_t dateTime);
		//++TD15028

		//ITransActiveView interface
		std::string  getMyPModelName(void);
		void setupPModel(void);
		// For dynamic language switching
		virtual void translateUI();

	protected:
		virtual void initUIControl();
		virtual void syncUIDataToPModel();

		// Implementation
	public slots:
		void OnOK();
		void OnDelete();
		void OnApply();
		void OnClearFiltersButton();
		void OnChangeEditAsset(const std::string & text);
		void OnChangeEditDescription(const std::string & text);
		void OnDatetimechangeDatetimepickerFromdate(const QDate & date);
		void OnDatetimechangeDatetimepickerFromtime(const QTime & time);
		void OnDatetimechangeDatetimepickerTodate(const QDate & date);
		void OnDatetimechangeDatetimepickerTotime(const QTime & time);
		void OnSubsystemButton();
		void OnSubsystemTypeButton();
		void OnSystemButton();
		void OnLocationButton();
		void OnOperatorButton();
		void OnAlarmTypeButton();
		void OnCancel();
		void OnSave();
		void OnFromCheckBoxClicked(int state);
		void OnToCheckBoxClicked(int state);
		void OnStateRadioButton(bool checked);
		void OnAckRadioButton(bool checked);
		void OnOMTypeButton(bool checked);
		void OnItemChange(QListWidgetItem* item);
		void OnSelchangeNamedFilter(int index);
		void OnTextchangeNamedFilter(const std::string & text);
		void OnTimer(); //TD17081
		
		void OnShowFilter(const std::string & filterName);

	signals:
		void showAlarmFilter(const std::string & filterName);

	private:
		/**
		* setUpdateNeeded
		*
		* Call this method when the controls are changed or a filter applied
		*
		* @param TRUE => enable 'Apply' button. FALSE => disable 'Apply' button.
		*/
		//void setUpdateNeeded(bool updateNeeded);

		void populateLocationsEditCtrl();
		void populateSubsystemsEditCtrl();
		void populateSubsystemTypesEditCtrl();
		void populateSystemsEditCtrl();
		void populateOperatorsEditCtrl();
		void populateAlarmTypesEditCtrl();
		void populateState(const TA_Base_Bus::Filter::FilterDetails& setting);
		void populateOMType(const TA_Base_Bus::Filter::FilterDetails& setting);

		void setAckRadioButton(AckFilter ackState);
		void setStateRadioButton(StateFilter alarmState);
		void setOMTypeButton(OMFilter omFlag);


		/**
		* clearAllDisplays
		*
		* This method is to clear or reset all displays and controls in the dialog
		*
		*/
		void clearAllDisplays();

		/**
		* SetAllSeverityFilterControls
		*
		* Set or Clear all the severity checkbox controls.
		*
		* @param TRUE => set severity filters. FALSE => clear them.
		*
		*/
		void SetAllSeverityFilterControls(bool doSet);


		/**
		* populateFilterSelections
		*
		* Set the filter controls based on the currently applied filter.
		*/
		void populateFilterSelections(TA_Base_Bus::Filter& filter);

		//TD17081++ - modified argument type from severityKey to the severityData 
		// to limit DB access
		/**
		* getSeverityColour
		*
		* Retrieve the severity color of the given IAlarmSeverityData
		*/

		//COLORREF getSeverityColour(unsigned long severityKey);
		//COLORREF getSeverityColour(TA_Base_Core::IAlarmSeverityData* severityData);
		//++TD17081 

		/**
		* enableButtons or disableButtons
		*
		* Enables or disable "OK" and "APPLY" buttons that can be greyed out.
		* This is used when an object in the filter manipulated or updated.
		* "CANCEL" button is always enabled.
		*
		*/
		void enableButtons();
		void disableButtons();

		/**
		* selectUserFilter
		*
		* Selects the 'User Defined' filter.
		*/
		void selectUserFilter();

		/**
		* populateNamedFilters
		*
		* Populates the named filter combo box with the current list of named filters.
		*
		* @param string - The current filter name used in the alarm control list and to be displayed
		*/
		void populateNamedFilters(std::string curName);

		void fromCheckBoxUpdated(bool isTicked);

		void toCheckBoxUpdated(bool isTicked);
		
		void setUpSlotConnection();

		virtual void notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args = NULL);

		void updateButtonState(T_ButtonStateMap& evBtnUpdate);


		// Block Filter Signal
		class CFilterCtrlSignalBlocker
		{
		public:
			CFilterCtrlSignalBlocker(AlarmFilterDlg* parent);
			~CFilterCtrlSignalBlocker();
		private:
			AlarmFilterDlg* m_parent;
		};

		friend class CFilterCtrlSignalBlocker;

	private:
		static const int ALARM_FILTER_DATA_INITIALIZE_TIMER; //TD17081

		Ui::alarmFilterGUIDlg ui;

		QTimer *m_timer;
		AlarmFilterDlgPModel * m_pTheRealPModel;
		bool m_bInitialized;
		QLineEdit m_fromDateMask;
		QLineEdit m_fromTimeMask;
		QLineEdit m_toDateMask;
		QLineEdit m_toTimeMask;

	};
} //TA_Base_App

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALARMFILTERDLG_H__CCB479D0_5367_4905_B748_6C2429D62472__INCLUDED_)

