#pragma once
#include "HttpReply.hpp"
#include <QtNetwork>

class Http: public QObject {
	Q_OBJECT
public:
	HttpReply* get(QUrl url);
	HttpReply* custom(QNetworkRequest req, QByteArray verb);

private:
	QNetworkAccessManager net;
};

extern Http http;
