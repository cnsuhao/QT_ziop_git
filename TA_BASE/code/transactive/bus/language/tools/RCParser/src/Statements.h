#pragma once
#include "IStatement.h"

FORWARD_DECLARE_5(DIALOGresource, DIALOGEXresource, MENUresource, POPUPresource, TOOLBARresource);


// CAPTION "captiontext"
struct CAPTIONstatement : IStatement
{
    std::string m_captiontext;

    virtual const char* my_type();
    virtual void accept( DIALOGresource* dialog, IVisitor* visitor );
};


// FONT pointsize, "typeface", weight, italic, charset
struct FONTstatement : IStatement
{
    std::string m_pointsize;
    std::string m_typeface;
    std::string m_weight;
    std::string m_italic;
    std::string m_charset;

    virtual const char* my_type();
    virtual void accept( DIALOGresource* dialog, IVisitor* visitor );
};


// MENU menuname
struct MENUstatement : IStatement
{
    std::string m_menuname;

    virtual const char* my_type();
    virtual void accept( DIALOGresource* dialog, IVisitor* visitor );
};


// MENUITEM text, result, [optionlist]
// MENUITEM SEPARATOR
struct MENUITEMstatement : IStatement
{
    std::string m_text;
    std::string m_result;
    bool m_is_separator;

    virtual const char* my_type();
    virtual void accept( MENUresource* menu, IVisitor* visitor );
    virtual void accept( POPUPresource* popup, IVisitor* visitor );
};


// STYLE style
struct STYLEstatement : IStatement
{
    std::string m_style;

    virtual const char* my_type();
};


// EXSTYLE extended-style
struct EXSTYLEstatement : IStatement
{
    std::string m_extended_style;

    virtual const char* my_type();
};


// Note: there is no document for TOOLBAR
// BUTTON id
struct BUTTONstatement : IStatement
{
    std::string m_id;

    virtual const char* my_type();
    virtual void accept( TOOLBARresource* toolbar, IVisitor* visitor );
};
