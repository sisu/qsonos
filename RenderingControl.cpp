#include "RenderingControl.hpp"
#include "Service.hpp"

RenderingControl::RenderingControl(Service& srv): service(srv) {
	service.defaultParams["InstanceID"] = "0";
	service.defaultParams["Channel"] = "Master";
//	service.subscribe(this);
	connect(&service, SIGNAL(gotResult(ArgMap)),
			this, SLOT(gotResult(ArgMap)));
}

void RenderingControl::setVolume(int vol) {
	service.call("SetVolume", "DesiredVolume", QString::number(vol));
	// TODO: really get the volume after change
	emit gotVolume(vol);
}
void RenderingControl::getVolume() {
	service.call("GetVolume");
}

void RenderingControl::gotResult(ArgMap args) {
	log()<<"renderingcontrol event "<<args;
	if (args.contains("CurrentVolume")) {
		emit gotVolume(args["CurrentVolume"].toInt());
	}
}
