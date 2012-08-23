#include "parse.hpp"
#include <QStringList>

ArgMap parseEvent(IXML_Document* doc) {
	ArgMap map;
	if (!doc) {
		return map;
	}
	QString pname = "e:property";
	for(Nodeptr i = doc->n.firstChild->firstChild; i; i=i->nextSibling) {
		if (i->nodeName!=pname) {
			log()<<"event node:"<<i->nodeName;
			continue;
		}
		Nodeptr j = i->firstChild;
		map[j->nodeName] = j->firstChild ? j->firstChild->nodeValue : "";
	}
	log()<<"vars:"<<map.size();
	return map;
}
ArgMap parseVars(IXML_Document* doc) {
	ArgMap map;
	for(Nodeptr i = doc->n.firstChild->firstChild; i; i=i->nextSibling) {
		map[i->nodeName] = QString::fromUtf8(i->firstChild ? i->firstChild->nodeValue : "");
	}
	log()<<"vars:"<<map.size();
	return map;
}
QList<ArgMap> parsePlaylist(IXML_Document* doc) {
	QList<ArgMap> res;
	Nodeptr root=doc->n.firstChild;
	for(Nodeptr i=root->firstChild; i; i=i->nextSibling) {
		ArgMap tags;
		for(Nodeptr j=i->firstChild; j; j=j->nextSibling) {
			QString name = QString::fromUtf8(j->nodeName);
			QString value = QString::fromUtf8(j->firstChild->nodeValue);
			name = name.right(name.size()-name.indexOf(':')-1);
			tags[name] = value;
		}
		res.push_back(tags);
	}
	return res;
}
int timeSeconds(QString time) {
	QStringList parts = time.split(':');
	int r=0;
	for(int i=0,t=1; i<parts.size(); ++i,t*=60)
		r += t*parts[i].toInt();
	return r;
}
