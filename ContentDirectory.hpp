#pragma once
#include <QList>
#include "common.hpp"

class Service;
class ContentDirectory {
public:
	ContentDirectory(Service& srv);

	void getPlaylist(int start, int count);

	Service& service;
};
