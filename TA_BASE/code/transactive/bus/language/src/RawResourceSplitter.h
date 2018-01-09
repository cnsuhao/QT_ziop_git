#pragma once


namespace TA_Base_Bus
{

    class RawResourceSplitter
    {
    public:

        RawResourceSplitter( const std::string& rawResource );

        UINT getUnsignedInteger( UINT index );
        int getInteger( UINT index );
        CString getCString(UINT index);
        const std::string& getString(UINT index);
        size_t size() { return m_columns.size(); }

        static void removeOutermostDoubleQuotationMarks( std::string& str );
        static void escapeCharacter( std::string& str );

    private:

        std::vector<std::string> m_columns;
    };

}
