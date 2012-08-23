#pragma once
#include "Device.hpp"
#include "AVTransport.hpp"
#include "RenderingControl.hpp"

class MediaRenderer {
public:
	MediaRenderer(Device& dev);

	Device& dev;
	AVTransport avtransport;
	RenderingControl renderingControl;
};
