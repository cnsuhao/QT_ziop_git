#pragma once


namespace TA_Base_Bus
{

    class IOptionalProperty
    {
    public:

        virtual ~IOptionalProperty() {}
        virtual void applyWnd( CWnd* pwnd ) = 0;
    };

    typedef boost::shared_ptr<IOptionalProperty> IOptionalPropertyPtr;

}
