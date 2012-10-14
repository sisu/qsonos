#pragma once
#include "Http.hpp"
#include <QList>
#include <QString>
#include <QDomNode>
#include <QUrl>
class Service;
class ControlPoint;

class Device: public QObject {
	Q_OBJECT
public:
	Device(QUrl baseURL, QDomNode doc, ControlPoint& cp, Device* parent=0);

	Device& getDeviceByType(QString type);
	Service& getServiceByType(QString type);

	QUrl baseURL;
	QDomNode doc;
	QString type;
	QList<Device*> devices;
	QList<Service*> services;
	ControlPoint& cp;

public slots:
	void init();

signals:
	void deviceReady();
};
