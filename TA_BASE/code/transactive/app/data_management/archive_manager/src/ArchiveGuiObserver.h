/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/data_management/archive_manager/src/ArchiveGuiObserver.cpp $
  * @author:  
  * @version: $Revision: #1  $
  *
  * Last modification: $DateTime:  $
  * Last modified by:  $Author:  $
  *
  * This Class is used by for formatting the user message box strings.
  *
  */

#if !defined(AFX_ARCHIVE_GUI_OBSERVER__1468A462_430F_4418_BC92_334001E5F340__INCLUDED_)
#define AFX_ARCHIVE_GUI_OBSERVER__1468A462_430F_4418_BC92_334001E5F340__INCLUDED_



#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "app/data_management/archive_manager/pmodel/src/IGuiObserver.h"



namespace TA_Base_App
{
    class ArchiveGuiObserver : public IGuiObserver
    {
	public:
		ArchiveGuiObserver();
		~ArchiveGuiObserver();
		virtual std::string getStringResourceFormated(UINT resourceId, std::string argument1);
		virtual std::string getStringResource(UINT resourceId);
		virtual std::string  getStringResourceFormated(UINT resourceId, std::string argument1,std::string argument2);
		virtual std::string  getStringResourceFormated(UINT resourceId, UINT argument1);
		virtual std::string  getStringResourceFormated(UINT resourceId, std::string argument1,UINT argument2);
		virtual std::string  getStringResourceFormated(UINT resourceId, std::string argument1,double argument2,std::string argument3);
		virtual std::string  getStringResourceFormated(UINT resourceId, std::string argument1,std::string argument2,std::string argument3);
	};
}
#endif //AFX_ARCHIVE_GUI_OBSERVER__1468A462_430F_4418_BC92_334001E5F340__INCLUDED_