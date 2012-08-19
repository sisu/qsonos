#pragma once

class Service;
class ContentDirectory {
public:
	ContentDirectory(Service& srv);

	Service& service;
};
