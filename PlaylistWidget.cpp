#include "PlaylistWidget.hpp"
#include "ZonePlayer.hpp"
#include "PlayerWindow.hpp"
#include "Service.hpp"
#include "parse.hpp"
#include <QTreeWidgetItem>
#include <QKeyEvent>

PlaylistWidget::PlaylistWidget(PlayerWindow& player):
	player(player),
	activeIdx(-1)
{
	connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)),
//			this, SLOT(setTrack(QTreeWidgetItem*,int)));
			this, SLOT(changeTrack()));
	setRootIsDecorated(0);

	QString titlearr[] = {"", "Title", "Artist", "Album"};
	int N = sizeof(titlearr)/sizeof(titlearr[0]);
	QStringList titles;
	for(int i=0; i<N; ++i) titles.append(titlearr[i]);
	setHeaderLabels(titles);
	setColumnCount(N);
}

void PlaylistWidget::setList(QList<ArgMap> items) {
	clear();
	QString attrs[] = {"originalTrackNumber", "title", "creator", "album"};
	foreach(ArgMap i, items) {
		QTreeWidgetItem* x = new QTreeWidgetItem(this);
		for(int j=0; j<columnCount(); ++j)
			x->setText(j, i[attrs[j]]);
	}
}

void PlaylistWidget::keyPressEvent(QKeyEvent* event) {
	if (event->key()==Qt::Key_Enter || event->key()==Qt::Key_Return)
		changeTrack();
	else
		QTreeWidget::keyPressEvent(event);
}

void PlaylistWidget::changeTrack() {
	log()<<"setTrack "<<currentIndex();
	player.player->mediaRenderer.avtransport.setTrack(currentIndex().row());
	player.player->mediaRenderer.avtransport.play();
}
void PlaylistWidget::handleChange(ArgMap args) {
//	log()<<"change"<<args;
	if (args.contains("CurrentTrack"))
		setActiveTrack(args["CurrentTrack"].toInt()-1);
}
void PlaylistWidget::handleResult(ArgMap res) {
	if (res.contains("Track"))
		setActiveTrack(res["Track"].toInt()-1);
	else if (res.contains("NumberReturned")) {
		int num = res["NumberReturned"].toInt();
		int total = res["TotalMatches"].toInt();
		fillPlaylist(parsePlaylist(res["Result"]), res["TotalMatches"].toInt());
	}
}
void PlaylistWidget::setActiveTrack(int idx) {
	if (idx==activeIdx) return;
	QTreeWidgetItem* item = topLevelItem(idx);
	if (!item) return;
	setCurrentItem(item);
	QFont font;
	if (activeIdx>=0) {
		QTreeWidgetItem* old = topLevelItem(activeIdx);
		for(int i=0; i<columnCount(); ++i) {
			old->setFont(i, font);
		}
	}
	font.setBold(1);
	for(int i=0; i<columnCount(); ++i) {
		item->setFont(i, font);
	}
	activeIdx = idx;
}

void PlaylistWidget::setPlayer(ZonePlayer* pl) {
	zone = pl;
	ContentDirectory& dir = pl->mediaServer.contentDir;
	connect(&dir.service, SIGNAL(gotResult(ArgMap)),
			this, SLOT(handleResult(ArgMap)));
	dir.getPlaylist(0, 100);
}

void PlaylistWidget::fillPlaylist(QList<ArgMap> items, int total) {
	// FIXME: make sure that indices are correct
	QString attrs[] = {"originalTrackNumber", "title", "creator", "album"};
	foreach(ArgMap i, items) {
		QTreeWidgetItem* x = new QTreeWidgetItem(this);
		for(int j=0; j<columnCount(); ++j)
			x->setText(j, i[attrs[j]]);
	}
	int curCount = topLevelItemCount();
	if (curCount < total) {
		ContentDirectory& dir = zone->mediaServer.contentDir;
		dir.getPlaylist(curCount, 100);
	}
}
