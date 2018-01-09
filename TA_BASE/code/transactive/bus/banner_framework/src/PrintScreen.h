/**
  * The source code in this file is the property of
  * Combuilder PTE LTD and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/bus/banner_framework/src/PrintScreen.h $
  * @author:  Noel Tajanlangit
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2012/02/06 16:15:14 $
  * Last modified by:  $Author: haijun.li $
  *
  * This class is responsible for capturing and printing the screen.
  *
  */


#if !defined(__PRINT_SCREEN_H_INCLUDED__)
#define __PRINT_SCREEN_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "bus/banner_framework/src/IPrintableObject.h"
#include <QPixmap>

namespace TA_Base_Bus
{
	class PrintScreen : public TA_Base_Bus::IPrintableObject
	{
	public:
		// Constructor
		PrintScreen();

		// Destructor
		~PrintScreen();

		/* printObject
		 *
		 * launches the print dialog and prints the screen shot captured during construction
		 *
		 * @Param	: none
		 *
		 * @Return	: none
		 */
		virtual void printObject();

	private: // methods

		/* captureScreen
		 *
		 * captures a screen shot and stores the image to a memory device dependent bitmap
		 *
		 * @Param	: const int screenNum - desired monitor to capture the screen shot
		 *
		 * @Return	: none
		 */
		void captureScreen(const int screenNum);

		
	private: // variables
	
		unsigned short m_numOfScreens;
		unsigned int m_SrcHeight;
		unsigned int m_ScrWidth;
		QPixmap m_pixMap;

	};
}


#endif //__PRINT_SCREEN_H_INCLUDED__