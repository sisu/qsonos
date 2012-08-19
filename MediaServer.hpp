#pragma once
#include "ContentDirectory.hpp"
class Device;

class MediaServer {
public:
	MediaServer(Device& dev);

	Device& dev;
	ContentDirectory contentDir;
};
