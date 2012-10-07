#include "Service.hpp"
#include "Device.hpp"
#include "xml.hpp"
#include "Http.hpp"
#include "ControlPoint.hpp"
#include <cassert>
#include <QNetworkReply>

Service::Service(Device& dev, QDomNode np): dev(dev), doc(np) {
	type = getValue(doc, "serviceType");
	actionURL = dev.baseURL.resolved(getValue(doc, "controlURL"));
	eventURL = dev.baseURL.resolved(getValue(doc, "eventSubURL"));
	log()<<"making service "<<type;
//	upnp.subscribe(this);
	getInfo();
	dev.cp.subscribe(*this);
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

#if 0
IXML_Document* Service::makeAction(QString name, ArgMap& params) const {
	Action action = getAction(name);
	foreach(QString arg, action.inArgs) {
		if (!params.contains(arg)) {
			if (!defaultParams.contains(arg))
				throw UPNPException("Action "+name+" missing parameter "+arg);
			params[arg] = defaultParams[arg];
		}
	}
	foreach(QString arg, params.keys()) {
		if (!action.inArgs.contains(arg))
			throw UPNPException("Extra argument for action "+name+" : "+arg);
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
	if (r) {
		throw UPNPException("Action "+name+" failed:\n"+ixmlPrintDocument(anode)+"\n"+ixmlPrintDocument(resp));
	}
//	log()<<ixmlPrintDocument(anode);
//	log()<<"sent action"<<r<<actionURL<<type;
//	if (resp) log()<<ixmlPrintDocument(resp);

	return parseVars(resp);
}
void Service::actionAsync(QString name, ArgMap& params) {
	IXML_Document* anode = makeAction(name, params);
	UpnpSendActionAsync(upnp.handle,
			qPrintable(actionURL),
			qPrintable(type),
			0,
			anode,
			upnpCallback,
			&upnp);
}
ArgMap Service::parseActionResult(IXML_Document* doc) {
	ArgMap res;
	if (doc) {
		for(QDomNode i=doc->n.firstChild->firstChild; i; i=i->nextSibling) {
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
#endif

void Service::gotDoc(QDomDocument doc) {
//	qDebug()<<"service desc "<<type;
	QDomNode root = doc.firstChild().nextSibling();
	qDebug()<<root.nodeName();
	for(QDomNode i : getChild(root, "actionList")) {
		actions.append(Action(i));
	}
}

void Service::getInfo() {
	QUrl url = dev.baseURL.resolved(getValue(doc, "SCPDURL"));
	connect(http.get(url), SIGNAL(xml(QDomDocument)),
			this, SLOT(gotDoc(QDomDocument)));
}

void Service::subscribeRes(QNetworkReply* reply) {
	qDebug()<<"subscribe res: "<<reply->readAll();
}
