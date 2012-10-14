#include "ContentDirectory.hpp"
#include "Service.hpp"
#include "common.hpp"
#include "parse.hpp"

ContentDirectory::ContentDirectory(Service& srv): service(srv) {
}
void ContentDirectory::getPlaylist(int start, int count) {
	ArgMap args;
	args["ObjectID"] = "Q:0";
	args["BrowseFlag"] = "BrowseDirectChildren";
	args["Filter"] = "*";
	args["StartingIndex"] = QString::number(start);
	args["RequestedCount"] = QString::number(count);
	args["SortCriteria"] = "";
	service.call("Browse", args);
}
