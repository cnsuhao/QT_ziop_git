#pragma once
#include "bus/language/src/IDialogResource.h"
#include "bus/language/src/IDialogItemResource.h"


namespace TA_Base_Bus
{

    class DialogResource : public IDialogResource
    {
    public:

        DialogResource( UINT id, const CString& caption );

        virtual void applyDialog( CDialog* pDialog );
        virtual void addOptionalProperty( IOptionalPropertyPtr optionalProperty );
        virtual void addDialogItemResource( IDialogItemResourcePtr dialogItemResource );
        virtual const CString& getCaption() { return m_caption; }

    private:

        void setWindowTextForPropertyPage( CPropertyPage* pPropertyPage );

    private:

        UINT m_id;
        CString m_caption;
        std::vector<IDialogItemResourcePtr> m_dialogItemResourceList;
        std::vector<IOptionalPropertyPtr> m_optionalPropertyList;
    };

    typedef boost::shared_ptr<DialogResource> DialogResourcePtr;

}
