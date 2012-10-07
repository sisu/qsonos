#include "HttpSession.hpp"
#include <QTcpSocket>

HttpSession::HttpSession(QTcpSocket* socket): socket(socket) {
	connect(socket, SIGNAL(readyRead()),
			this, SLOT(onData()));
}

void HttpSession::onData() {
	qDebug()<<"data from socket:";
	qDebug()<<socket->readAll();
}
