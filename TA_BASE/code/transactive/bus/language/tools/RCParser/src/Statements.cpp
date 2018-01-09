#include "StdAfx.h"
#include "Statements.h"
#include "IVisitor.h"


// CAPTION
const char* CAPTIONstatement::my_type()
{
    return "CAPTION statement";
}


void CAPTIONstatement::accept( DIALOGresource* dialog, IVisitor* visitor )
{
    visitor->visit( dialog, this );
}


// FONT
const char* FONTstatement::my_type()
{
    return "FONT statement";
}


void FONTstatement::accept( DIALOGresource* dialog, IVisitor* visitor )
{
    visitor->visit( dialog, this );
}


// MENU
const char* MENUstatement::my_type()
{
    return "MENU statement";
}


void MENUstatement::accept( DIALOGresource* dialog, IVisitor* visitor )
{
    visitor->visit( dialog, this );
}


// MENUITEM
const char* MENUITEMstatement::my_type()
{
    return "MENUITEM statement";
}


void MENUITEMstatement::accept( MENUresource* menu, IVisitor* visitor )
{
    visitor->visit( menu, this );
}


void MENUITEMstatement::accept( POPUPresource* popup, IVisitor* visitor )
{
    visitor->visit( popup, this );
}


// STYLE
const char* STYLEstatement::my_type()
{
    return "STYLE statement";
}


// EXSTYLE
const char* EXSTYLEstatement::my_type()
{
    return "EXSTYLE statement";
}


// BUTTON
const char* BUTTONstatement::my_type()
{
    return "BUTTON statement";
}


void BUTTONstatement::accept( TOOLBARresource* toolbar, IVisitor* visitor )
{
    visitor->visit( toolbar, this );
}
