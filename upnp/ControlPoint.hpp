#pragma once
#include "HttpServer.hpp"
#include <QtNetwork>
#include <QDomDocument>

class Service;
class QDataArray;
class Device;
class ControlPoint: public QObject {
	Q_OBJECT
public:
	ControlPoint();
	void listen();
	void discover(QString type);

	void subscribe(Service& srv);

	HttpServer httpServer;
signals:
	void newDevice(Device*);

public slots:
	void read();
	void gotDoc(QUrl, QDomDocument);

private:
	QUdpSocket sock;
	QNetworkAccessManager net;
	QString search;
	QNetworkInterface interface;

	void handleReply(QByteArray);
	void addDevice(Device&);

private slots:
	void addDevice();
};
