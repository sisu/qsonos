#include "PlaylistWidget.hpp"
#include "ZonePlayer.hpp"
#include "PlayerWindow.hpp"
#include <QTreeWidgetItem>
#include <QKeyEvent>

PlaylistWidget::PlaylistWidget(PlayerWindow& player):
	player(player),
	activeIdx(0)
{
	connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)),
//			this, SLOT(setTrack(QTreeWidgetItem*,int)));
			this, SLOT(setTrack()));
	setRootIsDecorated(0);
}

void PlaylistWidget::setList(QList<ArgMap> items) {
	clear();
	QString attrs[] = {"title", "creator", "album"};
	QString titlearr[] = {"Title", "Artist", "Album"};
	int N = sizeof(titlearr)/sizeof(titlearr[0]);
	QStringList titles;
	for(int i=0; i<N; ++i) titles.append(titlearr[i]);
	setHeaderLabels(titles);
	setColumnCount(N);
	foreach(ArgMap i, items) {
		QTreeWidgetItem* x = new QTreeWidgetItem(this);
		for(int j=0; j<N; ++j)
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
}
void PlaylistWidget::setActiveTrack(int idx) {
	QTreeWidgetItem* item = topLevelItem(idx);
	setCurrentItem(item);
	QFont font;
	if (activeIdx>=0) {
		QTreeWidgetItem* old = topLevelItem(activeIdx);
		for(int i=0; i<3; ++i) {
			old->setFont(i, font);
		}
	}
	font.setBold(1);
	for(int i=0; i<3; ++i) {
		item->setFont(i, font);
	}
	activeIdx = idx;
}
