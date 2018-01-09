#include "StdAfx.h"
#include "MyFutureMap.h"

MyFutureMap& MyFutureMap::getInstance()
{
    return *ACE_Unmanaged_Singleton<MyFutureMap, ACE_Null_Mutex>::instance();
}
