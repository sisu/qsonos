#include "HttpSession.hpp"
#include <QTcpSocket>

HttpSession::HttpSession(QTcpSocket* socket): socket(socket) {
	waitingData = 0;
	connect(socket, SIGNAL(readyRead()),
			this, SLOT(onData()));
	connect(socket, SIGNAL(disconnected()),
			this, SLOT(deleteLater()));
}

void HttpSession::onData() {
	qDebug()<<"data from socket:";
//	qDebug()<<socket->readAll();
	while(1) {
		if (waitingData) {
			qDebug()<<"trying to get "<<waitingData;
			int idx = curData.size()-waitingData;
			qint64 get = socket->read(curData.data()+idx, waitingData);
			Q_ASSERT(get>=0);
			waitingData -= get;
			if (waitingData==0) {
				qDebug()<<"http data:";
				qDebug()<<curData;
			}
		} else if (socket->canReadLine()) {
			QByteArray line = socket->readLine().trimmed();
			qDebug()<<line;
			if (line.isEmpty() && headers.contains("content-length")) {
				waitingData = headers["content-length"].toInt();
				curData.resize(waitingData);
			} else {
				int idx = line.indexOf(':');
				if (idx<0) continue;
				headers[line.left(idx).trimmed().toLower()] = line.mid(idx+1).trimmed();
			}
		} else break;
	}
}
