#pragma once

FORWARD_DECLARE_7(DIALOGresource, DIALOGEXresource, MENUresource, POPUPresource, STRINGresource, STRINGTABLEresource, TOOLBARresource);
FORWARD_DECLARE_7(CAPTIONstatement, FONTstatement, MENUstatement, MENUITEMstatement, STYLEstatement, EXSTYLEstatement, BUTTONstatement);
FORWARD_DECLARE_7(CHECKBOXcontrol, COMBOBOXcontrol, CONTROLcontrol, CTEXTcontrol, DEFPUSHBUTTONcontrol, EDITTEXTcontrol, GROUPBOXcontrol);
FORWARD_DECLARE_7(ICONcontrol, LISTBOXcontrol, LTEXTcontrol, PUSHBOXcontrol, PUSHBUTTONcontrol, RADIOBUTTONcontrol, RTEXTcontrol);


struct IVisitor
{
    virtual ~IVisitor() {}

    virtual void visit( DIALOGresource* dialog ) {}

    virtual void visit( DIALOGresource* parent, CAPTIONstatement* caption ) {}
    virtual void visit( DIALOGresource* parent, FONTstatement* font ) {}
    virtual void visit( DIALOGresource* parent, MENUstatement* menu ) {}

    virtual void visit( DIALOGresource* parent, CHECKBOXcontrol* checkbox ) {}
    virtual void visit( DIALOGresource* parent, COMBOBOXcontrol* combobox ) {}
    virtual void visit( DIALOGresource* parent, CONTROLcontrol* control ) {}
    virtual void visit( DIALOGresource* parent, CTEXTcontrol* ctext ) {}
    virtual void visit( DIALOGresource* parent, DEFPUSHBUTTONcontrol* defpushbutton ) {}
    virtual void visit( DIALOGresource* parent, EDITTEXTcontrol* edittext ) {}
    virtual void visit( DIALOGresource* parent, GROUPBOXcontrol* groupbox ) {}
    virtual void visit( DIALOGresource* parent, ICONcontrol* icon ) {}
    virtual void visit( DIALOGresource* parent, LISTBOXcontrol* listbox ) {}
    virtual void visit( DIALOGresource* parent, LTEXTcontrol* ltext ) {}
    virtual void visit( DIALOGresource* parent, PUSHBOXcontrol* pushbox ) {}
    virtual void visit( DIALOGresource* parent, PUSHBUTTONcontrol* pushbutton ) {}
    virtual void visit( DIALOGresource* parent, RADIOBUTTONcontrol* radiobutton ) {}
    virtual void visit( DIALOGresource* parent, RTEXTcontrol* rtext ) {}

    virtual void visit( MENUresource* menu ) {}
    virtual void visit( MENUresource* parent, POPUPresource* popup ) {}
    virtual void visit( MENUresource* parent, MENUITEMstatement* menuitem ) {}
    virtual void visit( POPUPresource* parent, POPUPresource* menuitem ) {}
    virtual void visit( POPUPresource* parent, MENUITEMstatement* menuitem ) {}

    virtual void visit( STRINGTABLEresource* string_table) {}
    virtual void visit( STRINGTABLEresource* parent, STRINGresource* string_ ) {}

    virtual void visit( TOOLBARresource* toolbar ) {}
    virtual void visit( TOOLBARresource* parent, BUTTONstatement* button ) {}
};
