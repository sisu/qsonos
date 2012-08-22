#pragma once
#include <QObject>
#include <QMap>
#include <QString>
#include "common.hpp"
class Service;

class AVTransport: public QObject {
	Q_OBJECT
public:
	AVTransport(Service& srv);
	Service& service;

signals:
	void lastChange(ArgMap);

public slots:
	void play();
	void pause();
	void stop();
	void previous();
	void next();
	void setTrack(int num);

	void getPositionInfo();

	void handleEvent(ArgMap);
};
