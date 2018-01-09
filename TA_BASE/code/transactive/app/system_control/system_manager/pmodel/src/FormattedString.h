#pragma once
#include <string>
#include <vector>
#include <boost/lexical_cast.hpp>
#include <boost/assign/std/vector.hpp>

namespace TA_Base_App
{
    using namespace boost::assign;

    struct FormattedString
    {
        typedef const std::string& F;
        FormattedString() {}
        FormattedString(F f) : format(f) {}
        FormattedString(F f, const std::vector<std::string>& ps) : format(f), params(ps) {}
        FormattedString(F f, F p1) : format(f) { params.push_back(p1); }
        FormattedString(F f, F p1, F p2) : format(f) { params += p1, p2; }
        FormattedString(F f, F p1, F p2, F p3) : format(f) { params += p1, p2, p3; }
        FormattedString(F f, F p1, F p2, F p3, F p4) : format(f) { params += p1, p2, p3, p4; }
        FormattedString(F f, F p1, F p2, F p3, F p4, F p5) : format(f) { params += p1, p2, p3, p4, p5; }

        template<typename P1>
        FormattedString(F f, const P1& p1) : format(f)
        {
            params.push_back(boost::lexical_cast<std::string>(p1));
        }

        template<typename P1, typename P2>
        FormattedString(F f, const P1& p1, const P2& p2) : format(f)
        {
            params.push_back(boost::lexical_cast<std::string>(p1));
            params.push_back(boost::lexical_cast<std::string>(p2));
        }

        template<typename P1, typename P2, typename P3>
        FormattedString(F f, const P1& p1, const P2& p2, const P3& p3) : format(f)
        {
            params.push_back(boost::lexical_cast<std::string>(p1));
            params.push_back(boost::lexical_cast<std::string>(p2));
            params.push_back(boost::lexical_cast<std::string>(p3));
        }

        template<typename P1, typename P2, typename P3, typename P4>
        FormattedString(F f, const P1& p1, const P2& p2, const P3& p3, const P4& p4) : format(f)
        {
            params.push_back(boost::lexical_cast<std::string>(p1));
            params.push_back(boost::lexical_cast<std::string>(p2));
            params.push_back(boost::lexical_cast<std::string>(p3));
            params.push_back(boost::lexical_cast<std::string>(p4));
        }

        template<typename P1, typename P2, typename P3, typename P4, typename P5>
        FormattedString(F f, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5) : format(f)
        {
            params.push_back(boost::lexical_cast<std::string>(p1));
            params.push_back(boost::lexical_cast<std::string>(p2));
            params.push_back(boost::lexical_cast<std::string>(p3));
            params.push_back(boost::lexical_cast<std::string>(p4));
            params.push_back(boost::lexical_cast<std::string>(p5));
        }

        FormattedString& setParam(size_t index, const std::string& value)
        {
            params[index] = value;
            return *this;
        }

        bool operator == (const FormattedString& rhs) const
        {
            return format == rhs.format && params == rhs.params;
        }

        bool operator != (const FormattedString& rhs) const
        {
            return !(*this == rhs);
        }

        bool operator < (const FormattedString& rhs) const
        {
            return format < rhs.format || (format == rhs.format && params < rhs.params);
        }

        FormattedString& operator = (const std::string& f)
        {
            format = f;
            return *this;
        }

        std::string format;
        std::vector<std::string> params;
    };
}
