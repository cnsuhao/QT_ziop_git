/**
  * The source code in this file is the property of
  * Combuilder PTE LTD and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/bus/banner_framework/src/PrintButton.cpp $
  * @author:  Noel R. Tajanlangit
  * @version: $Revision: #3 $
  *
  * Last modification: $DateTime: 2012/07/02 09:30:42 $
  * Last modified by:  $Author: Shiva $
  *
  * This class represents a print button that can be added
  * to the banner.
  *
  */

#include "bus/resources/resource.h"
#include "core/utilities/src/DebugUtil.h"

#include "bus/banner_framework/src/PrintButton.h"

#include <QImage.h>
#include <QBitmap.h>

namespace TA_Base_Bus
{

	void PrintButton::intialize()
	{
		QImage* icon = new QImage(":/res/printIcon.ico");
		QBitmap tempBitmap = QBitmap::fromImage(*icon);
		this->setIcon(tempBitmap);
	}

	void PrintButton::setPrintableObject(IPrintableObject* pPrintObject)
	{
		if (NULL != pPrintObject)
		{
			m_pPrintableObject = pPrintObject;
		}
	}

	void PrintButton::print()
	{
		if (NULL != m_pPrintableObject)
		{
			m_pPrintableObject->printObject();
		}
	}

}