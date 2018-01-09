#include "StdAfx.h"
#include "ControlStationApp.h"

int main(int argc, char* argv[])
{
    qRegisterMetaType<unsigned int>("unsigned int");
    qRegisterMetaType<WPARAM>("WPARAM");
    qRegisterMetaType<LPARAM>("LPARAM");

    ControlStationApp app(argc, argv);
    app.setQuitOnLastWindowClosed(true);
    return app.startApp();
}
