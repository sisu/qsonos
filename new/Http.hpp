#pragma once
#include "HttpReply.hpp"
#include <QtNetwork>

class Http: public QObject {
	Q_OBJECT
public:
	HttpReply* get(QUrl url);

private:
	QNetworkAccessManager net;
};

extern Http http;
