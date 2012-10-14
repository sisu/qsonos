#pragma once
#include "HttpReply.hpp"
#include <QtNetwork>
#include <QThreadStorage>

class Http {
public:
	static HttpReply* get(QUrl url);
	static HttpReply* custom(QNetworkRequest req, QByteArray verb);

private:
	Http();
	static QThreadStorage<QNetworkAccessManager> netStorage;
	static QNetworkAccessManager& net();
};
