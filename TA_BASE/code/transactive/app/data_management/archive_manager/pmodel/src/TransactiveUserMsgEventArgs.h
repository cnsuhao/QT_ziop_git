/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File:  $
  * @author:  
  * @version: $Revision:  $
  *
  * Last modification: $DateTime:  $
  * Last modified by:  $Author:  $
  *
  * 
  *
  */

#ifndef TRASNACTIVE_USER_MESSAGE__EVENT_ARGS_1234567890_H
#define TRASNACTIVE_USER_MESSAGE__EVENT_ARGS_1234567890_H
#include "bus/generic_gui_pmod/src/CommandGateway.h"

namespace TA_Base_App
{
	class TransactiveUserMsgEventArgs: public TA_Base_Bus::EventArgs
	{
	public:
		TransactiveUserMsgEventArgs():TA_Base_Bus::EventArgs()
		{

		}

		TransactiveUserMsgEventArgs(int uID):TA_Base_Bus::EventArgs()
		{
			m_uid = uID;
			m_args1Defined = false;	
			m_args2Defined = false;
			m_args3Defined = false;
		}

		TransactiveUserMsgEventArgs(int uID, std::string args1):TA_Base_Bus::EventArgs()
		{
			m_uid = uID;
			m_args1Defined = true;	
			m_args1 = args1;
			m_args2Defined = false;
			m_args3Defined = false;
		}

		TransactiveUserMsgEventArgs(int uID, std::string args1, std::string args2):TA_Base_Bus::EventArgs()
		{
			m_uid = uID;
			m_args1Defined = true;	
			m_args1 = args1;
			m_args2Defined = true;
			m_args2= args2;
			m_args3Defined = false;
		}

		
		TransactiveUserMsgEventArgs(int uID, std::string args1, std::string args2,std::string args3):TA_Base_Bus::EventArgs()
		{
			m_uid = uID;
			m_args1Defined = true;	
			m_args1 = args1;
			m_args2Defined = true;
			m_args2= args2;
			m_args3Defined = true;
			m_args3= args3;
		}

		~TransactiveUserMsgEventArgs()
		{
		}

		int getUID()
		{
			return m_uid;
		}

		void setUID(int uid)
		{
			m_uid= uid;
		}

		UINT getReturnValue()
		{
			return m_return;
		}

		void setReturnValue(UINT value)
		{
			m_return = value;
		}

		void setArgs1(std::string args)
		{
			m_args1Defined = true;
			m_args1 = args;
		}

		std::string getArgs1()
		{
			return m_args1;
		}

		bool getArgs1Defined()
		{
			return m_args1Defined;
		}

		std::string getArgs2()
		{
			return m_args2;
		}

		bool getArgs2Defined()
		{
			return m_args2Defined;
		}

		std::string getArgs3()
		{
			return m_args3;
		}

		bool getArgs3Defined()
		{
			return m_args3Defined;
		}

		std::string getCaption()
		{
			return m_caption;
		}

		void setCaption(std::string value)
		{
			m_caption = value;
		}


	private:

		int m_uid;	
		UINT m_return;
		bool m_args1Defined;
		bool m_args2Defined;
		bool m_args3Defined;
		std::string m_args1;
		std::string m_args2;
		std::string m_args3;
		std::string m_caption;

	};
}

#endif  // TRASNACTIVE_USER_MESSAGE__EVENT_ARGS_1234567890_H
