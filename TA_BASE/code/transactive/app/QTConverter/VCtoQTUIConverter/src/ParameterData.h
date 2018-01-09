#pragma once
#include <string>

struct Geometry
{
    std::string x;
    std::string y;
    std::string width;
    std::string height;
};

enum eControlType
{
    DIALOG = 0,
    DIALOGEX,
    STYLE,
    FONT,
    DEFPUSHBUTTON, 
    PUSHBUTTON,
    EDITTEXT, 
    COMBOBOX, 
    LISTBOX, 
    GROUPBOX, 
    LTEXT, 
    SCROLLBAR,
    CONTROL,
};

const char *const RPARAM_MENU = "ConvertMenu";
