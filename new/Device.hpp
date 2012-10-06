#pragma once
#include <QList>
#include <QString>
#include <QDomNode>
class Service;

class Device {
public:
	Device(QString baseURL, QDomNode doc);

	Device& getDeviceByType(QString type);
	Service& getServiceByType(QString type);

	QString baseURL;
	QDomNode doc;
	QString type;
	QList<Device*> devices;
	QList<Service*> services;
};
