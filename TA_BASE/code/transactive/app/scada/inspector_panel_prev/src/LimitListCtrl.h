#if !defined(AFX_LIMITLISTCTRL_H__8C394273_7B6C_11D7_B175_0050BAB1D931__INCLUDED_)
#define AFX_LIMITLISTCTRL_H__8C394273_7B6C_11D7_B175_0050BAB1D931__INCLUDED_

/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/scada/inspector_panel_prev/src/LimitListCtrl.h $
  * @author:  Andy Parker
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  * 
  * This class manages the list display used to show the basic information in
  * the CInformationDialog class.
  *
  */


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "app/scada/Inspector_Panel/src/DataPointListCtrl.h"

namespace TA_Base_App
{
	class CEquipment;
	class CLimitDetailsListCtrl;


	/////////////////////////////////////////////////////////////////////////////
	// CLimitListCtrl window

	class CLimitListCtrl : public CAbstractDataPointListCtrl, ICellManager
	{
		// Construction
		public:
			CLimitListCtrl(CEdit& limitInfo, CLimitDetailsListCtrl& limitDetails);
			//wenching++ (TD12132)
			void refreshDataPoints();
			//++wenching (TD12132)

		// Attributes
		public:

		// Operations

		// Implementation
		public:
			virtual ~CLimitListCtrl();

			virtual bool isAnyDataPointsDisplayedOnScreen() const;

			// ICellManager methods
			virtual CellType GetCellType(CEditableListCtrl* listCtrl, int item, int subItem);
            virtual unsigned long GetCellMaximumCharacters(CEditableListCtrl* listCtrl, int item, int subItem);
			virtual CellData GetCellData(CEditableListCtrl* listCtrl, int item, int subItem);
            virtual CString GetCellMask(CEditableListCtrl* listCtrl, int item, int subItem);
            virtual char GetCellMaskPromptChar(CEditableListCtrl* listCtrl, int item, int subItem);
			virtual bool UpdateCellValue(CEditableListCtrl* listCtrl, int item, int subItem, CString value);


			// Generated message map functions
		protected:

			DECLARE_MESSAGE_MAP()

			//
			// CAbstractDataPointListCtrl methods
			//
			void setupList();
			void displayDataPoints();
			void updateDataPointDisplay(int iPoint, int nItem, TA_Base_Bus::ScadaEntityUpdateType updateType);

		private:
			// Hide the default constructor
			CLimitListCtrl();

		protected:
			CEdit&					m_limitInfo;
			CLimitDetailsListCtrl&	m_limitDetails;
			volatile bool			m_dataPointsDisplayedOnScreen;
	};

	/////////////////////////////////////////////////////////////////////////////

	//{{AFX_INSERT_LOCATION}}
	// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
}

#endif // !defined(AFX_DATAPOINTLISTCTRL_H__8C394273_7B6C_11D7_B175_0050BAB1D931__INCLUDED_)
