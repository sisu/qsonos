#include "Device.hpp"
#include "common.hpp"
#include "xml.hpp"
#include "upnp.hpp"
#include "PlayerWindow.hpp"
#include <cassert>
#include <QApplication>
#include <upnp/upnpdebug.h>

PlayerWindow* wnd;

int main(int argc, char* argv[]) {
	qRegisterMetaType<ArgMap>("ArgMap");
	UpnpSetLogLevel(UPNP_ALL);
	QApplication app(argc,argv);

	wnd = new PlayerWindow;
	wnd->show();

	QObject::connect(&upnp, SIGNAL(newDevice(Device*)),
			wnd, SLOT(foundDevice(Device*)));

	upnp.start("ZonePlayer:1");

	return app.exec();
}
