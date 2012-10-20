#include "BrowseModel.hpp"
#include <QDebug>

BrowseModel::BrowseModel() {
	bases.append(BrowseItem("S:"));
	bases.append(BrowseItem("A:ARTIST"));
	bases.append(BrowseItem("A:ALBUM"));
}

QVariant BrowseModel::data(const QModelIndex& idx, int role) const {
	if (role != Qt::DisplayRole) return QVariant();
	BrowseItem* item = (BrowseItem*)idx.internalPointer();
	qDebug()<<"data "<<idx.isValid()<<role<<(item?item->id:"fail");
	if (!item) return QVariant();
	return item->id;
}
QModelIndex BrowseModel::index(int row, int col, const QModelIndex& par) const {
	qDebug()<<"index "<<row<<col<<par.isValid();
	if (!hasIndex(row, col, par)) {
		qDebug()<<"invalid";
		return QModelIndex();
	}
	if (!par.isValid()) {
		qDebug()<<"valid";
		return createIndex(row, col, (void*)&bases[row]);
	}
	return createIndex(row,col,0);
}
QModelIndex BrowseModel::parent(const QModelIndex& index) const {
	return QModelIndex();
}
int BrowseModel::rowCount(const QModelIndex& par) const {
	qDebug()<<"rowCount:"<<par.isValid();
	if (!par.isValid()) {
		return bases.size();
	}
	return 0;
}
int BrowseModel::columnCount(const QModelIndex& par) const {
	return 1;
}
