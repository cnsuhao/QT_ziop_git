#if !defined(SECURITYACCESSFACTORY_A16AAC60_54BE_4756_B682_2C67E80990D6__INCLUDED_)
#define SECURITYACCESSFACTORY_A16AAC60_54BE_4756_B682_2C67E80990D6__INCLUDED_

#include "core/data_access_interface/src/IDatabase.h"
#include "core/data_access_interface/src/DatabaseFactory.h"
#include <boost/shared_ptr.hpp>


namespace TA_Base_Core
{
    struct SecuritySetting
    {
        int m_automatic_logout_time;
        int m_failed_login_attempts;
        int m_password_reuse_max;
        int m_password_warn_time;
        int m_password_reuse_time;
        int m_password_life_time;
        int m_password_lock_time;
    };

    typedef boost::shared_ptr<SecuritySetting> SecuritySettingPtr;

    class SecuritySettingAccessFactory
    {
    public:

        virtual ~SecuritySettingAccessFactory()
        {
        }

        /**
          * getInstance
          *
          * Creates and returns an instance of this object.
          *
          * @return A reference to an instance of an SessionAccessFactory object.
          */
        static SecuritySettingAccessFactory& getInstance();

        virtual int getAutomaticLogoutTime();
        virtual int getFailedLoginAttemtps();
        virtual int getPasswordReuseMax();
        virtual int getPasswordWarnTime();
        virtual int getPasswordReuseTime();
        virtual int getPasswordLifeTime();
        virtual int getPasswordLockTime();
        virtual SecuritySettingPtr getSetting();

    private:

        //
        // These are private as this method is a singleton
        //
        SecuritySettingAccessFactory();
        SecuritySettingAccessFactory(const SecuritySettingAccessFactory& theSecuritySettingAccessFactory);
        SecuritySettingAccessFactory& operator=(const SecuritySettingAccessFactory &);

        virtual void reload();

        static SecuritySettingAccessFactory* m_instance;

        int m_AUTOMATIC_LOGOUT_TIME;
        int m_FAILED_LOGIN_ATTEMPTS;
        int m_PASSWORD_REUSE_MAX;
        int m_PASSWORD_WARN_TIME;
        int m_PASSWORD_REUSE_TIME;
        int m_PASSWORD_LIFE_TIME;
        int m_PASSWORD_LOCK_TIME;
        bool m_hasLoad;
        SecuritySettingPtr m_setting;
    };
} // closes TA_Base_Core

#endif // !defined(SessionAccessFactory_A16AAC60_54BE_4756_B682_2C67E80990D6__INCLUDED_)
