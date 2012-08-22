#include "ContentDirectory.hpp"
#include "Service.hpp"
#include "common.hpp"
#include "parse.hpp"

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
		res.append(parsePlaylist(doc));

		int total = ares["TotalMatches"].toInt();
		log()<<"res size:"<<res.size();
		if (res.size() == total)
			break;
	}
	return res;
}
