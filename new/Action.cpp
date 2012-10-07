#include "Action.hpp"
#include "xml.hpp"

Action::Action(QDomNode doc) {
	name = getValue(doc, "name");
//	qDebug()<<"action "<<name;
	for(QDomNode i : getChild(doc, "argumentList")) {
		QString name = getValue(i, "name");
		QString dir = getValue(i, "direction");
//			log()<<"arg"<<dir<<name;
		(dir=="in" ? inArgs : outArgs).append(name);
	}
}
