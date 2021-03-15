#include <windows.h>
#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
	ShowWindow(GetConsoleWindow(), SW_HIDE);
	QApplication app(argc, argv);

	MainWindow mainwindow;
	mainwindow.show();
	app.exec();
}
