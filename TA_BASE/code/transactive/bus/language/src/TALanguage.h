#pragma once
#include "bus/unicode_common_lib/src/TAUnicodeString.h"
//#include <afxwin.h>
//#include <afxext.h>
//
//
//// API
//void                        TALoadLang( UINT nApplicationID );
//void                        TALoadString( UINT nID, TAUnicodeString& strBuffer );
//void                        TALoadString( UINT nID, LPTSTR lpszBuf );
//TAUnicodeString             TALoadString( UINT nID );
//TAUnicodeString             TALoadDialogCaption( UINT nID );
//void                        TATranslateDialog( UINT nID, CDialog* pDialog );
//void                        TATranslateMenu( UINT nID, CMenu* pMenu );
//void                        TAAppendLangMenu( UINT nID, CMenu* pMenu );
//void                        TATranslateToolBar( UINT nID, CToolBar* pToolBar );
//void                        TASetCurrentLang( UINT nID );
//UINT                        TAGetCurrentLang();
//void                        TASetDefaultLang( UINT nID );
//BOOL                        TAOnToolTipText( UINT nID , NMHDR* pNMHDR, LRESULT* pResult );
//#define TAAfxFormatString1  TAAfxFormatString
//#define TAAfxFormatString2  TAAfxFormatString
//#define TAAfxFormatString3  TAAfxFormatString
//#define TAAfxFormatString4  TAAfxFormatString
//#define TAAfxFormatString5  TAAfxFormatString
//#define TAAfxFormatString6  TAAfxFormatString
//#define TAAfxFormatString7  TAAfxFormatString
//#define TAAfxFormatString8  TAAfxFormatString
//#define TAAfxFormatString9  TAAfxFormatString
//#define TAAfxFormatStringX  TAAfxFormatString
//TAUnicodeString             TAAfxFormatString( UINT nID, LPCTSTR s1=0, LPCTSTR s2=0, LPCTSTR s3=0, LPCTSTR s4=0, LPCTSTR s5=0,LPCTSTR s6=0, LPCTSTR s7=0, LPCTSTR s8=0, LPCTSTR s9=0 );


// Language Message ID
#define WM_TASETLANG 32767


// Language ID
#define LANGUAGE_ID_LANGUAGE                                                    65500
#define LANGUAGE_ID_EN_US                                                       65501
#define LANGUAGE_ID_ZH_CN                                                       65502
#define LANGUAGE_ID_FIRST                                                       LANGUAGE_ID_EN_US
#define LANGUAGE_ID_LAST                                                        LANGUAGE_ID_FIRST + 100
#define LANGUAGE_ID_NONE                                                        0


// Resource Type ID
#define RESOURCE_TYPE_ID_String                                                 1
#define RESOURCE_TYPE_ID_Dialog                                                 1000
#define RESOURCE_TYPE_ID_Text                                                   1010
#define RESOURCE_TYPE_ID_GroupBox                                               1020
#define RESOURCE_TYPE_ID_PushButton                                             1030
#define RESOURCE_TYPE_ID_PushBox                                                1040
#define RESOURCE_TYPE_ID_SysListView32                                          1050
#define RESOURCE_TYPE_ID_CheckBox                                               1060
#define RESOURCE_TYPE_ID_RadioButton                                            1070
#define RESOURCE_TYPE_ID_ToolBar                                                1200
#define RESOURCE_TYPE_ID_Menu                                                   1510
#define RESOURCE_TYPE_ID_Popup                                                  1520
#define RESOURCE_TYPE_ID_MenuItem                                               1530


// Application ID
#define APPLICATION_ID_ALL                                                      0
#define APPLICATION_ID_AlarmManager                                             100
#define APPLICATION_ID_AlarmGenerator                                           200
#define APPLICATION_ID_IscsAlarmDisplay                                         300
#define APPLICATION_ID_IscsAlarmActionBridge                                    400
#define APPLICATION_ID_AtsAlarmBridgeUtility                                    500
#define APPLICATION_ID_AtsAlarmGenerator                                        600
#define APPLICATION_ID_AtsIscsAlarmViewer                                       700
#define APPLICATION_ID_EventExtractor                                           800
#define APPLICATION_ID_EventViewer                                              900
#define APPLICATION_ID_CrystalReportsViewer                                     1000
#define APPLICATION_ID_NewTrendViewer                                           1100
#define APPLICATION_ID_TrendViewer                                              1200
#define APPLICATION_ID_HistoryViewer                                            1300
#define APPLICATION_ID_PlanManager                                              1400
#define APPLICATION_ID_SchedulingManager                                        1500
#define APPLICATION_ID_EcsManager                                               1600
#define APPLICATION_ID_IncidentManager                                          1700
#define APPLICATION_ID_JobRequestGui                                            1800
#define APPLICATION_ID_MmsController                                            1900
#define APPLICATION_ID_DataNodeController                                       2000
#define APPLICATION_ID_EquipmentStatusViewer                                    2100
#define APPLICATION_ID_InspectorPanel                                           2200
#define APPLICATION_ID_PowerDemandEditor                                        2300
#define APPLICATION_ID_DutyManager                                              2400
#define APPLICATION_ID_RegionManager                                            2500
#define APPLICATION_ID_SystemManager                                            2600
#define APPLICATION_ID_ControlStation                                           2700
#define APPLICATION_ID_SoundManager                                             2800
#define APPLICATION_ID_Banner                                                   2900
#define APPLICATION_ID_IscsPage                                                 3000
#define APPLICATION_ID_PlanPage                                                 3100
#define APPLICATION_ID_AtsPage                                                  3200
#define APPLICATION_ID_CallPage                                                 3300
#define APPLICATION_ID_ConfigurationEditor                                      3400
#define APPLICATION_ID_Pluginxxx                                                3500
#define APPLICATION_ID_VideoDisplayManager                                      3600
#define APPLICATION_ID_PaManager                                                3700
#define APPLICATION_ID_RadioAuthoriseGui                                        3800
#define APPLICATION_ID_RadioManagerGui                                          3900
#define APPLICATION_ID_RadioManagerGuiIcdv4                                     4000
#define APPLICATION_ID_RadioMonitorGui                                          4100
#define APPLICATION_ID_RadioEecAcceptGui                                        4200
#define APPLICATION_ID_StisManager                                              4300
#define APPLICATION_ID_TislogViewer                                             4400
#define APPLICATION_ID_TtisManager                                              4500
#define APPLICATION_ID_TelephoneManager                                         4600
#define APPLICATION_ID_TrainBorneDisplayManager                                 4700
#define APPLICATION_ID_TrainBorneManager                                        4800
#define APPLICATION_ID_TrainControlManager                                      4900
