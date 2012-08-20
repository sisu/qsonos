#include "AVTransport.hpp"
#include "Service.hpp"
#include "common.hpp"

AVTransport::AVTransport(Service& srv): service(srv) {
	service.defaultParams["InstanceID"] = "0";
	service.subscribe(this);
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
void AVTransport::setTrack(int num) {
	service.action("Seek",
			"Unit", "TRACK_NR",
			"Target", QString::number(num+1));
}

void AVTransport::handleEvent(ArgMap vars) {
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
}
