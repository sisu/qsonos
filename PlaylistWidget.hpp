#pragma once
#include "common.hpp"
#include <QList>
#include <QTreeWidget>
class PlayerWindow;
class ZonePlayer;

class QKeyEvent;

class PlaylistWidget: public QTreeWidget {
	Q_OBJECT
public:
	PlaylistWidget(PlayerWindow&);

	void setList(QList<ArgMap> items);
	void setActiveTrack(int idx);
	void setPlayer(ZonePlayer* pl);

	virtual void keyPressEvent(QKeyEvent*);

	PlayerWindow& player;

public slots:
	void changeTrack();
	void handleChange(ArgMap);
	void handleResult(ArgMap);

private:
	int activeIdx;
	ZonePlayer* zone;

	void fillPlaylist(QList<ArgMap> items, int total);
};
