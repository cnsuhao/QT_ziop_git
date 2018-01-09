#include "StdAfx.h"
#include "Resources.h"
#include "Statements.h"
#include "IVisitor.h"


// DIALOG
const char* DIALOGresource::my_type()
{
    return "DIALOG resource";
}


void DIALOGresource::accept( IVisitor* visitor )
{
    visitor->visit( this );

    for ( size_t i = 0; i < m_optional_statements.size(); ++i )
    {
        m_optional_statements[i]->accept( this, visitor );
    }

    for ( size_t i = 0; i < m_controls.size(); ++i )
    {
        m_controls[i]->accept( this, visitor );
    }
}


// DIALOGEX
const char* DIALOGEXresource::my_type()
{
    return "DIALOGEX resource";
}


// POPUP
const char* POPUPresource::my_type()
{
    return "POPUP resource";
}


void POPUPresource::accept( MENUresource* parent, IVisitor* visitor )
{
    visitor->visit( parent, this );

    for ( size_t i = 0; i < m_menu_items.size(); ++i )
    {
        if ( m_menu_items[i].m_popup )
        {
            m_menu_items[i].m_popup->accept( this, visitor );
        }
        else
        {
            m_menu_items[i].m_menuitem->accept( this, visitor );
        }
    }
}


void POPUPresource::accept( POPUPresource* parent, IVisitor* visitor )
{
    visitor->visit( parent, this );

    for ( size_t i = 0; i < m_menu_items.size(); ++i )
    {
        if ( m_menu_items[i].m_popup )
        {
            m_menu_items[i].m_popup->accept( this, visitor );
        }
        else
        {
            m_menu_items[i].m_menuitem->accept( this, visitor );
        }
    }
}


// menuID MENU
const char* MENUresource::my_type()
{
    return "MENU resource";
}


void MENUresource::accept( IVisitor* visitor )
{
    visitor->visit( this );

    for ( size_t i = 0; i < m_menu_items.size(); ++i )
    {
        if ( m_menu_items[i].m_popup )
        {
            m_menu_items[i].m_popup->accept( this, visitor );
        }
        else
        {
            m_menu_items[i].m_menuitem->accept( this, visitor );
        }
    }
}


// stringID string
const char* STRINGresource::my_type()
{
    return "STRING resource";
}


void STRINGresource::accept( STRINGTABLEresource* parent, IVisitor* visitor )
{
    visitor->visit( parent, this );
}


// STRINGTABLE
const char* STRINGTABLEresource::my_type()
{
    return "STRINGTABLE resource";
}


void STRINGTABLEresource::accept( IVisitor* visitor )
{
    visitor->visit( this );

    for ( size_t i = 0; i < m_strings.size(); ++i )
    {
        m_strings[i]->accept( this, visitor );
    }
}


// TOOLBAR
const char* TOOLBARresource::my_type()
{
    return "TOOLBAR resource";
}


void TOOLBARresource::accept( IVisitor* visitor )
{
    visitor->visit( this );

    for ( size_t i = 0; i < m_buttons.size(); ++i )
    {
        m_buttons[i]->accept( this, visitor );
    }
}
