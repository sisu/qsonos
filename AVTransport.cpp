#include "AVTransport.hpp"
#include "Service.hpp"
#include "common.hpp"

AVTransport::AVTransport(Service& srv): service(srv) {
	service.defaultParams["InstanceID"] = "0";
	service.subscribe(this);
}

void AVTransport::play() {
	service.call("Play", "Speed", "1");
}
void AVTransport::pause() {
	service.call("Pause");
}
void AVTransport::stop() {
	service.call("Stop");
}
void AVTransport::previous() {
	service.call("Previous");
}
void AVTransport::next() {
	service.call("Next");
}
void AVTransport::setTrack(int num) {
	service.call("Seek",
			"Unit", "TRACK_NR",
			"Target", QString::number(num+1));
}

void AVTransport::getPositionInfo() {
	service.call("GetPositionInfo");
}

void AVTransport::handleEvent(ArgMap vars) {
#if 0
//	log()<<"got avtransport event"<<vars.size();
	if (!vars.contains("LastChange")) return;
	QString change = vars["LastChange"];
	IXML_Document* doc = ixmlParseBuffer(qPrintable(change));
	if (!doc) {
		log()<<"failed parsing lastchange";
		return;
	}
//	log() << ixmlPrintDocument(doc);
	ArgMap args;
	for(Nodeptr i=doc->n.firstChild->firstChild->firstChild; i; i=i->nextSibling) {
//		log()<<i->nodeName<<i->firstAttr->nodeValue;
		args[QString::fromUtf8(i->nodeName)] = QString::fromUtf8(i->firstAttr->nodeValue);
	}
//	log()<<args;
	emit lastChange(args);
#endif
}
