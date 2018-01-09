/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/scada/inspector_panel/src/OverrideListCtrl.h $
  * @author:  Andy Parker
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: CM $
  * 
  * This class manages the list display used to show the basic information in
  * the CInformationDialog class.
  *
  */

#ifndef OVERRIDELISTCTRL_H
#define OVERRIDELISTCTRL_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "app/scada/inspector_panel/src/DataPointListCtrl.h"
#include "bus/scada/common_library/src/AuditMessageUtility.h"
#include "../pmodel/src/overridelistctrlmodel.h"

namespace TA_Base_Bus
{
	class DataPointProxySmartPtr;
}

namespace TA_Base_App
{
	/////////////////////////////////////////////////////////////////////////////
	// COverrideListCtrl window

	class COverrideListCtrl : public CAbstractDataPointListCtrl, ICellManager
	{
		// Construction
		public:
			COverrideListCtrl();
			void refreshDataPoints();

		// Attributes
		public:

		// Operations
		public:


		// Implementation
		public:
			virtual ~COverrideListCtrl();

			virtual bool isAnyDataPointsDisplayedOnScreen() const;

			// ICellManager methods
			virtual CellType GetCellType(CEditableListCtrl* listCtrl, int item, int subItem);
            virtual unsigned long GetCellMaximumCharacters(CEditableListCtrl* listCtrl, int item, int subItem);
			virtual CellData GetCellData(CEditableListCtrl* listCtrl, int item, int subItem);
            virtual TAUnicodeString GetCellMask(CEditableListCtrl* listCtrl, int item, int subItem);
            virtual char GetCellMaskPromptChar(CEditableListCtrl* listCtrl, int item, int subItem);
			virtual bool UpdateCellValue(CEditableListCtrl* listCtrl, int item, int subItem, const TAUnicodeString& value);

			//TD15438, override the function to get a chance to adjust columns width.
			virtual BOOL SetWindowPos( const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, UINT nFlags );
			
		protected:

            enum eForcedRefreshType
            {
                REFRESH_DEFAULT                         = 0x00,
                REFRESH_DESCRIPTION                     = 0x01,
                REFRESH_TYPE                            = 0x02,
                REFRESH_OVERRIDE                        = 0x04,
                REFRESH_OVERRIDE_VALUE                  = 0x08,
                REFRESH_ALL                             = 0xFFFFFFFF
            };

			bool inhibitOverrideConfirm(const int inhibitOverrideItem, unsigned int iPoint, bool enable);
			void displayItem(int nItem, DWORD refreshType = REFRESH_DEFAULT);
			bool isCellEnabled (int nItem, int nSubItem);
			//
			// CAbstractDataPointListCtrl methods
			//
			void setupList();
			void displayDataPoints();
			void updateDataPointDisplay(int iPoint, int nItem);

			std::string			getMyPModelName();
			void				setupPModel();

		protected:
			OverrideListCtrlModel*					m_realPModel;
			volatile bool							m_dataPointsDisplayedOnScreen;
			TA_Base_Bus::AuditMessageUtility		m_auditMessageUtility;
	};

	/////////////////////////////////////////////////////////////////////////////

	//{{AFX_INSERT_LOCATION}}
	// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
}

#endif // #ifndef OVERRIDELISTCTRL_H