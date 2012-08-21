#include "Service.hpp"
#include "Device.hpp"
#include "xml.hpp"
#include "upnp.hpp"
#include <upnp/upnptools.h>
#include <cassert>

Service::Service(Device& dev, Nodeptr np): dev(dev), doc(np) {
	type = getValue(doc, "serviceType");
	actionURL = dev.baseURL + getValue(doc, "controlURL");
	log()<<"making service "<<type;
	upnp.subscribe(this);
	getInfo();
}

void Service::processEvent(ArgMap vchanges) {
//	log()<<"event\n"<<ixmlPrintDocument(doc);
	log()<<"event";
	foreach(QString nm, vchanges.keys()) {
//		log()<<nm<<vchanges[nm];
		stateVars[nm] = vchanges[nm];
	}
	emit gotEvent(vchanges);
}
void Service::actionResult(ArgMap res) {
	emit gotResult(res);
}

IXML_Document* Service::makeAction(QString name, ArgMap& params) const {
	Action action = getAction(name);
	foreach(QString arg, action.inArgs) {
		if (!params.contains(arg)) {
			if (!defaultParams.contains(arg))
				throw UPNPException("Action "+name+" missing parameter "+arg);
			params[arg] = defaultParams[arg];
		}
	}
	log()<<"action"<<name;
	IXML_Document* anode=0;
	if (params.empty()) {
		anode = UpnpMakeAction(qPrintable(name), qPrintable(type), 0, 0);
	} else {
		foreach(QString arg, params.keys()) {
			int r = UpnpAddToAction(&anode,
					qPrintable(name),
					qPrintable(type),
					qPrintable(arg),
					qPrintable(params[arg]));
			assert(!r);
		}
	}
	return anode;
}
ArgMap Service::action(QString name, ArgMap& params) {
	IXML_Document* anode = makeAction(name, params);
	IXML_Document* resp;
	int r = UpnpSendAction(upnp.handle,
			qPrintable(actionURL),
			qPrintable(type),
			0,
			anode,
			&resp);
	log()<<ixmlPrintDocument(anode);
	log()<<"sent action"<<r<<actionURL<<type;
	if (resp) log()<<ixmlPrintDocument(resp);

	ArgMap res;
	if (resp) {
		for(Nodeptr i=resp->n.firstChild->firstChild; i; i=i->nextSibling) {
			res[i->nodeName] = QString::fromUtf8(i->firstChild->nodeValue);
		}
	}
	return res;
}
ArgMap Service::parseActionResult(IXML_Document* doc) {
	ArgMap res;
	if (doc) {
		for(Nodeptr i=doc->n.firstChild->firstChild; i; i=i->nextSibling) {
			res[i->nodeName] = QString::fromUtf8(i->firstChild->nodeValue);
		}
	}
	return res;
}

const Action& Service::getAction(QString name) const {
	for(int i=0; i<actions.size(); ++i) {
		const Action& a = actions[i];
		if (a.name==name) return a;
	}
	throw UPNPException("Action "+name+" not found");
}

void Service::subscribe(QObject* handler) {
	connect(this, SIGNAL(gotEvent(ArgMap)),
			handler, SLOT(handleEvent(ArgMap)));
}

void Service::getInfo() {
	QString url = dev.baseURL + getValue(doc, "SCPDURL");
	IXML_Document* desc;
	log()<<"loading service scpd"<<url;
	int r = UpnpDownloadXmlDoc(qPrintable(url), &desc);
	if (r) {
		log()<<"loading failed "<<r;
		desc = downloadDoc(url);
		if (!desc) {
			log()<<"failed again";
			return;
		}
//		assert(!r);
	}
	log()<<"got scpd";

	Nodeptr root = desc->n.firstChild;
#if 0
	for(Nodeptr i = getChild(root, "serviceStateTable")->firstChild; i; i=i->nextSibling) {
		Nodeptr j = i->firstChild;
	}
#endif
	for(Nodeptr i = getChild(root, "actionList")->firstChild; i; i=i->nextSibling) {
		actions.append(Action(i));
//		log()<<"action"<<actions.back().name;
	}
}
