#include "ControlPoint.hpp"
#include "Device.hpp"
#include "xml.hpp"
#include <QDomDocument>

namespace {
	const QHostAddress mAddr("239.255.255.250");
	const int mPort = 1900;

	QNetworkInterface getInterface() {
		foreach(QNetworkInterface i, QNetworkInterface::allInterfaces()) {
			if (!(i.flags() & QNetworkInterface::IsLoopBack))
				return i;
		}
		throw 0;
	}
};

ControlPoint::ControlPoint() {
	connect(&net, SIGNAL(finished(QNetworkReply*)),
			this, SLOT(gotDoc(QNetworkReply*)));
}

void ControlPoint::listen() {
	connect(&sock, SIGNAL(readyRead()),
			this, SLOT(read()));
	sock.bind(mPort, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
	sock.joinMulticastGroup(mAddr, getInterface());
}
void ControlPoint::discover(QString type) {
	search = type;
	static const QString dmsg = "M-SEARCH * HTTP/1.1\r\n"
		"HOST: 239.255.255.250:1900\r\n"
		"MAN: \"ssdp:discover\"\r\n"
		"MX: %1\r\n"
		"ST: %2\r\n"
		"\r\n";
	QString msg = dmsg.arg("5", type);
	sock.writeDatagram(msg.toUtf8(), mAddr, mPort);
}

void ControlPoint::read() {
	while (sock.hasPendingDatagrams()) {
		QByteArray data;
		data.resize(sock.pendingDatagramSize());
		sock.readDatagram(data.data(), data.size());
		handleReply(data);
	}
}
void ControlPoint::handleReply(QByteArray data) {
	QHttpResponseHeader h(data.data());
	if (h.statusCode()!=200) return;

	QString loc = h.value("location");
	qDebug()<<"getting "<<loc;
	net.get(QNetworkRequest(loc));
}

void ControlPoint::gotDoc(QNetworkReply* reply) {
	if (reply->error()) {
		qDebug()<<"failed downloading "<<reply->url();
	}
	QDomDocument doc;
	bool ok = doc.setContent(reply->readAll());
	if (!ok) {
		qDebug()<<"failed parse doc";
		qDebug()<<doc.toString();
	}
	QDomNode root = doc.firstChild().nextSibling();
	QDomNode devN = getChild(root, "device");
	if (!devN.isNull()) {
		try {
			Device* dev = new Device(reply->url().toString(), devN);
		} catch(UPNPException e) {
			qDebug()<<"Failed creating device "<<e.what();
		}
	}
}
