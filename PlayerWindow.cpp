#include "PlayerWindow.hpp"
#include "PlaylistWidget.hpp"
#include "ZonePlayer.hpp"
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
#include <QSettings>
#include <QKeySequence>

PlayerWindow::PlayerWindow(QWidget* par): QMainWindow(par), player(0) {
	readSettings();
	playlist = new PlaylistWidget(*this);
	setCentralWidget(playlist);
	makeToolbar();
}

bool hasPlayer=0;
void PlayerWindow::setPlayer(ZonePlayer* pl) {
	if (player) return;

	player = pl;

	connectToolbar();

	QList<ArgMap> plist = player->mediaServer.contentDir.getPlaylist();
	playlist->setList(plist);

	AVTransport* avtransport = &player->mediaRenderer.avtransport;
	connect(avtransport, SIGNAL(lastChange(ArgMap)),
			playlist, SLOT(handleChange(ArgMap)));
	connect(avtransport, SIGNAL(lastChange(ArgMap)),
			this, SLOT(handleChange(ArgMap)));

	connect(&avtransport->service, SIGNAL(gotResult(ArgMap)),
			playlist, SLOT(handleResult(ArgMap)));
	connect(&avtransport->service, SIGNAL(gotResult(ArgMap)),
			this, SLOT(handleResult(ArgMap)));

	RenderingControl* renderingControl = &player->mediaRenderer.renderingControl;
	connect(renderingControl, SIGNAL(gotVolume(int)),
			this, SLOT(gotVolume(int)));

	connect(volumeSlider, SIGNAL(valueChanged(int)),
			&player->mediaRenderer.renderingControl, SLOT(setVolume(int)));

	avtransport->getPositionInfo();
	renderingControl->getVolume();
	startTimer(1000);
}
void PlayerWindow::makeToolbar() {
	QToolBar* toolbar = addToolBar("toolbar");
//	QToolBar* toolbar = new QToolBar();
//	addToolBar(toolbar);
	toolbar->setObjectName("toolbar");
	QStyle* style = QApplication::style();

	QStyle::StandardPixmap icons[] = {QStyle::SP_MediaPlay, QStyle::SP_MediaPause, QStyle::SP_MediaStop, QStyle::SP_MediaSkipBackward, QStyle::SP_MediaSkipForward};
//	QObject* receivers[] = {&player->mediaRenderer.avtransport};
	const QString names[] = {"Play","Pause","Stop","Previous","Next"};
	const QString shortcuts[] = {"X", "C", "V", "Z", "B"};

	for(int i=0; i<5; ++i) {
		QAction* action = toolbar->addAction(style->standardIcon(icons[i]), names[i]);
		action->setShortcut(shortcuts[i]);
		buttonActions[i] = action;
	}

	playSlider = new QSlider(Qt::Horizontal, toolbar);
	playSlider->setTracking(0);
	toolbar->addWidget(playSlider);

	timeLabel = new QLabel(toolbar);
	toolbar->addWidget(timeLabel);

	QString choiseButtons[] = {"repeat.svg", "shuffle.svg"};
	QString choiseNames[] = {"Repeat", "Shuffle"};
	const char* choiseSlots[] = {SLOT(toggleRepeat(bool)),SLOT(toggleShuffle(bool))};
	for(int i=0; i<2; ++i) {
		QAction* a = toolbar->addAction(QIcon(choiseButtons[i]), choiseNames[i]);
		QAbstractButton* button = (QAbstractButton*)toolbar->widgetForAction(a);
		button->setCheckable(1);
		connect(button, SIGNAL(toggled(bool)),
				this, choiseSlots[i]);
	}

	toolbar->addSeparator();
	volumeSlider = new QSlider(Qt::Horizontal, toolbar);
	volumeSlider->setMaximumWidth(100);
	volumeSlider->setMaximum(100);
	volumeSlider->setTracking(1);
	toolbar->addWidget(volumeSlider);
}
void PlayerWindow::connectToolbar() {
	const char* slotnames[] = {SLOT(play()), SLOT(pause()), SLOT(stop()), SLOT(previous()), SLOT(next())};
	for(int i=0; i<5; ++i) {
		QAction* action = buttonActions[i];
		connect(action, SIGNAL(triggered()),
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
void PlayerWindow::gotVolume(int vol) {
	volumeSlider->setSliderPosition(vol);
}


void PlayerWindow::closeEvent(QCloseEvent* event) {
	QSettings settings;
	settings.setValue("geometry", saveGeometry());
	settings.setValue("windowState", saveState());
	QMainWindow::closeEvent(event);
}
void PlayerWindow::readSettings() {
	QSettings settings;
	restoreGeometry(settings.value("geometry").toByteArray());
	restoreState(settings.value("windowState").toByteArray());
}

void PlayerWindow::timerEvent(QTimerEvent* event) {
	if (player) {
		player->mediaRenderer.avtransport.getPositionInfo();
	}
	QMainWindow::timerEvent(event);
}

void PlayerWindow::toggleRepeat(bool on) {
}
void PlayerWindow::toggleShuffle(bool on) {
}
