#include "PlayerWindow.hpp"
#include "PlaylistWidget.hpp"
#include "common.hpp"
#include <QLabel>
#include <QPushButton>
#include <QToolButton>
#include <QApplication>
#include <QStyle>
#include <QToolBar>
#include <QAction>

PlayerWindow::PlayerWindow(QWidget* par): QMainWindow(par), player(0) {
	playlist = new PlaylistWidget(*this);
	setCentralWidget(playlist);
}

void PlayerWindow::setPlayer(ZonePlayer* pl) {
	player = pl;
	makeToolbar();
	QList<ArgMap> plist = player->mediaServer.contentDir.getPlaylist();
	playlist->setList(plist);
	connect(&player->mediaRenderer.avtransport, SIGNAL(lastChange(ArgMap)),
			playlist, SLOT(handleChange(ArgMap)));
}
void PlayerWindow::makeToolbar() {
	QToolBar* toolbar = addToolBar("toolbar");
	QStyle* style = QApplication::style();

	QStyle::StandardPixmap icons[] = {QStyle::SP_MediaPlay, QStyle::SP_MediaPause, QStyle::SP_MediaStop, QStyle::SP_MediaSkipBackward, QStyle::SP_MediaSkipForward};
//	QObject* receivers[] = {&player->mediaRenderer.avtransport};
	const char* slotnames[] = {SLOT(play()), SLOT(pause()), SLOT(stop()), SLOT(previous()), SLOT(next())};
	const QString names[] = {"Play","Pause","Stop","Previous","Next"};

	for(int i=0; i<5; ++i) {
		QAction* action = toolbar->addAction(style->standardIcon(icons[i]), names[i]);
		connect(action, SIGNAL(triggered()),
//				receivers[i], slotnames[i]);
				&player->mediaRenderer.avtransport, slotnames[i]);
	}
}
void PlayerWindow::foundDevice(Device* dev) {
	try {
		ZonePlayer* pl = new ZonePlayer(*dev);
		log()<<"found zoneplayer";
		setPlayer(pl);
	} catch(const UPNPException& e) {
		log()<<"not valid zoneplayer:"<<e.what();
	}
}
