#pragma once
#include "IControl.h"

FORWARD_DECLARE_3(IVisitor, DIALOGresource, DIALOGEXresource);


// CHECKBOX text, id, x, y, width, height [, style [, extended-style]]
struct CHECKBOXcontrol : IControl
{
    std::string m_text;
    std::string m_id;
    std::string m_x;
    std::string m_y;
    std::string m_width;
    std::string m_height;
    std::string m_style;

    virtual const char* my_type();
    virtual void accept( DIALOGresource* parent, IVisitor* visitor );
};


// COMBOBOX id, x, y, width, height [, style [, extended-style]]
struct COMBOBOXcontrol : IControl
{
    std::string m_id;
    std::string m_x;
    std::string m_y;
    std::string m_width;
    std::string m_height;
    std::string m_style;

    virtual const char* my_type();
    virtual void accept( DIALOGresource* parent, IVisitor* visitor );
};


// CONTROL text, id, class, style, x, y, width, height [, extended-style]
struct CONTROLcontrol : IControl
{
    std::string m_text;
    std::string m_id;
    std::string m_class;
    std::string m_style;
    std::string m_x;
    std::string m_y;
    std::string m_width;
    std::string m_height;
    std::string m_extended_style;

    virtual const char* my_type();
    virtual void accept( DIALOGresource* parent, IVisitor* visitor );
};


// CTEXT text, id, x, y, width, height [, style [, extended-style]]
struct CTEXTcontrol : IControl
{
    std::string m_text;
    std::string m_id;
    std::string m_x;
    std::string m_y;
    std::string m_width;
    std::string m_height;
    std::string m_style;

    virtual const char* my_type();
    virtual void accept( DIALOGresource* parent, IVisitor* visitor );
};


// DEFPUSHBUTTON text, id, x, y, width, height [, style [, extended-style]]
struct DEFPUSHBUTTONcontrol : IControl
{
    std::string m_text;
    std::string m_id;
    std::string m_x;
    std::string m_y;
    std::string m_width;
    std::string m_height;
    std::string m_style;

    virtual const char* my_type();
    virtual void accept( DIALOGresource* parent, IVisitor* visitor );
};


// EDITTEXT id, x, y, width, height [, style [, extended-style]]
struct EDITTEXTcontrol : IControl
{
    std::string m_id;
    std::string m_x;
    std::string m_y;
    std::string m_width;
    std::string m_height;
    std::string m_style;

    virtual const char* my_type();
    virtual void accept( DIALOGresource* parent, IVisitor* visitor );
};


// GROUPBOX text, id, x, y, width, height [, style [, extended-style]]
struct GROUPBOXcontrol : IControl
{
    std::string m_text;
    std::string m_id;
    std::string m_x;
    std::string m_y;
    std::string m_width;
    std::string m_height;
    std::string m_style;

    virtual const char* my_type();
    virtual void accept( DIALOGresource* parent, IVisitor* visitor );
};


// ICON text, id, x, y [, width, height, style [, extended-style]]
struct ICONcontrol : IControl
{
    std::string m_text;
    std::string m_id;
    std::string m_x;
    std::string m_y;
    std::string m_width;
    std::string m_height;
    std::string m_style;

    virtual const char* my_type();
    virtual void accept( DIALOGresource* parent, IVisitor* visitor );
};


// LISTBOX id, x, y, width, height [, style [, extended-style]]
struct LISTBOXcontrol : IControl
{
    std::string m_id;
    std::string m_x;
    std::string m_y;
    std::string m_width;
    std::string m_height;
    std::string m_style;

    virtual const char* my_type();
    virtual void accept( DIALOGresource* parent, IVisitor* visitor );
};


// LTEXT text, id, x, y, width, height [, style [, extended-style]]
struct LTEXTcontrol : IControl
{
    std::string m_text;
    std::string m_id;
    std::string m_x;
    std::string m_y;
    std::string m_width;
    std::string m_height;
    std::string m_style;

    virtual const char* my_type();
    virtual void accept( DIALOGresource* parent, IVisitor* visitor );
};


// PUSHBOX text, id, x, y, width, height [, style [, extended-style]]
struct PUSHBOXcontrol : IControl
{
    std::string m_text;
    std::string m_id;
    std::string m_x;
    std::string m_y;
    std::string m_width;
    std::string m_height;
    std::string m_style;

    virtual const char* my_type();
    virtual void accept( DIALOGresource* parent, IVisitor* visitor );
};


// PUSHBUTTON text, id, x, y, width, height [, style [, extended-style]]
struct PUSHBUTTONcontrol : IControl
{
    std::string m_text;
    std::string m_id;
    std::string m_x;
    std::string m_y;
    std::string m_width;
    std::string m_height;
    std::string m_style;

    virtual const char* my_type();
    virtual void accept( DIALOGresource* parent, IVisitor* visitor );
};


// RADIOBUTTON text, id, x, y, width, height [, style [, extended-style]]
struct RADIOBUTTONcontrol : IControl
{
    std::string m_text;
    std::string m_id;
    std::string m_x;
    std::string m_y;
    std::string m_width;
    std::string m_height;
    std::string m_style;

    virtual const char* my_type();
    virtual void accept( DIALOGresource* parent, IVisitor* visitor );
};


// RTEXT text, id, x, y, width, height [, style [, extended-style]]
struct RTEXTcontrol : IControl
{
    std::string m_text;
    std::string m_id;
    std::string m_x;
    std::string m_y;
    std::string m_width;
    std::string m_height;
    std::string m_style;

    virtual const char* my_type();
    virtual void accept( DIALOGresource* parent, IVisitor* visitor );
};
