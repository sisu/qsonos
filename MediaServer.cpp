#include "MediaServer.hpp"
#include "Device.hpp"

MediaServer::MediaServer(Device& dev):
	dev(dev),
	contentDir(dev.getServiceByType("ContentDirectory:1"))
{
}
