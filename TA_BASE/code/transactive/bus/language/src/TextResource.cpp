#include "StdAfx.h"
#include "bus/language/src/TextResource.h"


namespace TA_Base_Bus
{

    TextResource::TextResource( UINT id, const CString& caption )
        : AbstractDialogItemResource( id ),
          m_caption( caption )
    {
    }

}
