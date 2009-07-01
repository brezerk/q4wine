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
	QPoint startPos;
	bool drag, dragstarted;

signals:
	void startDrag();
	void startDrop(QList<QUrl> files);
};

#endif // DRAGLISTWIDGET_H
