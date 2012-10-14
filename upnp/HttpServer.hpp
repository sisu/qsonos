#pragma once
#include <QTcpServer>
#include <QMap>
#include <QString>
class Service;

class HttpServer: public QTcpServer {
	Q_OBJECT
public:
	HttpServer();

	void start();

	QMap<QString,Service*> handlers;
private slots:
	void getConnection();
};
