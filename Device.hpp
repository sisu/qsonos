#pragma once
#include "Service.hpp"
#include <upnp/upnp.h>
#include <QList>
#include <QString>

class Device {
public:
	Device(QString baseURL, Nodeptr doc);

	Device& getDeviceByType(QString type);
	Service& getServiceByType(QString type);

	QString baseURL;
	Nodeptr doc;
	QString type;
	QList<Device*> devices;
	QList<Service*> services;
};
