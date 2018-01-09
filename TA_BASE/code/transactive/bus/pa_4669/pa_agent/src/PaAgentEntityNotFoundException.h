#if !defined(_PAAGENTENTITYNOTFOUNDEXCEPTION__INCLUDED_)
#define _PAAGENTENTITYNOTFOUNDEXCEPTION__INCLUDED_

#include "core/exceptions/src/TransactiveException.h"


namespace TA_Base_Bus
{
    class PaAgentEntityNotFoundException : public TA_Base_Core::TransactiveException
    {
        // Operations
    public:
        PaAgentEntityNotFoundException() throw();
        PaAgentEntityNotFoundException(std::string msg) throw();
        virtual ~PaAgentEntityNotFoundException() throw();
    };

} // namespace TA_Base_Bus

#endif // !defined(_PAAGENTENTITYNOTFOUNDEXCEPTION__INCLUDED_)