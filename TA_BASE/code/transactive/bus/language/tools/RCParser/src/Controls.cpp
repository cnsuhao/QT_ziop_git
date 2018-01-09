#include "StdAfx.h"
#include "Controls.h"
#include "IVisitor.h"


// CHECKBOX
const char* CHECKBOXcontrol::my_type()
{
    return "CHECKBOX control";
}


void CHECKBOXcontrol::accept( DIALOGresource* parent, IVisitor* visitor )
{
    visitor->visit( parent, this );
}


// COMBOBOX
const char* COMBOBOXcontrol::my_type()
{
    return "COMBOBOX control";
}


void COMBOBOXcontrol::accept( DIALOGresource* parent, IVisitor* visitor )
{
    visitor->visit( parent, this );
}


// CONTROL
const char* CONTROLcontrol::my_type()
{
    return "CONTROL control";
}

void CONTROLcontrol::accept( DIALOGresource* parent, IVisitor* visitor )
{
    visitor->visit( parent, this );
}


// LTEXT
const char* CTEXTcontrol::my_type()
{
    return "CTEXT control";
}


void CTEXTcontrol::accept( DIALOGresource* parent, IVisitor* visitor )
{
    visitor->visit( parent, this );
}


// DEFPUSHBUTTON
const char* DEFPUSHBUTTONcontrol::my_type()
{
    return "DEFPUSHBUTTON control";
}

void DEFPUSHBUTTONcontrol::accept( DIALOGresource* parent, IVisitor* visitor )
{
    visitor->visit( parent, this );
}


// EDITTEXT
const char* EDITTEXTcontrol::my_type()
{
    return "EDITTEXT control";
}

void EDITTEXTcontrol::accept( DIALOGresource* parent, IVisitor* visitor )
{
    visitor->visit( parent, this );
}


// GROUPBOX
const char* GROUPBOXcontrol::my_type()
{
    return "GROUPBOX control";
}

void GROUPBOXcontrol::accept( DIALOGresource* parent, IVisitor* visitor )
{
    visitor->visit( parent, this );
}


// ICON
const char* ICONcontrol::my_type()
{
    return "ICON control";
}

void ICONcontrol::accept( DIALOGresource* parent, IVisitor* visitor )
{
    visitor->visit( parent, this );
}


// LISTBOX
const char* LISTBOXcontrol::my_type()
{
    return "LISTBOX control";
}


void LISTBOXcontrol::accept( DIALOGresource* parent, IVisitor* visitor )
{
    visitor->visit( parent, this );
}


// LTEXT
const char* LTEXTcontrol::my_type()
{
    return "LTEXT control";
}


void LTEXTcontrol::accept( DIALOGresource* parent, IVisitor* visitor )
{
    visitor->visit( parent, this );
}


// PUSHBOX
const char* PUSHBOXcontrol::my_type()
{
    return "PUSHBOX control";
}


void PUSHBOXcontrol::accept( DIALOGresource* parent, IVisitor* visitor )
{
    visitor->visit( parent, this );
}


// PUSHBUTTON
const char* PUSHBUTTONcontrol::my_type()
{
    return "PUSHBUTTON control";
}


void PUSHBUTTONcontrol::accept( DIALOGresource* parent, IVisitor* visitor )
{
    visitor->visit( parent, this );
}


// RADIOBUTTON
const char* RADIOBUTTONcontrol::my_type()
{
    return "RADIOBUTTON control";
}


void RADIOBUTTONcontrol::accept( DIALOGresource* parent, IVisitor* visitor )
{
    visitor->visit( parent, this );
}


// RTEXT
const char* RTEXTcontrol::my_type()
{
    return "RTEXT control";
}


void RTEXTcontrol::accept( DIALOGresource* parent, IVisitor* visitor )
{
    visitor->visit( parent, this );
}
