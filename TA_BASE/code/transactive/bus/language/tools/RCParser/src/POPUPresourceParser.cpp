#include "StdAfx.h"
#include "POPUPresourceParser.h"
#include "MENUITEMstatementParser.h"
#include "Resources.h"
#include "Statements.h"
#include "RegexFactory.h"


IResourcePtr POPUPresourceParser::parse()
{
    const char* popup_resource_regex_str =
        "(?x)"
        "\\A \\s* POPUP \\s+ (\"[^\"]*\") \\s+"     // $1 text
        "BEGIN"
        "(.*?)"                                     // $2 item-definitions
        "END \\s* \\z"
        "";

    boost::smatch m;
    const boost::regex& popup_resource_regex = RegexFactory::instance().create_regex( popup_resource_regex_str );

    if ( boost::regex_search( m_string, m, popup_resource_regex ) )
    {
        POPUPresource* popup_resource = new POPUPresource;

        popup_resource->m_text = m.str(1);

        const std::string& item_definitions = m.str(2);

        // MENUITEM text, result, [optionlist]
        // MENUITEM SEPARATOR
        #define option_list "(?:,\\s*(?:CHECKED|GRAYED|HELP|INACTIVE|MENUBARBREAK|MENUBREAK))*"
        #define menuitem_statement "\\s* MENUITEM \\s+ \"[^\"]*\", \\s* \\w+ \\s* " option_list " | \\s* MENUITEM \\s+ SEPARATOR \\s*"
        //#define menuitem_statement "\\s* MENUITEM \\s+ \"[^\"]*\", \\s* \\w+ \\s* | \\s* MENUITEM \\s+ SEPARATOR \\s*"
        const char* menu_item_regex_str =
            "(?x)"                                                 //recur
            " ( \\s* POPUP .*? BEGIN \\s* ( " menuitem_statement " | (?1) )+ \\s* END \\s* )"   // $1 POPUP
            " | "
            " ( " menuitem_statement " ) "                                                      // $4 MENUITEM
            "";

        const boost::regex& menu_item_regex = RegexFactory::instance().create_regex( menu_item_regex_str );
        boost::sregex_iterator it( item_definitions.begin(), item_definitions.end(), menu_item_regex );
        boost::sregex_iterator end;

        for ( ; it != end; ++it )
        {
            const boost::regex& is_popup_regex = RegexFactory::instance().create_regex( "(?x) \\A \\s* POPUP \\s*" );

            const std::string& menu_item_string = it->str();

            // recursive
            if ( boost::regex_search( menu_item_string, is_popup_regex ) )
            {
                POPUPresourceParser popup_resource_parser( menu_item_string );
                MENUITEM menuitem = boost::dynamic_pointer_cast<POPUPresource>( popup_resource_parser.parse() );

                popup_resource->m_menu_items.push_back( menuitem );
            }
            else
            {
                MENUITEMstatementParser menuitem_statement_parser( menu_item_string );
                MENUITEM menuitem = boost::dynamic_pointer_cast<MENUITEMstatement>( menuitem_statement_parser.parse() );

                popup_resource->m_menu_items.push_back( menuitem );
            }
        }

        return IResourcePtr( popup_resource );
    }

    std::cout << "parse failed:\n" << m_string << std::endl;
    return IResourcePtr();
};
