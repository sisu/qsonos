#include "AVTransport.hpp"
#include "Service.hpp"

AVTransport::AVTransport(Service& srv): service(srv) {
	service.defaultParams["InstanceID"] = "0";
}

void AVTransport::play() {
	service.action("Play", "Speed", "1");
}
void AVTransport::pause() {
	service.action("Pause");
}
void AVTransport::stop() {
	service.action("Stop");
}
void AVTransport::previous() {
	service.action("Previous");
}
void AVTransport::next() {
	service.action("Next");
}
