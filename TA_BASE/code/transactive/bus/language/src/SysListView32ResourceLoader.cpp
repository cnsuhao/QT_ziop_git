#include "StdAfx.h"
#include "bus/language/src/SysListView32ResourceLoader.h"
#include "bus/language/src/SysListView32Resource.h"
#include "bus/language/src/CharacterCodeConverter.h"


namespace TA_Base_Bus
{

    SysListView32ResourceLoader::SysListView32ResourceLoader( const std::string& rawResource )
        : AbstractDialogItemResourceLoader( rawResource )
    {
    }


    IResourcePtr SysListView32ResourceLoader::createResource()
    {
        UINT id = m_splitter.getUnsignedInteger( SYSLISTVIEW32_ID );
        const std::string& columnCaptionList = m_splitter.getString( COLUMN_CAPTION_LIST );

        std::map<UINT, CString> columnNameMap;

        static const boost::regex columnCaptionListRegex
        (
            "(?x)"
            " ^ C [[:digit:]]+ [ ]* \" [^\"]+ \" "            // first column
            " ( ,[ ]* C [[:digit:]]+ [ ]* \" [^\"]+ \" )* $ " // the rest columns
        );

        if ( boost::regex_match( columnCaptionList, columnCaptionListRegex ) )
        {
            static const boost::regex columnCaptionRegex
            (
                "(?x)"
                " C ( [[:digit:]]+ ) [ ]* \" ( [^\"]+ ) \" " // $1 column index, $2 column caption
            );

            boost::sregex_iterator it( columnCaptionList.begin(), columnCaptionList.end(), columnCaptionRegex );
            boost::sregex_iterator end;

            for ( ; it != end; ++it )
            {
                std::string columnIndex = it->str(1);
                std::string columnCaption = it->str(2);

                columnNameMap[ boost::lexical_cast<UINT>(columnIndex) ] = CharacterCodeConverter::convertUTF8toCString( columnCaption.c_str() );
            }
        }

        SysListView32ResourcePtr sysListView32Resource( new SysListView32Resource( id, columnNameMap ) );

        addOptionalPropertyToResource( sysListView32Resource );

        return sysListView32Resource;
    }

}
