#include "Device.hpp"
#include "xml.hpp"
#include "common.hpp"
#include "Service.hpp"
#include <cassert>

Device::Device(QUrl baseURL, QDomNode doc): baseURL(baseURL), doc(doc) {
	type = getValue(doc, "deviceType");
	log()<<"making device "<<type;

	// FIXME: leak
	for(QDomNode i : getChild(doc, "serviceList")) {
		services.append(new Service(*this, i));
	}
	for(QDomNode i : getChild(doc, "deviceList")) {
		devices.append(new Device(baseURL, i));
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
