#pragma once
#include "Action.hpp"
#include "common.hpp"
#include <upnp/ixml.h>
#include <QObject>
class Device;

class Service: public QObject {
	Q_OBJECT
public:
	Service(Device&, Nodeptr);

	void processEvent(ArgMap vchanges);
	void actionResult(ArgMap res);

	ArgMap action(QString name, ArgMap& params); void subscribe(QObject* handler); 
	template<class...A>
	ArgMap action(QString name, A... params) {
		ArgMap pmap;
		setParams(pmap, params...);
		return action(name, pmap);
	}
	void setParams(ArgMap&) {}
	template<class...A>
	void setParams(ArgMap& pmap, QString name, QString value, A... params) {
		pmap[name] = value;
		setParams(pmap, params...);
	}
	static ArgMap parseActionResult(IXML_Document* doc);

	const Action& getAction(QString name) const;

	Device& dev;
	Nodeptr doc;
	QString type;
	ArgMap stateVars;
	QList<Action> actions;
	QString actionURL;
	ArgMap defaultParams;

signals:
	void gotEvent(ArgMap vars);
	void gotResult(ArgMap vars);

private:
	void getInfo();
	IXML_Document* makeAction(QString name, ArgMap& params) const;
};
