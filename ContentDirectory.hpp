#pragma once
#include <QList>
#include "common.hpp"

class Service;
class ContentDirectory {
public:
	ContentDirectory(Service& srv);

	QList<ArgMap> getPlaylist();

	Service& service;
};
