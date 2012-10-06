#pragma once
#include "common.hpp"
#include <QString>
#include <QDomNode>

struct DomNodeIterator {
	DomNodeIterator(QDomNode n): node(n) {}
	DomNodeIterator& operator++() {
		node = node.nextSibling();
		return *this;
	}
	DomNodeIterator operator++(int) {
		DomNodeIterator old(*this);
		++*this;
		return old;
	}
	QDomNode& operator*() { return node; }
	QDomNode* operator->() { return &node; }

	bool operator==(const DomNodeIterator& i) const { return node==i.node; }
	bool operator!=(const DomNodeIterator& i) const { return !(*this==i); }

	QDomNode node;
};
inline DomNodeIterator begin(QDomNode n) { return DomNodeIterator(n.firstChild()); }
inline DomNodeIterator end(QDomNode) { return DomNodeIterator(QDomNode()); }

inline QDomNode getChild(QDomNode node, QString name) {
	for(QDomNode i: node) {
		if (i.nodeName()==name) return i;
	}
	return QDomNode();
}
inline QString getValue(QDomNode node, QString name) {
	return getChild(node,name).firstChild().nodeValue();
}
