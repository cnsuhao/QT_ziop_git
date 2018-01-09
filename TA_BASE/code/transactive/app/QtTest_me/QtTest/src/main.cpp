#include "StdAfx.h"
#include "myApplication.h"
#include "core\utilities\src\RunParams.h"
//#include <QtGui/QApplication>
#include <QTextCodec>
#include <QTranslator>


int main(int argc, char *argv[])
{
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForCStrings(codec);

    TA_Base_App::myApplication b(argc, argv);

    //QTranslator translator;
    //translator.load( "C:\\Code\\PRIV_GenericGUI_QT\\Language\\QTest_zh_CN" );
    //b.installTranslator( &translator );

    return b.startApp();
}
//int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char*, int nShowCmd)
//{
//    QApplication a(argc, argv);
//    TA_Base_App::myApplication b;
//    b.InitInstance();
//    return a.exec();
//}
