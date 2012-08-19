#include "upnp.hpp"
#include "Device.hpp"
#include "common.hpp"
#include "xml.hpp"
#include <cassert>
#include <QSet>
#include <QMap>
#include <QString>

UPNP upnp;

void UPNP::regEventHandler(Upnp_SID sid, Service* srv) {
	eventHandler[sid] = srv;
}

Device* makeDevice(Upnp_Discovery* d) {
	log()<<"downloading doc"<<d->Location;
	IXML_Document* doc = downloadDoc(d->Location);
	assert(doc);
	log()<<"got doc"<<doc->n.nodeName;
	QString dloc = d->Location;
	QString baseUrl = dloc.left(dloc.indexOf('/', 7));
	return new Device(baseUrl, getChild(doc->n.firstChild, "device"));
}
QMap<QString,QString> getEVars(IXML_Document* doc) {
	QMap<QString,QString> map;
	if (!doc) {
		return map;
	}
	QString pname = "e:property";
	for(Nodeptr i = doc->n.firstChild->firstChild; i; i=i->nextSibling) {
		if (i->nodeName!=pname) {
			log()<<"event node:"<<i->nodeName;
			continue;
		}
		Nodeptr j = i->firstChild;
		map[j->nodeName] = j->firstChild ? j->firstChild->nodeValue : "";
	}
	log()<<"vars:"<<map.size();
	return map;
}

void UPNP::handleEvent(Upnp_EventType type, void* event) {
	switch(type) {
		case UPNP_DISCOVERY_ADVERTISEMENT_ALIVE:
		case UPNP_DISCOVERY_SEARCH_RESULT:
		{
			Upnp_Discovery* d = (Upnp_Discovery*)event;
			QString id = d->DeviceId;
			if (!curDevices.contains(id)) {
				curDevices.insert(id);
				Device* dev = makeDevice(d);
				emit newDevice(dev);
			}
			break;
		}
		case UPNP_EVENT_RECEIVED:
		{
			assert(event);
			Upnp_Event* e = (Upnp_Event*)event;
			if (eventHandler.contains(e->Sid))
				eventHandler[e->Sid]->processEvent(getEVars(e->ChangedVariables));
//			log()<<"got event:"<<ixmlPrintDocument(e->ChangedVariables);
			break;
		}
		default:
			break;
	}
}
int callback(Upnp_EventType type, void* event, void* cookie) {
	log()<<"event "<<type;
	// libupnp sometimes decides to give 0 as cookie so don't use it
	(void)cookie;
//	UPNP* upnp = (UPNP*)cookie;
//	assert(upnp);
	upnp.handleEvent(type, event);
	return 0;
}

void UPNP::start(QString type) {
	type = "urn:schemas-upnp-org:device:"+type;
	int r = UpnpInit(0, 0);
	assert(r == UPNP_E_SUCCESS);
	r = UpnpRegisterClient(callback, 0, &handle);
	assert(r == UPNP_E_SUCCESS);
	r = UpnpSearchAsync(handle, 3, qPrintable(type), this);
	assert(r == UPNP_E_SUCCESS);
}

void UPNP::subscribe(Service* srv) {
	QString url = srv->dev.baseURL + getValue(srv->doc, "eventSubURL");
	int timeout=0;
	Upnp_SID id;
	int r = UpnpSubscribe(handle, qPrintable(url), &timeout, id);
	log()<<"subscribe"<<url<<":"<<r<<" ; "<<id;
	regEventHandler(id, srv);
}

IXML_Document* downloadDoc(QString url) {
	void* handle;
	char* contentType;
	int contentLength;
	int httpStatus;
	int r = UpnpOpenHttpGet(qPrintable(url), &handle, &contentType, &contentLength, &httpStatus, 1);
//	log()<<"openhttpget:"<<r;
	if (r) return 0;

	const size_t BS = 10<<10;
	char buf[BS+1];
	QByteArray res;
//	log()<<"downloading doc";
	while(1) {
		size_t size = BS;
		r = UpnpReadHttpGet(handle, buf, &size, 1);
//		log()<<"read "<<size;
		buf[size] = 0;
		res += buf;
		if (r) break;
		if (size < BS) break;
	}
//	log()<<"got document"<<res.size();
//	log()<<res.data();

	return ixmlParseBuffer(res.data());
}
