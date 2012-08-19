#pragma once
#include "MediaRenderer.hpp"
#include "MediaServer.hpp"
#include "Device.hpp"

class ZonePlayer {
public:
	ZonePlayer(Device& dev);

	Device& dev;
	MediaRenderer mediaRenderer;
	MediaServer mediaServer;
};
