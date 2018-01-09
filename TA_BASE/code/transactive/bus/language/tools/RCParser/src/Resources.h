#pragma once
#include "IResource.h"
#include "IStatement.h"
#include "IControl.h"

FORWARD_DECLARE_3(IVisitor, POPUPresource, MENUITEMstatement);

struct MENUITEM
{
    MENUITEM( POPUPresourcePtr popup ) : m_popup(popup) {}
    MENUITEM( MENUITEMstatementPtr menuitem ) : m_menuitem(menuitem) {}
    POPUPresourcePtr m_popup;
    MENUITEMstatementPtr m_menuitem;
};


// nameID DIALOG x, y, width, height  [optional-statements] {control-statement  . . . }
struct DIALOGresource : IResource
{
    std::string m_nameID;
    std::string m_x;
    std::string m_y;
    std::string m_width;
    std::string m_height;
    IStatementPtrList m_optional_statements;
    IControlPtrList m_controls;

    virtual const char* my_type();
    virtual void accept( IVisitor* visitor );
};


// nameID DIALOGEX x, y, width, height [ , helpID] [optional-statements]  {control-statements}
struct DIALOGEXresource : IResource
{
    std::string m_nameID;
    int x;
    int y;
    int width;
    int height;
    int m_helpID;
    IStatementPtrList m_optional_statements;
    IControlPtrList m_controls;

    virtual const char* my_type();
    virtual void accept( IVisitor* visitor ) {}
};


// POPUP text, [optionlist] {item-definitions ...}
struct POPUPresource : IResource
{
    std::string m_text;
    std::vector<MENUITEM> m_menu_items;

    virtual const char* my_type();
    virtual void accept( MENUresource* parent, IVisitor* visitor );
    virtual void accept( POPUPresource* parent, IVisitor* visitor );
};


// menuID MENU  [optional-statements]  {item-definitions ... }
struct MENUresource : IResource
{
    std::string m_munuID;
    std::vector<MENUITEM> m_menu_items;

    virtual const char* my_type();
    virtual void accept( IVisitor* visitor );
};


// stringID string
struct STRINGresource : IResource
{
    std::string m_stringID;
    std::string m_string;

    virtual const char* my_type();
    virtual void accept( STRINGTABLEresource* parent, IVisitor* visitor );
};


// STRINGTABLE  [optional-statements] {stringID string  ...}
struct STRINGTABLEresource : IResource
{
    IResourcePtrList m_strings;

    virtual const char* my_type();
    virtual void accept( IVisitor* visitor );
};


// Note: there is no document for TOOLBAR
// toolbarID TOOLBAR  x, y
struct TOOLBARresource : IResource
{
    std::string m_toolbarID;
    std::string m_x;
    std::string m_y;
    IStatementPtrList m_buttons;

    virtual const char* my_type();
    virtual void accept( IVisitor* visitor );
};
