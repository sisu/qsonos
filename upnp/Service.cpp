#include "Service.hpp"
#include "Device.hpp"
#include "xml.hpp"
#include "Http.hpp"
#include "ControlPoint.hpp"
#include <cassert>
#include <QNetworkReply>
#include <QtSoapMessage>

Service::Service(Device& dev, QDomNode np): dev(dev), doc(np) {
	type = getValue(doc, "serviceType");
	actionURL = dev.baseURL.resolved(getValue(doc, "controlURL"));
	eventURL = dev.baseURL.resolved(getValue(doc, "eventSubURL"));
	log()<<"making service "<<type;
//	upnp.subscribe(this);
	getInfo();
	dev.cp.subscribe(*this);

	soap.setHost(actionURL.host(), 0, actionURL.port());
	connect(&soap, SIGNAL(responseReady()),
			this, SLOT(actionRes()));
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

QtSoapMessage Service::makeAction(QString name, ArgMap& params) const {
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
	QtSoapMessage msg;
	msg.setMethod(name, type);
	foreach(QString arg, params.keys())
		msg.addMethodArgument(arg, "", params[arg]);
	return msg;
}
#if 0
ArgMap Service::parseActionResult(IXML_Document* doc) {
	ArgMap res;
	if (doc) {
		for(QDomNode i=doc->n.firstChild->firstChild; i; i=i->nextSibling) {
			res[i->nodeName] = QString::fromUtf8(i->firstChild->nodeValue);
		}
	}
	return res;
}
#endif

Action Service::getAction(QString name) const {
	auto it = actionMap.find(name);
	if (it!=actionMap.end())
		return it.value();
	throw UPNPException("Action "+name+" not found");
}

void Service::subscribe(QObject* handler) {
	connect(this, SIGNAL(gotEvent(ArgMap)),
			handler, SLOT(handleEvent(ArgMap)));
}
void Service::call(QString aname, ArgMap& args) {
	QtSoapMessage msg = makeAction(aname, args);
	qDebug()<<"making call to"<<actionURL;
	qDebug()<<msg.toXmlString(1);
	soap.setAction("\""+type+"#"+aname+"\"");
	soap.submitRequest(msg, actionURL.path());
}

void Service::gotDoc(QDomDocument doc) {
//	qDebug()<<"service desc "<<type;
	QDomNode root = doc.documentElement();
	for(QDomNode i : getChild(root, "actionList")) {
		Action a(i);
		actions.append(a);
		actionMap.insert(a.name, a);
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

void Service::actionRes() {
	QtSoapMessage msg = soap.getResponse();
	if (msg.isFault()) {
		qDebug()<<"action failed: "<<msg.faultString().toString();
	}
	qDebug()<<"action result:";
	qDebug()<<msg.toXmlString(1);
}
