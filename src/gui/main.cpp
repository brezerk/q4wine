/*
 * main.cpp
 *
 */

#include <QApplication>
#include <QPushButton>

#include "version.h"
 
int main(int argc, char **argv)
{
	QApplication app (argc, argv);
	QPushButton button ("Hello world!");
	button.show();
	return app.exec();
}

