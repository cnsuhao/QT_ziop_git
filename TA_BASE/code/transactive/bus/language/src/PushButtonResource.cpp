#include "StdAfx.h"
#include "bus/language/src/PushButtonResource.h"


namespace TA_Base_Bus
{

    PushButtonResource::PushButtonResource( UINT id, const CString& caption )
        : AbstractDialogItemResource( id ),
          m_caption( caption )
    {
    }

}
