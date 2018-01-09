#pragma once

#include "bus/generic_gui_pmod/src/TransActiveViewModel.h"
#include "core/message/types/OperatorAudit_MessageTypes.h"

namespace TA_Base_App
{

	class SystemManagerDlgModel
        : public TA_Base_Bus::AbstractPModel
	{
	public:
		SystemManagerDlgModel(TA_Base_Bus::IApplicationController* appController, const char * pmodelName);
		virtual ~SystemManagerDlgModel(void);
    public:
        /**
          * notifyUpdate
          * 
          * Calls the notifyUpdate() function of the AdminManager to retrieve the 
          * up-to-date process data.
          */
        virtual void notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args=NULL);

		virtual void sessionIdChanged();

	public:
		std::string getSessionId();
        void refreshData();
        void registerUpdate();
        void unregisterUpdate();
        bool checkPermissions(std::vector<UINT> & vtErrorIdList);
		std::string getSystemControllerHostName();

		/**
		* login
		*
		* Starts a System Manager session.
		*
		* @return error code:
		*         ERR_AUTH_SOCKET_ERROR           Cannot identify local console
		*         ERR_AUTH_SERVICE_ERROR          Authentication service error
		*         ERR_NO_SYSTEM_CONTROLLER_ERROR  No System Controller
		*         ERR_GET_ADMIN_ERROR             Cannot get System Controller
		*         ERR_AUTH_NO_ERROR               No error
		*
		* @param hostName      Hostname of the System Controller to log into.
		* @param portNumber    Port number the System Controller is running on.
		* @param sessionId     The session ID for this login.
		* @param operatorName  The name of the operator.
		*/
		int login(const std::string& hostName,
			const std::string& portNumber,
			const std::string& sessionId,
			const std::string& operatorName);
		/**
		* logout
		*
		* Terminates a session.
		*/
		void logout();

		/**
		* getProcesses
		*
		* Returns the process information last retrieved from the System 
		* Controller.
		*
		* @return a vector of ProcessData structs.
		*/
		std::vector<TA_Base_Core::ProcessData> getProcesses();

		bool isServiceProcessFromName(const std::string& entityName) const;
		bool isServiceProcessFromDescription(const std::string& entityDescription) const;
		bool isAutoFailBackFromDescription(const std::string& entityDescription) const;

		bool isManagedProcessFromDescription(const std::string& entityDescription) const;

		/**
		* shutdownSystemController
		*
		* Ask the System Controller to shut down.
		*
		* @exception SystemControllerException  An error occurred communicating
		*                                       with the System Controller.
		*/
		void shutdownSystemController();
		int doLogin(int & errorId, const std::string & sessionId, const std::string & operatorName);
		void sendAuditMessage(const std::string & sessionId, const TA_Base_Core::MessageType & msgType);
	public:
        /**
          * Whether or not all of the access controlled functionality is
          * allowed for the currently logged in user.
          */
        bool m_isStartProcessAllowed;
        bool m_isStopProcessAllowed;
        bool m_isSetProcessParamAllowed;
        bool m_isShutdownAllowed;
        bool m_isSetOperatingModeAllowed;
        bool m_isSetOptionsAllowed;

		/**
		* True if the user is logged in.
		*/
		bool m_userLoggedIn;
	};

}