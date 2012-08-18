#include "Device.hpp"
#include "xml.hpp"
#include "common.hpp"
#include <cassert>

Device::Device(QString baseURL, Nodeptr doc): baseURL(baseURL), doc(doc) {
	type = getValue(doc, "deviceType");
	log()<<"making device "<<type;

	// FIXME: leak
	Nodeptr serviceList = getChild(doc, "serviceList");
	if (serviceList) {
		for(Nodeptr i=serviceList->firstChild; i; i=i->nextSibling) {
			services.append(new Service(*this, i));
		}
	}
	Nodeptr deviceList = getChild(doc, "deviceList");
	if (deviceList) {
		for(Nodeptr i=deviceList->firstChild; i; i=i->nextSibling) {
			devices.append(new Device(baseURL, i));
		}
	}
}

Device& Device::getDeviceByType(QString type) {
	type = "urn:schemas-upnp-org:device:"+type;
	foreach(Device* dev, devices) {
		if (dev->type == type) return *dev;
	}
	// TODO
	throw 5;
}
Service& Device::getServiceByType(QString type) {
	type = "urn:schemas-upnp-org:service:"+type;
	foreach(Service* srv, services) {
		if (srv->type == type) return *srv;
	}
	// TODO
	throw 7;
}
