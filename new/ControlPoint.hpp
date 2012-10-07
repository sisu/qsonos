#pragma once
#include <QtNetwork>
#include <QDomDocument>

class QDataArray;
class ControlPoint: public QObject {
	Q_OBJECT
public:
	ControlPoint();
	void listen();
	void discover(QString type);

public slots:
	void read();
	void gotDoc(QUrl, QDomDocument);

private:
	QUdpSocket sock;
	QNetworkAccessManager net;
	QString search;

	void handleReply(QByteArray);
};
