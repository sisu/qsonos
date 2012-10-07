#include "Http.hpp"
#include "HttpReply.hpp"

Http http;

HttpReply* Http::get(QUrl url) {
	qDebug()<<"getting "<<url;
	return new HttpReply(net.get(QNetworkRequest(url)));
}
