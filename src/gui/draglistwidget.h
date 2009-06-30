#ifndef DRAGLISTWIDGET_H
#define DRAGLISTWIDGET_H

#include <QListWidget>
#include <QtGui>

class DragListWidget : public QListWidget
{
Q_OBJECT
public:
	DragListWidget(QWidget *parent = 0);
protected:
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void dragEnterEvent(QDragEnterEvent *event);
	void dragMoveEvent(QDragMoveEvent *event);
	void dropEvent(QDropEvent *event);
private:
	void StartDrug();
	QPoint startPos;
};

#endif // DRAGLISTWIDGET_H
