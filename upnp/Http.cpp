#include "Http.hpp"
#include "HttpReply.hpp"

QThreadStorage<QNetworkAccessManager> Http::netStorage;

HttpReply* Http::get(QUrl url) {
	qDebug()<<"getting "<<url;
	return new HttpReply(net().get(QNetworkRequest(url)));
}
HttpReply* Http::custom(QNetworkRequest req, QByteArray verb) {
	qDebug()<<"custom "<<verb;
	return new HttpReply(net().sendCustomRequest(req, verb));
}
QNetworkAccessManager& Http::net() {
	return netStorage.localData();
}
