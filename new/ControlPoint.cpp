#include "ControlPoint.hpp"
#include "Device.hpp"
#include "xml.hpp"
#include "Http.hpp"
#include "Service.hpp"
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
	ArgMap args;
	foreach(QByteArray line, data.split('\n')) {
		int idx = line.indexOf(':');
		if (idx<0) continue;
		args[line.left(idx).trimmed().toLower()] = line.right(line.size()-idx-1).trimmed();
	}

	if (!args.contains("location")) return;
	QUrl loc = args["location"];
	connect(http.get(loc), SIGNAL(xml(QUrl,QDomDocument)),
			this, SLOT(gotDoc(QUrl,QDomDocument)));
}

void ControlPoint::gotDoc(QUrl url, QDomDocument doc) {
	QDomNode root = doc.firstChild().nextSibling();
	QDomNode devN = getChild(root, "device");
	if (!devN.isNull()) {
		try {
			Device* dev = new Device(url, devN, *this);
			(void)dev;
		} catch(UPNPException e) {
			qDebug()<<"Failed creating device "<<e.what();
		}
	}
}

void ControlPoint::subscribe(Service& srv) {
	QNetworkRequest req(srv.eventURL);
	req.setRawHeader("HOST", srv.dev.baseURL.encodedHost());
	req.setRawHeader("CALLBACK", "");
	req.setRawHeader("NT", "upnp:event");
	connect(http.custom(req, "SUBSCRIBE"), SIGNAL(got(QNetworkReply*)),
			&srv, SLOT(subscribeRes(QNetworkReply*)));
}
