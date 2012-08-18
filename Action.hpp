#pragma once
#include <QString>
#include <QList>
#include <upnp/ixml.h>

class Action {
public:
	Action(Nodeptr);
	QString name;
	QList<QString> inArgs;
	QList<QString> outArgs;
};
