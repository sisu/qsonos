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
			if (!(i.flags() & QNetworkInterface::IsLoopBack) && !i.addressEntries().empty())
				return i;
		}
		throw 0;
	}
};

ControlPoint::ControlPoint() {
	interface = getInterface();
}

void ControlPoint::listen() {
	connect(&sock, SIGNAL(readyRead()),
			this, SLOT(read()));
	sock.bind(mPort, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
	sock.joinMulticastGroup(mAddr, interface);
	httpServer.start();
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
		args[line.left(idx).trimmed().toLower()] = line.mid(idx+1).trimmed();
	}

	if (!args.contains("location")) return;
	QUrl loc = args["location"];
	connect(Http::get(loc), SIGNAL(xml(QUrl,QDomDocument)),
			this, SLOT(gotDoc(QUrl,QDomDocument)));
}

void ControlPoint::gotDoc(QUrl url, QDomDocument doc) {
	QDomNode devN = getChild(doc.documentElement(), "device");
	Device* dev = new Device(url, devN, *this);
	dev->init();
	emit newDevice(dev);
}

void ControlPoint::subscribe(Service& srv) {
	// XXX: Qt sets accept-language header, which is prohibited in UPnP-1.1
	// and there is currently no way to disable it.
	qDebug()<<"subscribing"<<srv.eventURL;
	QNetworkRequest req(srv.eventURL);
	QString host = srv.dev.baseURL.host()+":"+QString::number(srv.dev.baseURL.port());
	req.setRawHeader("HOST", host.toUtf8());
	QString addr = interface.addressEntries()[0].ip().toString() + ":" + QString::number(httpServer.serverPort());
	req.setRawHeader("CALLBACK", ("<http://"+addr+"/0>").toUtf8());
	req.setRawHeader("NT", "upnp:event");
	req.setRawHeader("Accept-Language", QByteArray());
	connect(Http::custom(req, "SUBSCRIBE"), SIGNAL(got(QNetworkReply*)),
			&srv, SLOT(subscribeRes(QNetworkReply*)));

	foreach(QByteArray x, req.rawHeaderList())
		qDebug()<<x<<":"<<req.rawHeader(x);
}

void ControlPoint::addDevice(Device& dev) {
	emit newDevice(&dev);
}
void ControlPoint::addDevice() {
	Device* dev = qobject_cast<Device*>(sender());
	Q_ASSERT(dev);
//	dev->moveToThread(QApplication::instance()->thread());
	emit newDevice(dev);
}
