#pragma once
#include "common.hpp"
#include <QList>
#include <QTreeWidget>
class PlayerWindow;

class QKeyEvent;

class PlaylistWidget: public QTreeWidget {
	Q_OBJECT
public:
	PlaylistWidget(PlayerWindow&);

	void setList(QList<ArgMap> items);
	void setActiveTrack(int idx);

	virtual void keyPressEvent(QKeyEvent*);

	PlayerWindow& player;

public slots:
	void setTrack();
	void handleChange(ArgMap);

private:
	int activeIdx;
};
