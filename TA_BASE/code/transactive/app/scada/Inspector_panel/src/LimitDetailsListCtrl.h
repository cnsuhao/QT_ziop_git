/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/scada/inspector_panel/src/LimitDetailsListCtrl.h $
  * @author:  Andy Parker
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: CM $
  * 
  * This class manages the list display used to show the basic control information in
  * the CControlDialog class.
  *
  */


#ifndef LIMITDETAILSLISTCTRL_H
#define LIMITDETAILSLISTCTRL_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "app/scada/inspector_panel/src/DataPointListCtrl.h"
#include "../pmodel/src/limitdetailllistctrlmodel.h"

namespace TA_Base_Bus
{
	class DataPointProxySmartPtr;
}

namespace TA_Base_App
{
	class CEquipment;

	/////////////////////////////////////////////////////////////////////////////
	// CDataPointListCtrl window

	class CLimitDetailsListCtrl : public CAbstractDataPointListCtrl, /* CEditableListCtrl, */ ICellManager
	{
		// Construction
		public:
			CLimitDetailsListCtrl();

		// Attributes
		public:

		// Operations
		public:
			void refreshAllData();
			void setDataPoint(int iPoint);

		// Overrides
			// ClassWizard generated virtual function overrides
			//{{AFX_VIRTUAL(CLimitDetailsListCtrl)
			protected:
			//virtual void PreSubclassWindow();
			//}}AFX_VIRTUAL

		// Implementation
		public:
			virtual ~CLimitDetailsListCtrl();

			virtual bool isAnyDataPointsDisplayedOnScreen() const;

			// ICellManager methods
			virtual CellType GetCellType(CEditableListCtrl* listCtrl, int item, int subItem);
            virtual unsigned long GetCellMaximumCharacters(CEditableListCtrl* listCtrl, int item, int subItem);
			virtual CellData GetCellData(CEditableListCtrl* listCtrl, int item, int subItem);
            virtual TAUnicodeString GetCellMask(CEditableListCtrl* listCtrl, int item, int subItem);
            virtual char GetCellMaskPromptChar(CEditableListCtrl* listCtrl, int item, int subItem);
			virtual bool UpdateCellValue(CEditableListCtrl* listCtrl, int item, int subItem, const TAUnicodeString& value);

			

			void updateDataPoint(unsigned long key, TA_Base_Bus::ScadaEntityUpdateType updateType);
			
			void updatePermissions(bool allowed);
			void clearList();

			// Generated message map functions
		protected:
			//{{AFX_MSG(CLimitDetailsListCtrl)
			afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
			//}}AFX_MSG

			DECLARE_MESSAGE_MAP()

			bool overrideAlarmConditionConfirm ( const TAUnicodeString & typeOfChange, unsigned int iPoint, bool enable );
			bool enableAlarmConfirm(bool isEnable, const std::string& alarmType);
			void displayLimitDetails();
			void refreshLimitDetails();
			bool limitDetailChangeConfirm(const int alarmThreshold, unsigned int iPoint, bool enable);
			bool isCellEnabled (int nItem, int nSubItem);

			virtual void setupList();
			virtual void displayDataPoints();
			virtual void updateDataPointDisplay(int iPoint, int nItem);
			virtual void updateDataPoint(unsigned int iPoint);
          
			std::string			getMyPModelName();
			void				setupPModel();   
			
	private:
			LimitDetaillListCtrlModel*		m_realPModel;
			int							m_iPoint;
			bool						m_accessAllowed;
			volatile bool				m_dataPointsDisplayedOnScreen;
	};
}

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(LIMITDETAILSLISTCTRL_H)
