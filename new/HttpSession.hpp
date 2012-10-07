#pragma once
#include <QObject>
#include "common.hpp"
class QTcpSocket;

class HttpSession: public QObject {
	Q_OBJECT
public:
	HttpSession(QTcpSocket* socket);

private slots:
	void onData();

private:
	QTcpSocket* socket;
	ArgMap headers;
	QByteArray curData;
	size_t waitingData;
};
