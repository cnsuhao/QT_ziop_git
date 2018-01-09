#pragma once
#include "bus/language/src/IResource.h"
#include "bus/language/src/TALanguage.h"


namespace TA_Base_Bus
{

    typedef std::pair<UINT, UINT> ResourceKey;


    template<UINT resourceTypeID> struct ResourceKeyTemplate
    {
        ResourceKeyTemplate(UINT id)
            : m_key( id, resourceTypeID )
        {
        }

        operator const ResourceKey& ()
        {
            return m_key;
        }

    private:

        ResourceKey m_key;
    };

    typedef ResourceKeyTemplate<RESOURCE_TYPE_ID_String> StringResourceKey;
    typedef ResourceKeyTemplate<RESOURCE_TYPE_ID_Dialog> DialogResourceKey;
    typedef ResourceKeyTemplate<RESOURCE_TYPE_ID_ToolBar> ToolBarResourceKey;
    typedef ResourceKeyTemplate<RESOURCE_TYPE_ID_Menu> MenuResourceKey;


    class IResourceContainer
    {
    public:

        virtual ~IResourceContainer() {}

        virtual void addResource( const ResourceKey& id, IResourcePtr resource ) = 0;
        virtual IResourcePtr getResource( const ResourceKey& id ) = 0;
        virtual bool isExist( const ResourceKey& id ) = 0;
        virtual bool isExist( IResourcePtr resource ) = 0;
    };

    typedef boost::shared_ptr<IResourceContainer> IResourceContainerPtr;

}
