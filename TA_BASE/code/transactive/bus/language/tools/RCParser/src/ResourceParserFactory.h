#pragma once

FORWARD_DECLARE_3(IResource, IResourceParser, IVisitor);


// TODO: give it a better name, ResourceParserFactory or ResourceFactoryParser ?
class ResourceParserFactory
{
public:

    ResourceParserFactory( const std::string& s );

    IResourcePtrList parse();

private:

    IResourceParserPtrList create_resource_parsers();
    IResourceParserPtr create_resource_parser( const std::string& resource_type, const std::string& resource_string );

private:

    std::string m_string;
    typedef IResourceParserPtr (*create_resource_parser_fun) ( const std::string& );
    std::map<std::string, create_resource_parser_fun> m_create_map;
};
