#ifndef _ctlQStringH_
#define _ctlQStringH_

#include <QString>

class QStringEx :public QString
{
public:
    QStringEx(const QChar *unicode, int size):QString(unicode,size){}; // Qt5: don't cap size < 0
    QStringEx(const QChar *unicode):QString(unicode){}; // Qt5: merge with the above
	QStringEx(QChar c):QString(c){};
	QStringEx(char* c):QString(c){};
	QStringEx(int size, QChar c):QString(size,c){};
	QStringEx(const QLatin1String &latin1):QString(latin1){};
    QStringEx(const QString & s):QString(s){};
    QStringEx():QString(){};
    std::string toStdString() const
    { const QByteArray asc =toLocal8Bit (); return std::string(asc.constData(), asc.length()); }

	QString fromStdString(const std::string &s)
	{ return fromLocal8Bit(s.data(), int(s.size())); }
 
};

#endif