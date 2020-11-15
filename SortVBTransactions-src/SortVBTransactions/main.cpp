#include <windows.h>
#include <QApplication>
#include <QSettings>
#include <QFileDialog>
#include "mainwindow.h"
#include <vector>
#include <sstream>
#include <iterator>
#include "main.h"
#include "Transaction.h"
#include "TransactionGroup.h"
#include "AreaOfFinance.h"

int main(int argc, char *argv[])
{
	ShowWindow(GetConsoleWindow(), SW_HIDE);
	QApplication app(argc, argv);

	MainWindow mainwindow;
	mainwindow.show();
	app.exec();
}
