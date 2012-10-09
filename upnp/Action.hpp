#pragma once
#include <QString>
#include <QList>
#include <QDomNode>

class Action {
public:
	Action(QDomNode);
	QString name;
	QList<QString> inArgs;
	QList<QString> outArgs;
};
