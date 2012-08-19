#pragma once

class Service;
class ContentDirectory {
public:
	ContentDirectory(Service& srv);

	void getPlaylist();

	Service& service;
};
