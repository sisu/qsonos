#pragma once
#include <QObject>
#include "common.hpp"
class QTcpSocket;
class HttpServer;

class HttpSession: public QObject {
	Q_OBJECT
public:
	HttpSession(QTcpSocket* socket, HttpServer& server);

private slots:
	void onData();

private:
	QTcpSocket* socket;
	ArgMap headers;
	QByteArray curData;
	size_t waitingData;
	HttpServer& server;

	void sendEvent();
};
