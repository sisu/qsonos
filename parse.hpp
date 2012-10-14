#pragma once
#include "common.hpp"
#include <QList>
#include <QString>

//ArgMap parseVars(IXML_Document* doc);
//ArgMap parseEvent(IXML_Document* doc);
QList<ArgMap> parsePlaylist(QString str);
int timeSeconds(QString time);
