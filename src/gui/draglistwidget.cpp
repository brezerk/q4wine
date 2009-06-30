#include "draglistwidget.h"

DragListWidget::DragListWidget(QWidget *parent) : QListWidget (parent)
{
	setAcceptDrops(true);
	//connect(this, SIGNAL(startDrag()),this, SLOT(startDrag()), Qt::QueuedConnection);
}

void DragListWidget::mousePressEvent(QMouseEvent *event){
	if (itemAt(event->pos())){
	   if (event->button() == Qt::LeftButton)
	   startPos = event->pos();
	   drag=FALSE;
	   dragstarted=TRUE;
	} else {
	   dragstarted=FALSE;
	}

	QListWidget::mousePressEvent(event);
}

void DragListWidget::mouseMoveEvent(QMouseEvent *event){
   	if (itemAt(event->pos()) && dragstarted){
	   if (event->buttons() & Qt::LeftButton){
		   int distance = (event->pos() - startPos).manhattanLength();
		   if (distance > QApplication::startDragDistance())
			   if (currentItem() && !drag){
				emit startDrag();
				drag=TRUE;
			   }
			   return;
	   }
	}
	QListWidget::mouseMoveEvent(event);
}

void DragListWidget::dragEnterEvent(QDragEnterEvent *event){
	if (event->mimeData()->hasFormat("text/uri-list"))
	{
		DragListWidget *source = qobject_cast<DragListWidget *>(event->source());
		if (source && source != this){
		   event->acceptProposedAction();
		}
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

	  qDebug()<<event->mimeData()->text();
	  qDebug()<<event->mimeData()->formats();
	qDebug()<<"Drop!";
}


