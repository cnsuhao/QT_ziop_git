#ifndef TA_UNIOCDE_STRING_H__
#define TA_UNIOCDE_STRING_H__

#include <QString>
#include <string>
#include <ostream>
#include <istream>
#include "../utf8/source/utf8.h"
#ifdef WIN32
    #ifndef _UNICODE
        #define TAUnicodeString TA_Base_Bus::TAUtfString
    #else
        #define TAUnicodeString TA_Base_Bus::TAUtfString
    #endif //unicode
#endif //win32

namespace TA_Base_Bus
{
#ifdef WIN32

    /**
     *  The class used to encapsulate the QString,
     *	It provide convenient method for transfer between QString and std::string.
     *  The class is strictly used in UI(view) layer.
     */
    class TAUtfString
    {
    public:
        /** Constructor function for TAUtfString
        */
        TAUtfString();
        TAUtfString(const char* value);
        TAUtfString(const QChar*  value);
        TAUtfString(const std::string& value);
        TAUtfString(const QString&  value);
        TAUtfString(const TAUtfString& value);

        /** Virtual  deconstructor function for TAUtfString
        */
        virtual ~TAUtfString();

        /** Overloading operations.
         */
        bool operator==(const TAUtfString& strRight) const;
        bool operator==(const char* strRight) const;
        bool operator!=(const TAUtfString& strRight) const;
        bool operator!=(const char* strRight) const;

        TAUtfString& operator=(const TAUtfString& strSource);
        TAUtfString& operator=(const QString& strSource);
        TAUtfString& operator=(const std::string& strSource);
        TAUtfString& operator=(const char* strSource);

        TAUtfString  operator+(const TAUtfString& strSource);
        TAUtfString  operator+(const std::string& strSource);
        TAUtfString  operator+(const QString& strSource);
        TAUtfString  operator+(const char* strSource);

        TAUtfString& operator+=(const TAUtfString& strSource);
        TAUtfString& operator+=(const std::string& strSource);
        TAUtfString& operator+=(const QString& strSource);
        TAUtfString& operator+=(const char* strSource);

        /** overload operator <<, Writes to the stream.
        *
        *
        *  @param ostream
        *  @param str
        *  @return friend std::ostream &
        */
        //friend  std::istream &	operator>>(std::istream & ostream, const TAUtfString & str);
        friend  std::ostream&   operator<<(std::ostream& out, const TAUtfString& str);

        ///Overloading Conversion Operators
        operator QString() const;
        operator std::string() const;

    public:

        /** return the reference to internel Qstring member
         *
         */
        QString q_str() const;

        /** return std::string
         *
         */
        std::string  std_str() ;

        //other overloaded function for QString
    public:

        //Appends the string str onto the end of this string.
        TAUtfString& 	append(const QString& str);
        TAUtfString& 	append(const std::string& str);
        TAUtfString& 	append(const TAUtfString& str);

        ///Returns the character at the given index position in the string.
        ///The position must be a valid index position in the string(i.e., 0 <= position < size()).
        const QChar	at(int position) const;

        ///Removes n characters from the end of the string.
        ///If n is greater than or equal to size(), the result is an empty string.
        void chop(int n);

        //Clears the contents of the string and makes it null.
        // Remove it due to it is hard to handle NULL
        //void clear();


        ///Lexically compares this string with the other string and returns
        ///an integer less than, equal to, or greater than zero if this string is less than, equal to, or greater than the other string.
        int	compare(const QString& other, bool caseSensitivity = true) const;
        int	compare(const std::string& other, bool caseSensitivity = true) const;
        int	compare(const TAUtfString& other, bool caseSensitivity = true) const;

        ///Returns true if this string contains an occurrence of the string str; otherwise returns false.
        ///If caseSensitivity==true, the search is case sensitive; otherwise the search is case insensitive.
        bool contains(const QString& str, bool caseSensitivity = true) const;
        bool contains(const std::string& str, bool caseSensitivity = true) const;
        bool contains(const TAUtfString& str, bool caseSensitivity = true) const;

        ///Returns the number of (potentially overlapping) occurrences of the string str in this string.
        ///If caseSensitivity==true, the search is case sensitive; otherwise the search is case insensitive.
        int	count(const QString& str, bool caseSensitivity = true) const;
        int	count(const std::string& str, bool caseSensitivity = true) const;
        int	count(const TAUtfString& str, bool caseSensitivity = true) const;

        ///Returns a pointer to the data stored in the QString. The pointer can be used to access and modify the characters that compose the string.
        ///For convenience, the data is '\0'-terminated.
        const QChar* data() const;


        ///Returns true if the string ends with s; otherwise returns false.
        ///If caseSensitivity==true, the search is case sensitive; otherwise the search is case insensitive.
        bool  endsWith(const QString& s, bool caseSensitivity = true) const;
        bool  endsWith(const std::string&  s, bool caseSensitivity = true) const;
        bool  endsWith(const TAUtfString&  s, bool caseSensitivity = true) const;

        ///Returns the index position of the first occurrence of the string str in this string, searching forward from index position from.Returns - 1 if str is not found.
        ///If caseSensitivity==true, the search is case sensitive; otherwise the search is case insensitive.
        int	indexOf(const QString& str, int from = 0, bool caseSensitivity = true) const;
        int	indexOf(const std::string& str, int from = 0, bool caseSensitivity = true) const;
        int	indexOf(const TAUtfString& str, int from = 0, bool caseSensitivity = true) const;

        //Inserts the string str at the given index position and returns a reference to this string.
        TAUtfString& 	insert(int position, const QString& str);
        TAUtfString& 	insert(int position, const std::string& str);
        TAUtfString& 	insert(int position, const TAUtfString& str);

        ///Returns true if the string has no characters; otherwise returns false.
        bool isEmpty() const;

        ///Returns the index position of the last occurrence of the string str in this string, searching backward from index position from.
        ///If from is -1 (default), the search starts at the last character;
        ///if from is -2, at the next to last character and so on. Returns -1 if str is not found.
        ///If caseSensitivity==true,, the search is case sensitive; otherwise the search is case insensitive.
        int	lastIndexOf(const QString& str, int from = -1, bool caseSensitivity = true) const;
        int	lastIndexOf(const std::string& str, int from = -1, bool caseSensitivity = true) const;
        int	lastIndexOf(const TAUtfString& str, int from = -1, bool caseSensitivity = true) const;


        /** Extracts the left part of a string
         *
         */
        TAUtfString left(int nCount) const;

        /** Returns the number of characters in the object.
        *
        */
        int length();

        /** Extracts the middle part of a string
        *
        */
        TAUtfString mid(int iFirst, int nCount = -1) const;

        ///Prepends the string str to the beginning of this string and returns a reference to this string.
        TAUtfString& 	prepend(const QString& str);
        TAUtfString& 	prepend(const std::string& str);
        TAUtfString& 	prepend(const TAUtfString& str);

        ///Replaces n characters beginning at index position with the string after and returns a reference to this string.
        ///Note: If the specified position index is within the string, but position + n goes outside the strings range,
        ///  then n will be adjusted to stop at the end of the string.
        TAUtfString& 	replace(int position, int n, const QString& after);
        TAUtfString& 	replace(int position, int n, const std::string& after);
        TAUtfString& 	replace(int position, int n, const TAUtfString& after);

        TAUtfString& 	replace(const QString& before, QString after, bool caseSensitivity = true);
        TAUtfString& 	replace(const std::string& before, std::string after, bool caseSensitivity = true);
        TAUtfString& 	replace(const TAUtfString& before, TAUtfString after, bool caseSensitivity = true);

        ///Attempts to allocate memory for at least size characters. If you know in advance how large the string will be,
        ///you can call this function, and if you resize the string often you are likely to get better performance.
        ///If size is an underestimate, the worst that will happen is that the QString will be a bit slower.
        void	reserve(int size);

        ///Sets the size of the string to size characters.
        ///If size is greater than the current size, the string is extended to make it size characters long with the extra characters added to the end.
        ///The new characters are uninitialized.
        ///If size is less than the current size, characters are removed from the end.
        void	resize(int size);


        ///Sets the string to the printed value of n in the specified base, and returns a reference to the string.
        ///The base is 10 by default and must be between 2 and 36. For bases other than 10, n is treated as an unsigned integer.
        TAUtfString& setNum(short num, int base = 10);
        TAUtfString& setNum(unsigned short num, int base = 10);
        TAUtfString& setNum(int num, int base = 10);
        TAUtfString& setNum(unsigned int num, int base = 10);
        TAUtfString& setNum(long num, int base = 10);
        TAUtfString& setNum(unsigned long num, int base = 10);
        TAUtfString& setNum(long long num, int base = 10);
        TAUtfString& setNum(unsigned long long num, int base = 10);
        TAUtfString& setNum(float num, char f = 'g', int prec = 6);
        TAUtfString& setNum(double num, char f = 'g', int prec = 6);

        ///Returns the number of characters in this string.
        int	size() const;

        bool	startsWith(const QString& s, bool caseSensitivity = true) const;
        bool	startsWith(const std::string& s, bool caseSensitivity = true) const;
        bool	startsWith(const TAUtfString& s, bool caseSensitivity = true) const;


        /**
         *  Returns the string converted to a short using base base, which is 10 by default and must be between 2 and 36, or 0.
         *  Returns 0 if the conversion fails.
         *  If a conversion error occurs, *ok is set to false; otherwise *ok is set to true.
         *  If base is 0, the C language convention is used : If the string begins with "0x", base 16 is used;
         *  if the string begins with "0", base 8 is used; otherwise, base 10 is used.
         *  The string conversion will always happen in the 'C' locale.
        */
        short  toShort(bool* ok = 0, int base = 10) const;
        unsigned short toUShort(bool* ok = 0, int base = 10) const;
        int toInt(bool* ok = 0, int base = 10) const;
        unsigned int  toUInt(bool* ok = 0, int base = 10) const;
        long toLong(bool* ok = 0, int base = 10) const;
        unsigned long toULong(bool* ok = 0, int base = 10) const;
        long long  toLongLong(bool* ok = 0, int base = 10) const;
        unsigned long long toULongLong(bool* ok = 0, int base = 10) const;
        float toFloat(bool* ok = 0) const;
        double toDouble(bool* ok = 0) const;

        ///Returns a lowercase copy of the string.
        TAUtfString toLower() const;

        ///Returns an uppercase copy of the string.
        TAUtfString toUpper() const;

        ///Returns a string that has whitespace removed from the start and the end.
        TAUtfString trimmed() const;

        /** Extracts the right part of a string
         *
         */
        TAUtfString right(int nCount) const;


        /** Deletes a character or characters from a string
         *  @param iIndex
         *  @param nCount
         *  @return int
         */
        int remove(int iIndex, int nCount = 1);


        /*
        Returns the string converted to a float value.
        If a conversion error occurs, *ok is set to false; otherwise *ok is set to true.Returns 0.0 if the conversion fails.
        The string conversion will always happen in the 'C' locale
        */

        static TAUtfString number(int num, int base = 10);
        static TAUtfString number(unsigned int num, int base = 10);
        static TAUtfString number(long num, int base = 10);
        static TAUtfString number(unsigned long num, int base = 10);
        static TAUtfString number(long long num, int base = 10);
        static TAUtfString number(unsigned long long num, int base = 10);
        static TAUtfString number(double num, char f = 'g', int prec = 6);


    public:

        /** return multi byte string hold by string object, no need character conversion
         *
         *
         *  @return std::string
         */
        std::string toMultiByteStdString() const;


        /** change the content according to input multibyte string
         *  indeed, this is ansi/utf-8 string
         *
         *  @param szSource: ansi/utf-8 stirng
         *  @return void
         */
        void fromMultiByteString(const char* szSource);

        /** change the content according to input multibyte string
        *
        *  @param szSource: ansi/utf-8 stirng
        *  @return void
        */
        void fromMultiByteString(const std::string& szSource);


        /** Create string object according to input multi byte string
         *
         *  @param szSource: ansi/utf-8 stirng
         *  @return TA_Base_Bus::TAUtfString
         */
        static TAUtfString createFromMultitByteString(const char* szSource);

        /** Create string object according to input multi byte string
        *
        *
        *  @param szSource
        *  @return TA_Base_Bus::TAUtfString
        */
        static TAUtfString createFromMultitByteString(const std::string& szSource);

        /**
          * TODO
          */
        //static TAUtfString CreateFromBstrString(CComBSTR str);


    public:

        /////////////////////////*****************************************///////////////////////////
        ///                           other static helper function                              /////
        /////////////////////////*****************************************///////////////////////////
    public:

        /** determine the string is utf8 format or not
         *
         */
        static bool IsValidUtf8(const char* szSource)
        {
            return utf8::is_valid(szSource, szSource + strlen(szSource));
        }



    private:
        QString m_strValue;  ///> internal QString member
    };

#endif //WIN32
}
#endif
