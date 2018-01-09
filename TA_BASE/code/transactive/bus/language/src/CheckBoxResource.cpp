#include "StdAfx.h"
#include "bus/language/src/CheckBoxResource.h"


namespace TA_Base_Bus
{

    CheckBoxResource::CheckBoxResource( UINT id, const CString& caption )
        : AbstractDialogItemResource( id ),
          m_caption( caption )
    {
    }

}
