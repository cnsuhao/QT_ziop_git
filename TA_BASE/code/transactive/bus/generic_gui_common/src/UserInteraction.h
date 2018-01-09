#ifndef _USER_INTERACTION_H__
#define _USER_INTERACTION_H__

#include "bus/generic_gui_common/src/PModelDataType.h"
#include "boost/shared_ptr.hpp"
#include <string>
#include <sstream>
#include <vector>

namespace TA_Base_Bus
{
	const unsigned int PARAM_ENCODE_NONE_0 = 0;  ///< no input parameter
	const unsigned int PARAM_ENCODE_STR_1 = 1; ///< has one parameter, which is string data type
	const unsigned int PARAM_ENCODE_INT_1 = 2; ///< has one parameter, which is int data type


	const unsigned int NOTIFY_TYPE_CONFIG_ENTITY_DATA_DELETE = 1;  ///< notify the user that config entity data has been deleted 

	/** the data context related to UI Notification 
	 *	
	 */
	class UINotificationDataContext
	{
	public:
		
		/** Constructor function for UINotificationDataContext
		 * 
		 *
		 *  @param paramEncode
		 *  @param transActiveMessageID
		 *  @return 
		 */
		UINotificationDataContext(unsigned int paramEncode, unsigned int transActiveMessageID = 0)
			: m_paramEncode(paramEncode)
			, m_transMsgId(transActiveMessageID)
		{
		}

		/** destructor for UINotificationDataContext
		 * 
		 *
		 *  @return 
		 */
		virtual ~UINotificationDataContext() {};

		/** push one param to the vector  
		 *	
		 */
		void pushParamValue(const std::string & strValue)
		{
			m_vtParamValue.push_back(strValue);
		}

		/** clear all parameters of UI Notification data structure 
		 *	
		 */
		void clearAllParams()
		{
			m_vtParamValue.clear();
		}

		/** return the param value based on index
		 * 
		 *
		 *  @param index
		 *  @return std::string
		 */
		std::string getParamValue(unsigned int index)
		{
			if (index >= 0 && index < m_vtParamValue.size() )
			{
				return m_vtParamValue[index];
			}
			return "";
		}

		/** return the indication of the content for parameter values
		 * 
		 *
		 *  @return unsigned int
		 */
		unsigned int getParamEncode()
		{
			return m_paramEncode;
		}

		/** return the corresponding TransActive message ID 
		 *	
		 */
		unsigned int getTransActiveMessageId()
		{
			return m_transMsgId;
		}

	private:
		std::vector<std::string> m_vtParamValue; // the parameter values for UI notification
		unsigned int m_transMsgId;   ///<  The corresponding TransActive message ID 
		unsigned int m_paramEncode;  ///<  indication of the content of parameter value pair
	};

	typedef boost::shared_ptr<UINotificationDataContext> UINotificationDataContextPtr;  ///<  shared pointer type of UINotificationDataContext 


	/**  callback function, the view can call this function to notify PModel  
	 *	
	 */
	typedef void (*UINotifyCallBack)(UINotificationDataContextPtr context); 

	/** the EventArgs type for UI Notification
	 *	it means the view just process notification, no need to reply
	 */
	class UINotificationEventArgs : public EventArgs
	{
	public:
		/** constructor function for UINotificationEventArgs
		 * 
		 *
		 *  @param context
		 *  @param typeCode
		 *  @return 
		 */
		UINotificationEventArgs(UINotificationDataContextPtr context, int typeCode)
			: EventArgs(typeCode)
			, m_dataContext(context)
			, m_callback(NULL)
		{

		}

		/** set CallBack function, so view can notify the event sender reversely
		 * 
		 *
		 *  @param callBack
		 *  @return void
		 */
		void setNotifyCallBack(UINotifyCallBack callBack)
		{
			m_callback = callBack;
		}

		/** get the data context of UI Notification 
		 *	
		 */
		UINotificationDataContextPtr getMyDataContext()
		{
			return m_dataContext;
		}

	private:
		UINotificationDataContextPtr m_dataContext;  ///< data context for UINotification 
		UINotifyCallBack m_callback; ///< callback function for UINotify
	};


	typedef boost::shared_ptr<UINotificationEventArgs> UINotificationEventArgsPtr; ///<  shared pointer type of UINotificationEventArgs

	/** The service center for User Notification, it can used to receive all user notification
	 *	
	 */
	class IUserNotificationServiceCenter
	{
	public:
		/** destructor function for IUserNotificationServiceCenter
		 * 
		 *
		 *  @return 
		 */
		virtual ~IUserNotificationServiceCenter() {};

		/** process user notification according different EventArgs
		 *  NOTED: should not block main UI thread when process the UI notification
		 *
		 *  @param uiEvent
		 *  @return void
		 */
		virtual void processUserNotification(UINotificationEventArgsPtr uiEvent) = 0;


	};


	/** the EventArgs type for UI Reply 
	 *  it means the view need reply after processing the user notification	
	 */
	class UIReplyEventArgs : public UINotificationEventArgs
	{
	public:
		
		/** constructor function for UIReplyEventArgs
		 * 
		 *
		 *  @param context
		 *  @param typeCode
		 *  @return 
		 */
		UIReplyEventArgs(UINotificationDataContextPtr context, int typeCode)
			: UINotificationEventArgs(context, typeCode)
		{
			m_replyType = UIR_IDNONE;;
		}

		/** return UI Reply type
		 *	
		 */
		UIReplyType getUIReplyType()
		{
			return m_replyType;
		}

		/** set UI Reply type
		 *	
		 */
		void setUIReplyType(UIReplyType replyType)
		{
			m_replyType = replyType;
		}
	protected:
		UIReplyType m_replyType;  ///< UI reply type
	};

	typedef boost::shared_ptr<UIReplyEventArgs> UIReplyEventArgsPtr;  ///< shared pointer type for UIReplyEventArgs


	/** the interface for User Interactive reply 
	 *	it means the view need reply after processing the notification	
	 */
	class IUserInteractiveReply
	{
	public:
		/** virtual function for IUserInteractiveReply
		 * 
		 *
		 *  @return 
		 */
		virtual ~IUserInteractiveReply() {};

		/** tell the view to process the ui notification and do proper reply
		 * 
		 *
		 *  @param uiEvent
		 *  @return void
		 */
		virtual void requestUserReply(TA_Base_Bus::UIReplyEventArgsPtr uiEvent) = 0;
	};
}

#endif
