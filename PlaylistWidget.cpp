#include "PlaylistWidget.hpp"
#include "ZonePlayer.hpp"
#include "PlayerWindow.hpp"
#include <QTreeWidgetItem>
#include <QKeyEvent>

PlaylistWidget::PlaylistWidget(PlayerWindow& player): player(player) {
	connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)),
//			this, SLOT(setTrack(QTreeWidgetItem*,int)));
			this, SLOT(setTrack()));
	setRootIsDecorated(0);
}

void PlaylistWidget::setList(QList<ArgMap> items) {
	QString attrs[] = {"title", "creator", "album"};
	QStringList titles{"Title", "Artist", "Album"};
//	int N = sizeof(titles)/sizeof(titles[0]);
	int N = titles.size();
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
		setTrack();
	else
		QTreeWidget::keyPressEvent(event);
}

void PlaylistWidget::setTrack() {
	log()<<"setTrack "<<currentIndex();
	player.player->mediaRenderer.avtransport.setTrack(currentIndex().row());
}
