#include "StdAfx.h"
#include "MENUresourceParser.h"
#include "POPUPresourceParser.h"
#include "Resources.h"
#include "RegexFactory.h"


IResourcePtr MENUresourceParser::parse()
{
    const char* menu_id_regex_str =
        "(?x)"
        "\\A \\s* (\\w+) \\s+ MENU \\s+ .*? "                   // $1 menuID
        "BEGIN"
        "(.*?)"                                                 // $2 item-definitions
        "END \\s* \\z"
        "";

    boost::smatch m;
    const boost::regex& menu_id_regex = RegexFactory::instance().create_regex( menu_id_regex_str );

    if ( boost::regex_search( m_string, m, menu_id_regex ) )
    {
        MENUresource* menu_resource = new MENUresource;

        menu_resource->m_munuID = m.str(1);

        const std::string& item_definitions = m.str(2);

        // the item_definitions looks like a popup resource
        std::stringstream fake_popup_resource_strm;
        fake_popup_resource_strm<< "POPUP \"\" BEGIN " << item_definitions << " END";

        POPUPresourceParser popup_resource_parser( fake_popup_resource_strm.str() );
        IResourcePtr resource = popup_resource_parser.parse();
        POPUPresourcePtr popup_resource = boost::dynamic_pointer_cast<POPUPresource>( resource );
        assert( popup_resource );

        menu_resource->m_menu_items = popup_resource->m_menu_items;

        return IResourcePtr( menu_resource );
    }

    std::cout << "parse failed:\n" << m_string << std::endl;
    return IResourcePtr();
};
