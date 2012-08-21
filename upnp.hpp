#pragma once
#include <upnp/upnp.h>
#include <QString>
#include <QMap>
#include <QSet>
#include "common.hpp"

class Service;
class Device;

class UPNP: public QObject {
	Q_OBJECT
public:
	void start(QString type);
	void regEventHandler(Upnp_SID sid, Service*);
	void subscribe(Service* srv);
	void setControlURL(Service* srv);

	void handleEvent(Upnp_EventType type, void* event);

	UpnpClient_Handle handle;
	QMap<QString,Service*> eventHandler;
	QMap<QString,Service*> controlURLs;
	QSet<QString> curDevices;

signals:
	void newDevice(Device* dev);
};
extern UPNP upnp;

ArgMap parseVars(IXML_Document* doc);
IXML_Document* downloadDoc(QString url);
