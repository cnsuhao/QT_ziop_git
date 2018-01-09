/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/core/exceptions/src/SoundPlayerException.cpp $
  * @author:  Ripple
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * <description>
  *
  */

#include "SoundPlayerException.h"

namespace TA_Base_Core
{
	SoundPlayerException::SoundPlayerException()
	{

	}

	SoundPlayerException::SoundPlayerException(const std::string& msg) 
		: TransactiveException(msg)
	{

	}
		

	SoundPlayerException::~SoundPlayerException()
	{

	}
}

