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

const QString templatePath = "TemplatePath";
const QString transactionsPath = "TransactionsPath";
const QString outputFile = "outputFile";

inline void initializeFileDialog(QFileDialog &dialog, QString pathInConfig = "")
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

		QFile file(m_template_location);
		if (file.open(QIODevice::ReadOnly)) {
			QTextStream stream(&file);
			QString temp_line;

			while (stream.readLineInto(&temp_line)) {
				QStringList list1(temp_line.split(";", QString::KeepEmptyParts));
				if (list1.length() > 0 && !list1[0].isEmpty()) {
					cTransactionGroups *itm = new cTransactionGroups();
					itm->setText(0, QApplication::translate("MainWindow", list1[0].toStdString().c_str(), nullptr));
					itm->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
					ui->treeWidget->addTopLevelItem(itm);
				}
				else if(list1.length() > 1 && !list1[1].isEmpty()){
					cTransactionGroups *itm = new cTransactionGroups();
					itm->setText(0, QApplication::translate("MainWindow", list1[1].toStdString().c_str(), nullptr));
					itm->setText(1, QApplication::translate("MainWindow", list1[2].toStdString().c_str(), nullptr));
					itm->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
					ui->treeWidget->findItems("", Qt::MatchContains).last()->addChild(itm);
				}
			}
		}
		else {
			//Error File could not be opened
		}

		if(!m_transaction_location.isEmpty()){
			ui->pushButton->setEnabled(true);
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

void MainWindow::setOutputPath() 
{
	QSettings settings(QDir::currentPath() + "/config.ini", QSettings::IniFormat);
	QString temp = settings.value(outputFile, QString("%1/ProcessedTransaction.csv").arg(QDir::currentPath())).toString();

	m_output_location = QFileDialog::getSaveFileName(this, tr("Save File"), temp,tr("*.csv"));

	settings.setValue(outputFile, m_output_location);
}

void MainWindow::generateProcessedFile() {
	setOutputPath();

	WCHAR currentPath_tchar[MAX_PATH + 1] = L"";
	DWORD pathLen = GetCurrentDirectory(MAX_PATH, currentPath_tchar);
	//Let's convert to string...
	std::wstring currentPath_wstring(currentPath_tchar); //convert to wstring
	std::string currentPath(currentPath_wstring.begin(), currentPath_wstring.end());//and convert to string.

	/**********************/
	//Read all rows of transaction
	/**********************/
	QStringList csv_rows;
	QFile file(m_transaction_location);
	if (file.open(QIODevice::ReadOnly)) {
		QTextStream stream(&file);
		QString line;
		QString temp_line;
		while (stream.readLineInto(&temp_line)) {
			if (!temp_line.isEmpty()) {
				line += temp_line;
				if (line.back() == '\"' ||
					line.back() == ';')
				{
					//current line ends
					csv_rows.append(line);
					line.clear();
				}
			}
		}
	}
	else {
		//Error File could not be opened
	}

	/**********************/
	//Processing row vector
	/**********************/
	// Delete first unnescessary rows including "buchungstag" in the first column
	for (auto it_vec = csv_rows.begin(); it_vec != csv_rows.end(); it_vec++) {
		QStringList list1 = it_vec->split(";",QString::SkipEmptyParts);
		if (list1.size() < 1) { // Simple format check
			return;
		}
		if (list1[0] == "\"Buchungstag\"") {
			csv_rows.erase(csv_rows.begin(), it_vec+1);
		}
		if (list1.size() > 1 && list1[1] == "\"Anfangssaldo\"") {
			csv_rows.erase(it_vec, csv_rows.end());
			break;
		}
	}


	//Process transactions
	for (int i = 0; i < ui->treeWidget->topLevelItemCount(); i++) {
		cTransactionGroups* transactionGroupPtr = dynamic_cast<cTransactionGroups*>(ui->treeWidget->topLevelItem(i));
		if (transactionGroupPtr != nullptr) {
			transactionGroupPtr->clear();
		}
	}

	QVector<TransactionPtr> unsorted_transactions;
	for (auto it_vec = csv_rows.cbegin(); it_vec != csv_rows.cend(); it_vec++) {
		QStringList list1 = it_vec->split(";");
		bool inserted_b = false;

		TransactionPtr transPtr(new Transaction(list1));
		for (int i = 0; i < ui->treeWidget->topLevelItemCount(); i++) {
			cTransactionGroups* transactionGroupPtr = dynamic_cast<cTransactionGroups*>(ui->treeWidget->topLevelItem(i));
			if (transactionGroupPtr != nullptr) {
				if (transactionGroupPtr->insertTransaction(transPtr)) {
					inserted_b = true;
					break;
				}
			}
		}
		if (!inserted_b) {
			unsorted_transactions.push_back(transPtr);
		}
	}

	/**********************/
	//Output of processed data
	file.setFileName(m_output_location);
	if (file.open(QIODevice::ReadWrite)) {
		file.resize(0);
		QTextStream stream(&file);
		for (int i = 0; i < ui->treeWidget->topLevelItemCount(); i++) {
			cTransactionGroups* transactionGroupPtr = dynamic_cast<cTransactionGroups*>(ui->treeWidget->topLevelItem(i));
			if (transactionGroupPtr != nullptr) {
				transactionGroupPtr->write(stream);
			}
		}
		//Print all unsorted transactions
		if (!unsorted_transactions.isEmpty()) {
			stream << endl << "Unsortierte Transaktionen" << endl;
			for (auto it = unsorted_transactions.cbegin(); it != unsorted_transactions.cend(); it++) {
				stream << it->data()->getLine() << endl;
			}
		}
		file.close();
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
