#include "banner.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	BannerApp a(argc, argv);
	qApp->setAttribute(Qt::AA_DontShowIconsInMenus, false);
	return a.startApp();
}
