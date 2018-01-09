/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/system_control/system_manager/src/ProcessListCtrl.cpp $
  * @author:  Dominique Beauregard
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2012/02/06 16:15:14 $
  * Last modified by:  $Author: haijun.li $
  *
  * This class is a specialised List View control that
  * displays process information. It is derived from
  * MFC's CListCtrl class.
  *
  */

#pragma warning(disable:4786)

#include "StdAfx.h"
#include "SystemManager.h"
#include "ProcessListCtrl.h"
#include "../pmodel/src/ProcessListCtrlModel.h"
#include "../pmodel/src/AppServiceConsts.h"
#include "../pmodel/src/SystemManagerConsts.h"

#include "bus/unicode_common_lib/src/TAUnicodeString.h"

#include "core/process_management/IDL/src/ProcessManagementDataDefinitionsCorbaDef.h"
#include <afxcmn.h>
#include <sstream>
#include <string>

#include "core/utilities/src/DateFormat.h" // TD12474 

namespace TA_Base_App
{

	const TCHAR * SYSMGR_NOT_APPLICABLE = _T("N/A");
	const TCHAR * SYSMGR_CONTROL = _T("Control");
	const TCHAR * SYSMGR_MONITOR = _T("Monitor");

    ProcessListCtrl::ProcessListCtrl()
        : CListCtrl()
    {
    }



    ProcessListCtrl::~ProcessListCtrl()
    {
        m_pModel->detachView(this);
    }

    BEGIN_MESSAGE_MAP(ProcessListCtrl, CListCtrl)
    //{{AFX_MSG_MAP(ProcessListCtrl)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    ON_NOTIFY(HDN_ITEMCLICKA, 0, OnHeaderClicked)  // ASCII 
    ON_NOTIFY(HDN_ITEMCLICKW, 0, OnHeaderClicked)  // Unicode
    //}}AFX_MSG_MAP
    END_MESSAGE_MAP()

    void ProcessListCtrl::populate(std::vector<TA_Base_Core::ProcessData> processes)
    {
        if (m_pRealModel->m_isFrozen)
        {
            return;
        }

        DeleteAllItems();
        for (unsigned int i = 0; i < processes.size(); i++)
        {
			std::string entityDesc = m_pRealModel->getEntityDescription(
				processes[i].ProcessId.in());

			TAUnicodeString tmpUniStr = TAUnicodeString::CreateFromMultitByteString(entityDesc.c_str());

            int pos = this->InsertItem(i, tmpUniStr.c_str());
            this->SetItemData(pos, pos);

			tmpUniStr = TAUnicodeString::CreateFromMultitByteString( processes[i].host.in());
            this->SetItemText(pos, HOST_COL, tmpUniStr.c_str());

            bool isManagedProcess = m_pRealModel->isManagedProcessFromName(processes[i].ProcessId.in());

            populateProcessState(pos,processes[i].state, processes[i].started, isManagedProcess);

            populateProcessOperationMode(pos, processes[i].operationMode);

            populateProcessNewState(pos, processes[i].requestedState, isManagedProcess);

			// Populate restarts
            std::ostringstream restarts;
            restarts << processes[i].restarts;

			tmpUniStr = TAUnicodeString::CreateFromMultitByteString(restarts.str().c_str());
            
			this->SetItemText(pos, RESTARTS_COL, tmpUniStr.c_str());

			// Populate debug level
			tmpUniStr = TAUnicodeString::CreateFromMultitByteString( processes[i].debugLevel.in() );
			this->SetItemText(pos, DEBUG_LEVEL_COL, tmpUniStr.c_str());
        }
		sortListControl();
   }


    void ProcessListCtrl::populateProcessState(int insertPosition,
                                               TA_Base_Core::EProcessStatus state,
                                               const CTime& processStartTime,
                                               bool isManagedProcess)
    {
		TA_Base_Core::DateFormat dateFormat; 
		std::string dateFormatStr = dateFormat.getDateFormat(TA_Base_Core::DateFormat::DateTimeFormat1);

		std::string itemStatusColDesc;
		TAUnicodeString tmpUniStr = processStartTime.Format(dateFormatStr.c_str());
		std::string itemSinceColDesc = tmpUniStr.toMultiByteStdString();

		m_pRealModel->getRuningStatusDesc(state, itemStatusColDesc, itemSinceColDesc, isManagedProcess);


		tmpUniStr = TAUnicodeString::CreateFromMultitByteString(itemStatusColDesc.c_str());
		SetItemText(insertPosition, STATUS_COL, tmpUniStr.c_str());

		tmpUniStr = TAUnicodeString::CreateFromMultitByteString(itemSinceColDesc.c_str());
		SetItemText(insertPosition, RUNNING_SINCE_COL, tmpUniStr.c_str());

    }


    void ProcessListCtrl::populateProcessOperationMode(int insertPosition,
                                                       TA_Base_Core::EOperationMode operationMode)
    {
        switch(operationMode)
        {
        case(TA_Base_Core::Control):
            SetItemText(insertPosition, OPERATION_MODE_COL, SYSMGR_CONTROL);
            break;

        case(TA_Base_Core::Monitor):
            SetItemText(insertPosition, OPERATION_MODE_COL, SYSMGR_MONITOR);
            break;

        default:
            SetItemText(insertPosition, OPERATION_MODE_COL, SYSMGR_NOT_APPLICABLE);
        }
    }
    
    
    void ProcessListCtrl::populateProcessNewState(int insertPosition,
                                                  TA_Base_Core::EProcessStatus state,
                                                  bool isManagedProcess)
    {
        if (!isManagedProcess)
        {
            // We don't have requested states for non-managed processes so just set this to N/A
            SetItemText(insertPosition, NEW_STATUS_COL, SYSMGR_NOT_APPLICABLE);
            return;
        }

        std::string statusColDesc = m_pRealModel->getProcessNewStateDesc(state);

		TAUnicodeString tmpUniStr = TAUnicodeString::CreateFromMultitByteString(statusColDesc.c_str());

        SetItemText(insertPosition, NEW_STATUS_COL, tmpUniStr.c_str());

    }


    void ProcessListCtrl::setupHeader()
    {
        this->SetExtendedStyle(LVS_EX_HEADERDRAGDROP | LVS_EX_FULLROWSELECT);

        LVCOLUMN* col = new LVCOLUMN;
        col->mask = LVCF_FMT |LVCF_SUBITEM |LVCF_TEXT |LVCF_WIDTH;
        col->fmt = LVCFMT_LEFT;
        col->cx = 250;
        col->pszText = _T("Process Name");
        col->iSubItem = 0;
        int colNum = this->InsertColumn(PROCESS_ID_COL, col);

        col->cx = 120;
        col->pszText = _T("Location");
        colNum = this->InsertColumn(HOST_COL, col);

        col->cx = 120;
        col->pszText = _T("Operation Mode");
        colNum = this->InsertColumn(OPERATION_MODE_COL, col);

        col->cx = 150;
        col->pszText = _T("Status");
        colNum = this->InsertColumn(STATUS_COL, col);

        col->pszText = _T("Requested Status");
        colNum = this->InsertColumn(NEW_STATUS_COL, col);

        col->pszText = _T("Running Since");
        colNum = this->InsertColumn(RUNNING_SINCE_COL, col);

        col->cx = 75;
        col->pszText = _T("Restarts");
        colNum = this->InsertColumn(RESTARTS_COL, col);

		col->cx = 100;
		col->pszText = _T("Debug Level");
		colNum = this->InsertColumn(DEBUG_LEVEL_COL, col);

        delete col;
    }

    void ProcessListCtrl::PreSubclassWindow()
    {
		attachPModel();

        setupHeader();
        updateSortArrow(0);

        CListCtrl::PreSubclassWindow();
    }

    void ProcessListCtrl::update(std::vector<TA_Base_Core::ProcessData> processes)
    {
        if (m_pRealModel->m_isFrozen)
        {
            return;
        }

        // Get selected item
        CStringList SelectedProcesses;
        POSITION pos = GetFirstSelectedItemPosition();
        while (pos != NULL)
        {
            SelectedProcesses.AddTail(getProcessName(GetNextSelectedItem(pos)).c_str());
        }

        // Update display
        populate(processes);

        // Reselect items selected before update
        select(SelectedProcesses);
    }

    void ProcessListCtrl::freeze()
    {
        m_pRealModel->m_isFrozen = true;
    }

    void ProcessListCtrl::unfreeze()
    {
        m_pRealModel->m_isFrozen = false;
    }

    std::string ProcessListCtrl::getProcessName(unsigned int index)
    {
		TAUnicodeString tmpUniStr = GetItemText(index, PROCESS_ID_COL).GetBuffer();
        return tmpUniStr.toMultiByteStdString();
    }

    std::string ProcessListCtrl::getStatus(unsigned int index)
    {
        TAUnicodeString tmpUniStr = GetItemText(index, STATUS_COL).GetBuffer();
        return tmpUniStr.toMultiByteStdString();
	}

    bool ProcessListCtrl::select(CStringList& processes)
    {
        bool isSelected = false;
        POSITION pos;

        // Iterate through the list in head-to-tail order.
        for(pos = processes.GetHeadPosition(); pos != NULL; )
        {
            LVFINDINFO itemInfo = {0};
            itemInfo.flags = LVFI_STRING;
            TAUnicodeString tmpProcess = processes.GetNext(pos);
            itemInfo.psz   = tmpProcess.c_str();
            int newItem = FindItem(&itemInfo);
            if (newItem != (-1)) // item found
            {
                SetItemState(newItem, LVIS_SELECTED , LVIS_SELECTED);
                isSelected = true;
            }
        }

        return isSelected;
    }

    void ProcessListCtrl::OnHeaderClicked(NMHDR* pNMHDR, LRESULT* pResult)
    {
        HD_NOTIFY* hdn = (HD_NOTIFY*)pNMHDR;

        if (hdn->iButton == 0)
        {
            // Check if the user clicked on header using left mouse button.
            if(hdn->iItem == m_pRealModel->m_sortedCol)
            {
                m_pRealModel->m_sortAscending = !m_pRealModel->m_sortAscending;
            }
            else
            {
                m_pRealModel->m_sortAscending = true;
            }

            // Update the direction and location of the sort arrow.
            updateSortArrow(hdn->iItem);

			sortListControl();
       }

        *pResult = 0;
    }

	void ProcessListCtrl::sortListControl()
	{
		// Put some parameters into a struct so it can be passed to SortItems.
		SortParams params;
		params.hwnd = this->GetSafeHwnd();
		params.colIndex = m_pRealModel->m_sortedCol;
		params.sortAscending = m_pRealModel->m_sortAscending;

		// Update the item data (required by SortItems).
		int nCount = GetItemCount();
		for (int pos = 0; pos < nCount; pos++)
		{
			SetItemData(pos, pos);
		}

		if (m_pRealModel->m_sortedCol == RESTARTS_COL)
		{
			SortItems((PFNLVCOMPARE)compareIntegers, (DWORD)&params);
		}
		else if (m_pRealModel->m_sortedCol == RUNNING_SINCE_COL)
		{
			SortItems((PFNLVCOMPARE)compareDates, (DWORD)&params);
		}
		// TD16592 ++
		else if (m_pRealModel->m_sortedCol == PROCESS_ID_COL)
		{
			SortItems((PFNLVCOMPARE)compareStrings2, (DWORD)&params);
		}
		// ++ TD16592
		else
		{
			SortItems((PFNLVCOMPARE)compareStrings, (DWORD)&params);
		}
	}

    int CALLBACK ProcessListCtrl::compareStrings(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
    {
        SortParams* sp = (SortParams*)lParamSort;
	    
        // Get the text out of the required column.
        ProcessListCtrl* plist = (ProcessListCtrl*)CWnd::FromHandle(sp->hwnd);
	    TAUnicodeString item1, item2;
	    item1 = plist->GetItemText(lParam1, sp->colIndex);
	    item2 = plist->GetItemText(lParam2, sp->colIndex);


        // Make the comparison
        if (sp->sortAscending)
        {
            return item1.str().CompareNoCase(item2.c_str());
        }
        else
        {
            return item2.str().CompareNoCase(item1.c_str());
        }   
    }

	// TD16592 ++
	int CALLBACK ProcessListCtrl::compareStrings2(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
	{
		SortParams* sp = (SortParams*)lParamSort;

		// Get the text out of the required column.
		ProcessListCtrl* plist = (ProcessListCtrl*)CWnd::FromHandle(sp->hwnd);
		TAUnicodeString item1, item2;
		item1 = plist->GetItemText(lParam1, sp->colIndex);
		item2 = plist->GetItemText(lParam2, sp->colIndex);

		std::string strItem1 = item1.toMultiByteStdString();
		std::string strItem2 = item2.toMultiByteStdString();
		
        SystemManager * pApp = dynamic_cast<SystemManager*> (AfxGetApp());
        std::string locationName = pApp->getApplicationController()->getEntity()->getLocationName();
        return ProcessListCtrlModel::compareString2(sp, locationName.c_str(), strItem1.c_str(), strItem2.c_str());


	}
	// ++ TD16592

    int CALLBACK ProcessListCtrl::compareIntegers(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
    {
		SortParams* sp = (SortParams*)lParamSort;

		// Get the text out of the required column.
		ProcessListCtrl* plist = (ProcessListCtrl*)CWnd::FromHandle(sp->hwnd);
		TAUnicodeString item1, item2;
		item1 = plist->GetItemText(lParam1, sp->colIndex);
		item2 = plist->GetItemText(lParam2, sp->colIndex);

		std::string strItem1 = item1.toMultiByteStdString();
		std::string strItem2 = item2.toMultiByteStdString();

		return ProcessListCtrlModel::compareIntegers(lParamSort, strItem1.c_str(), strItem2.c_str());
    }

	int CALLBACK ProcessListCtrl::compareDates(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
    {
        SortParams* sp = (SortParams*)lParamSort;
	    
        // Get the text out of the required column.
        ProcessListCtrl* plist = (ProcessListCtrl*)CWnd::FromHandle(sp->hwnd);
	    TAUnicodeString item1, item2;
	    item1 = plist->GetItemText(lParam1, sp->colIndex);
	    item2 = plist->GetItemText(lParam2, sp->colIndex);

		std::string strItem1 = item1.toMultiByteStdString();
		std::string strItem2 = item2.toMultiByteStdString();

		return ProcessListCtrlModel::compareDates(sp, strItem1.c_str(), strItem2.c_str());


    }

    void ProcessListCtrl::updateSortArrow(int newColumnNum)
    {
        // Add sort arrows to list columns
	    HDITEM HeaderItem; 
	    HeaderItem.mask = HDI_FORMAT | HDI_BITMAP; 
	    CHeaderCtrl* HeaderCtrl = GetHeaderCtrl(); 
	    HeaderCtrl->GetItem(newColumnNum, &HeaderItem); 
	    if (HeaderItem.hbm != 0) 
	    { 
		    DeleteObject(HeaderItem.hbm); 
		    HeaderItem.hbm = 0; 
	    } 
	    
	    HeaderItem.fmt |= HDF_BITMAP | HDF_BITMAP_ON_RIGHT; 

	    // Load the required image
	    HeaderItem.hbm = (HBITMAP)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE((m_pRealModel->m_sortAscending) ? IDB_UP : IDB_DOWN), IMAGE_BITMAP, 0, 0, LR_LOADMAP3DCOLORS); 

	    // Setup the header with the image
	    HeaderCtrl->SetItem(newColumnNum, &HeaderItem); 
	    if (m_pRealModel->m_sortedCol != newColumnNum) 
	    {
		    // Remove image from original sort column
            removeSortArrow();

            // Now store the new location of the arrow.
            m_pRealModel->m_sortedCol = newColumnNum;
        }
    }

    void ProcessListCtrl::removeSortArrow()
    {
   	    HDITEM HeaderItem; 
	    HeaderItem.mask = HDI_FORMAT | HDI_BITMAP; 
	    CHeaderCtrl* HeaderCtrl = GetHeaderCtrl(); 

   		// Remove image from original sort column
		HeaderCtrl->GetItem(m_pRealModel->m_sortedCol, &HeaderItem); 
		HeaderItem.fmt &= ~(HDF_BITMAP | HDF_BITMAP_ON_RIGHT); 
		if (HeaderItem.hbm != 0) 
		{ 
			DeleteObject(HeaderItem.hbm); 
			HeaderItem.hbm = 0; 
		} 
		HeaderCtrl->SetItem(m_pRealModel->m_sortedCol, &HeaderItem);
    }

    std::string ProcessListCtrl::getMyPModelName()
    {
        return PMODEL_PROCESS_LIST_CTRL;
    }

	void ProcessListCtrl::setupPModel()
	{
		m_pRealModel = dynamic_cast<ProcessListCtrlModel*> (m_pModel);
		TA_ASSERT(m_pRealModel != NULL, "PModel has been initialized successfully");
	}

	void ProcessListCtrl::notifyUpdate( const char * updateType, TA_Base_Bus::EventArgs * args/*=NULL*/ )
	{

	}


} // namespace TA_Base_App
