#include "CtlConfigParser.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/lexical_cast.hpp>
#include <map>

/**
  * <!-- common control xml config file format -->
  *
  * <root>
  *     <i>
  *         <id> ...... </id>
  *         <caption> ...... </caption>
  *         ...
  *     </i>
  *     ...
  * </root>
  */
static const std::string TAG_root               = "root";
static const std::string TAG_i                  = "i";
static const std::string TAG_id                 = "id";
static const std::string TAG_visiblity          = "visiblity";
static const std::string TAG_caption            = "caption";
static const std::string TAG_action_id          = "action_id";
static const std::string TAG_tooltip            = "tooltip";
static const std::string TAG_help_file          = "help_file";
static const std::string TAG_maximum_length     = "maximum_length";
static const std::string TAG_enable_rtrim       = "enable_rtrim";
static const std::string TAG_style_sheet        = "style_sheet";
static const std::string VALUE_true             = "true";
static const std::string VALUE_yes              = "yes";
static const std::string VALUE_1                = "1";
#define isTrue(x) ( x == VALUE_true || x == VALUE_yes || x == VALUE_1 )


typedef std::map<std::string, CtlConfig> ConfigMap;
typedef std::map<std::string, ConfigMap> FileConfigMap;

static FileConfigMap m_config;


void CtlConfigParser::load( const std::string& file )
{
    if ( m_config.find( file ) != m_config.end() )
    {
        return;
    }

    try
    {
        ConfigMap& config = m_config[file];
        boost::property_tree::ptree pt;
        boost::property_tree::xml_parser::read_xml( file, pt, boost::property_tree::xml_parser::trim_whitespace | boost::property_tree::xml_parser::no_comments );
        boost::property_tree::ptree& root = pt.get_child( TAG_root );

        for ( boost::property_tree::ptree::iterator it = root.begin(); it != root.end(); ++it )
        {
            if ( it->first == TAG_i )
            {
                boost::property_tree::ptree& items = it->second;

                if ( ! items.empty() )
                {
                    boost::property_tree::ptree::iterator iIt = items.begin();

                    if ( iIt->first == TAG_id )
                    {
                        CtlConfig& cfg = config[ iIt->second.data() ];

                        for ( ; iIt != items.end(); ++iIt )
                        {
                            const std::string& tag = iIt->first;
                            const std::string& text = iIt->second.data();

                            if ( tag == TAG_id )                    { cfg.id            = text; }
                            else if ( tag == TAG_visiblity )        { cfg.visiblity     = isTrue( text ); }
                            else if ( tag == TAG_caption )          { cfg.caption       = text; }
                            else if ( tag == TAG_action_id )        { cfg.actionID      = text; }
                            else if ( tag == TAG_tooltip )          { cfg.tooltip       = text; }
                            else if ( tag == TAG_help_file )        { cfg.helpFile      = text; }
                            else if ( tag == TAG_maximum_length )   { try { cfg.maximumLength = boost::lexical_cast<int>(text); } catch (...) {} }
                            else if ( tag == TAG_enable_rtrim )     { cfg.enableRTrim   = isTrue( text ); }
                            else if ( tag == TAG_style_sheet )      { cfg.styleSheet    = text; }
                        }
                    }
                }
            }
        }
    }
    catch ( boost::property_tree::ptree_error& )
    {
    }
}


void CtlConfigParser::unload( const std::string& file )
{
    if ( file.empty() )
    {
        m_config.clear();
    }
    else
    {
        FileConfigMap::iterator it = m_config.find( file );

        if ( it != m_config.end() )
        {
            m_config.erase( it );
        }
    }
}


CtlConfig* CtlConfigParser::get( const std::string& id )
{
    for ( FileConfigMap::iterator it = m_config.begin(); it != m_config.end(); ++it )
    {
        ConfigMap& configMap = it->second;
        ConfigMap::iterator findIt = configMap.find( id );

        if ( findIt != configMap.end() )
        {
            return &( findIt->second );
        }
    }

    return NULL;
}
