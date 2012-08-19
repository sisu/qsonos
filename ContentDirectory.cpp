#include "ContentDirectory.hpp"
#include "Service.hpp"
#include "common.hpp"

ContentDirectory::ContentDirectory(Service& srv): service(srv) {
}
QList<ArgMap> ContentDirectory::getPlaylist() {
	QList<ArgMap> res;
	// libupnp fails if the server reply is not fucking tiny
	const int count = 5;

	while(1) {
		log()<<"getting playlist";
		ArgMap args;
		args["ObjectID"] = "Q:0";
		args["BrowseFlag"] = "BrowseDirectChildren";
		args["Filter"] = "*";
		args["StartingIndex"] = QString::number(res.size());
		args["RequestedCount"] = QString::number(count);
		args["SortCriteria"] = "";
		ArgMap ares = service.action("Browse", args);
	//	log()<<res["Result"];
		IXML_Document* doc = ixmlParseBuffer(ares["Result"].toUtf8().constData());
		if (!doc) {
			log()<<"failed parsing result";
			return res;
		}
	//	log()<<QString::fromUtf8(ixmlPrintDocument(doc));
		Nodeptr root=doc->n.firstChild;
		for(Nodeptr i=root->firstChild; i; i=i->nextSibling) {
			ArgMap tags;
			for(Nodeptr j=i->firstChild; j; j=j->nextSibling) {
				QString name = QString::fromUtf8(j->nodeName);
				QString value = QString::fromUtf8(j->firstChild->nodeValue);
				name = name.right(name.size()-name.indexOf(':')-1);
				tags[name] = value;
			}
//			log()<<tags;
			res.push_back(tags);
		}
		int total = ares["TotalMatches"].toInt();
		log()<<"res size:"<<res.size();
		if (res.size() == total)
			break;
	}
	return res;
}
