#include "StdAfx.h"
#include "bus/language/src/ToolBarResourceLoader.h"
#include "bus/language/src/ToolBarResource.h"
#include "bus/language/src/CharacterCodeConverter.h"
#include "bus/language/src/TALanguage.h"


namespace TA_Base_Bus
{

    ToolBarResourceLoader::ToolBarResourceLoader( const std::string& rawResource )
        : m_splitter( rawResource )
    {
    }


    IResourcePtr ToolBarResourceLoader::createResource()
    {
        std::map<int, CString> buttonCaptionMap;

        const std::string& buttonCaptionList = m_splitter.getString(TOOLBAR_BUTTON_CAPTION_LIST);

        static const boost::regex buttonCaptionListRegex
        (
            "(?x)"
            " ^ B [[:digit:]]+ [ ]* \" [^\"]+ \" "            // first button
            " ( ,[ ]* B [[:digit:]]+ [ ]* \" [^\"]+ \" )* $ " // the rest buttons
        );

        if ( boost::regex_match( buttonCaptionList, buttonCaptionListRegex ) )
        {
            static const boost::regex buttonCaptionRegex
            (
                "(?x)"
                " B ( [[:digit:]]+ ) [ ]* \" ( [^\"]+ ) \" " // $1 button index, $2 button caption
            );

            boost::sregex_iterator it( buttonCaptionList.begin(), buttonCaptionList.end(), buttonCaptionRegex );
            boost::sregex_iterator end;

            for ( ; it != end; ++it )
            {
                std::string buttonIndex = it->str(1);
                std::string buttonCaption = it->str(2);

                buttonCaptionMap[ boost::lexical_cast<int>(buttonIndex) ] = CharacterCodeConverter::convertUTF8toCString( buttonCaption.c_str() );
            }
        }

        return IResourcePtr( new ToolBarResource(buttonCaptionMap) );
    }


    void ToolBarResourceLoader::addResourceToContainer( IResourcePtr resource, IResourceContainerPtr containder )
    {
        UINT id = m_splitter.getUnsignedInteger( TOOLBAR_ID );

        containder->addResource( ToolBarResourceKey(id), resource );
    }

}
