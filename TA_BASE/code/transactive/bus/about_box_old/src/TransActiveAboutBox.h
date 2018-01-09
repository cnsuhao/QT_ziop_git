/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/about_box/src/TransActiveAboutBox.h $
  * @author:  Ripple
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2015/10/09 16:15:26 $
  * Last modified by:  $Author: huirong.luo $
  *
  */
// TransActiveAboutBox.h: interface for the CTransActiveAboutBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRANSACTIVEABOUTBOX_H__AC0FBB63_5001_4DD0_B06D_09AEDA25F372__INCLUDED_)
#define AFX_TRANSACTIVEABOUTBOX_H__AC0FBB63_5001_4DD0_B06D_09AEDA25F372__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTransActiveAboutBox
{
public:

	static void DisplayAboutBox( const char buildVersion[],
							     const char buildDate[],
								 const char componentName[],
								 const char componentVersion[],
								 const int headerArraySize,
								 char* headerArray[]);
};

#endif // !defined(AFX_TRANSACTIVEABOUTBOX_H__AC0FBB63_5001_4DD0_B06D_09AEDA25F372__INCLUDED_)

