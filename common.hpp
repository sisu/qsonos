#pragma once
#include <QDebug>
#include <QString>

inline QDebug log() {
	return QDebug(QtWarningMsg);
}

class UPNPException {
public:
	UPNPException(QString err): error(err) {}
	QString what() const { return error; }
	QString error;
};
