
/**
* The source code in this file is the property of
* ST Engineering Electronics (LSG) and is not for redistribution
* in any form.
*
* Source:   $File:   $
* @author:  Luo Huirong
* @version: $Revision: #1 $
*
* Last modification: $DateTime: 2016/11/25 13:15:14 $
* Last modified by:  $Author: Luo Huirong $
*
* The  head file provide common type definition for all common control
*   
*
*/

#pragma once
#ifndef _TA_UI_CommonType_
#define _TA_UI_CommonType_

#include <String>
#include <list>
 
#include "bus/common_control/src/common/ctlCommonEnhance.h"
QT_BEGIN_NAMESPACE

///define list of std::string
typedef  QDESIGNER_WIDGET_EXPORT std::list<std::string>  STDStringList;

///define sorty type
enum QDESIGNER_WIDGET_EXPORT  UISortType
{
	UINUMBER = 1,
	UISTRING = 2,
	UIDATETIME = 3,
	UIDATE = 4,
	UITIME = 5,
};


QT_END_NAMESPACE


#endif
