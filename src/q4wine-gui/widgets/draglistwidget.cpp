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

	if (QApplication::keyboardModifiers()==Qt::CTRL){
		QListWidgetItem *item = this->itemAt(event->x(), event->y());

		if (item){
			QListWidget::itemClicked ( item );
			if (!item->isSelected()){
				item->setSelected(TRUE);
			} else {
				item->setSelected(FALSE);
			}
		}
	} else {
		QListWidget::mousePressEvent(event);
	}
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
	QList<QUrl> list = event->mimeData()->urls();
	for (int i=0; i < list.count(); i++){
	  //Accept only .exe, .bat or .com files
	  if (list.at(i).toLocalFile().contains(".exe", Qt::CaseInsensitive) || list.at(i).toLocalFile().contains(".bat", Qt::CaseInsensitive) || list.at(i).toLocalFile().contains(".com", Qt::CaseInsensitive)){
		event->acceptProposedAction();
		break;
	  }
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

	  emit(startDrop(event->mimeData()->urls()));
}


