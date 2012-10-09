#pragma once
#include "Action.hpp"
#include "common.hpp"
#include <QObject>
#include <QUrl>
#include <QtSoapHttpTransport>
class Device;
class QNetworkReply;
class QtSoapMessage;

class Service: public QObject {
	Q_OBJECT
public:
	Service(Device&, QDomNode);

	void processEvent(ArgMap vchanges);
	void actionResult(ArgMap res);

	void subscribe(QObject* handler);

	template<class...A>
	ArgMap call(QString name, A... params) {
		ArgMap pmap;
		setParams(pmap, params...);
		return call(name, pmap);
	}
	void setParams(ArgMap&) {}
	template<class...A>
	void setParams(ArgMap& pmap, QString name, QString value, A... params) {
		pmap[name] = value;
		setParams(pmap, params...);
	}
//	static ArgMap parseActionResult(IXML_Document* doc);
	void call(QString action, ArgMap& args);

	Action getAction(QString name) const;

	Device& dev;
	QDomNode doc;
	QString type;
	ArgMap stateVars;
	QList<Action> actions;
	QHash<QString,Action> actionMap;
	QUrl actionURL;
	QUrl eventURL;
	ArgMap defaultParams;
	QtSoapHttpTransport soap;

public slots:
	void gotDoc(QDomDocument doc);
	void subscribeRes(QNetworkReply*);

signals:
	void gotEvent(ArgMap vars);
	void gotResult(ArgMap vars);

private:
	void getInfo();
	QtSoapMessage makeAction(QString name, ArgMap& params) const;
};
