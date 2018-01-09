#include "StdAfx.h"
#include "bus/language/src/RadioButtonResource.h"


namespace TA_Base_Bus
{

    RadioButtonResource::RadioButtonResource( UINT id, const CString& caption )
        : AbstractDialogItemResource( id ),
          m_caption( caption )
    {
    }

}
