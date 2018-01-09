#include "StdAfx.h"
#include "MenuHelper.h"
#include "Resources.h"
#include "Statements.h"


void MenuHelper::visited( MENUresource* menu )
{
    m_parent_map[menu] = NULL;
}


void MenuHelper::visited( MENUresource* parent, POPUPresource* popup )
{
    m_parent_map[popup] = parent;
    m_children_map[parent].push_back(popup);
}


void MenuHelper::visited( MENUresource* parent, MENUITEMstatement* menuitem )
{
    m_children_map[parent].push_back(menuitem);
}


void MenuHelper::visited( POPUPresource* parent, POPUPresource* popup )
{
    m_parent_map[popup] = parent;
    m_children_map[parent].push_back(popup);
}


void MenuHelper::visited( POPUPresource* parent, MENUITEMstatement* menuitem )
{
    m_children_map[parent].push_back(menuitem);
}


MENUresource* MenuHelper::get_menu( POPUPresource* parent, POPUPresource* popup )
{
    IResource* root = parent;

    while ( m_parent_map[ root ] != NULL )
    {
        root = m_parent_map[ root ];
    }

    MENUresource* menu = dynamic_cast<MENUresource*>(root);
    assert( menu != NULL );

    return menu;
}


MENUresource* MenuHelper::get_menu( POPUPresource* parent, MENUITEMstatement* menuitem )
{
    return get_menu( parent, static_cast<POPUPresource*>(NULL) );
}


std::string MenuHelper::get_nested_index( POPUPresource* parent, POPUPresource* popup )
{
    std::vector<size_t> nested_index;

    nested_index.push_back( m_children_map[parent].size() );

    IResource* root = parent;

    while ( m_parent_map[ root ] != NULL )
    {
        root = m_parent_map[ root ];
        assert( 0 < m_children_map[root].size() );
        nested_index.push_back( m_children_map[root].size() - 1 );
    }

    std::stringstream nested_index_strm;

    for ( size_t i = nested_index.size(); 0 < i; --i )
    {
        nested_index_strm << nested_index[i - 1];

        if ( 1 < i )
        {
            nested_index_strm << ",";
        }
    }

    return nested_index_strm.str();
}


std::string MenuHelper::get_nested_index( MENUresource* parent, POPUPresource* popup )
{
    return boost::lexical_cast<std::string>( m_children_map[parent].size() );
}
