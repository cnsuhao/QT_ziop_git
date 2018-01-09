/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/scada/DataNodeController/src/DataNodeTabControl.cpp $
  * @author:  Ripple
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  */
#include "stdafx.h"
#include "DataNodeController.h"
#include "DataNodeTabControl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

namespace TA_Base_App
{
	CDataNodeControllerTabControl::CDataNodeControllerTabControl()
	{
		AddPage(m_SummaryDlg, CSummaryDialog::IDD, CString("Summary"));
		AddPage(m_NotesDlg, CNotesDialog::IDD, CString("Notes"));
		AddPage(m_InhibitDlg, CInhibitDialog::IDD, CString("Inhibit"));
	}

	CDataNodeControllerTabControl::~CDataNodeControllerTabControl()
	{

	}

} // TA_Base_App

