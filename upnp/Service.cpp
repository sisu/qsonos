#include "Service.hpp"
#include "Device.hpp"
#include "xml.hpp"
#include "Http.hpp"
#include "ControlPoint.hpp"
#include <cassert>
#include <QNetworkReply>
#include <QtSoapMessage>

Service::Service(Device& dev, QDomNode np):
	QObject(&dev), dev(dev), doc(np) {
	type = getValue(doc, "serviceType");
	actionURL = dev.baseURL.resolved(getValue(doc, "controlURL"));
	eventURL = dev.baseURL.resolved(getValue(doc, "eventSubURL"));
	log()<<"making service "<<type;

	soap.setHost(actionURL.host(), 0, actionURL.port());
	connect(&soap, SIGNAL(responseReady()),
			this, SLOT(actionRes()));
}
void Service::init() {
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
	// XXX: This makes every argument have type string.
	// UPnP doesn't seem to require type specifications for arguments
	// so hopefully it doesn't matter.
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
	throw UPNPException("Action "+name+" not found from service "+type);
}

void Service::subscribe(QObject* handler) {
	connect(this, SIGNAL(gotEvent(ArgMap)),
			handler, SLOT(handleEvent(ArgMap)));
}
void Service::call(QString aname, ArgMap& args) {
	QtSoapNamespaces::instance().registerNamespace("u", type);
	QtSoapMessage msg = makeAction(aname, args);
	qDebug()<<"making call to"<<actionURL;
//	qDebug()<<msg.toXmlString(1);
	soap.setAction("\""+type+"#"+aname+"\"");
	soap.submitRequest(msg, actionURL.path());
}

void Service::gotDoc(QDomDocument doc) {
	qDebug()<<"service desc "<<type;
	QDomNode root = doc.documentElement();
	for(QDomNode i : getChild(root, "actionList")) {
		Action a(i);
		actions.append(a);
		actionMap.insert(a.name, a);
	}
}

void Service::getInfo() {
	QUrl url = dev.baseURL.resolved(getValue(doc, "SCPDURL"));
	HttpReply* reply = Http::get(url);
	connect(reply, SIGNAL(xml(QDomDocument)),
			this, SLOT(gotDoc(QDomDocument)));
	reply->wait();
}

void Service::subscribeRes(QNetworkReply* reply) {
	qDebug()<<"sid: "<<reply->rawHeader("sid");
}

void Service::actionRes() {
	QtSoapMessage msg = soap.getResponse();
	if (msg.isFault()) {
		qDebug()<<"action failed: "<<msg.faultString().toString()<<";"<<msg.faultCode()<<";"<<msg.faultDetail().toString();
		return;
	}
	qDebug()<<"action result:";
//	qDebug()<<msg.toXmlString(1);
//	qDebug()<<msg.returnValue().id();
	const QtSoapType& res = msg.method();
	QString name = res.name().name();
	// remove "Response"-suffix fron name
	name.resize(name.size()-8);
	Action action = getAction(name);
	ArgMap outArgs;
	foreach(QString arg, action.outArgs) {
		const QtSoapType& val = res[arg];
//		qDebug()<<arg<<val.value().toString();
		outArgs[arg] = val.value().toString();
	}
	emit gotResult(outArgs);
//	qDebug()<<type.type();
//	qDebug()<<type.name().name()<<type.toString();
//	qDebug()<<type["Track"].value().toString();
}
