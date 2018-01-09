#pragma once
#include "IVisitor.h"
#include "IDMap.h"
#include "MenuHelper.h"


class LanguageFileGenerator : public IVisitor
{
public:

    LanguageFileGenerator( const std::string& application_id, const std::string& rc_h, const std::string& language );

    //////////////////////////////////////////////////////////////////////////
    virtual void visit( DIALOGresource* resource );

    virtual void visit( DIALOGresource* parent, CAPTIONstatement* caption ) {}
    virtual void visit( DIALOGresource* parent, FONTstatement* font ) {}
    virtual void visit( DIALOGresource* parent, CHECKBOXcontrol* checkbox );
    virtual void visit( DIALOGresource* parent, COMBOBOXcontrol* combobox ) {}
    virtual void visit( DIALOGresource* parent, CONTROLcontrol* control );
    virtual void visit( DIALOGresource* parent, CTEXTcontrol* ctext );
    virtual void visit( DIALOGresource* parent, DEFPUSHBUTTONcontrol* defpushbutton );
    virtual void visit( DIALOGresource* parent, GROUPBOXcontrol* groupbox );
    virtual void visit( DIALOGresource* parent, LISTBOXcontrol* listbox ) {}
    virtual void visit( DIALOGresource* parent, LTEXTcontrol* ltext );
    virtual void visit( DIALOGresource* parent, PUSHBOXcontrol* pushbox );
    virtual void visit( DIALOGresource* parent, PUSHBUTTONcontrol* pushbutton );
    virtual void visit( DIALOGresource* parent, RADIOBUTTONcontrol* radiobutton );
    virtual void visit( DIALOGresource* parent, RTEXTcontrol* rtext );

    virtual void visit( MENUresource* menu );
    virtual void visit( MENUresource* parent, POPUPresource* popup );
    virtual void visit( MENUresource* parent, MENUITEMstatement* menuitem );
    virtual void visit( POPUPresource* parent, POPUPresource* popup );
    virtual void visit( POPUPresource* parent, MENUITEMstatement* menuitem );

    virtual void visit( STRINGTABLEresource* parent, STRINGresource* string_ );

    virtual void visit( TOOLBARresource* toolbar );
    virtual void visit( TOOLBARresource* parent, BUTTONstatement* button ) {}
    //////////////////////////////////////////////////////////////////////////

private:

    void output_line( const std::string& s1, const std::string& s2, const std::string& s3, const std::string& s4, const std::string& s5 = STR_NULL, const std::string& s6 = STR_NULL, const std::string& s7 = STR_NULL );
    static void output_align_to_nth_tab( std::ostream& os, const std::string& s, size_t nth_tab = 0 );

private:

    std::string                 m_application_id;
    std::ofstream               m_language;
    std::vector<size_t>         m_colmn_width_in_tabs;
    IDMap                       m_id_map;
    MenuHelper                  m_menu_helper;
    static const std::string    STR_NULL;
};
