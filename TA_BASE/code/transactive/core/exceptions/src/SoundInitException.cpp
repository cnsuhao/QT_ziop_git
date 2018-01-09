/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/core/exceptions/src/SoundInitException.cpp $
  * @author:  Justin Ebedes
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * This exception is raised when there is an error
  * initialising the sound card.
  *
  */

#include "SoundInitException.h"


namespace TA_Base_Core
{
	SoundInitException::SoundInitException()
	{

	}

	SoundInitException::SoundInitException(const std::string& msg) 
		: TransactiveException(msg)
	{

	}
		

	SoundInitException::~SoundInitException()
	{

	}
}

