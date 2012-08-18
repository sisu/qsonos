#pragma once
#include <QString>
#include <upnp/ixml.h>
#include "common.hpp"

inline Nodeptr getChild(Nodeptr node, QString name) {
//	log()<<"getChild "<<name;
	for(Nodeptr i = node->firstChild; i; i=i->nextSibling) {
//		log()<<i->nodeName;
		if (i->nodeName==name) return i;
	}
	return 0;
}
inline QString getValue(Nodeptr node, QString name) {
	return getChild(node,name)->firstChild->nodeValue;
}
