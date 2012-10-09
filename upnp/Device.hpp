#pragma once
#include <QList>
#include <QString>
#include <QDomNode>
#include <QUrl>
class Service;
class ControlPoint;

class Device {
public:
	Device(QUrl baseURL, QDomNode doc, ControlPoint& cp);

	Device& getDeviceByType(QString type);
	Service& getServiceByType(QString type);

	QUrl baseURL;
	QDomNode doc;
	QString type;
	QList<Device*> devices;
	QList<Service*> services;
	ControlPoint& cp;
};
