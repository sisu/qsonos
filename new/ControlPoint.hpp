#pragma once
#include <QtNetwork>

class QDataArray;
class ControlPoint: public QObject {
	Q_OBJECT
public:
	ControlPoint();
	void listen();
	void discover(QString type);

public slots:
	void read();
	void gotDoc(QNetworkReply*);

private:
	QUdpSocket sock;
	QNetworkAccessManager net;
	QString search;

	void handleReply(QByteArray);
};
