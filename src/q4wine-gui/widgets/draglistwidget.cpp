/***************************************************************************
 *   Copyright (C) 2008 by Malakhov Alexey                                 *
 *   brezerk@gmail.com                                                     *
 *                                                                         *
 *   This program is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *                                                                         *
 ***************************************************************************/

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
		std::auto_ptr<QListWidgetItem> item (this->itemAt(event->x(), event->y()));

		if (item.get()){
			QListWidget::itemClicked ( item.get() );
			if (!item->isSelected()){
				item->setSelected(TRUE);
			} else {
				item->setSelected(FALSE);
			}
		}

		item.release();
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
	std::auto_ptr<DragListWidget> source (qobject_cast<DragListWidget *>(event->source()));
	if (source.get() && source.get() != this){
		event->setDropAction(Qt::MoveAction);
		event->accept();
	}
	source.release();
}

void DragListWidget::dropEvent(QDropEvent *event){
	  std::auto_ptr<DragListWidget> source (qobject_cast<DragListWidget *>(event->source()));
	  if (source.get() && source.get() != this){
		event->setDropAction(Qt::MoveAction);
		event->accept();
	  }
	  source.release();
	  emit(startDrop(event->mimeData()->urls()));
}


