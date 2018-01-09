#pragma once


struct ID
{
    ID( const std::string& name, const std::string& value )
        : m_name( name ),
          m_value( value )
    {
    }

    ID()
        : m_name( "<INVALID ID>" ),
          m_value( "<INVALID VALUE>" )
    {
    }

    ID( const ID& rhs )
    {
        m_name = rhs.m_name;
        m_value = rhs.m_value;
    }

    ID& operator = ( const ID& rhs )
    {
        m_name = rhs.m_name;
        m_value = rhs.m_value;
        return *this;
    }

    bool operator == ( const ID& rhs ) const
    {
        return ( m_value == rhs.m_value ) || ( m_name == rhs.m_name );
    }

    bool operator< ( const ID& rhs ) const
    {
        return ( m_value < rhs.m_value ) || ( m_value == rhs.m_value && m_name < rhs.m_name);
    }

    std::string m_value;
    std::string m_name;
};


struct IDMap
{
public:

    IDMap();

    void load_id_definition_from_file( const std::string& rc_h );
    ID& look_up( const std::string& );

private:

    static bool load_windows_definition();

public:

    std::map<std::string, ID> m_map;
    static std::map<std::string, ID> m_common_map;
    static boost::mutex m_mutex;
};
