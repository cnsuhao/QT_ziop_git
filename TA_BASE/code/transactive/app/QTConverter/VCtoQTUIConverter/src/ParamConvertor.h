#pragma once
#include "ParameterData.h"
#include <vector>
#include <ostream>
#include <map>
#include <set>

class ParamConvertor
{
    typedef std::pair<std::string, std::vector<std::string>> popupItemmenuPair;

public:
    ParamConvertor();
    ~ParamConvertor( void );
    void readConfigure();
    void readRc();
    void convert();
private:
    void filterRc();
    std::string getFirstWord(std::string& inString );
    std::string getFirstWordCommaSeparator(std::string& inString);
    void removeSpaces(std::string& inString);
    bool getGeometry(const std::string& inString, Geometry& geometry, bool ishasMenu,const std::string& controlType);
    std::string generateGeometry(const std::string& inString, bool ishasMenu, const std::string& controlType);
    bool is_digit(const char *str);
    void removeSpecialCharacter(std::string& inString);
    std::string generateProperty(const std::string& inString, bool ishasMenu = false, const std::string& controlType = "");
    std::string generateMenus(const std::vector<popupItemmenuPair>& menuInfos,const std::string& menuId);

    std::string getRCFileName();

    //Read from mapping configuration file
    // pair:rc attribute name<---------> ui attribute name
    typedef std::pair<std::string, std::string> AttributePair;
    typedef std::vector<AttributePair> vectAttributePair;
    std::map<std::string,vectAttributePair> m_mapConfigs;

    //Read from .rc file.
    typedef std::pair<std::string, std::string> KeyWordAndLinePair;  //<keyword:layer, linestring>
    typedef std::vector<AttributePair> vectRcLinesForDlg;
    std::map<std::string,vectRcLinesForDlg> m_mapAllRcLines;   //<dlgId, vectRcLinesForDlg>

    //Menu
    typedef std::vector<std::string> vectLayerMenuText;
    std::map<std::string, vectLayerMenuText> m_mapAllMenuInfos;

    std::map<std::string, bool> m_mapDialogHasMenu;  //DialogId, isHasMenu.

    std::string m_dialogWidth;
};
