#include "ParamConvertor.h"
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/filesystem/fstream.hpp"
#include "boost/filesystem/exception.hpp"
#include "boost/tokenizer.hpp"
#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/DataConversion.h"
#include "core/utilities/src/RunParams.h"
#include "bus/scada/common_library/src/CommonDefs.h"
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <afx.h>

namespace fs = boost::filesystem;


std::string KeyWord[] =
{
    //for dialog
    "DIALOGEX",
    "DIALOG",
    "STYLE",
    "EXSTYLE",
    "CAPTION",
    "MENU",
    "CHECKBOX",
    "FONT",
    "DEFPUSHBUTTON", 
    "PUSHBUTTON",
    "EDITTEXT", 
    "COMBOBOX", 
    "LISTBOX", 
    "GROUPBOX",
    "LTEXT",
    "CTEXT",
    "SCROLLBAR",
    "ICON",
    "CONTROL",
    "BEGIN",
    "END",

    //for menu
    "POPUP",
    "MENUITEM",
};

//std::string MenuKeyWord[] =
//{
//    "POPUP",
//    "MENUITEM",
//    "BEGIN",
//    "END"
//};

ParamConvertor::ParamConvertor()
{
}

ParamConvertor::~ParamConvertor( void )
{
}

void ParamConvertor::convert()
{
    if (m_mapAllRcLines.size() == 0)
    {
        LOG ( SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugError,"rc file or config file is null");
        return;
    }
    
    for(std::map<std::string,vectRcLinesForDlg>::iterator itFoundRcLine = m_mapAllRcLines.begin(); itFoundRcLine != m_mapAllRcLines.end(); ++itFoundRcLine)
    {
        bool hasAddQWidget = false;
        std::stringstream strBegin;
        std::stringstream strEnd;
        std::string dialogId = (*itFoundRcLine).first; 
        std::string fileName = dialogId + ".ui";
        std::ofstream ofQtUI( fileName.c_str(), std::ios::out | std::ios::trunc );
        std::string menuId = "";
 
        strBegin << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << "\n";         //if language is Chinese, should be set GB2312
        strBegin << "<ui version=\"4.0\">" << "\n"; 
        strBegin << " <class>" << dialogId << "</class>" << "\n";
        strBegin << " <widget class=\"QWidget\" name=\"" << dialogId  << "\">\n";
        for (vectRcLinesForDlg::iterator itVectRcLine = (*itFoundRcLine).second.begin();itVectRcLine != (*itFoundRcLine).second.end(); ++itVectRcLine)
        {
            std::string controlType = (*itVectRcLine).first.substr(0,(*itVectRcLine).first.size() - 2);
            std::string layer = (*itVectRcLine).first.substr((*itVectRcLine).first.size() - 1,(*itVectRcLine).first.size());
            if (layer == "2" && !hasAddQWidget)
            {
               // strBegin << "  <widget class=\"QWidget\" name=\"centralWidget\">" << "\n";
              //  strEnd << "  </widget>" << "\n";
                hasAddQWidget = true;
            }

            std::string strTemp = (*itVectRcLine).second;
            removeSpecialCharacter(strTemp);
            
            LOG ( SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugDebug, "Begin to convert : dialogId = %s, controlType= %s, line = %s",dialogId.c_str(),controlType.c_str(), strTemp.c_str()); 

            if (std::string::npos != strTemp.find("NOT WS_VISIBLE"))
            {
                continue;
            }

            if (controlType == "DIALOG" || (controlType == "DIALOGEX"))
            {
                strBegin << generateProperty(strTemp,m_mapDialogHasMenu[dialogId],controlType);
                continue;
            }

            if (controlType == "MENU")
            {
                menuId = getFirstWord(strTemp);
                continue;
            }
            if (controlType == "CAPTION")
            {
                //  <property name="windowTitle"><string>fff</string></property>
                strTemp = strTemp.substr(1,strTemp.size() - 2);
                strBegin << "  <property name=\"windowTitle\"><string>" << strTemp <<"</string></property>" << "\n";
                continue;
            }
            if (controlType == "FONT")
            {
                // <property name="font"><font><family>MS Serif</family><pointsize>9</pointsize></font></property>
                std::string fontSize = getFirstWordCommaSeparator(strTemp);
                std::string fontType = getFirstWordCommaSeparator(strTemp);
                fontType = fontType.substr(1,fontType.size() - 2);
                strBegin << "  <property name=\"font\"><font><family>" << fontType << "</family><pointsize>" << fontSize <<"</pointsize></font></property>" << "\n";
                continue;
            }
            if (controlType == "DEFPUSHBUTTON" || (controlType == "PUSHBUTTON"))
            {
                //<widget class="QPushButton" name="pushButton_2"><property name="geometry"><rect><x>370</x><y>380</y><width>75</width><height>23</height></rect></property><property name="text"><string>cancel</string></property></widget>
                std::string strText = getFirstWordCommaSeparator(strTemp);
                std::string strId = getFirstWordCommaSeparator(strTemp);
                strText = strText.substr(1,strText.size() - 2);
                strBegin << "<widget class=\"CtlButton\" name=\"" << strId << "\">" << "\n";

                strBegin << generateProperty(strTemp,m_mapDialogHasMenu[dialogId]);

                strBegin << "<property name=\"text\"><string>" << strText << "</string></property>" << "\n";

                if (controlType == "DEFPUSHBUTTON")
                {
                    //    <property name="default"><bool>true</bool></property>
                    strBegin << "<property name=\"default\"><bool>true</bool></property>" << "\n";
                }
                strBegin << "</widget>" << "\n";
                continue;

            }
            if (controlType == "EDITTEXT")
            {
                //<widget class="QPlainTextEdit" name="plainTextEdit">
                std::string strId = getFirstWordCommaSeparator(strTemp);
                strBegin << "<widget class=\"CtlLineEdit\" name=\"" << strId << "\">" << "\n";

                strBegin << generateProperty(strTemp,m_mapDialogHasMenu[dialogId]);
                strBegin << "</widget>" << "\n";
                continue;
            }
            if (controlType == "COMBOBOX")
            {
                //<widget class="QComboBox" name="comboBox">
                std::string strId = getFirstWordCommaSeparator(strTemp);
                strBegin << "<widget class=\"CtlComboBox\" name=\"" << strId << "\">" << "\n";

                strBegin << generateProperty(strTemp,m_mapDialogHasMenu[dialogId],controlType);
                strBegin << "</widget>" << "\n";
                continue;
            }
            if (controlType == "LISTBOX")
            {
                //<widget class="QListView" name="listView">
                std::string strId = getFirstWordCommaSeparator(strTemp);
                strBegin << "<widget class=\"CtlListWidget\" name=\"" << strId << "\">" << "\n";

                strBegin << generateProperty(strTemp,m_mapDialogHasMenu[dialogId]);
                strBegin << "</widget>" << "\n";
                continue;
            }
            if (controlType == "GROUPBOX")
            {
                //<widget class="QGroupBox" name="groupBox"><property name="geometry"><rect><x>250</x><y>410</y><width>120</width><height>80</height></rect></property><property name="title"><string>GroupBox</string></property></widget>
                std::string strText = getFirstWordCommaSeparator(strTemp);
                std::string strId = getFirstWordCommaSeparator(strTemp);
                strText = strText.substr(1,strText.size() - 2);
                strBegin << "<widget class=\"CtlGroupBox\" name=\"" << strId << "\">" << "\n";

                strBegin << generateProperty(strTemp,m_mapDialogHasMenu[dialogId]);
                strBegin << "<property name=\"title\"><string>" << strText << "</string></property>" << "\n";
                strBegin << "</widget>" << "\n";
                continue;
            }
            if (controlType == "LTEXT" || (controlType == "CTEXT"))
            {
                //<widget class="QLabel" name="label">
                std::string strText = getFirstWordCommaSeparator(strTemp);
                std::string strId = getFirstWordCommaSeparator(strTemp);
                strText = strText.substr(1,strText.size() - 2);
                strBegin << "<widget class=\"CtlLabel\" name=\"" << strId << "\">" << "\n";

                strBegin << generateProperty(strTemp,m_mapDialogHasMenu[dialogId]);
                strBegin << "<property name=\"text\"><string>" << strText << "</string></property>" << "\n";
                strBegin << "</widget>" << "\n";
                continue;
            }
            if (controlType == "SCROLLBAR")
            {
                //<widget class="QScrollBar" name="horizontalScrollBar"><property name="orientation"><enum>Qt::Horizontal</enum></property></widget>
                std::string strId = getFirstWordCommaSeparator(strTemp);
                strBegin << "<widget class=\"CtlScrollBar\" name=\"" << strId << "\">" << "\n";

                strBegin << generateProperty(strTemp,m_mapDialogHasMenu[dialogId]);

                if (std::string::npos != strTemp.find("SBS_VERT"))
                {
                    strBegin << "<property name=\"orientation\"><enum>Qt::Vertical</enum></property>" << "\n";
                }
                else
                {
                    strBegin << "<property name=\"orientation\"><enum>Qt::Horizontal</enum></property>" << "\n";
                }
                strBegin << "</widget>" << "\n";
                continue;

            }
            if (controlType == "CHECKBOX")
            {
                //<widget class="QCheckBox" name="checkBox"><property name="text"><string>CheckBox</string></property></widget>
                std::string strText = getFirstWordCommaSeparator(strTemp);
                std::string strId = getFirstWordCommaSeparator(strTemp);
                strText = strText.substr(1,strText.size() - 2);
                strBegin << "<widget class=\"CtlCheckBox\" name=\"" << strId << "\">" << "\n";

                strBegin << generateProperty(strTemp,m_mapDialogHasMenu[dialogId]);

                strBegin << "<property name=\"text\"><string>" << strText << "</string></property>" << "\n";
                strBegin << "</widget>" << "\n";
                continue;
            }
            if (controlType == "CONTROL")
            {
                if (std::string::npos != strTemp.find("BS_AUTORADIOBUTTON"))
                {
                    //<widget class="QRadioButton" name="radioButton"><property name="text"><string>RadioButton</string></property></widget>
                    std::string strText = getFirstWordCommaSeparator(strTemp);
                    std::string strId = getFirstWordCommaSeparator(strTemp);
                    strText = strText.substr(1,strText.size() - 2);
                    strBegin << "<widget class=\"CtlRadioButton\" name=\"" << strId << "\">" << "\n";

                    strBegin << generateProperty(strTemp,m_mapDialogHasMenu[dialogId]);

                    strBegin << "<property name=\"text\"><string>" << strText << "</string></property>" << "\n";
                    strBegin << "</widget>" << "\n";
                    continue;
                }
                if (std::string::npos != strTemp.find("BS_AUTOCHECKBOX") || (std::string::npos != strTemp.find("BS_AUTO3STATE")))
                {
                    //<widget class="QCheckBox" name="checkBox"><property name="text"><string>CheckBox</string></property></widget>
                    std::string strText = getFirstWordCommaSeparator(strTemp);
                    std::string strId = getFirstWordCommaSeparator(strTemp);
                    strText = strText.substr(1,strText.size() - 2);
                    strBegin << "<widget class=\"CtlCheckBox\" name=\"" << strId << "\">" << "\n";

                    strBegin << generateProperty(strTemp,m_mapDialogHasMenu[dialogId]);

                    strBegin << "<property name=\"text\"><string>" << strText << "</string></property>" << "\n";
                    strBegin << "</widget>" << "\n";
                    continue;
                }
                if (std::string::npos != strTemp.find("SS_BLACKFRAME"))
                {
                    //<widget class="QFrame" name="frame"><property name="frameShape"><enum>QFrame::StyledPanel</enum></property><property name="frameShadow"><enum>QFrame::Raised</enum></property></widget>
                    std::string strText = getFirstWordCommaSeparator(strTemp);
                    std::string strId = getFirstWordCommaSeparator(strTemp);
                    strBegin << "<widget class=\"QFrame\" name=\"" << strId << "\">" << "\n";

                    strBegin << generateProperty(strTemp,m_mapDialogHasMenu[dialogId]);

                    strBegin << "<property name=\"frameShape\"><enum>QFrame::StyledPanel</enum></property><property name=\"frameShadow\"><enum>QFrame::Raised</enum></property>" << "\n";
                    strBegin << "</widget>" << "\n";
                    continue;
                }

                if (std::string::npos != strTemp.find("msctls_trackbar32"))
                {
                    //   <widget class="QSlider" name="horizontalSlider"><property name="orientation"><enum>Qt::Horizontal</enum></property></widget>
                    std::string strText = getFirstWordCommaSeparator(strTemp);
                    std::string strId = getFirstWordCommaSeparator(strTemp);
                    strBegin << "<widget class=\"QSlider\" name=\"" << strId << "\">" << "\n";

                    strBegin << generateProperty(strTemp,m_mapDialogHasMenu[dialogId]);

                    if (std::string::npos != strTemp.find("TBS_VERT"))
                    {
                        strBegin << "<property name=\"orientation\"><enum>Qt::Vertical</enum></property>" << "\n";
                    }
                    else
                    {
                        strBegin << "<property name=\"orientation\"><enum>Qt::Horizontal</enum></property>" << "\n";
                    }
                    strBegin << "</widget>" << "\n";
                    continue;
                }

                if (std::string::npos != strTemp.find("msctls_updown32"))
                {
                    //   <widget class="QSpinBox" name="spinBox"><property name="geometry"><rect><x>80</x><y>400</y><width>42</width><height>22</height></rect></property></widget>
                    std::string strText = getFirstWordCommaSeparator(strTemp);
                    std::string strId = getFirstWordCommaSeparator(strTemp);
                    strBegin << "<widget class=\"CtlSpinBox\" name=\"" << strId << "\">" << "\n";

                    strBegin << generateProperty(strTemp,m_mapDialogHasMenu[dialogId]);

                    strBegin << "</widget>" << "\n";
                    continue;
                }

                if (std::string::npos != strTemp.find("msctls_progress32"))
                {
                    //   <widget class="QProgressBar" name="progressBar"><property name="value"><number>24</number></property></widget>
                    std::string strText = getFirstWordCommaSeparator(strTemp);
                    std::string strId = getFirstWordCommaSeparator(strTemp);
                    strBegin << "<widget class=\"QProgressBar\" name=\"" << strId << "\">" << "\n";

                    strBegin << generateProperty(strTemp,m_mapDialogHasMenu[dialogId]);

                    strBegin << "<property name=\"value\"><number>0</number></property>" << "\n";
                    strBegin << "</widget>" << "\n";
                    continue;
                }
                if (std::string::npos != strTemp.find("SysListView32"))
                {
                    //   <widget class="QListView" name="listView"></widget>
                    std::string strText = getFirstWordCommaSeparator(strTemp);
                    std::string strId = getFirstWordCommaSeparator(strTemp);
                    strBegin << "<widget class=\"CtlListWidget\" name=\"" << strId << "\">" << "\n";

                    strBegin << generateProperty(strTemp,m_mapDialogHasMenu[dialogId]);

                    strBegin << "</widget>" << "\n";
                    continue;
                }
                if (std::string::npos != strTemp.find("SysTreeView32"))
                {
                    //   <widget class="QTreeView" name="listView"></widget>
                    std::string strText = getFirstWordCommaSeparator(strTemp);
                    std::string strId = getFirstWordCommaSeparator(strTemp);
                    strBegin << "<widget class=\"CtlTreeWidget\" name=\"" << strId << "\">" << "\n";

                    strBegin << generateProperty(strTemp,m_mapDialogHasMenu[dialogId]);

                    strBegin << "</widget>" << "\n";
                    continue;
                }
                if (std::string::npos != strTemp.find("SysTabControl32"))
                {
                    //    <widget class="QTabWidget" name="tabWidget"><widget class="QWidget" name="tab"><attribute name="title"><string>Tab 1</string></attribute></widget></widget>
                    std::string strText = getFirstWordCommaSeparator(strTemp);
                    std::string strId = getFirstWordCommaSeparator(strTemp);
                    strBegin << "<widget class=\"CtlTabWidget\" name=\"" << strId << "\">" << "\n";

                    strBegin << generateProperty(strTemp,m_mapDialogHasMenu[dialogId]);

                    strBegin << "<widget class=\"QWidget\" name=\"tab\"><attribute name=\"title\"><string>Tab 1</string></attribute></widget>" <<"\n";
                    strBegin << "</widget>" << "\n";
                    continue;
                }
                if (std::string::npos != strTemp.find("SysDateTimePick32"))
                {
                    // <widget class="QDateTimeEdit" name="dateTimeEdit"></widget>
                    std::string strText = getFirstWordCommaSeparator(strTemp);
                    std::string strId = getFirstWordCommaSeparator(strTemp);
                    if (std::string::npos != strTemp.find("DTS_UPDOWN"))
                    {
                        strBegin << "<widget class=\"CtlTimeEdit\" name=\"" << strId << "\">" << "\n";
                    }
                    else
                    {
                        strBegin << "<widget class=\"CtlDateTimeEdit\" name=\"" << strId << "\">" << "\n";
                    }                    

                    strBegin << generateProperty(strTemp,m_mapDialogHasMenu[dialogId]); 

                    strBegin << "</widget>" << "\n";
                    continue;
                }
                if (std::string::npos != strTemp.find("SysMonthCal32"))
                {
                    //<widget class="QCalendarWidget" name="calendarWidget"></widget>
                    std::string strText = getFirstWordCommaSeparator(strTemp);
                    std::string strId = getFirstWordCommaSeparator(strTemp);
                    strBegin << "<widget class=\"CtlDateTimeEdit\" name=\"" << strId << "\">" << "\n";

                    strBegin << generateProperty(strTemp,m_mapDialogHasMenu[dialogId]);

                    strBegin << "</widget>" << "\n";
                    continue;
                }
                if (std::string::npos != strTemp.find("BS_COMMANDLINK"))
                {
                    //   <widget class="QCommandLinkButton" name="commandLinkButton"><property name="text"><string>CommandLinkButton</string></property></widget>
                    std::string strText = getFirstWordCommaSeparator(strTemp);
                    std::string strId = getFirstWordCommaSeparator(strTemp);
                    strText = strText.substr(1,strText.size() - 2);
                    strBegin << "<widget class=\"QCommandLinkButton\" name=\"" << strId << "\">" << "\n";

                    strBegin << generateProperty(strTemp,m_mapDialogHasMenu[dialogId]);

                    strBegin << "<property name=\"text\"><string>" << strText << "</string></property>" << "\n";
                    strBegin << "</widget>" << "\n";
                    continue;
                }
                if (std::string::npos != strTemp.find("RICHEDIT") || (std::string::npos != strTemp.find("RichEdit")))
                {
                    //<widget class="QTextEdit" name="textEdit"></widget>
                    std::string strText = getFirstWordCommaSeparator(strTemp);
                    std::string strId = getFirstWordCommaSeparator(strTemp);
                    strBegin << "<widget class=\"CtlTextEdit\" name=\"" << strId << "\">" << "\n";

                    strBegin << generateProperty(strTemp,m_mapDialogHasMenu[dialogId]);

                    strBegin << "</widget>" << "\n";
                    continue;
                }
                if (std::string::npos != strTemp.find("BS_OWNERDRAW"))
                {
                    //button
                    std::string strText = getFirstWordCommaSeparator(strTemp);
                    std::string strId = getFirstWordCommaSeparator(strTemp);
                    strText = strText.substr(1,strText.size() - 2);
                    strBegin << "<widget class=\"CtlButton\" name=\"" << strId << "\">" << "\n";

                    strBegin << generateProperty(strTemp,m_mapDialogHasMenu[dialogId]);

                    strBegin << "<property name=\"text\"><string>" << strText << "</string></property>" << "\n";
                    strBegin << "</widget>" << "\n";
                    continue;
                }
                if (std::string::npos != strTemp.find("SS_LEFTNOWORDWRAP"))
                {
                    //<widget class="QLabel" name="label">
                    std::string strText = getFirstWordCommaSeparator(strTemp);
                    std::string strId = getFirstWordCommaSeparator(strTemp);
                    strText = strText.substr(1,strText.size() - 2);
                    strBegin << "<widget class=\"CtlLabel\" name=\"" << strId << "\">" << "\n";

                    strBegin << generateProperty(strTemp,m_mapDialogHasMenu[dialogId]);
                    strBegin << "<property name=\"text\"><string>" << strText << "</string></property>" << "\n";
                    strBegin << "</widget>" << "\n";
                    continue;
                }
                
            }

            LOG ( SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugWarn, "Cannot convert this line : %s, %s",controlType.c_str(), (*itVectRcLine).second.c_str());            
        }

        if (TA_Base_Core::RunParams::getInstance().get(RPARAM_MENU).compare("1") == 0)
        {
            //for menu;
            std::vector<popupItemmenuPair> popupAndItems;
            std::map<std::string, vectLayerMenuText>::iterator itFoundMenu = m_mapAllMenuInfos.find(menuId);
            if (itFoundMenu != m_mapAllMenuInfos.end())
            {
                std::vector<std::string> menuItems;
                for (vectLayerMenuText::iterator itMenuText = (*itFoundMenu).second.begin(); itMenuText != (*itFoundMenu).second.end(); ++itMenuText )
                {
                    std::string menuitem = *itMenuText;
                    std::string popup = getFirstWordCommaSeparator(menuitem);
                    removeSpecialCharacter(menuitem);
                    removeSpecialCharacter(popup);
                    if (popup.size() >= 2)
                    {
                        popup = popup.substr(1,popup.size() - 2);
                    }
                    if (menuitem.size() >= 2)
                    {
                        if (menuitem == "SEPARATOR")
                        {
                            menuitem = "separator";
                        }
                        else
                        {
                            menuitem = menuitem.substr(1,menuitem.size() - 2); 
                        }                            
                    }                    

                    std::vector<std::string> menuItems; 
                    menuItems.push_back(menuitem);
                    bool isFoundPopup = false;
                    for (std::vector<popupItemmenuPair>::iterator itFound = popupAndItems.begin(); itFound != popupAndItems.end(); ++itFound)
                    {
                        if ((*itFound).first == popup)
                        {
                            (*itFound).second.push_back(menuitem);
                            isFoundPopup = true;
                        }
                    }

                    if (!isFoundPopup)
                    {
                        popupAndItems.push_back(std::make_pair(popup,menuItems));
                    }                    
                }

                strBegin << generateMenus(popupAndItems,menuId);
            }
        }

        strEnd << " </widget>" << "\n";
        strEnd << "</ui>" << "\n";

        ofQtUI << strBegin.str() << strEnd.str();

        ofQtUI.close();
    }
}

void ParamConvertor::readConfigure()
{
    if (m_mapConfigs.size() == 0)
    {
        fs::ifstream configFile( "config.ini" );
        if ( !configFile )
        {
            std::string msg ("Unable to open file:config.ini");
            LOG ( SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugDebug, msg.c_str());
            return;
        }

        char line[2048]; // 2048 - a number PFTA that should by larger than any single line in the file
        do
        {
            // read a line of values ( a record in db terms )
            configFile.getline(line, sizeof(line));

            // break into values and stuff into a vector for ease of manipulation
            boost::escaped_list_separator<char> sep("\\",",","\"");
            std::string strLine(line);
            boost::tokenizer<boost::escaped_list_separator<char> > tokens ( strLine ,  sep);
            boost::tokenizer<boost::escaped_list_separator<char> >::iterator token = tokens.begin();
            std::vector<std::string> values;
            while ( token != tokens.end() ) 
            {
                values.push_back( *token );
                token++;
            }

            // run some checks to ensure we have the right number
            if ( values.empty() || (!values.empty() && values[0].compare("LayerName") == 0))
            {
                continue; // Skip header row
            }

            // we have the correct number of parameters, store them
            std::string key = values[0];
            values.erase(values.begin());

            vectAttributePair attributepairs;
            while (values.size())
            {
                if( !values[0].empty() )
                {
                    attributepairs.push_back( std::make_pair(values[0], values[1]) );
                }
                values.erase(values.begin());
                values.erase(values.begin());
            }

            std::map<std::string,vectAttributePair>::iterator itMapConfig = m_mapConfigs.find(key);
            if (itMapConfig == m_mapConfigs.end())
            {
                m_mapConfigs.insert(std::make_pair(key,attributepairs) );
            }
            else
            {
                for(vectAttributePair::iterator itVectPair = attributepairs.begin(); itVectPair != attributepairs.end(); ++itVectPair)
                {
                    (*itMapConfig).second.push_back(*itVectPair);
                }
            }
        }while (!configFile.eof());   
    }
}

void ParamConvertor::readRc()
{
    filterRc();
}

void ParamConvertor::filterRc()
{
    if (m_mapAllRcLines.size() == 0 && (m_mapAllMenuInfos.size() == 0))
    {
        //fs::ifstream rcFile( "D:\\workspace-code\\Qt\\TA_BASE\\transactive\\app\\QTConverter\\VCtoQTUIConverter\\source.rc" );
        std::string rcFileName = getRCFileName();
        if (rcFileName.empty())
        {
            std::string msg ("Unable to find rc file in this path");
            LOG ( SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugDebug, msg.c_str());
            return;
        }

        fs::ifstream rcFile( rcFileName );
        if ( !rcFile )
        {
            std::string msg ("Unable to open rc file in this path");
            LOG ( SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugDebug, msg.c_str());
            return;
        }

        char line[2048]; // 2048 - a number PFTA that should by larger than any single line in the file
        //int number = 0;
        std::string temp = "";
        std::string dialogId = "";
        std::string menuId = "";
        vectRcLinesForDlg vectRcLines;
        vectLayerMenuText vectLayerMenu;
        bool isStartDlgFlag = false;
        bool isStartMenuFlag = false;
        std::string popupString = "";
        bool isFoundPopup = false;
        int layer = 1;
        bool isHasMenu = false;

        do
        {
            // read a line of values
            rcFile.getline(line, sizeof(line));
            std::string strLine(line);
            removeSpaces(strLine);

            if (temp == "/*")
            {
                std::size_t found = strLine.find("*/");
                if (found!=std::string::npos)
                {
                    temp = "";
                    //number = 0;
                    continue;
                }
                else
                {
                    continue;
                }
            }
            else
            {
                temp = "";
                //number = 0;
            }

            temp = strLine.substr(0,2);

            if (temp == "//")
            {
                temp = "";
                //number = 0;
                continue;                
            }
            if (temp == "/*")
            {
                continue;
            }

            bool isFound = false;

            if ((std::string::npos != strLine.find("DIALOGEX"))||(std::string::npos != strLine.find("DIALOG")))//remove _DIALOG_ situation.
            {
                dialogId = getFirstWord(strLine);
            }
            if (std::string::npos != strLine.find("MENU") && !isStartDlgFlag && !isStartMenuFlag)
            {
                menuId = getFirstWord(strLine);
            }

            std::string keyword = getFirstWord(strLine);

            for(int i = 0; i<sizeof(KeyWord) / sizeof(std::string) + 1; i++)
            {
                //std::size_t foundKeyword = strLine.find(KeyWord[i]);
                
                if (keyword == KeyWord[i])
                {
                    isFound = true;
                    if (KeyWord[i] == "DIALOGEX" || (KeyWord[i] == "DIALOG"))
                    {
                        isStartDlgFlag = true;
                        if (std::string::npos != strLine.find("DISCARDABLE"))
                        {
                            std::string strTemp = getFirstWord(strLine);
                        }
                    }
                    if (KeyWord[i] == "MENU" && !isStartDlgFlag && !isStartMenuFlag)
                    {
                        isStartMenuFlag= true;
                        continue;
                    }
                    if (KeyWord[i] == "POPUP" && isStartMenuFlag)
                    {
                       popupString = strLine;//Only support two layer menu.
                       isFoundPopup = true;
                       continue;
                    }
                    if (KeyWord[i] == "BEGIN")
                    {
                        //isStartFlag = true;
                        layer++;
                        continue;
                    }
                    if (KeyWord[i] == "END")
                    {
                        //isStartFlag = true;
                        layer--;
                    }
                    if ((KeyWord[i] == "END") && isStartDlgFlag)
                    {
                        std::size_t found = dialogId.find(",");
                        if (found!=std::string::npos)
                        {
                            dialogId = dialogId.substr(0,found);
                        }
                        
                        std::map<std::string, vectRcLinesForDlg>::iterator itfounddlg = m_mapAllRcLines.find(dialogId);
                        if (itfounddlg == m_mapAllRcLines.end())
                        {
                             m_mapAllRcLines.insert(std::make_pair(dialogId,vectRcLines) );
                             m_mapDialogHasMenu.insert(std::make_pair(dialogId,isHasMenu));
                        }
                        dialogId = "";
                        vectRcLines.clear();
                        isStartDlgFlag = false;
                        isHasMenu = false;
                        continue;
                    }
                    if ((KeyWord[i] == "END") && isStartMenuFlag && (layer == 1))
                    {
                        std::map<std::string, vectLayerMenuText>::iterator itfoundMenu = m_mapAllMenuInfos.find(menuId);
                        if (itfoundMenu == m_mapAllMenuInfos.end())
                        {
                            m_mapAllMenuInfos.insert(std::make_pair(menuId,vectLayerMenu) );
                        }
                        menuId = "";
                        vectLayerMenu.clear();
                        isStartMenuFlag = false;
                        continue;
                    }
                    if (isStartDlgFlag)
                    {
                        //std::string keyword = getFirstWord(strLine);
                        if (keyword == "MENU")
                        {
                            isHasMenu = true;
                        }
                        char strLayer[2] = {0};
                        _itoa(layer,strLayer,10);
                        keyword = keyword + ":" + strLayer;
                        vectRcLines.push_back( std::make_pair(keyword, strLine) );
                    }
                    if (isStartMenuFlag && ((KeyWord[i] == "MENUITEM")))
                    {
                        std::string strTemp = getFirstWordCommaSeparator(strLine);
                        //if (strTemp.size() > 0 && strTemp[0] != '"')
                        //{
                        //    continue;//for separator
                        //}

                        std::string strMenuItems = "";
                        if (!isFoundPopup)
                        {
                            strMenuItems = strTemp;
                        }
                        else
                        {
                            strMenuItems = popupString + "," +  strTemp;
                        }
                        
                        vectLayerMenu.push_back( strMenuItems );
                    }
                    break;
                }
            }

            if (isStartDlgFlag && !isFound)
            {
                vectRcLinesForDlg::iterator itLast = vectRcLines.end() - 1;  //for multi line
                (*itLast).second = (*itLast).second + keyword + strLine;
            }
            if (isStartMenuFlag && !isFound)
            {
                vectLayerMenuText::iterator itLast = vectLayerMenu.end() - 1;
                if( "\""!= (*itLast).substr((*itLast).size()-1, 1))
                {
                    (*itLast) = (*itLast) + keyword + strLine;
                }                
            }

        }while (!rcFile.eof()); 
    }
}

std::string ParamConvertor::getFirstWord(std::string& inString)
{
    removeSpaces(inString);

    std::size_t pos = inString.find_first_of(" ") ;
    if ( pos != std::string::npos )
    {
        std::string firstword = inString.substr(0,pos);

        inString = inString.substr(pos + 1, inString.size() - (pos + 1));
        removeSpaces(inString);
        return firstword;
    }
    else
    {
        std::string firstword = inString;
        inString.clear();
        return firstword;
    }
}

std::string ParamConvertor::getFirstWordCommaSeparator(std::string& inString)
{
    removeSpaces(inString);

    std::size_t pos = inString.find_first_of(",") ;
    if (pos != std::string::npos)
    {
        std::string strTemp = inString.substr(0,pos);
        std::string firstword = strTemp;

        while (!firstword.empty() && (firstword[0] == '"') && (firstword[firstword.size()-1] != '"'))//"xxx,xxx,",xxx,xxx
        {
            strTemp = inString.substr(pos + 1,inString.size() - (pos + 1));
            pos = strTemp.find_first_of(",") ;
            if (pos != std::string::npos)
            {
                firstword = firstword + "," + strTemp.substr(0,pos);
            }
            else
            {
                firstword = firstword + "," + strTemp;
            }            
        }

        inString = inString.substr(firstword.size() + 1, inString.size() - firstword.size() - 1);//skip comma
        removeSpaces(inString);
        return firstword;
    }
    else
    {
        std::string firstword = inString;
        inString.clear();
        return firstword;
    }
}

void ParamConvertor::removeSpaces(std::string& inString)
{
    std::string temp = inString;
    //remove spaces before first word.
    for(int i=0; i<temp.size();i++)
    {
        if (temp[i] == ' ' || (temp[i] == '\t'))
        {
            inString.erase(inString.begin());
        }
        else
        {
            return;
        }
    }
}

bool ParamConvertor::getGeometry(const std::string& inString, Geometry& geometry, bool ishasMenu, const std::string& controlType)
{
    boost::escaped_list_separator<char> sep("\\",",","\"");
    boost::tokenizer<boost::escaped_list_separator<char> > tokens ( inString ,  sep);
    boost::tokenizer<boost::escaped_list_separator<char> >::iterator token = tokens.begin();
    std::vector<std::string> values;
    while ( token != tokens.end() ) 
    {
        std::string strTemp = *token;
        removeSpaces(strTemp);
        if (is_digit(strTemp.c_str()))
        {
           values.push_back( strTemp );          
        }
        if (values.size() == 4)
        {
            break;
        }

        token++;
    }

    int nSize = values.size();
    if (nSize != 4)
    {
        return false;
    }

    geometry.x = TA_Base_Core::DataConversion::toString(atol(values[0].c_str()) * 2);
    geometry.y = TA_Base_Core::DataConversion::toString(atol(values[1].c_str()) * 2);
    geometry.width = TA_Base_Core::DataConversion::toString(atol(values[2].c_str()) * 2);
    if (controlType == "COMBOBOX")
    {
        //geometry.height = TA_Base_Core::DataConversion::toString(atol(values[3].c_str()) / 3);
        geometry.height = "24";//12 * 2, Combobox's height is always 12(it is related to font)
    }
    else
    {        
        geometry.height = TA_Base_Core::DataConversion::toString(atol(values[3].c_str()) * 2);
    }

    if (TA_Base_Core::RunParams::getInstance().get(RPARAM_MENU).compare("1") == 0 && ishasMenu)
    {
        if (controlType == "DIALOG" || (controlType == "DIALOGEX"))
        {
            geometry.height = TA_Base_Core::DataConversion::toString(atol(geometry.height.c_str())+ 20);
            m_dialogWidth = geometry.width;
        }
        else
        {
            geometry.y = TA_Base_Core::DataConversion::toString(atol(geometry.y.c_str()) + 20);
        }       
    }

    return true;
}

std::string ParamConvertor::generateGeometry(const std::string& inString, bool ishasMenu, const std::string& controlType)
{
    Geometry geometry;
    bool isGeometry = getGeometry(inString,geometry,ishasMenu,controlType);
    std::string strGeometry = "";
    if (isGeometry)
    {
        //<property name="geometry"><rect><x>0</x><y>0</y><width>616</width><height>536</height></rect></property>
        strGeometry = "  <property name=\"geometry\"><rect><x>" + geometry.x + "</x><y>" + geometry.y + "</y><width>" + geometry.width + "</width><height>" + geometry.height + "</height></rect></property>" + "\n";
    }
    return strGeometry;
}

std::string ParamConvertor::generateProperty(const std::string& inString, bool ishasMenu, const std::string& controlType)
{
    std::string strProperty = "";
    if (std::string::npos != inString.find("TCS_VERTICAL"))
    {
        //    <property name="tabPosition"><enum>QTabWidget::West</enum></property>
        strProperty = " <property name=\"tabPosition\"><enum>QTabWidget::West</enum></property>";
        strProperty = strProperty + "\n";
    }
    if (std::string::npos != inString.find("WS_DISABLED"))
    {
        //    <property name="enabled"><bool>false</bool></property>
        strProperty = " <property name=\"enabled\"><bool>false</bool></property>";
        strProperty = strProperty + "\n";
    }
    std::string strGeometry = generateGeometry(inString,ishasMenu,controlType);
    if (!strGeometry.empty())
    {
        strProperty = strProperty + strGeometry;
    }

    return strProperty;
}

std::string ParamConvertor::generateMenus(const std::vector<popupItemmenuPair>& menuInfos,const std::string& menuId)
{
    /************************************************************************/
    /*  <widget class="QMenuBar" name="menuBar">
    <widget class="QMenu" name="menuNihao">
    <property name="title">
    <string>nihao</string>
    </property>
    <addaction name="actionHello"/>
    </widget>
    <addaction name="menuNihao"/>
    </widget>
    <action name="actionHello">
    <property name="text">
    <string>hello</string>
    </property>
    </action>                                                                    */
    /************************************************************************/
    std::string strMenu = "";
    if (!menuId.empty())
    {

        strMenu = "<widget class=\"QMenuBar\" name=\"" + menuId + "\">\n";
        std::string inString = "0,0," + TA_Base_Core::DataConversion::toString(atol(m_dialogWidth.c_str()) / 2) + ",10";
        std::string strGeometry = generateGeometry(inString,false,"QMenuBar");
        if (!strGeometry.empty())
        {
            strMenu = strMenu + strGeometry;
        }
        for (std::vector<popupItemmenuPair>::const_iterator itMenu = menuInfos.begin(); itMenu != menuInfos.end(); ++itMenu)
        {
            strMenu = strMenu + "<widget class=\"QMenu\" name=\"" + (*itMenu).first + "\">\n";
            strMenu = strMenu + "<property name=\"title\"><string>" + (*itMenu).first + "</string></property>\n";
            for (std::vector<std::string>::const_iterator itMenuItem = (*itMenu).second.begin(); itMenuItem != (*itMenu).second.end(); ++itMenuItem)
            {
                if (!(*itMenuItem).empty())
                {
                    strMenu = strMenu + "<addaction name=\"" + (*itMenuItem) + "\"/>\n";
                }            
            }

            strMenu = strMenu + "</widget>\n";
            strMenu = strMenu + "<addaction name=\"" + (*itMenu).first + "\"/>\n";
        }

        strMenu = strMenu + "</widget>\n";

        for (std::vector<popupItemmenuPair>::const_iterator itMenu = menuInfos.begin(); itMenu != menuInfos.end(); ++itMenu)
        {
            for (std::vector<std::string>::const_iterator itMenuItem = (*itMenu).second.begin(); itMenuItem != (*itMenu).second.end(); ++itMenuItem)
            {
                if (!(*itMenuItem).empty())
                {
                    strMenu = strMenu + "<action name=\"" + (*itMenuItem) + "\">\n";
                    strMenu = strMenu + "<property name=\"text\"><string>" + (*itMenuItem) + "</string></property>\n";
                    strMenu = strMenu + "</action>\n";
                }
            }
        }
    }
    return  strMenu;
}

bool ParamConvertor::is_digit(const char *str)
{
    size_t len = strlen(str);
    while(len > 0) {
        if (*str < '0' || *str > '9') {
            return false;
        }
        str++;
        len--;
    }
    return true; 
}

void ParamConvertor::removeSpecialCharacter(std::string& inString)
{
    //inString.erase(std::remove(inString.begin(), inString.end(), '&'), inString.end());
    std::vector<std::pair<std::string, std::string>> specialCharacterPairs;
    specialCharacterPairs.push_back(std::pair<std::string, std::string>("&","&amp;"));
    specialCharacterPairs.push_back(std::pair<std::string, std::string>("<","&lt;"));
    specialCharacterPairs.push_back(std::pair<std::string, std::string>(">","&gt;"));

    for (std::vector<std::pair<std::string, std::string>>::iterator itFound = specialCharacterPairs.begin(); itFound != specialCharacterPairs.end(); ++itFound)
    {
        std::size_t pos = inString.find((*itFound).first);
        while (pos != std::string::npos)
        {
            inString.replace(pos,(*itFound).first.length(),(*itFound).second);
            pos = inString.find((*itFound).first,pos + (*itFound).second.length());
        }
    }
}

std::string ParamConvertor::getRCFileName()
{
    CFileFind finder;
    BOOL bWorking = finder.FindFile("*.*");
    while (bWorking)
    {
        bWorking = finder.FindNextFile();
        CString name = finder.GetFileName();
        CString extend = name.Right( name.GetLength() - name.ReverseFind('.') - 1);
        if(!finder.IsDots())
        {
            if (extend == "rc")
            {
                return name.GetBuffer(0);
            }
        }
    }
    return "";
}