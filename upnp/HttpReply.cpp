#include "HttpReply.hpp"
#include <QNetworkReply>
#include <QDomDocument>
#include <QDebug>

HttpReply::HttpReply(QNetworkReply* reply) {
	connect(reply, SIGNAL(finished()),
			this, SLOT(finished()));
}

void HttpReply::finished() {
	deleteLater();
	QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
	Q_ASSERT(reply);
//	qDebug()<<"httpreply res "<<reply->url();
	if (reply->error()) {
		qDebug()<<"failed receiving "<<reply->url()<<":"<<reply->errorString();
		return;
	}
	emit got(reply);
	if (receivers(SIGNAL(xml(QDomDocument)))
		|| receivers(SIGNAL(xml(QUrl,QDomDocument)))) {
		QDomDocument doc;
		bool ok = doc.setContent(reply->readAll());
		if (ok) {
			emit xml(doc);
			emit xml(reply->url(), doc);
		} else {
			qDebug()<<"failed parsing doc";
			qDebug()<<doc.toString();
		}
	}
}
