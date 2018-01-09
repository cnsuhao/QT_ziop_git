#include "TransActiveMessageConfigParser.h"
#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <map>

/**
  * <!-- TAMessageLibrary.xml format -->
  *
  * <Library>
  *     <Messages>
  *         <Message Obsolete="false" AuditEvent="false">
  *             <ID> ...... </ID>
  *             <ResID> ...... </ResID>
  *             <MessageType> ...... </MessageType>
  *             <HelpID> ...... </HelpID>
  *             <MsgBoxStyle> ...... </MsgBoxStyle>
  *             <String> ...... </String>
  *             <Parameter> ...... </Parameter>
  *             ...
  *         </Message>
  *         ...
  *    </Messages>
  * </Library>
  */
#define TAG_Library            "Library"
#define TAG_Messages           "Messages"
#define TAG_Message            "Message"
#define TAG_Obsolete           "Obsolete"
#define TAG_AuditEvent         "AuditEvent"
#define TAG_ID                 "ID"
#define TAG_ResID              "ResID"
#define TAG_MessageType        "MessageType"
#define TAG_HelpID             "HelpID"
#define TAG_MsgBoxStyle        "MsgBoxStyle"
#define TAG_String             "String"
#define TAG_Parameter          "Parameter"
#define VALUE_true             "true"
#define isEqual(x, y) ( 0 == ::strcmp( (x), (y) ) )

typedef std::map<int, TA_Base_Bus::TransActiveMessageConfig> MessageConfigMap;
typedef std::map<std::string, MessageConfigMap> File2MessageConfigMap;
static File2MessageConfigMap m_fileConfigMap;
static std::string m_configFileName;


namespace TA_Base_Bus
{
    void TransActiveMessageConfigParser::loadFile(const std::string& fileName)
    {
        if (m_fileConfigMap.find(fileName) != m_fileConfigMap.end())
        {
            return;
        }

        MessageConfigMap& configMap = m_fileConfigMap[fileName];
        m_configFileName = fileName;

        try
        {
            rapidxml::file<> file(fileName.c_str());
            rapidxml::xml_document<> doc;
            doc.parse<0>(file.data());
            rapidxml::xml_node<>* library = doc.first_node();
            rapidxml::xml_node<>* messages = library->first_node();

            for (rapidxml::xml_node<>* message = messages->first_node(); message; message = message->next_sibling())
            {
                TransActiveMessageConfig* cfg = NULL;

                for (rapidxml::xml_node<>* item = message->first_node(); item; item = item->next_sibling())
                {
                    const char* tag = item->name();
                    const char* text = item->value();

                    if (isEqual(tag, TAG_ID))
                    {
                        int id = boost::lexical_cast<int>(text);
                        cfg = &configMap[id];
                        cfg->id = id;
                        cfg->obsolete = isEqual(message->first_attribute()->value(), VALUE_true);
                        cfg->auditEvent = isEqual(message->last_attribute()->value(), VALUE_true);
                    }
                    else if (isEqual(tag, TAG_ResID))        { cfg->resID = text; }
                    else if (isEqual(tag, TAG_MessageType))  { cfg->messageType = boost::lexical_cast<int>(text); }
                    else if (isEqual(tag, TAG_HelpID))       { cfg->helpID = boost::lexical_cast<int>(text); }
                    else if (isEqual(tag, TAG_MsgBoxStyle))  { cfg->messageBoxStyle = boost::lexical_cast<int>(text); }
                    else if (isEqual(tag, TAG_String))       { cfg->string = text; if (cfg->string.find("\\n") != std::string::npos) { boost::replace_all(cfg->string, "\\n", "\n"); } }
                    else if (isEqual(tag, TAG_Parameter))    { cfg->parameters.push_back(text); }
                }
            }
        }
        catch (rapidxml::parse_error&)
        {
        }
    }


    TransActiveMessageConfig* TransActiveMessageConfigParser::get(int id)
    {
        if (! m_configFileName.empty())
        {
            File2MessageConfigMap::iterator it = m_fileConfigMap.find(m_configFileName);

            if (it != m_fileConfigMap.end())
            {
                MessageConfigMap& configMap = it->second;
                MessageConfigMap::iterator configIt = configMap.find(id);

                if (configIt != configMap.end())
                {
                    return &configIt->second;
                }
            }
        }

        return NULL;
    }
}
