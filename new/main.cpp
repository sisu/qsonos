#include "ControlPoint.hpp"
#include <QApplication>

int main(int argc, char** argv) {
	QApplication app(argc,argv);
	ControlPoint cp;
	cp.listen();
	cp.discover("urn:schemas-upnp-org:device:ZonePlayer:1");
	return app.exec();
}
