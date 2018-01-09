#include "StdAfx.h"
#include "bus/language/src/ResourceFileLoader.h"
#include "bus/language/src/TALanguage.h"
#include "bus/language/src/StringResourceLoader.h"
#include "bus/language/src/DialogResourceLoader.h"
#include "bus/language/src/TextResourceLoader.h"
#include "bus/language/src/GroupBoxResourceLoader.h"
#include "bus/language/src/PushButtonResourceLoader.h"
#include "bus/language/src/PushBoxResourceLoader.h"
#include "bus/language/src/SysListView32ResourceLoader.h"
#include "bus/language/src/CheckBoxResourceLoader.h"
#include "bus/language/src/RadioButtonResourceLoader.h"
#include "bus/language/src/ToolBarResourceLoader.h"
#include "bus/language/src/MenuResourceLoader.h"
#include "bus/language/src/PopupResourceLoader.h"
#include "bus/language/src/MenuItemResourceLoader.h"


namespace TA_Base_Bus
{

    ResourceFileLoader& ResourceFileLoader::instance()
    {
        static ResourceFileLoader* singleton = new ResourceFileLoader;
        return *singleton;
    }


    ResourceContainerPtr ResourceFileLoader::loadResource( const std::string& resourceFilePath, UINT applicationID )
    {
        std::ifstream ifs( resourceFilePath.c_str() );

        if ( !ifs )
        {
            return ResourceContainerPtr();
        }

        // check is UTF-8 encoding
        const unsigned char BOM[3] = { 0xEF, 0xBB, 0xBF };
        unsigned char head[3] = { 0 };
        ifs.read( (char*)head, 3 );

        if ( head[0] != BOM[0] || head[1] != BOM[1] || head[2] != BOM[2] )
        {
            return ResourceContainerPtr();
        }

        ResourceContainerPtr resourceContainer;

        static const boost::regex e
        (
            "(?x)"
            " ^ [[:space:]]* "    // leading space
            " ( [[:digit:]]+ ) "  // $1, APPLICATION_ID
            " [\\t]+ "            // the tab separator
            " ( [[:digit:]]+ ) "  // $2, RESOURCE_TYPE_ID
            " [\\t]+  "           // other columns
        );

        for ( std::string line; std::getline( ifs, line ); )
        {
            boost::smatch m;

            if ( boost::regex_search( line, m, e ) )
            {
                UINT appID = boost::lexical_cast<UINT>( m.str(1) );

                // TODO: performance ( lines are ordered by app id )
                if ( appID != APPLICATION_ID_ALL && appID != applicationID )
                {
                    continue;
                }

                UINT resourceTypeId = boost::lexical_cast<UINT>( m.str(2) );
                IResourceLoaderPtr resourceLoader = createResourceLoader( resourceTypeId, line );

                if ( resourceLoader )
                {
                    IResourcePtr resource = resourceLoader->createResource();

                    if ( resource )
                    {
                        if ( !resourceContainer )
                        {
                            resourceContainer = ResourceContainerPtr( new ResourceContainer );
                        }

                        resourceLoader->addResourceToContainer( resource, resourceContainer );
                    }
                }
            }
        }

        return resourceContainer;
    }


    ResourceFileLoader::ResourceFileLoader()
    {
        m_resourceLoaderCreatorMap.insert( ResourceLoaderCreatorMap::value_type( StringResourceLoader::getResourceTypeID(), StringResourceLoader::create ) ) ;
        m_resourceLoaderCreatorMap.insert( ResourceLoaderCreatorMap::value_type( DialogResourceLoader::getResourceTypeID(), DialogResourceLoader::create ) ) ;
        m_resourceLoaderCreatorMap.insert( ResourceLoaderCreatorMap::value_type( TextResourceLoader::getResourceTypeID(), TextResourceLoader::create ) ) ;
        m_resourceLoaderCreatorMap.insert( ResourceLoaderCreatorMap::value_type( GroupBoxResourceLoader::getResourceTypeID(), GroupBoxResourceLoader::create ) ) ;
        m_resourceLoaderCreatorMap.insert( ResourceLoaderCreatorMap::value_type( PushButtonResourceLoader::getResourceTypeID(), PushButtonResourceLoader::create ) ) ;
        m_resourceLoaderCreatorMap.insert( ResourceLoaderCreatorMap::value_type( PushBoxResourceLoader::getResourceTypeID(), PushBoxResourceLoader::create ) ) ;
        m_resourceLoaderCreatorMap.insert( ResourceLoaderCreatorMap::value_type( SysListView32ResourceLoader::getResourceTypeID(), SysListView32ResourceLoader::create ) ) ;
        m_resourceLoaderCreatorMap.insert( ResourceLoaderCreatorMap::value_type( CheckBoxResourceLoader::getResourceTypeID(), CheckBoxResourceLoader::create ) ) ;
        m_resourceLoaderCreatorMap.insert( ResourceLoaderCreatorMap::value_type( RadioButtonResourceLoader::getResourceTypeID(), RadioButtonResourceLoader::create ) ) ;
        m_resourceLoaderCreatorMap.insert( ResourceLoaderCreatorMap::value_type( ToolBarResourceLoader::getResourceTypeID(), ToolBarResourceLoader::create ) ) ;
        m_resourceLoaderCreatorMap.insert( ResourceLoaderCreatorMap::value_type( MenuResourceLoader::getResourceTypeID(), MenuResourceLoader::create ) ) ;
        m_resourceLoaderCreatorMap.insert( ResourceLoaderCreatorMap::value_type( PopupResourceLoader::getResourceTypeID(), PopupResourceLoader::create ) ) ;
        m_resourceLoaderCreatorMap.insert( ResourceLoaderCreatorMap::value_type( MenuItemResourceLoader::getResourceTypeID(), MenuItemResourceLoader::create ) ) ;
    }


    IResourceLoaderPtr ResourceFileLoader::createResourceLoader( UINT resourceTypeId, const std::string& line )
    {
        ResourceLoaderCreatorMap::iterator it = m_resourceLoaderCreatorMap.find( resourceTypeId );

        if ( it != m_resourceLoaderCreatorMap.end() )
        {
            return it->second( line );
        }

        return IResourceLoaderPtr();
    }

}
