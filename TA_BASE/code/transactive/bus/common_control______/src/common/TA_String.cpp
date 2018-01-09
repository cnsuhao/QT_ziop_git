class TA_String :public QString
{
public:
    TA_String(const QChar *unicode, int size):QString(unicode,size){}; // Qt5: don't cap size < 0
    TA_String(const QChar *unicode):QString(unicode){}; // Qt5: merge with the above
    TA_String(QChar c);
    TA_String(int size, QChar c);
    TA_String(const QLatin1String &latin1);
    TA_String(const QString & s):QString(s){};
    TA_String():QString(){};
    std::string toStdString() const
    { const QByteArray asc =toLocal8Bit (); return std::string(asc.constData(), asc.length()); }

};