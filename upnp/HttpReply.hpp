#pragma once
#include <QObject>
#include <QDomDocument>
#include <QUrl>
class QNetworkReply;

class HttpReply: public QObject {
	Q_OBJECT
public:
	HttpReply(QNetworkReply*);

signals:
	void got(QNetworkReply*);
	void xml(QDomDocument);
	void xml(QUrl, QDomDocument);

private slots:
	void finished();

private:
	QNetworkReply* reply;
};
