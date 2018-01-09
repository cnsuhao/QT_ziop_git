#if !defined(AFX_SYSTEMMANAGER_H__DDF277E9_975E_4C52_8E47_A00562AE406D__INCLUDED_1)
#define AFX_SYSTEMMANAGER_H__DDF277E9_975E_4C52_8E47_A00562AE406D__INCLUDED_1

#include "bus/generic_gui_view/src/TransActiveWinApp.h"

namespace TA_Base_Bus
{
    class GenericGUI; // Forward declaration
}

namespace TA_Base_App
{

    class myApplication : public TA_Base_Bus::TransActiveWinApp
    {

    public:
        myApplication(int argc, char *argv[]);
        virtual ~myApplication(){};

        TA_Base_Bus::IGUIApplicationController * createApplicationController();

        virtual TA_Base_Bus::ITransActiveView* getMainView();
        virtual TA_Base_Bus::ITransActiveView* createMainView();


         //void invoke(const char * methodName, const TA_Base_Bus::NameValueParams & params);

        // virtual void requestUserReply(TA_Base_Bus::UIReplyEventArgsPtr uiEvent);
    private:

         /**
          * runWithoutControlStation
          *
          * If the System Manager cannot connect to the Control Station then it must attempt to
          * run via other means. This method attempts to achieve this.
          *
          * @return This returns a boolean indicating whether the application should enter
          * the application message pump:
          *    True for Yes
          *    False for No
          */
        bool runWithoutControlStation();


    };

} // namespace TA_Base_App

#endif // !defined(AFX_SYSTEMMANAGER_H__DDF277E9_975E_4C52_8E47_A00562AE406D__INCLUDED_)