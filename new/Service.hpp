#pragma once
#include "Action.hpp"
#include "common.hpp"
#include <QObject>
#include <QUrl>
class Device;
class QNetworkReply;

class Service: public QObject {
	Q_OBJECT
public:
	Service(Device&, QDomNode);

	void processEvent(ArgMap vchanges);
	void actionResult(ArgMap res);

	ArgMap action(QString name, ArgMap& params);
	void actionAsync(QString name, ArgMap& params);
	void subscribe(QObject* handler);

	template<class...A>
	ArgMap action(QString name, A... params) {
		ArgMap pmap;
		setParams(pmap, params...);
		return action(name, pmap);
	}
	template<class...A>
	void actionAsync(QString name, A... params) {
		ArgMap pmap;
		setParams(pmap, params...);
		return actionAsync(name, pmap);
	}
	void setParams(ArgMap&) {}
	template<class...A>
	void setParams(ArgMap& pmap, QString name, QString value, A... params) {
		pmap[name] = value;
		setParams(pmap, params...);
	}
//	static ArgMap parseActionResult(IXML_Document* doc);

	const Action& getAction(QString name) const;

	Device& dev;
	QDomNode doc;
	QString type;
	ArgMap stateVars;
	QList<Action> actions;
	QUrl actionURL;
	QUrl eventURL;
	ArgMap defaultParams;

public slots:
	void gotDoc(QDomDocument doc);
	void subscribeRes(QNetworkReply*);

signals:
	void gotEvent(ArgMap vars);
	void gotResult(ArgMap vars);

private:
	void getInfo();
//	IXML_Document* makeAction(QString name, ArgMap& params) const;
};