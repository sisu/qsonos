#include "Device.hpp"
#include "xml.hpp"
#include "common.hpp"
#include "Service.hpp"
#include <QApplication>

Device::Device(QUrl baseURL, QDomNode doc, ControlPoint& cp, Device* parent):
	QObject(parent), baseURL(baseURL), doc(doc), cp(cp) {
	type = getValue(doc, "deviceType");
	log()<<"making device "<<type;

	for(QDomNode i : getChild(doc, "serviceList")) {
		services.append(new Service(*this, i));
	}
	for(QDomNode i : getChild(doc, "deviceList")) {
		devices.append(new Device(baseURL, i, cp, this));
	}
}
void Device::init() {
	foreach(Device* d, devices)
		d->init();
	foreach(Service* s, services)
		s->init();
	if (!parent()) {
		moveToThread(QApplication::instance()->thread());
		emit deviceReady();
	}
}

Device& Device::getDeviceByType(QString type) {
	type = "urn:schemas-upnp-org:device:"+type;
	foreach(Device* dev, devices) {
		if (dev->type == type) return *dev;
	}
	throw UPNPException("device not found: "+type);
}
Service& Device::getServiceByType(QString type) {
	type = "urn:schemas-upnp-org:service:"+type;
	foreach(Service* srv, services) {
		if (srv->type == type) return *srv;
	}
	throw UPNPException("service not found "+type);
}
