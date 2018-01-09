#include "PaAgentEntityNotFoundException.h"

namespace TA_Base_Bus
{


    PaAgentEntityNotFoundException::PaAgentEntityNotFoundException() throw():
    TA_Base_Core::TransactiveException()
    {

    }

    PaAgentEntityNotFoundException::PaAgentEntityNotFoundException( std::string msg ) throw():
    TA_Base_Core::TransactiveException()
    {

    }

    PaAgentEntityNotFoundException::~PaAgentEntityNotFoundException() throw()
    {

    }

}