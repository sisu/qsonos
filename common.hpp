#pragma once
#include <QDebug>

inline QDebug log() {
	return QDebug(QtWarningMsg);
}
