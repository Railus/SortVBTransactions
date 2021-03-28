#include <windows.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDialog>
#include <QSettings>
#include <QStandardPaths>
#include <QImageReader>
#include <QApplication>
#include <QTextStream>
#include "main.h"
#include "Transaction.h"
#include "TransactionGroups.h"
#include "HelperFunctions.h"

const QString templatePath = "TemplatePath";
const QString transactionsPath = "TransactionsPath";
const QString outputFile = "outputFile";

void initializeFileDialog(QFileDialog &dialog, QString pathInConfig = "")
{
	if (!pathInConfig.isEmpty()) {
		QSettings settings(QDir::currentPath() + "/config.ini", QSettings::IniFormat);
		QString temp = settings.value(pathInConfig, QDir::currentPath()).toString();
		dialog.setDirectory(temp);
	}
	else {
		dialog.setDirectory(QDir::currentPath());
	}

	QStringList mime_type_filters = {"text/csv"};

	dialog.setMimeTypeFilters(mime_type_filters);
	dialog.selectMimeTypeFilter("text/csv");
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	QObject::connect(ui->actionTemplate, &QAction::triggered, this, &MainWindow::openTemplateTriggered);
	QObject::connect(ui->actionTransaction, &QAction::triggered, this, &MainWindow::openTransactionTriggered);
	QObject::connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::generateProcessedFile);
	QObject::connect(ui->addButton, &QPushButton::clicked, this, &MainWindow::addItem);
	QObject::connect(ui->removeButton, &QPushButton::clicked, this, &MainWindow::removeItem);
	ui->label->setText(tr("Version %1.%2").arg(VERSION_MAJOR).arg(VERSION_MINOR));
}

void MainWindow::openTemplateTriggered()
{
	QFileDialog file_dialog(this, QStringLiteral("Open Template"));
	initializeFileDialog(file_dialog, templatePath);
	const auto result = file_dialog.exec();
	if (result == QDialog::DialogCode::Accepted) 
	{
		m_template_location = file_dialog.selectedFiles().first();

		QSettings settings(QDir::currentPath() + "/config.ini", QSettings::IniFormat);
		settings.setValue(templatePath, file_dialog.directory().absolutePath());

		//Delete current visualized template
		while (nullptr != ui->treeWidget->topLevelItem(0)) {
			delete ui->treeWidget->topLevelItem(0);
		}

		if (!HelperFunctions::createTemplateTree(m_template_location, *ui->treeWidget))
		{
			if (!m_transaction_location.isEmpty()) {
				ui->pushButton->setEnabled(true);
			}
		}
		else {
			m_template_location.clear();
			ui->pushButton->setEnabled(false);
		}

	}
	else {
		m_transaction_location.clear();
		ui->pushButton->setEnabled(false);
	}
}

void MainWindow::openTransactionTriggered()
{
	QFileDialog file_dialog(this, QStringLiteral("Open Transaction"));
	initializeFileDialog(file_dialog, transactionsPath);
	const auto result = file_dialog.exec();
	if (result == QDialog::DialogCode::Accepted)
	{
		m_transaction_location = file_dialog.selectedFiles().first();

		QSettings settings(QDir::currentPath() + "/config.ini", QSettings::IniFormat);
		settings.setValue(transactionsPath, file_dialog.directory().absolutePath());

		if (!m_template_location.isEmpty()) {
			ui->pushButton->setEnabled(true);
		}
	}
	else {
		m_transaction_location.clear();
		ui->pushButton->setEnabled(false);
	}
}

bool MainWindow::selectOutputPath() {

	bool error_b = false;
	QSettings settings(QDir::currentPath() + "/config.ini", QSettings::IniFormat);
	QString temp = settings.value(outputFile, QString("%1/ProcessedTransaction.csv").arg(QDir::currentPath())).toString();

	m_output_location = QFileDialog::getSaveFileName(this, tr("Save File"), temp,tr("*.csv"));

	if (!m_output_location.isEmpty())
	{
		settings.setValue(outputFile, m_output_location);
	}
	else {
		error_b = true;
	}
	return error_b;
}

void MainWindow::generateProcessedFile() {
	bool error_b = false;
	QStringList csv_rows;
	QVector<TransactionPtr> unsorted_transactions;

	error_b |= selectOutputPath();

	if (!error_b) {
		error_b |= HelperFunctions::getTransactionRowsFromFile(m_transaction_location, csv_rows);
	}

	if (!error_b) {
		error_b |= HelperFunctions::fillTransactionsIntoTreeWidget(*ui->treeWidget, csv_rows, unsorted_transactions);
	}

	if (!error_b) {
		error_b |= HelperFunctions::createSortedFile(*ui->treeWidget, m_output_location, unsorted_transactions);
	}	
}

void MainWindow::addItem()
{
	cTransactionGroups *itm = new cTransactionGroups();
	itm->setText(0, QApplication::translate("MainWindow", "GroupTest", nullptr));
	itm->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);

	// Check if any item is selected
	if (!ui->treeWidget->selectedItems().isEmpty()) {
		ui->treeWidget->selectedItems().first()->addChild(itm);
	}
	else {
		ui->treeWidget->addTopLevelItem(itm);
	}
}

void MainWindow::removeItem()
{
	// Check if any item is selected
	if (!ui->treeWidget->selectedItems().isEmpty()) {
		delete ui->treeWidget->selectedItems().first();
	}
}

MainWindow::~MainWindow()
{
    delete ui;
}
