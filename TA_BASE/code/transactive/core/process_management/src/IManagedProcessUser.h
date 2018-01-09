/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution in any form.
  * 
  * Source:    $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/core/process_management/src/IManagedProcessUser.h $
  * @author:   Ross Tucker
  * @version:  $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last mofified by:  $Author: CM $
  *
  * This abstract interface should be implemented by any class that uses
  * a ManagedProcess
  */

#if !defined(IMANAGEDPROCESSUSER)
#define IMANAGEDPROCESSUSER

namespace TA_Base_Core
{
    class IManagedProcessUser
    {
    public:
        /**
          * Destructor
          */
        virtual ~IManagedProcessUser(){}

        /**
          * onTerminate
          *
          * This method ensures that ManagedProcess can inform
          * all ManagedProcessUsers when the they are required to
          * terminate.
          *
          * @exception TODO
          */
        virtual void onTerminate() = 0;
    };
}

#endif // !defined(IMANAGEDPROCESSUSER)
