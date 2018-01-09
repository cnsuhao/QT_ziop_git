#include "StdAfx.h"
#include "RunAnys.h"

RunAnys& RunAnys::getInstance()
{
    return *ACE_Unmanaged_Singleton<RunAnys, ACE_Null_Mutex>::instance();
}
