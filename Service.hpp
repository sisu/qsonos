#pragma once
#include "Action.hpp"
#include <upnp/ixml.h>
#include <QString>
#include <QMap>
#include <QObject>
class Device;

class Service: public QObject {
	Q_OBJECT
public:
	Service(Device&, Nodeptr);

	void processEvent(QMap<QString,QString> vchanges);
	void action(QString name, QMap<QString,QString>& params);
	void subscribe(QObject* handler);

	template<class...A>
	void action(QString name, A... params) {
		QMap<QString,QString> pmap;
		setParams(pmap, params...);
		action(name, pmap);
	}
	void setParams(QMap<QString,QString>&) {}
	template<class...A>
	void setParams(QMap<QString,QString>& pmap, QString name, QString value, A... params) {
		pmap[name] = value;
		setParams(pmap, params...);
	}

	Action& getAction(QString name);

	Device& dev;
	Nodeptr doc;
	QString type;
	QMap<QString,QString> stateVars;
	QList<Action> actions;
	QString actionURL;
	QMap<QString,QString> defaultParams;

signals:
	void gotEvent(QMap<QString,QString> vars);

private:
	void getInfo();
};
