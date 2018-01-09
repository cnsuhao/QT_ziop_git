#include <QAxFactory>
#include <QObject>


class ControlStation2 : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("ClassID", "{037EDEC7-80F2-11D7-B177-0050BAB1D931}")
    Q_CLASSINFO("InterfaceID", "{037EDEC6-80F2-11D7-B177-0050BAB1D931}")

public:

    ControlStation2(QObject* parent);

public slots:

    long logout();
    long giveApplicationFocus(long processId);
};
