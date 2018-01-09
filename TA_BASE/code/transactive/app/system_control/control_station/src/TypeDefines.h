#pragma once
#include <boost/preprocessor.hpp>

namespace TA_Base_Core
{
#define TYPE_DEFINE_SHARED_CLASS(n) \
    class n; \
    typedef boost::shared_ptr<n> BOOST_PP_CAT(n, Ptr); \
    typedef std::vector<BOOST_PP_CAT(n,*)> BOOST_PP_CAT(n, List); \
    typedef std::vector<BOOST_PP_CAT(n, Ptr)> BOOST_PP_CAT(n, PtrList);
#define MACRO(r, data, elem) TYPE_DEFINE_SHARED_CLASS(elem)
#define CLASS_LIST (IOperator)(IConsole)(IProfile)(ILocation)(ControlStation)(AuditMessageSender)(IEntityData)(IGui)
    BOOST_PP_SEQ_FOR_EACH(MACRO,, CLASS_LIST);
#undef MACRO
#undef CLASS_LIST
#undef TYPE_DEFINE_SHARED_CLASS
}
