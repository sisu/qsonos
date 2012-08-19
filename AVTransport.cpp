#include "AVTransport.hpp"
#include "Service.hpp"
#include "common.hpp"

AVTransport::AVTransport(Service& srv): service(srv) {
	service.defaultParams["InstanceID"] = "0";
	connect(&service, SIGNAL(gotEvent(QMap<QString,QString>)),
			this, SLOT(handleEvent(QMap<QString,QString>)));
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

void AVTransport::handleEvent(QMap<QString,QString> vars) {
//	log()<<"got avtransport event"<<vars.size();
	if (!vars.contains("LastChange")) return;
	QString change = vars["LastChange"];
	IXML_Document* doc = ixmlParseBuffer(qPrintable(change));
	if (!doc) {
		log()<<"failed parsing lastchange";
		return;
	}
	log() << ixmlPrintDocument(doc);
}
