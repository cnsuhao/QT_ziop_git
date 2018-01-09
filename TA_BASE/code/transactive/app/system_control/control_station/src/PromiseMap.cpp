#include "StdAfx.h"
#include "PromiseAnyMap.h"

PromiseAnyMap& PromiseAnyMap::getInstance()
{
    return *ACE_Unmanaged_Singleton<PromiseAnyMap, ACE_Null_Mutex>::instance();
}
