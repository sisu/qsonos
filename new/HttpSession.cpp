#include "HttpSession.hpp"
#include <QTcpSocket>

HttpSession::HttpSession(QTcpSocket* socket): socket(socket) {
	connect(socket, SIGNAL(readyRead()),
			this, SLOT(onData()));
	connect(socket, SIGNAL(disconnected()),
			this, SLOT(deleteLater()));
}

void HttpSession::onData() {
	qDebug()<<"data from socket:";
	qDebug()<<socket->readAll();
}
