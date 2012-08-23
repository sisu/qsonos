#pragma once
#include <QObject>
#include "common.hpp"
class Service;

class RenderingControl: public QObject {
	Q_OBJECT
public:
	RenderingControl(Service& srv);
	Service& service;
};
