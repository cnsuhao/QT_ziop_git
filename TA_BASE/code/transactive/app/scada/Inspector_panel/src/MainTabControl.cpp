/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/scada/inspector_panel/src/MainTabControl.cpp $
  * @author:  Andy Parker
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: CM $
  * 
  * This class handles the tab control displayed on the main dialog screen.  It manages the 
  * three associated dialogs CInformationDialog, CControlDialog and CTagDialog.
  */

#include "app/scada/Inspector_Panel/src/stdafx.h"

#include "app/scada/inspector_panel/src/InformationDialog.h"
#include "app/scada/inspector_panel/src/ControlDialog.h"
#include "app/scada/inspector_panel/src/LimitDialog.h"
#include "app/scada/inspector_panel/src/NotesDialog.h"
#include "app/scada/inspector_panel/src/TagDialog.h"
#include "app/scada/inspector_panel/src/InhibitDialog.h"
#include "app/scada/inspector_panel/src/OverrideDialog.h"
#include "app/scada/inspector_panel/src/MeterDialog.h"
#include "app/scada/inspector_panel/src/StatusDialog.h"

#include "app/scada/inspector_panel/src/InspectorPanel.h"
#include "app/scada/inspector_panel/src/MainTabControl.h"


#include "bus/application_types/src/apptypes.h"

#include "core/utilities/src/DebugUtil.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace TA_Base_App
{
	/////////////////////////////////////////////////////////////////////////////
	// CMainTabControl

	CMainTabControl::CMainTabControl() 
		: m_equipment (NULL), 
		m_heightDifference (0),//, m_rightsMgr (NULL)
		m_pTransactiveApp(NULL)
	{
 		FUNCTION_ENTRY ("CMainTabControl()");

		m_pTransactiveApp = dynamic_cast<TA_Base_Bus::ITransActiveWinApp*>(AfxGetApp());
		TA_ASSERT(m_pTransactiveApp !=NULL,"Invalid m_pTransactiveApp");
		this->attachPModel();
		this->initChildTabs();

		FUNCTION_EXIT;
	}



	CMainTabControl::~CMainTabControl()
	{
		FUNCTION_ENTRY ("~CMainTabControl()");

		clearChildTabs();

		FUNCTION_EXIT;
	}


	void CMainTabControl::setupPModel()
	{
		m_realPModel = dynamic_cast<MainTabControlModel*> (m_pModel);
		TA_ASSERT(m_realPModel != NULL, "PModel should be valid");
	}


	std::string CMainTabControl::getMyPModelName()
	{
		return PMODEL_MAIN_TAB_CONTROL;
	}

	void CMainTabControl::clearChildTabs()
	{
		std::map<ETabNumbers, ITabDialog*>::iterator tabIterator = m_tabDialogs.begin();
		for(;tabIterator!=m_tabDialogs.end();++tabIterator)
		{
			delete tabIterator->second;
			tabIterator->second = NULL;
		}
		m_tabDialogs.clear();
	}

	void CMainTabControl::initChildTabs()
	{
		this->clearChildTabs();

		std::vector<ETabNumbers> configTabs = m_realPModel->getConfigureTabs();
		for(unsigned int index=0; index < configTabs.size(); index++)
		{
			switch(configTabs[index])
			{
			case InformationTab:
				{
					CInformationDialog* informationDialog = new CInformationDialog;
					m_tabDialogs[InformationTab] = informationDialog;
					
					//TAUnicodeString UnicodeStr(_T("Information"));
					AddPage(*informationDialog,IDD_INFORMATION,  TALoadString(STRING_ID_INSPECTOR_PANEL_0027).c_str() );
				}
				break;
 			case ControlTab:
				{
					CControlDialog* controlDialog = new CControlDialog;
					m_tabDialogs[ControlTab] = controlDialog;

					//TAUnicodeString UnicodeStr(_T("Control"));
					AddPage(*controlDialog,IDD_CONTROL,TALoadString(STRING_ID_INSPECTOR_PANEL_0028).c_str() );
				}
				break;
			case NotesTab:
				{
					CNotesDialog* notesDialog = new CNotesDialog(m_pTransactiveApp);
					m_tabDialogs[NotesTab] = notesDialog;

					//TAUnicodeString UnicodeStr(_T("Notes"));
					AddPage(*notesDialog,IDD_NOTES,TALoadString(STRING_ID_INSPECTOR_PANEL_0029).c_str());
				}
				break;
			case TagTab:
				{
					CTagDialog* tagDialog = new CTagDialog(m_pTransactiveApp);
					m_tabDialogs[TagTab] = tagDialog;

					//TAUnicodeString UnicodeStr(_T(" Tag"));
					AddPage(*tagDialog,IDD_TAG, TALoadString(STRING_ID_INSPECTOR_PANEL_0030).c_str());
				}
				break;
			case InhibitTab:
				{
					CInhibitDialog* inhibitDialog = new CInhibitDialog;
					m_tabDialogs[InhibitTab] = inhibitDialog;

					//TAUnicodeString UnicodeStr(_T("Inhibit"));
					AddPage(*inhibitDialog,IDD_INHIBIT,TALoadString(STRING_ID_INSPECTOR_PANEL_0031).c_str());
				}
				break;
			case OverrideTab:
				{
					COverrideDialog* overrideDialog = new COverrideDialog;
					m_tabDialogs[OverrideTab] = overrideDialog;

					//TAUnicodeString UnicodeStr(_T("Override"));
					AddPage(*overrideDialog,IDD_OVERRIDE,TALoadString(STRING_ID_INSPECTOR_PANEL_0032).c_str());
				}
				break;


			case LimitsTab:
				{
					CLimitDialog* limitDialog = new CLimitDialog;
					m_tabDialogs[LimitsTab] = limitDialog;

					//TAUnicodeString UnicodeStr(_T("Limits"));
					AddPage(*limitDialog,IDD_LIMITS,TALoadString(STRING_ID_INSPECTOR_PANEL_0033).c_str());
				}
				break;
			
			case MeterTab:
				{
					CMeterDialog* meterDialog = new CMeterDialog;
					m_tabDialogs[MeterTab] = meterDialog;

					//TAUnicodeString UnicodeStr(_T("Meter"));
					AddPage(*meterDialog,IDD_METER,TALoadString(STRING_ID_INSPECTOR_PANEL_0034).c_str());
				}
				break;

			case StatusTab:
				{
					CStatusDialog* statusDialog = new CStatusDialog(m_pTransactiveApp);
					m_tabDialogs[StatusTab] = statusDialog;

					//TAUnicodeString UnicodeStr(_T("Status"));
					AddPage(*statusDialog,IDD_STATUS,TALoadString(STRING_ID_INSPECTOR_PANEL_0035).c_str());
				}
				break;
				
			}
		}
	}

	void CMainTabControl::refreshAllTab()
	{
		FUNCTION_ENTRY ("setEquipment()");

		std::map<ETabNumbers, ITabDialog*>::iterator it;

		for (it = m_tabDialogs.begin(); it != m_tabDialogs.end(); it++)
		{
			if ((*it).second != NULL)
			{
				(*it).second->refreshAllData();
			}
		}

//		m_equipment = equipment;

		FUNCTION_EXIT;
	}

	void CMainTabControl::updateControlTab()
	{
		m_tabDialogs[ControlTab]->refreshAllData();
	}

    void CMainTabControl::refreshStatusTab()
    {
		FUNCTION_ENTRY ("setStatusTabEquipment()");

		std::map<ETabNumbers, ITabDialog*>::iterator it = m_tabDialogs.find(StatusTab);

        if (it != m_tabDialogs.end())
        {
            it->second->refreshAllData();
		}

		FUNCTION_EXIT;
    }


	void CMainTabControl::updateDataPoints(unsigned long key, TA_Base_Bus::ScadaEntityUpdateType updateType)
	{
		FUNCTION_ENTRY ("updateDataPoints()");

		std::map<ETabNumbers, ITabDialog*>::iterator it;

		for (it = m_tabDialogs.begin(); it != m_tabDialogs.end(); it++)
		{
			if ((*it).second != NULL)
			{
				(*it).second->updateDataPoint(key, updateType);
			}
		}

		FUNCTION_EXIT;
	}

	void CMainTabControl::updateDataNode(TA_Base_Bus::ScadaEntityUpdateType updateType)
	{
		FUNCTION_ENTRY ("updateDataNode()");

		std::map<ETabNumbers, ITabDialog*>::iterator it;

		for (it = m_tabDialogs.begin(); it != m_tabDialogs.end(); it++)
		{
			if ((*it).second != NULL)
			{
				(*it).second->updateDataNode(updateType);
			}
		}

		FUNCTION_EXIT;
	}

	bool CMainTabControl::isUnsavedChangeOnTab(ETabNumbers tab)
	{
		FUNCTION_ENTRY ("isUnsavedChangeOnTab()");

		bool unsavedChange = false;

		std::map<ETabNumbers, ITabDialog*>::iterator it = m_tabDialogs.find(tab);

		if (it != m_tabDialogs.end())
		{
			unsavedChange = (*it).second->isUnsavedChangeOnTab();
		}

		FUNCTION_EXIT;

		return unsavedChange;
	}

	bool CMainTabControl::isOkToChangeTab(ETabNumbers tab)
	{
		FUNCTION_ENTRY ("isOkToChangeTab()");

		bool okToChange = true;

		std::map<ETabNumbers, ITabDialog*>::iterator it = m_tabDialogs.find(tab);

		if (it != m_tabDialogs.end())
		{
			okToChange = (*it).second->isOkToChangeTab();
		}
		
		FUNCTION_EXIT;

		return okToChange;
	}

	bool CMainTabControl::isAnyDataPointsDisplayedInTab(ETabNumbers tab)
	{
		FUNCTION_ENTRY ("isAnyDataPointsDisplayedInTab()");

		bool result = true;	// asssuming that there are some dp displayed on screen

		// find if we have the specified tab
		std::map<ETabNumbers, ITabDialog*>::iterator it = m_tabDialogs.find(tab);

		// if found one
		if (it != m_tabDialogs.end())
		{
			// check if any data point is displayed
			result = (*it).second->isAnyDataPointsDisplayedInTab();
		}
		// else do nothing

		return result;

		FUNCTION_EXIT;
	}

	int CMainTabControl::resize()
	{
		CRect originalTabRect;
		int yOffset = 0;
		
		GetWindowRect(&originalTabRect);
		originalTabRect.NormalizeRect();

		if (m_heightDifference == 0)
		{
			CRect dialogRect;

			std::map<ETabNumbers, ITabDialog*>::iterator it = m_tabDialogs.begin();

			if (it != m_tabDialogs.end() && (*it).second != NULL)
			{
				(*it).second->getWindowRect(&dialogRect);
				dialogRect.NormalizeRect();

				m_heightDifference = originalTabRect.Height() - dialogRect.Height();
			}
		}

		{
			unsigned int dialogHeight = getMinimumDialogHeight();

			std::map<ETabNumbers, ITabDialog*>::iterator it;
			
			for (it = m_tabDialogs.begin(); it != m_tabDialogs.end(); it++)
			{
				if ((*it).second != NULL)
				{
					(*it).second->resize(dialogHeight);
				}
			}

			SetWindowPos(NULL, 0, 0, originalTabRect.Width(), dialogHeight+m_heightDifference, SWP_NOMOVE|SWP_NOZORDER);
		}

		{
			CRect newTabRect;
			GetWindowRect(&newTabRect);
			newTabRect.NormalizeRect();

			yOffset = newTabRect.Height() - originalTabRect.Height();
		}

		return yOffset;
	}

	unsigned int CMainTabControl::getMinimumDialogHeight()
	{
		unsigned int minimumHeight = 0;
		unsigned int height = 0;

		std::map<ETabNumbers, ITabDialog*>::iterator it;

		for (it = m_tabDialogs.begin(); it != m_tabDialogs.end(); it++)
		{
			if ((*it).second != NULL)
			{
				height = (*it).second->getMinimumHeight();
				if (height > minimumHeight)
				{
					minimumHeight = height;
				}
			}
		}

		return minimumHeight;
	}

	void CMainTabControl::updatePermissions()
	{
		std::map<ETabNumbers, ITabDialog*>::iterator it;

		for (it = m_tabDialogs.begin(); it != m_tabDialogs.end(); it++)
		{           
			if ((*it).second != NULL)
			{
				(*it).second->updatePermissions();
			}
		}
	}


	void CMainTabControl::PreSubclassWindow()
	{
		CTabDialogCtrl::PreSubclassWindow();

		for (std::map<ETabNumbers, ITabDialog*>::iterator it = m_tabDialogs.begin(); it != m_tabDialogs.end(); it++)
		{
            // TD#1601 - include tab pages in tab sequencing
            CDialog* pDialog = dynamic_cast<CDialog*>((*it).second);
            if (0 != pDialog)
            {
                pDialog->ModifyStyleEx(0, WS_EX_CONTROLPARENT);
            }
		}
	}


	void CMainTabControl::resetList()
	{
		m_tabDialogs[InformationTab]->resetList();
	}

	BEGIN_MESSAGE_MAP(CMainTabControl, CTabDialogCtrl)
		//{{AFX_MSG_MAP(CMainTabControl)
		//}}AFX_MSG_MAP
	END_MESSAGE_MAP()
}
