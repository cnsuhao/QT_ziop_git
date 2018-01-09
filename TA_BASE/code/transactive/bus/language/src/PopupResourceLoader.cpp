#include "StdAfx.h"
#include "bus/language/src/PopupResourceLoader.h"
#include "bus/language/src/PopupResource.h"
#include "bus/language/src/MenuResource.h"
#include "bus/language/src/TALanguage.h"
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>


namespace TA_Base_Bus
{

    PopupResourceLoader::PopupResourceLoader( const std::string& rawResource )
        : m_splitter( rawResource )
    {
    }


    IResourcePtr PopupResourceLoader::createResource()
    {
        CString caption = m_splitter.getCString( POPUP_CAPTION );
        const std::string& popupNestedIndexString = m_splitter.getString( POPUP_NESTED_INDEX );

        std::vector<UINT> popupNestedIndex;

        typedef boost::tokenizer< boost::char_separator<char> > tokenizer;
        boost::char_separator<char> sep( ", .:;-" );
        tokenizer tokens( popupNestedIndexString, sep );

        for ( tokenizer::iterator it = tokens.begin(); it != tokens.end(); ++it )
        {
            popupNestedIndex.push_back( boost::lexical_cast<UINT>(*it) );
        }

        return PopupResourcePtr( new PopupResource(caption, popupNestedIndex) );
    }


    void PopupResourceLoader::addResourceToContainer( IResourcePtr popupResource, IResourceContainerPtr containder )
    {
        UINT menuID = m_splitter.getUnsignedInteger( MENU_ID );
        IResourcePtr resource = containder->getResource( MenuResourceKey(menuID) );
        PopupResourcePtr popup = boost::dynamic_pointer_cast<PopupResource>( popupResource );
        MenuResourcePtr menu = boost::dynamic_pointer_cast<MenuResource>( resource );

        if ( menu && popup )
        {
            menu->addPopupResource( popup );
        }
    }

}
