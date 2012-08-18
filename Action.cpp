#include "Action.hpp"
#include "xml.hpp"

Action::Action(Nodeptr doc) {
	name = getValue(doc, "name");
	Nodeptr alist = getChild(doc, "argumentList");
	if (alist) {
		for(Nodeptr i = alist->firstChild; i; i=i->nextSibling) {
			QString name = getValue(i, "name");
			QString dir = getValue(i, "direction");
//			log()<<"arg"<<dir<<name;
			(dir=="in" ? inArgs : outArgs).append(name);
		}
	}
}
