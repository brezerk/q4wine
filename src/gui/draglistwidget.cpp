#include "draglistwidget.h"

DragListWidget::DragListWidget(QWidget *parent) : QListWidget (parent)
{
	setAcceptDrops(true);
}

void DragListWidget::mousePressEvent(QMouseEvent *event){
	if (event->button() == Qt::LeftButton)
		startPos = event->pos();
	QListWidget::mousePressEvent(event);
}

void DragListWidget::mouseMoveEvent(QMouseEvent *event){
	if (event->buttons() & Qt::LeftButton){
		int distance = (event->pos() - startPos).manhattanLength();
		if (distance > QApplication::startDragDistance())
			StartDrug();
	}
	QListWidget::mouseMoveEvent(event);
}

void DragListWidget::dragEnterEvent(QDragEnterEvent *event){
	  DragListWidget *source = qobject_cast<DragListWidget *>(event->source());
	  if (source && source != this){
			event->setDropAction(Qt::MoveAction);
			event->accept();
	  }
}

void DragListWidget::dragMoveEvent(QDragMoveEvent *event){
	  DragListWidget *source = qobject_cast<DragListWidget *>(event->source());
	  if (source && source != this){
			event->setDropAction(Qt::MoveAction);
			event->accept();
	  }
}

void DragListWidget::dropEvent(QDropEvent *event){
	  DragListWidget *source = qobject_cast<DragListWidget *>(event->source());
	  if (source && source != this){
			event->setDropAction(Qt::MoveAction);
			event->accept();
	  }

	  qDebug()<<event->mimeData();
	qDebug()<<"Drop!";
}


void DragListWidget::StartDrug(){
	QListWidgetItem * item = currentItem();

	if (item){
		QMimeData *mimeData = new QMimeData;
		QList<QUrl> urls;
		urls<<QUrl::fromLocalFile("/home/brezerk/systemsettings.desktop");
		mimeData->setUrls(urls);
		//mimeData->setText("application/x-desktop");
		//mimeData->setData("", "[Desktop Entry]\nExec=systemsettings -caption\"%c\" %i\nIcon=preferences-system\nType=Application\nX-DocPath=systemsettings/index.html\nX-KDE-StartupNotify=true\nGenericName=System Settings\nName=System Settings\nX-DBUS-StartupType=Unique\nCategories=Qt;KDE;System;");
		QDrag *drag = new QDrag(this);
		drag->setMimeData(mimeData);
		drag->setPixmap(item->icon().pixmap(32));
		drag->start(Qt::MoveAction);
		qDebug()<<"Ololo!";
	 }
}
