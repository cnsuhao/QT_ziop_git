#include "StdAfx.h"
#include "bus/language/src/GroupBoxResource.h"


namespace TA_Base_Bus
{

    GroupBoxResource::GroupBoxResource( UINT id, const CString& caption )
        : AbstractDialogItemResource( id ),
          m_caption( caption )
    {
    }

}
