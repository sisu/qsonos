#include "HttpSession.hpp"
#include "HttpServer.hpp"
#include "xml.hpp"
#include "Service.hpp"
#include <QTcpSocket>

HttpSession::HttpSession(QTcpSocket* socket, HttpServer& server):
	socket(socket), server(server) {
	waitingData = 0;
	connect(socket, SIGNAL(readyRead()),
			this, SLOT(onData()));
	connect(socket, SIGNAL(disconnected()),
			this, SLOT(deleteLater()));
}

void HttpSession::onData() {
//	qDebug()<<"data from socket:";
//	qDebug()<<socket->readAll();
	while(1) {
		if (waitingData) {
//			qDebug()<<"trying to get "<<waitingData;
			int idx = curData.size()-waitingData;
			qint64 get = socket->read(curData.data()+idx, waitingData);
			Q_ASSERT(get>=0);
			if (get==0) break;
			waitingData -= get;
			if (waitingData==0) {
				sendEvent();
			}
		} else if (socket->canReadLine()) {
			QByteArray line = socket->readLine().trimmed();
//			qDebug()<<line;
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

static ArgMap parseEvent(QByteArray str) {
	ArgMap map;
	QDomDocument doc;
	bool ok = doc.setContent(str);
	if (!ok) {
		return map;
	}
	QString pname = "e:property";
	for(QDomNode i : doc.documentElement()) {
		if (i.nodeName()!=pname) {
			qDebug()<<"event node"<<i.nodeName();
			continue;
		}
		QDomNode j = i.firstChild();
		map[j.nodeName()] = j.firstChild().nodeValue();
	}
	return map;
}
void HttpSession::sendEvent() {
//	qDebug()<<"http data:";
//	qDebug()<<curData;
	if (!headers.contains("sid")) {
		qDebug()<<"Missing SID in headers"<<headers;
		return;
	}
	QString sid = headers["sid"];
	if (server.handlers.contains(sid)) {
//		qDebug()<<"handler found";
		server.handlers[sid]->processEvent(parseEvent(curData));
	} else {
		qDebug()<<"Missing handler for SID"<<sid;
	}
}
