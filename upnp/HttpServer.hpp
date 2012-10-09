#pragma once
#include <QTcpServer>

class HttpServer: public QTcpServer {
	Q_OBJECT
public:
	HttpServer();

	void start();

private slots:
	void getConnection();
};
