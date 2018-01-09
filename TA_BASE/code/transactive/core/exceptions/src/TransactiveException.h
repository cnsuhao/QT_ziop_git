/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/core/exceptions/src/TransactiveException.h $
  * @author:  Ripple
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * This class is at the top of the exception hierarchy. All
  * exception classes should be derived from this class, or
  * from a class that in turn is derived from this class.
  */

#if !defined(AFX_TransactiveException_H__E2DE1FE6_479C_11D7_B150_0080C8E32E69__INCLUDED_)
#define AFX_TransactiveException_H__E2DE1FE6_479C_11D7_B150_0080C8E32E69__INCLUDED_

#include <exception>
#include <string>
#include <memory>
#include <sstream>
#include <typeinfo>

/**
  * TA_THROW
  *
  * This macro throws exceptions while providing logging. It takes the
  * place of the throw keyword, and is used identically.
  * eg: TA_THROW(CCTVException("A CCTV error occurred"));
  *
  * @param exc  The exception (with parameters) to throw
  */
#define TA_THROW(exc)															\
{																				\
	LOG(SourceInfo,																\
		TA_Base_Core::DebugUtil::ExceptionConstruction,			                \
		std::string(#exc).substr(0, (std::string(#exc).find("("))).c_str(),		\
		(exc).what());															\
	throw exc;																	\
}

#define TA_THROW2(exc)															\
{																				\
    const TA_Base_Core::TransactiveException& ref = exc;                        \
	LOG(SourceInfo,																\
		TA_Base_Core::DebugUtil::ExceptionConstruction,			                \
        typeid(ref).name(),                                                     \
        ref.what());															\
	throw ref;																	\
}

namespace TA_Base_Core
{
    class TransactiveException : public std::exception
    {
    public:

        TransactiveException() {}
        TransactiveException(const std::string& msg);

        virtual const char* what() const;
        virtual ~TransactiveException() {}

        template<typename T>
        TransactiveException& operator << (const T& msg)
        {
            std::stringstream strm;
            strm << m_msg << msg;
            m_msg = strm.str();
            return *this;
        }

    protected:

        // Description of the reason for the exception.  It is protected (rather than
        // private) so that derived classes can build the message in their constructor
        // after the member initialisation list.
        mutable std::string m_msg;
    };

    struct UnimplementException: TransactiveException
    {
        UnimplementException() {}
        UnimplementException(const std::string& msg);
        virtual ~UnimplementException() {}
    };
}

template<typename R, typename P1 = char*, typename P2 = char*, typename P3 = char*, typename P4 = char*, typename P5 = char*>
R makeTransactiveException(const P1& s1 = "", const P2& s2 = "", const P3& s3 = "", const P4& s4 = "", const P5& s5 = "")
{
    std::stringstream strm;
    strm << s1 << s2 << s3 << s4 << s5;
    return R(strm.str().c_str());
}

template<typename R, typename T, typename P1 = char*, typename P2 = char*, typename P3 = char*, typename P4 = char*, typename P5 = char*>
R makeTransactiveException1(const T& p, const P1& s1 = "", const P2& s2 = "", const P3& s3 = "", const P4& s4 = "", const P5& s5 = "")
{
    std::stringstream strm;
    strm << s1 << s2 << s3 << s4 << s5;
    return R(p, strm.str().c_str());
}

template<typename R, typename T1, typename T2, typename P1 = char*, typename P2 = char*, typename P3 = char*, typename P4 = char*, typename P5 = char*>
R makeTransactiveException2(const T1& p1, const T2& p2, const P1& s1 = "", const P2& s2 = "", const P3& s3 = "", const P4& s4 = "", const P5& s5 = "")
{
    std::stringstream strm;
    strm << s1 << s2 << s3 << s4 << s5;
    return R(p1, p2, strm.str().c_str());
}

#endif // !defined(AFX_TransactiveException_H__E2DE1FE6_479C_11D7_B150_0080C8E32E69__INCLUDED_)
