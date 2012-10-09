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

signals:
	void newDevice(Device*);

public slots:
	void read();
	void gotDoc(QUrl, QDomDocument);

private:
	QUdpSocket sock;
	QNetworkAccessManager net;
	QString search;
	HttpServer httpServer;
	QNetworkInterface interface;

	void handleReply(QByteArray);
};
