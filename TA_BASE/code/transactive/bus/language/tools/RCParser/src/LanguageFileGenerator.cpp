#include "StdAfx.h"
#include "LanguageFileGenerator.h"
#include "Resources.h"
#include "Statements.h"
#include "Controls.h"


const std::string RESOURCE_TYPE_ID_String              = "1";
const std::string RESOURCE_TYPE_ID_Dialog              = "1000";
const std::string RESOURCE_TYPE_ID_Text                = "1010";
const std::string RESOURCE_TYPE_ID_Groupbox            = "1020";
const std::string RESOURCE_TYPE_ID_PushButton          = "1030";
const std::string RESOURCE_TYPE_ID_PushBox             = "1040";
const std::string RESOURCE_TYPE_ID_SysListView32       = "1050";
const std::string RESOURCE_TYPE_ID_CheckBox            = "1060";
const std::string RESOURCE_TYPE_ID_RadioButton         = "1070";
const std::string RESOURCE_TYPE_ID_ToolBar             = "1200";
const std::string RESOURCE_TYPE_ID_Menu                = "1510";
const std::string RESOURCE_TYPE_ID_Popup               = "1520";
const std::string RESOURCE_TYPE_ID_MenuItem            = "1530";

const std::string LanguageFileGenerator::STR_NULL      = "<STRING_NULL>";


LanguageFileGenerator::LanguageFileGenerator( const std::string& application_id, const std::string& rc_h, const std::string& language )
    : m_application_id( application_id )
{
    m_language.open( language.c_str() );

    if ( !m_language )
    {
        std::cout << "open failed: " << language << std::endl;
        return;
    }

    m_id_map.load_id_definition_from_file( rc_h );

    m_colmn_width_in_tabs.push_back( 2 );  // width of colomn 1
    m_colmn_width_in_tabs.push_back( 2 );
    m_colmn_width_in_tabs.push_back( 2 );
    m_colmn_width_in_tabs.push_back( 6 );  // width of colomn 4
    m_colmn_width_in_tabs.push_back( 10 );
    m_colmn_width_in_tabs.push_back( 10 );
    m_colmn_width_in_tabs.push_back( 10 ); // width of colomn 7
}


void LanguageFileGenerator::visit( DIALOGresource* dialog )
{
    ID& id = m_id_map.look_up( dialog->m_nameID );
    const char* captiontext = "\"\"";

    for ( size_t i = 0; i < dialog->m_optional_statements.size(); ++i )
    {
        if ( "CAPTION statement" == dialog->m_optional_statements[i]->my_type() )
        {
            CAPTIONstatement* caption_statement = dynamic_cast<CAPTIONstatement*>( dialog->m_optional_statements[i].get() );
            captiontext = caption_statement->m_captiontext.c_str();
            break;
        }
    }

    // APPLICATION_ID, RESOURCE_TYPE_ID, DIALOG_ID, PARENT_NAME_ID, DIALOG_CAPTION, OPTIONAL_PROPERTY_LIST
    output_line( m_application_id, RESOURCE_TYPE_ID_Dialog, id.m_value, id.m_name, captiontext );
}


void LanguageFileGenerator::visit( DIALOGresource* parent, CHECKBOXcontrol* checkbox )
{
    ID& id = m_id_map.look_up(checkbox->m_id);
    ID& parent_id = m_id_map.look_up( parent->m_nameID );
    // APPLICATION_ID, RESOURCE_TYPE_ID, CHECKBOX_ID, PARENT_ID, CHECKBOX_NAME_ID, CHECKBOX_CAPTION, OPTIONAL_PROPERTY_LIST
    output_line( m_application_id, RESOURCE_TYPE_ID_CheckBox, id.m_value, parent_id.m_value, id.m_name, checkbox->m_text );
}


void LanguageFileGenerator::visit( DIALOGresource* parent, CONTROLcontrol* control )
{
    ID& id = m_id_map.look_up(control->m_id);
    ID& parent_id = m_id_map.look_up( parent->m_nameID );

    if ( "Button" == control->m_class )
    {
        // APPLICATION_ID, RESOURCE_TYPE_ID, PUSHBUTTON_ID, PARENT_ID, PUSHBUTTON_NAME_ID, PUSHBUTTON_CAPTION, OPTIONAL_PROPERTY_LIST
        output_line( m_application_id, RESOURCE_TYPE_ID_PushButton, id.m_value, parent_id.m_value, id.m_name, control->m_text );
    }
    else if ( "SysListView32" == control->m_class  )
    {
        // APPLICATION_ID, RESOURCE_TYPE_ID, SYSLISTVIEW32_ID, PARENT_ID, SYSLISTVIEW32_NAME_ID, COLUMN_CAPTION_LIST/*C0 "Column 1", C1 "Column 2", ...*/, OPTIONAL_PROPERTY_LIST
        const char* COLUMN_CAPTION_LIST = "C0 \"TODO\", C1 \"TODO\", C2 \"TODO\", C3 \"TODO\"";
        output_line( m_application_id, RESOURCE_TYPE_ID_SysListView32, id.m_value, parent_id.m_value, id.m_name, COLUMN_CAPTION_LIST );
    }
}


void LanguageFileGenerator::visit( DIALOGresource* parent, CTEXTcontrol* ctext )
{
    ID& id = m_id_map.look_up(ctext->m_id);
    ID& parent_id = m_id_map.look_up( parent->m_nameID );
    // APPLICATION_ID, RESOURCE_TYPE_ID, TEXT_ID, PARENT_ID, TEXT_NAME_ID, TEXT_CAPTION, OPTIONAL_PROPERTY_LIST
    output_line( m_application_id, RESOURCE_TYPE_ID_Text, id.m_value, parent_id.m_value, id.m_name, ctext->m_text );
}


void LanguageFileGenerator::visit( DIALOGresource* parent, DEFPUSHBUTTONcontrol* defpushbutton )
{
    ID& id = m_id_map.look_up(defpushbutton->m_id);
    ID& parent_id = m_id_map.look_up( parent->m_nameID );
    // APPLICATION_ID, RESOURCE_TYPE_ID, PUSHBUTTON_ID, PARENT_ID, PUSHBUTTON_NAME_ID, PUSHBUTTON_CAPTION, OPTIONAL_PROPERTY_LIST
    output_line( m_application_id, RESOURCE_TYPE_ID_PushButton, id.m_value, parent_id.m_value, id.m_name, defpushbutton->m_text );
}


void LanguageFileGenerator::visit( DIALOGresource* parent, GROUPBOXcontrol* groupbox )
{
    ID& id = m_id_map.look_up(groupbox->m_id);
    ID& parent_id = m_id_map.look_up( parent->m_nameID );
    // APPLICATION_ID, RESOURCE_TYPE_ID, GROUPBOX_ID, PARENT_ID, GROUPBOX_NAME_ID, GROUPBOX_CAPTION, OPTIONAL_PROPERTY_LIST
    output_line( m_application_id, RESOURCE_TYPE_ID_Groupbox, id.m_value, parent_id.m_value, id.m_name, groupbox->m_text );
}


void LanguageFileGenerator::visit( DIALOGresource* parent, LTEXTcontrol* ltext )
{
    ID& id = m_id_map.look_up(ltext->m_id);
    ID& parent_id = m_id_map.look_up( parent->m_nameID );
    // APPLICATION_ID, RESOURCE_TYPE_ID, TEXT_ID, PARENT_ID, TEXT_NAME_ID, TEXT_CAPTION, OPTIONAL_PROPERTY_LIST
    output_line( m_application_id, RESOURCE_TYPE_ID_Text, id.m_value, parent_id.m_value, id.m_name, ltext->m_text );
}


void LanguageFileGenerator::visit( DIALOGresource* parent, PUSHBOXcontrol* pushbox )
{
    ID& id = m_id_map.look_up(pushbox->m_id);
    ID& parent_id = m_id_map.look_up( parent->m_nameID );
    // APPLICATION_ID, RESOURCE_TYPE_ID, PUSHBOX_ID, PARENT_ID, PUSHBOX_NAME_ID, PUSHBOX_CAPTION, OPTIONAL_PROPERTY_LIST
    output_line( m_application_id, RESOURCE_TYPE_ID_PushBox, id.m_value, parent_id.m_value, id.m_name, pushbox->m_text );
}


void LanguageFileGenerator::visit( DIALOGresource* parent, PUSHBUTTONcontrol* pushbutton )
{
    ID& id = m_id_map.look_up(pushbutton->m_id);
    ID& parent_id = m_id_map.look_up( parent->m_nameID );
    // APPLICATION_ID, RESOURCE_TYPE_ID, PUSHBUTTON_ID, PARENT_ID, PUSHBUTTON_NAME_ID, PUSHBUTTON_CAPTION, OPTIONAL_PROPERTY_LIST
    output_line( m_application_id, RESOURCE_TYPE_ID_PushButton, id.m_value, parent_id.m_value, id.m_name, pushbutton->m_text );
}


void LanguageFileGenerator::visit( DIALOGresource* parent, RADIOBUTTONcontrol* radiobutton )
{
    ID& id = m_id_map.look_up(radiobutton->m_id);
    ID& parent_id = m_id_map.look_up( parent->m_nameID );
    // APPLICATION_ID, RESOURCE_TYPE_ID, RADIOBUTTON_ID, PARENT_ID, RADIOBUTTON_NAME_ID, RADIOBUTTON_CAPTION, OPTIONAL_PROPERTY_LIST
    output_line( m_application_id, RESOURCE_TYPE_ID_RadioButton, id.m_value, parent_id.m_value, id.m_name, radiobutton->m_text );
}


void LanguageFileGenerator::visit( DIALOGresource* parent, RTEXTcontrol* rtext )
{
    ID& id = m_id_map.look_up(rtext->m_id);
    ID& parent_id = m_id_map.look_up( parent->m_nameID );
    // APPLICATION_ID, RESOURCE_TYPE_ID, TEXT_ID, PARENT_ID, TEXT_NAME_ID, TEXT_CAPTION, OPTIONAL_PROPERTY_LIST
    output_line( m_application_id, RESOURCE_TYPE_ID_Text, id.m_value, parent_id.m_value, id.m_name, rtext->m_text );
}


void LanguageFileGenerator::visit( MENUresource* menu )
{
    ID& id = m_id_map.look_up(menu->m_munuID);
    // APPLICATION_ID, RESOURCE_TYPE_ID, MENU_ID, MENU_NAME_ID
    output_line( m_application_id, RESOURCE_TYPE_ID_Menu, id.m_value, id.m_name );

    m_menu_helper.visited( menu );
}


void LanguageFileGenerator::visit( MENUresource* parent, POPUPresource* popup )
{
    ID& parent_id = m_id_map.look_up( parent->m_munuID );
    std::string nested_index = m_menu_helper.get_nested_index( parent, popup );
    // APPLICATION_ID, RESOURCE_TYPE_ID, MENU_ID, POPUP_NESTED_INDEX, POPUP_CAPTION
    output_line( m_application_id, RESOURCE_TYPE_ID_Popup, parent_id.m_value, nested_index, popup->m_text );

    m_menu_helper.visited( parent, popup );
}


void LanguageFileGenerator::visit( MENUresource* parent, MENUITEMstatement* menuitem )
{
    if ( false == menuitem->m_is_separator )
    {
        ID& id = m_id_map.look_up(menuitem->m_result);
        ID& parent_id = m_id_map.look_up( parent->m_munuID );
        // APPLICATION_ID, RESOURCE_TYPE_ID, MENUITEM_ID, MENU_ID, MENUITEM_NAME_ID, MENUITEM_CAPTION
        output_line( m_application_id, RESOURCE_TYPE_ID_MenuItem, id.m_value, parent_id.m_value, id.m_name, menuitem->m_text );
    }

    m_menu_helper.visited( parent, menuitem );
}


void LanguageFileGenerator::visit( POPUPresource* parent, POPUPresource* popup )
{
    MENUresource* menu = m_menu_helper.get_menu( parent, popup );
    ID& menu_id = m_id_map.look_up( menu->m_munuID );
    std::string nested_index = m_menu_helper.get_nested_index( parent, popup );
    // APPLICATION_ID, RESOURCE_TYPE_ID, MENU_ID, POPUP_NESTED_INDEX, POPUP_CAPTION
    output_line( m_application_id, RESOURCE_TYPE_ID_Popup, menu_id.m_value, nested_index, popup->m_text );

    m_menu_helper.visited( parent, popup );
}


void LanguageFileGenerator::visit( POPUPresource* parent, MENUITEMstatement* menuitem )
{
    if ( false == menuitem->m_is_separator )
    {
        MENUresource* menu = m_menu_helper.get_menu( parent, menuitem );
        ID& id = m_id_map.look_up( menuitem->m_result );
        ID& menu_id = m_id_map.look_up( menu->m_munuID );
        // APPLICATION_ID, RESOURCE_TYPE_ID, MENUITEM_ID, MENU_ID, MENUITEM_NAME_ID, MENUITEM_CAPTION
        output_line( m_application_id, RESOURCE_TYPE_ID_MenuItem, id.m_value, menu_id.m_value, id.m_name, menuitem->m_text );
    }

    m_menu_helper.visited( parent, menuitem );
}


void LanguageFileGenerator::visit( STRINGTABLEresource* string_table, STRINGresource* string_ )
{
    ID& id = m_id_map.look_up(string_->m_stringID);
    output_line( m_application_id, RESOURCE_TYPE_ID_String, id.m_value, id.m_name, string_->m_string );
}


void LanguageFileGenerator::visit( TOOLBARresource* toolbar )
{
    ID& id = m_id_map.look_up(toolbar->m_toolbarID);
    const char* TOOLBAR_BUTTON_CAPTION_LIST = "B0 \"TODO\", B1 \"TODO\", B2 \"TODO\", B3 \"TODO\"";
    // APPLICATION_ID, RESOURCE_TYPE_ID, TOOLBAR_ID, TOOLBAR_NAME_ID, TOOLBAR_BUTTON_CAPTION_LIST/*B0 "Button 0", B1 "Button 1", ...*/
    output_line( m_application_id, RESOURCE_TYPE_ID_ToolBar, id.m_value, id.m_name, TOOLBAR_BUTTON_CAPTION_LIST );
}


void LanguageFileGenerator::output_line( const std::string& s1, const std::string& s2, const std::string& s3, const std::string& s4, const std::string& s5, const std::string& s6, const std::string& s7 )
{
    std::vector<const std::string*> strings;

    strings.push_back( &s1 );
    strings.push_back( &s2 );
    strings.push_back( &s3 );
    strings.push_back( &s4 );

    if ( &s5 != &STR_NULL )
    {
        strings.push_back( &s5 );

        if ( &s6 != &STR_NULL )
        {
            strings.push_back( &s6 );

            if ( &s7 != &STR_NULL )
            {
                strings.push_back( &s7 );
            }
        }
    }

    for ( size_t i = 0; i < strings.size() - 1; ++i )
    {
        output_align_to_nth_tab( m_language, *strings[i], m_colmn_width_in_tabs[i] );
    }

    m_language << *strings.back() << "\n";
}


void LanguageFileGenerator::output_align_to_nth_tab( std::ostream& os, const std::string& s, size_t nth_tab )
{
    const size_t TAB_LENGTH = 8;

    os << s;

    if ( 0 < nth_tab )
    {
        size_t size = s.size();
        size_t cur_tab = size / TAB_LENGTH;

        if ( cur_tab < nth_tab )
        {
            for ( size_t i = 0; i < nth_tab - cur_tab; ++i )
            {
                os << "\t";
            }
        }
        else
        {
            os << "\t"; // at least one tab
        }
    }
}
