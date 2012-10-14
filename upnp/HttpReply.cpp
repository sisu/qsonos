#include "HttpReply.hpp"
#include <QNetworkReply>
#include <QDomDocument>
#include <QDebug>
#include <QApplication>
#include <QEventLoop>

HttpReply::HttpReply(QNetworkReply* reply): reply(reply) {
	reply->setParent(this);
	connect(reply, SIGNAL(finished()),
			this, SLOT(finished()));
}

void HttpReply::finished() {
	deleteLater();
	qDebug()<<"httpreply res "<<reply->url();
	if (reply->error()) {
		qDebug()<<"failed receiving "<<reply->url()<<":"<<reply->errorString();
		return;
	}
	emit got(reply);
	if (receivers(SIGNAL(xml(QDomDocument)))
		|| receivers(SIGNAL(xml(QUrl,QDomDocument)))) {
		QDomDocument doc;
		QByteArray data = reply->readAll();
		bool ok = doc.setContent(data);
		if (ok) {
			emit xml(doc);
			emit xml(reply->url(), doc);
		} else {
			qDebug()<<"failed parsing doc";
			qDebug()<<data;
		}
	}
}

void HttpReply::wait() {
	while(!reply->isFinished()) {
		QApplication::instance()->processEvents(QEventLoop::WaitForMoreEvents | QEventLoop::AllEvents);
	}
}
