#pragma once

FORWARD_DECLARE_5(IResource, MENUresource, POPUPresource, IStatement, MENUITEMstatement);


struct MenuHelper
{
    void visited( MENUresource* menu );
    void visited( MENUresource* parent, POPUPresource* popup );
    void visited( MENUresource* parent, MENUITEMstatement* menuitem );
    void visited( POPUPresource* parent, POPUPresource* popup );
    void visited( POPUPresource* parent, MENUITEMstatement* menuitem );
    MENUresource* get_menu( POPUPresource* parent, POPUPresource* popup );
    MENUresource* get_menu( POPUPresource* parent, MENUITEMstatement* menuitem );
    std::string get_nested_index( POPUPresource* parent, POPUPresource* popup );
    std::string get_nested_index( MENUresource* parent, POPUPresource* popup );

    struct Menuitem
    {
        Menuitem() : m_resource(NULL), m_statement(NULL) {}
        Menuitem(IResource* resource) : m_resource(resource), m_statement(NULL) {}
        Menuitem(IStatement* statement) : m_resource(NULL), m_statement(statement) {}
        IResource* m_resource;
        IStatement* m_statement;
    };

    std::map<IResource*, IResource*> m_parent_map;
    std::map< IResource*, std::vector<Menuitem> > m_children_map;
};
