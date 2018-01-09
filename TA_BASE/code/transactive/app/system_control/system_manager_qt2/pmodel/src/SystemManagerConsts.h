#ifndef SYSTEM_MANAGER_CONSTS_H__
#define SYSTEM_MANAGER_CONSTS_H__

#include "AppServiceConsts.h"


namespace TA_Base_App
{
    #define  APP_METHOD_LOGIN "login"
    #define  APP_METHOD_LOCATE_SYSTEM_CONTROLLER "locate_system_controller"

    #define  RPARAM_HOSTNAME "HostName"
    #define  RPARAM_PORTNUMBER "PortNumber"

    #define PMODEL_DATA_CHANGE_SESSION_ID "session_id_change"
    
    #define WM_SHUTDOWN_SYSTEM_CONTROLLER (WM_USER + 1)

    #define PMODEL_CHANGE_OPERATIOIN_MODE_DLG "ChangeOperationModeDlgModel"
    #define PMODEL_OPTIONS_DLG "OptionsDlgModel"
    #define PMODEL_PROCESS_LIST_CTRL "ProcessListCtrlModel"
    #define PMODEL_PROCESS_PARAMS_DLG "ProcessParamsDlgModel"
    #define PMODEL_SHUTDOWN_SYSTEM_CONTROLLER_DLG "ShutdownSystemControllerDlgModel"
    #define PMODEL_START_PROCESS_DLG "StartProcessDlgModel"
    #define PMODEL_STOP_PROCESS_DLG "StopProcessDlgModel"
    #define PMODEL_SYSTEM_CONTROLLER_DLG "SystemControllerDlgModel"
    #define PMODEL_SYSTEM_MANAGER_DLG "SystemManagerDlgModel"
    #define PMODEL_TRANSACTIVE_LOGIN_DLG "TransActiveLoginDlgModel"

    const int APP_NOTIFY_ERROR_STOP_PROCESS = 1;  ///<  error happen when stop process
    const int APP_NOTIFY_ERROR_START_PROCESS = 2;  ///<  error happen when stop process
    const int APP_NOTIFY_ERROR_SET_PARAM = 3;  ///<  error happen when set process parameter
    const int APP_NOTIFY_ERROR_CHANGE_OPMODE = 4;  ///<  error happen when change operation mode
    
}

#endif
