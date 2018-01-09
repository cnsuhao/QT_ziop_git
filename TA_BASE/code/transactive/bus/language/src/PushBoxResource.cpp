#include "StdAfx.h"
#include "bus/language/src/PushBoxResource.h"


namespace TA_Base_Bus
{

    PushBoxResource::PushBoxResource( UINT id, const CString& caption )
        : AbstractDialogItemResource( id ),
          m_caption( caption )
    {
    }

}
