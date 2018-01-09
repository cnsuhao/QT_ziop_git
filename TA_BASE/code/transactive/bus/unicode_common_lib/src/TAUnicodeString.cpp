//#include <cstringt.h>
#include <string>
#include "TAUnicodeString.h"

#define  fromMultipleByte(x) QString::fromUtf8(x)
#define  toMultipleByte(x) x.toUtf8()

namespace TA_Base_Bus
{
#ifdef WIN32


    TAUtfString::TAUtfString()
    {
    }

    TAUtfString::TAUtfString(const char* value)
    {
        m_strValue = fromMultipleByte(value);
    }

    TAUtfString::TAUtfString(const QChar*  value)
    {
        m_strValue = QString(value);
    }

    TAUtfString::TAUtfString(const std::string& value)
    {
        m_strValue = fromMultipleByte(value.c_str());
    }

    TAUtfString::TAUtfString(const QString&  value)
    {
        m_strValue = value;
    }

    TAUtfString::TAUtfString(const TAUtfString& value)
        : m_strValue(value.q_str())
    {
    }


    TAUtfString::~TAUtfString()
    {
    }

    bool TAUtfString::operator==(const TAUtfString& strRight) const
    {
        return m_strValue == strRight.m_strValue;
    }
    bool TAUtfString::operator==(const char* strRight) const
    {
        QString qstrRight(strRight);
        return m_strValue == qstrRight;
    }


    bool TAUtfString::operator!=(const TAUtfString& strRight) const
    {
        return m_strValue != strRight.m_strValue;
    }

    bool TAUtfString::operator!=(const char* strRight) const
    {
        QString qstrRight(strRight);
        return m_strValue != qstrRight;
    }

    TAUtfString& TAUtfString::operator=(const TAUtfString& strSource)
    {
        m_strValue = strSource.q_str();
        return *this;
    }

    TAUtfString& TAUtfString::operator=(const QString& strSource)
    {
        m_strValue = strSource;
        return *this;
    }
    TAUtfString& TAUtfString::operator=(const std::string& strSource)
    {
        m_strValue = fromMultipleByte(strSource.c_str());
        return *this;
    }

    TAUtfString& TAUtfString::operator=(const char* strSource)
    {
        m_strValue = fromMultipleByte(strSource);
        return *this;
    }

    TAUtfString  TAUtfString::operator+(const TAUtfString& strSource)
    {
        QString newStr = m_strValue + strSource.q_str();
        return TAUtfString(newStr);
    }
    TAUtfString  TAUtfString::operator+(const QString& strSource)
    {
        QString newStr = m_strValue + strSource;
        return TAUtfString(newStr);
    }
    TAUtfString  TAUtfString::operator+(const std::string& strSource)
    {
        QString qstrSource = fromMultipleByte(strSource.c_str());
        QString newStr = m_strValue + qstrSource;
        return TAUtfString(newStr);
    }
    TAUtfString  TAUtfString::operator+(const char* strSource)
    {
        QString qstrSource = fromMultipleByte(strSource);
        QString newStr = m_strValue + qstrSource;
        return TAUtfString(newStr);
    }

    TAUtfString& TAUtfString::operator+=(const TAUtfString& strSource)
    {
        m_strValue.append(strSource.q_str());
        return *this;
    }

    TAUtfString& TAUtfString::operator+=(const QString& strSource)
    {
        m_strValue.append(strSource);
        return *this;
    }

    TAUtfString& TAUtfString::operator+=(const std::string& strSource)
    {
        QString qstrSource = fromMultipleByte(strSource.c_str());
        m_strValue.append(qstrSource);
        return *this;
    }

    TAUtfString& TAUtfString::operator+=(const char* strSource)
    {
        QString qstrSource = fromMultipleByte(strSource);
        m_strValue.append(qstrSource);
        return *this;
    }

    /*std::istream &	operator>>(std::istream & ostream, const TAUtfString & str)
    {
    ostream >> str.toMultiByteStdString();
    return ostream;
    }*/

    std::ostream&   operator<<(std::ostream& out, const TAUtfString& str)
    {
        out << str.toMultiByteStdString();
        return out;
    }



    TAUtfString::operator QString() const
    {
        return m_strValue;
    }
    TAUtfString::operator std::string() const
    {
        const QByteArray asc = toMultipleByte(m_strValue); return std::string(asc.constData(), asc.length());
    }

    QString TAUtfString::q_str() const
    {
        return m_strValue;
    }

    std::string  TAUtfString::std_str()
    {
        const QByteArray asc = toMultipleByte(m_strValue); return std::string(asc.constData(), asc.length());
    }


    TAUtfString& 	TAUtfString::append(const QString& str)
    {
        m_strValue.append(str);
        return *this;
    }
    TAUtfString& 	TAUtfString::append(const std::string& str)
    {
        QString qstr = TAUtfString::createFromMultitByteString(str).q_str();
        m_strValue.append(qstr);
        return *this;
    }
    TAUtfString& 	TAUtfString::append(const TAUtfString& str)
    {
        m_strValue.append(str.q_str());
        return *this;
    }

    const QChar	TAUtfString::at(int position) const
    {
        return m_strValue.at(position);
    }

    void TAUtfString::chop(int n)
    {
        m_strValue.chop(n);
    }

    int	TAUtfString::compare(const QString& other, bool caseSensitivity) const
    {
        Qt::CaseSensitivity sensitivity = Qt::CaseSensitive;

        if (sensitivity == false) sensitivity = Qt::CaseInsensitive;

        return m_strValue.compare(other, sensitivity);
    }
    int	TAUtfString::compare(const std::string& other, bool caseSensitivity) const
    {
        QString qstr = TAUtfString::createFromMultitByteString(other).q_str();
        Qt::CaseSensitivity sensitivity = Qt::CaseSensitive;

        if (sensitivity == false) sensitivity = Qt::CaseInsensitive;

        return m_strValue.compare(qstr, sensitivity);
    }
    int	TAUtfString::compare(const TAUtfString& other, bool caseSensitivity) const
    {
        Qt::CaseSensitivity sensitivity = Qt::CaseSensitive;

        if (sensitivity == false) sensitivity = Qt::CaseInsensitive;

        return m_strValue.compare(other.q_str(), sensitivity);
    }

    bool TAUtfString::contains(const QString& str, bool caseSensitivity) const
    {
        Qt::CaseSensitivity sensitivity = Qt::CaseSensitive;

        if (sensitivity == false) sensitivity = Qt::CaseInsensitive;

        return m_strValue.contains(str, sensitivity);
    }
    bool TAUtfString::contains(const std::string& str, bool caseSensitivity) const
    {
        QString qstr = TAUtfString::createFromMultitByteString(str).q_str();
        Qt::CaseSensitivity sensitivity = Qt::CaseSensitive;

        if (sensitivity == false) sensitivity = Qt::CaseInsensitive;

        return m_strValue.contains(qstr, sensitivity);
    }
    bool TAUtfString::contains(const TAUtfString& str, bool caseSensitivity) const
    {
        Qt::CaseSensitivity sensitivity = Qt::CaseSensitive;

        if (sensitivity == false) sensitivity = Qt::CaseInsensitive;

        return m_strValue.contains(str.q_str(), sensitivity);
    }


    int	TAUtfString::count(const QString& str, bool caseSensitivity) const
    {
        Qt::CaseSensitivity sensitivity = Qt::CaseSensitive;

        if (sensitivity == false) sensitivity = Qt::CaseInsensitive;

        return m_strValue.count(str, sensitivity);
    }
    int	TAUtfString::count(const std::string& str, bool caseSensitivity) const
    {
        QString qstr = TAUtfString::createFromMultitByteString(str).q_str();
        Qt::CaseSensitivity sensitivity = Qt::CaseSensitive;

        if (sensitivity == false) sensitivity = Qt::CaseInsensitive;

        return m_strValue.count(qstr, sensitivity);
    }
    int	TAUtfString::count(const TAUtfString& str, bool caseSensitivity) const
    {
        Qt::CaseSensitivity sensitivity = Qt::CaseSensitive;

        if (sensitivity == false) sensitivity = Qt::CaseInsensitive;

        return m_strValue.count(str.q_str(), sensitivity);
    }

    const QChar* TAUtfString::data() const
    {
        return m_strValue.data();
    }


    bool  TAUtfString::endsWith(const QString& s, bool caseSensitivity) const
    {
        Qt::CaseSensitivity sensitivity = Qt::CaseSensitive;

        if (sensitivity == false) sensitivity = Qt::CaseInsensitive;

        return m_strValue.endsWith(s, sensitivity);
    }
    bool  TAUtfString::endsWith(const std::string&  s, bool caseSensitivity) const
    {
        QString qstr = TAUtfString::createFromMultitByteString(s).q_str();
        Qt::CaseSensitivity sensitivity = Qt::CaseSensitive;

        if (sensitivity == false) sensitivity = Qt::CaseInsensitive;

        return m_strValue.endsWith(qstr, sensitivity);
    }
    bool TAUtfString::endsWith(const TAUtfString&  s, bool caseSensitivity) const
    {
        Qt::CaseSensitivity sensitivity = Qt::CaseSensitive;

        if (sensitivity == false) sensitivity = Qt::CaseInsensitive;

        return m_strValue.endsWith(s.q_str(), sensitivity);
    }


    int	TAUtfString::indexOf(const QString& str, int from, bool caseSensitivity) const
    {
        Qt::CaseSensitivity sensitivity = Qt::CaseSensitive;

        if (sensitivity == false) sensitivity = Qt::CaseInsensitive;

        return m_strValue.indexOf(str, sensitivity);
    }
    int	TAUtfString::indexOf(const std::string& str, int from, bool caseSensitivity) const
    {
        QString qstr = TAUtfString::createFromMultitByteString(str).q_str();
        Qt::CaseSensitivity sensitivity = Qt::CaseSensitive;

        if (sensitivity == false) sensitivity = Qt::CaseInsensitive;

        return m_strValue.endsWith(qstr, sensitivity);
    }
    int	TAUtfString::indexOf(const TAUtfString& str, int from, bool caseSensitivity) const
    {
        Qt::CaseSensitivity sensitivity = Qt::CaseSensitive;

        if (sensitivity == false) sensitivity = Qt::CaseInsensitive;

        return m_strValue.indexOf(str.q_str(), sensitivity);
    }


    TAUtfString& 	TAUtfString::insert(int position, const QString& str)
    {
        m_strValue.insert(position, str);
        return *this;
    }
    TAUtfString& 	TAUtfString::insert(int position, const std::string& str)
    {
        QString qstr = TAUtfString::createFromMultitByteString(str).q_str();
        m_strValue.insert(position, qstr);
        return *this;
    }
    TAUtfString& 	TAUtfString::insert(int position, const TAUtfString& str)
    {
        m_strValue.insert(position, str.q_str());
        return *this;
    }

    bool TAUtfString::isEmpty() const
    {
        return m_strValue.isEmpty();
    }


    int	TAUtfString::lastIndexOf(const QString& str, int from, bool caseSensitivity) const
    {
        Qt::CaseSensitivity sensitivity = Qt::CaseSensitive;

        if (sensitivity == false) sensitivity = Qt::CaseInsensitive;

        return m_strValue.lastIndexOf(str, from, sensitivity);
    }
    int	TAUtfString::lastIndexOf(const std::string& str, int from, bool caseSensitivity) const
    {
        QString qstr = TAUtfString::createFromMultitByteString(str).q_str();
        Qt::CaseSensitivity sensitivity = Qt::CaseSensitive;

        if (sensitivity == false) sensitivity = Qt::CaseInsensitive;

        return m_strValue.lastIndexOf(qstr, from, sensitivity);
    }

    int	TAUtfString::lastIndexOf(const TAUtfString& str, int from, bool caseSensitivity) const
    {
        Qt::CaseSensitivity sensitivity = Qt::CaseSensitive;

        if (sensitivity == false) sensitivity = Qt::CaseInsensitive;

        return m_strValue.lastIndexOf(str.q_str(), from, sensitivity);
    }

    TAUtfString TAUtfString::left(int nCount) const
    {
        return TAUtfString(m_strValue.left(nCount).data());
    }

    int TAUtfString::length()
    {
        return m_strValue.length();
    }

    TAUtfString TAUtfString::mid(int iFirst, int nCount) const
    {
        return TAUtfString(m_strValue.mid(iFirst, nCount).data());
    }

    TAUtfString& 	TAUtfString::prepend(const QString& str)
    {
        m_strValue.prepend(str);
        return *this;
    }
    TAUtfString& 	TAUtfString::prepend(const std::string& str)
    {
        QString qstr = TAUtfString::createFromMultitByteString(str).q_str();
        m_strValue.prepend(qstr);
        return *this;
    }
    TAUtfString& 	TAUtfString::prepend(const TAUtfString& str)
    {
        m_strValue.prepend(str.q_str());
        return *this;
    }


    TAUtfString& 	TAUtfString::replace(int position, int n, const QString& after)
    {
        m_strValue.replace(position, after);
        return *this;
    }
    TAUtfString& 	TAUtfString::replace(int position, int n, const std::string& after)
    {
        QString qstr = TAUtfString::createFromMultitByteString(after).q_str();
        m_strValue.replace(position, qstr);
        return *this;
    }
    TAUtfString& 	TAUtfString::replace(int position, int n, const TAUtfString& after)
    {
        m_strValue.replace(position, after.q_str());
        return *this;
    }

    TAUtfString& 	TAUtfString::replace(const QString& before, QString after, bool caseSensitivity)
    {
        Qt::CaseSensitivity sensitivity = Qt::CaseSensitive;

        if (sensitivity == false) sensitivity = Qt::CaseInsensitive;

        m_strValue.replace(before, after, sensitivity);
        return *this;
    }
    TAUtfString& 	TAUtfString::replace(const std::string& before, std::string after, bool caseSensitivity)
    {
        QString qbefore = TAUtfString::createFromMultitByteString(before).q_str();
        QString qafter = TAUtfString::createFromMultitByteString(after).q_str();
        Qt::CaseSensitivity sensitivity = Qt::CaseSensitive;

        if (sensitivity == false) sensitivity = Qt::CaseInsensitive;

        m_strValue.replace(qbefore, qafter, sensitivity);
        return *this;
    }
    TAUtfString& 	TAUtfString::replace(const TAUtfString& before, TAUtfString after, bool caseSensitivity)
    {
        Qt::CaseSensitivity sensitivity = Qt::CaseSensitive;

        if (sensitivity == false) sensitivity = Qt::CaseInsensitive;

        m_strValue.replace(before.q_str(), after.q_str(), sensitivity);
        return *this;
    }



    void	TAUtfString::reserve(int size)
    {
        m_strValue.reserve(size);
    }
    void	TAUtfString::resize(int size)
    {
        //QChar c = ' ';
        m_strValue.resize(size);
    }

    /*setNum********************************************************/
    TAUtfString& TAUtfString::setNum(short num, int base)
    {
        m_strValue.setNum(num, base);
        return *this;
    }
    TAUtfString& TAUtfString::setNum(unsigned short num, int base)
    {
        m_strValue.setNum(num, base);
        return *this;
    }
    TAUtfString& TAUtfString::setNum(int num, int base)
    {
        m_strValue.setNum(num, base);
        return *this;
    }
    TAUtfString& TAUtfString::setNum(unsigned int  num, int base)
    {
        m_strValue.setNum(num, base);
        return *this;
    }
    TAUtfString& TAUtfString::setNum(long num, int base)
    {
        m_strValue.setNum(num, base);
        return *this;
    }
    TAUtfString& TAUtfString::setNum(unsigned long num, int base)
    {
        m_strValue.setNum(num, base);
        return *this;
    }
    TAUtfString& TAUtfString::setNum(long long num, int base)
    {
        m_strValue.setNum(num, base);
        return *this;
    }
    TAUtfString& TAUtfString::setNum(unsigned long long num, int base)
    {
        m_strValue.setNum(num, base);
        return *this;
    }
    TAUtfString& TAUtfString::setNum(float num, char f, int prec)
    {
        m_strValue.setNum(num, f, prec);
        return *this;
    }
    TAUtfString& TAUtfString::setNum(double num, char f, int prec)
    {
        m_strValue.setNum(num, f, prec);
        return *this;
    }

    int	TAUtfString::size() const
    {
        return m_strValue.size();
    }

    bool	TAUtfString::startsWith(const QString& s, bool caseSensitivity) const
    {
        Qt::CaseSensitivity sensitivity = Qt::CaseSensitive;

        if (sensitivity == false) sensitivity = Qt::CaseInsensitive;

        return m_strValue.startsWith(s, sensitivity);
    }
    bool	TAUtfString::startsWith(const std::string& s, bool caseSensitivity) const
    {
        QString qstr = TAUtfString::createFromMultitByteString(s).q_str();
        Qt::CaseSensitivity sensitivity = Qt::CaseSensitive;

        if (sensitivity == false) sensitivity = Qt::CaseInsensitive;

        return m_strValue.startsWith(qstr, sensitivity);
    }
    bool	TAUtfString::startsWith(const TAUtfString& s, bool caseSensitivity) const
    {
        Qt::CaseSensitivity sensitivity = Qt::CaseSensitive;

        if (sensitivity == false) sensitivity = Qt::CaseInsensitive;

        return m_strValue.startsWith(s.q_str(), sensitivity);
    }

    /*toShort********************************************************/
    short  TAUtfString::toShort(bool* ok, int base) const
    {
        return m_strValue.toShort(ok, base);
    }
    unsigned short  TAUtfString::toUShort(bool* ok, int base) const
    {
        return m_strValue.toUShort(ok, base);
    }
    int  TAUtfString::toInt(bool* ok, int base) const
    {
        return m_strValue.toInt(ok, base);
    }
    unsigned int  TAUtfString::toUInt(bool* ok, int base) const
    {
        return m_strValue.toUInt(ok, base);
    }
    long  TAUtfString::toLong(bool* ok, int base) const
    {
        return m_strValue.toLong(ok, base);
    }
    unsigned long  TAUtfString::toULong(bool* ok, int base) const
    {
        return m_strValue.toULong(ok, base);
    }
    long long  TAUtfString::toLongLong(bool* ok, int base) const
    {
        return m_strValue.toLongLong(ok, base);
    }
    unsigned long long  TAUtfString::toULongLong(bool* ok, int base) const
    {
        return m_strValue.toULongLong(ok, base);
    }
    float  TAUtfString::toFloat(bool* ok) const
    {
        return m_strValue.toFloat(ok);
    }
    double  TAUtfString::toDouble(bool* ok) const
    {
        return m_strValue.toDouble(ok);
    }

    TAUtfString TAUtfString::toLower() const
    {
        return TAUtfString(m_strValue.toLower());
    }

    TAUtfString TAUtfString::toUpper() const
    {
        return TAUtfString(m_strValue.toUpper());
    }

    TAUtfString TAUtfString::trimmed() const
    {
        return TAUtfString(m_strValue.trimmed());
    }

    TAUtfString TAUtfString::right(int nCount) const
    {
        return TAUtfString(m_strValue.right(nCount).data());
    }

    int TAUtfString::remove(int iIndex, int nCount)
    {
        m_strValue = m_strValue.remove(iIndex, nCount);
        return 1;
    }

    /*NUMBER********************************************************/

    TAUtfString TAUtfString::number(int num, int base)
    {
        QString qstr = QString::number(num, base);
        return TAUtfString(qstr);
    }
    TAUtfString TAUtfString::number(unsigned int  num, int base)
    {
        QString qstr = QString::number(num, base);
        return TAUtfString(qstr);
    }
    TAUtfString TAUtfString::number(long num, int base)
    {
        QString qstr = QString::number(num, base);
        return TAUtfString(qstr);
    }
    TAUtfString TAUtfString::number(unsigned long num, int base)
    {
        QString qstr = QString::number(num, base);
        return TAUtfString(qstr);
    }
    TAUtfString TAUtfString::number(long long num, int base)
    {
        QString qstr = QString::number(num, base);
        return TAUtfString(qstr);
    }
    TAUtfString TAUtfString::number(unsigned long long num, int base)
    {
        QString qstr = QString::number(num, base);
        return TAUtfString(qstr);
    }
    TAUtfString TAUtfString::number(double num, char f, int prec)
    {
        QString qstr = QString::number(num, f, prec);
        return TAUtfString(qstr);
    }



    std::string TAUtfString::toMultiByteStdString() const
    {
        const QByteArray asc = m_strValue.toUtf8();
        return std::string(asc.constData(), asc.length());
    }

    void TAUtfString::fromMultiByteString(const char* szSource)
    {
        m_strValue = fromMultipleByte(szSource);
    }

    void TAUtfString::fromMultiByteString(const std::string& szSource)
    {
        fromMultiByteString(szSource.c_str());
    }

    TAUtfString TAUtfString::createFromMultitByteString(const char* szSource)
    {
        TAUtfString tempString;
        tempString.fromMultiByteString(szSource);
        return tempString;
    }

    TAUtfString TAUtfString::createFromMultitByteString(const std::string& szSource)
    {
        TAUtfString tempString;
        tempString.fromMultiByteString(szSource.c_str());
        return tempString;
    }

    //TAUtfString TAUtfString::CreateFromBstrString(CComBSTR str)
    //{
    //    TAUtfString tempString;
    //    //TODO
    //    return tempString;
    //}

#endif  //WIN32
}
