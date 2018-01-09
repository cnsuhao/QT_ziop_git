#include "OptionsDlgModelService.h"
#include "SystemControllerService.h"
#include "Factory.h"


namespace TA_Base_App
{
    std::vector<std::string> OptionsDlgModelService::getRefereshRates()
    {
        static const char* const str[ ] =
        {
            "1", "2", "3", "4", "5", "6", "7", "8", "9", "10",
            "11", "12", "13", "14", "15", "20", "25", "30",
            "40", "50", "60", "90", "120", "150", "180",
            "240", "300", "600", "900", "1200", "1800", "3600", "43200", "86400"
        };
        static std::vector<std::string> rates(str, str + sizeof(str) / sizeof(char*));
        return rates;
    }


    void OptionsDlgModelService::onSetRefreshRate(size_t rate)
    {
        Factory::instance().getSystemControllerService()->setRefreshRate(rate);
    }
}
