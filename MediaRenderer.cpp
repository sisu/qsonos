#include "MediaRenderer.hpp"

MediaRenderer::MediaRenderer(Device& dev):
	dev(dev),
	avtransport(dev.getServiceByType("AVTransport:1")),
	renderingControl(dev.getServiceByType("RenderingControl:1"))
{
}
