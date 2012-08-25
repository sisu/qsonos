#pragma once
#include <QObject>
#include "common.hpp"
class Service;

class RenderingControl: public QObject {
	Q_OBJECT
public:
	RenderingControl(Service& srv);
	Service& service;

signals:
	void gotVolume(int);

public slots:
	void setVolume(int vol);
	void getVolume();

	void gotResult(ArgMap);
};
