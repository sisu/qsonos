#include "parse.hpp"
#include <QStringList>
#include "xml.hpp"

#if 0
ArgMap parseEvent(IXML_Document* doc) {
	ArgMap map;
	if (!doc) {
		return map;
	}
	QString pname = "e:property";
	for(QDomNode i = doc->n.firstChild->firstChild; i; i=i->nextSibling) {
		if (i->nodeName!=pname) {
			log()<<"event node:"<<i->nodeName;
			continue;
		}
		QDomNode j = i->firstChild;
		map[j->nodeName] = j->firstChild ? j->firstChild->nodeValue : "";
	}
	log()<<"vars:"<<map.size();
	return map;
}
ArgMap parseVars(IXML_Document* doc) {
	ArgMap map;
	for(QDomNode i = doc->n.firstChild->firstChild; i; i=i->nextSibling) {
		map[i->nodeName] = QString::fromUtf8(i->firstChild ? i->firstChild->nodeValue : "");
	}
	log()<<"vars:"<<map.size();
	return map;
}
#endif
QList<ArgMap> parsePlaylist(QString str) {
	QDomDocument doc;
	bool ok = doc.setContent(str);
	Q_ASSERT(ok);

	QList<ArgMap> res;
	QDomNode root=doc.documentElement();
	for(QDomNode i : root) {
		ArgMap tags;
		for(QDomNode j : i) {
			QString name = j.nodeName();
			QString value = j.firstChild().nodeValue();
			name = name.mid(name.indexOf(':')+1);
			tags[name] = value;
		}
		res.push_back(tags);
	}
	return res;
}
int timeSeconds(QString time) {
	QStringList parts = time.split(':');
	int r=0;
	for(int i=parts.size()-1,t=1; i>=0; --i,t*=60)
		r += t*parts[i].toInt();
	return r;
}
