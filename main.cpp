#include "Device.hpp"
#include "common.hpp"
#include "PlayerWindow.hpp"
#include "upnp/ControlPoint.hpp"
#include <cassert>
#include <QApplication>
#include <QMetaType>
#include <QDomDocument>

PlayerWindow* wnd;

int main(int argc, char* argv[]) {
	qRegisterMetaType<ArgMap>("ArgMap");
	qRegisterMetaType<QDomDocument>("QDomDocument");
	QApplication app(argc,argv);
	app.setOrganizationName("qsonos");
	app.setApplicationName("qsonos");

	wnd = new PlayerWindow;
	wnd->show();

	ControlPoint cp;
	QObject::connect(&cp, SIGNAL(newDevice(Device*)),
			wnd, SLOT(foundDevice(Device*)));

	cp.listen();
	cp.discover("urn:schemas-upnp-org:device:ZonePlayer:1");

	return app.exec();
}
