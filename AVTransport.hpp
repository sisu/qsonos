#pragma once
#include <QObject>
class Service;

class AVTransport: public QObject {
	Q_OBJECT
public:
	AVTransport(Service& srv);
	Service& service;

public slots:
	void play();
	void pause();
	void stop();
	void previous();
	void next();
};
