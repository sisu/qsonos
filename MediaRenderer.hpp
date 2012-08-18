#pragma once
#include "Device.hpp"
#include "AVTransport.hpp"

class MediaRenderer {
public:
	MediaRenderer(Device& dev);

	Device& dev;
	AVTransport avtransport;
};
