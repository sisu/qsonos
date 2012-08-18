#include "ZonePlayer.hpp"

ZonePlayer::ZonePlayer(Device& dev):
	dev(dev),
	mediaRenderer(dev.getDeviceByType("MediaRenderer:1"))
{
}
