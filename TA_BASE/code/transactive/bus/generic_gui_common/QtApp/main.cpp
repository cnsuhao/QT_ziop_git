#include "qtapp.h"
#include <QtGui/QApplication>
#include <QLocale>
#include <QTranslator>
#include <QTextCodec>
#include <QtCore/qglobal.h>
#include <io.h>
#include "bus/generic_gui_common/DyDLL/dydll.h"
int main(int argc, char *argv[])
{
	const std::string	CENTRAL_TSI_NAME_1 = "OCA";
	const std::string	CENTRAL_TSI_NAME_2 = "OCB";

	int ret=CENTRAL_TSI_NAME_1.compare("abc");
	 ret=CENTRAL_TSI_NAME_1.compare("X_OCA_X");
    ret=CENTRAL_TSI_NAME_1.compare("OCA_X");
	 ret=CENTRAL_TSI_NAME_1.compare("OC");
    
	QApplication a(argc, argv);
	//QTextCodec *codec = QTextCodec::codecForName("UTF-8");  
	QTextCodec *codec = QTextCodec::codecForName("GBK");  
	QTextCodec::setCodecForCStrings(codec);

	int ab [ 4 ] = {2, 4, 7, 11}, bb = 9;
	int *p, *q;
	char * name = "123456";
	p = &bb;
	q = ab + 2;
	name = name + 1;
	//cout<< *p << " " << *q << " " << name;


		int i=1, j=2, m=10;
	for (; i<3;i++)
	{
		if (j=i)
		{
			m=100;
		}
		else
		{
           m=10001;
		}

	}

	//QtApp w;
	//w.show();

	//中4E2D  文6587 

	//中%E4%B8%AD   文%E6%96%87
	QTranslator translator;
	translator.load("qtapp_zh.qm");
	a.installTranslator(&translator);	
	QString s="中文";
	QByteArray  ch= s.toLocal8Bit();
	QString bbc=QObject::tr("Host %1 found").arg(s);
	DyDLL* b=new DyDLL();
	b->show();
	myDialog d;
	d.show();

	QtApp ma;
	ma.show();
    
	return a.exec();
}


 