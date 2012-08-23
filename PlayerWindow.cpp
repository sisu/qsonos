#include "PlayerWindow.hpp"
#include "PlaylistWidget.hpp"
#include "common.hpp"
#include "parse.hpp"
#include <QLabel>
#include <QPushButton>
#include <QToolButton>
#include <QApplication>
#include <QStyle>
#include <QToolBar>
#include <QAction>
#include <QSlider>

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
	connect(&player->mediaRenderer.avtransport, SIGNAL(lastChange(ArgMap)),
			this, SLOT(handleChange(ArgMap)));
	connect(&player->mediaRenderer.avtransport.service, SIGNAL(gotResult(ArgMap)),
			playlist, SLOT(handleResult(ArgMap)));
	connect(&player->mediaRenderer.avtransport.service, SIGNAL(gotResult(ArgMap)),
			this, SLOT(handleResult(ArgMap)));

	player->mediaRenderer.avtransport.getPositionInfo();
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

	playSlider = new QSlider(Qt::Horizontal, toolbar);
	toolbar->addWidget(playSlider);

	timeLabel = new QLabel(toolbar);
	toolbar->addWidget(timeLabel);
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

static QString formatTime(int sec) {
	char buf[128];
	sprintf(buf,"%02d:%02d", sec/60, sec%60);
	return buf;
}

void PlayerWindow::handleChange(ArgMap args) {
	(void)args;
}
void PlayerWindow::handleResult(ArgMap args) {
	if (args.contains("RelTime") && args.contains("TrackDuration")) {
		int reltime = timeSeconds(args["RelTime"]);
		int duration = timeSeconds(args["TrackDuration"]);
		log()<<"track time"<<reltime<<duration<<" ; "<<args["RelTime"]<<args["TrackDuration"];
		playSlider->setMaximum(duration);
		playSlider->setSliderPosition(reltime);
		timeLabel->setText(formatTime(reltime)+" / "+formatTime(duration));
	}
}
