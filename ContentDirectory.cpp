#include "ContentDirectory.hpp"
#include "Service.hpp"
#include "common.hpp"

ContentDirectory::ContentDirectory(Service& srv): service(srv) {
}
void ContentDirectory::getPlaylist() {
	log()<<"getting playlist";
	QMap<QString,QString> args;
	args["ObjectID"] = "Q:0";
	args["BrowseFlag"] = "BrowseDirectChildren";
	args["Filter"] = "*";
	args["StartingIndex"] = "0";
	args["RequestedCount"] = "10";
	args["SortCriteria"] = "";
	service.action("Browse", args);
}
