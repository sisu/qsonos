#pragma once
#include <QObject>
class QTcpSocket;

class HttpSession: public QObject {
	Q_OBJECT
public:
	HttpSession(QTcpSocket* socket);

private slots:
	void onData();

private:
	QTcpSocket* socket;
};
