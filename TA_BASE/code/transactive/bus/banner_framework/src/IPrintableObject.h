/**
* The source code in this file is the property of
* Combuilder PTE LTD and is not for redistribution
* in any form.
*
* Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/bus/banner_framework/src/IPrintableObject.h $
* @author:  Noel Tajanlangit
* @version: $Revision: #2 $
*
* Last modification: $DateTime: 2012/02/06 16:15:14 $
* Last modified by:  $Author: haijun.li $
*
* This class is an interface for all printable objects.
*
*/


#if !defined(__IPRINTABLE_OBJECT_H_INCLUDED__)
#define __IPRINTABLE_OBJECT_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace TA_Base_Bus
{
	class IPrintableObject
	{
	public:
		virtual ~IPrintableObject(){};

		virtual void printObject() = 0 ;
	};

}

#endif // __IPRINTABLE_OBJECT_H_INCLUDED__