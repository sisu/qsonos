#pragma once
#include "common.hpp"
#include <upnp/upnp.h>
#include <QList>

ArgMap parseVars(IXML_Document* doc);
ArgMap parseEvent(IXML_Document* doc);
QList<ArgMap> parsePlaylist(IXML_Document* doc);
