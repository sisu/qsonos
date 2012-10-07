#pragma once
#include <QList>
#include <QString>
#include <QDomNode>
#include <QUrl>
class Service;

class Device {
public:
	Device(QUrl baseURL, QDomNode doc);

	Device& getDeviceByType(QString type);
	Service& getServiceByType(QString type);

	QUrl baseURL;
	QDomNode doc;
	QString type;
	QList<Device*> devices;
	QList<Service*> services;
};
