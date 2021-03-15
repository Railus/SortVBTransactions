#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDialog>
#include <QSettings>
#include <QStandardPaths>
#include <QImageReader>
#include <iostream>
#include <windows.h>
#include <QApplication>
#include "mainwindow.h"
#include <vector>
#include <sstream>
#include <iterator>
#include "main.h"
#include "Transaction.h"
#include "TransactionGroup.h"
#include "AreaOfFinance.h"
#include "TransactionItem.h"
#include <QTextStream>

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
					//itm->setText(1, QApplication::translate("MainWindow", "KeywordTest", nullptr));
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


	//OLD
	std::vector<AreaOfFinance> aof_vec;
	std::ifstream ifStream(m_template_location.toStdString(), std::ios::in);

	for (std::string line; std::getline(ifStream, line); ) {
		std::istringstream iss(line);
		std::vector<std::string> row((std::istream_iterator<WordDelimitedBy<';'>>(iss)), std::istream_iterator<WordDelimitedBy<';'>>());
		if (row.size() < 1 || row.size() > 3) { // Simple format check
			return;
		}
		if (!row[0].empty()) {
			AreaOfFinance aof(row[0]);
			aof_vec.push_back(aof);
		}
		else if (row[0].empty() && !row[1].empty() && !row[2].empty()) {
			TransactionGroup transactionGroup(row[1]);
			//Split keywords
			std::istringstream iss(row[2]);
			std::vector<std::string> keywords((std::istream_iterator<WordDelimitedBy<','>>(iss)), std::istream_iterator<WordDelimitedBy<','>>());
			for (auto it = keywords.begin(); it != keywords.end(); it++) {
				if (it->front() == ' ') it->erase(0, 1);
				transactionGroup.addKeyword(*it);
			}
			aof_vec.back().addTransGroup(transactionGroup);
		}
	}
	ifStream.close();
	ifStream.clear();

	SonstigeAreaOfFinance sonst_aof("Sonstige");
	TransactionGroup sonst("");
	sonst.addKeyword(" ");
	sonst_aof.addTransGroup(sonst);

	aof_vec.push_back(sonst_aof);

	for (std::string line; std::getline(ifStream, line); ) {
		std::istringstream iss(line);
		std::vector<std::string> row((std::istream_iterator<WordDelimitedBy<';'>>(iss)), std::istream_iterator<WordDelimitedBy<';'>>());
		if (row.size() < 1 || row.size() > 3) { // Simple format check
			return;
		}
		if (!row[0].empty()) {
			AreaOfFinance aof(row[0]);
			aof_vec.push_back(aof);
		}
		else if (row[0].empty() && !row[1].empty() && !row[2].empty()) {
			TransactionGroup transactionGroup(row[1]);
			//Split keywords
			std::istringstream iss(row[2]);
			std::vector<std::string> keywords((std::istream_iterator<WordDelimitedBy<','>>(iss)), std::istream_iterator<WordDelimitedBy<','>>());
			for (auto it = keywords.begin(); it != keywords.end(); it++) {
				if (it->front() == ' ') it->erase(0, 1);
				transactionGroup.addKeyword(*it);
			}
			aof_vec.back().addTransGroup(transactionGroup);
		}
	}
	ifStream.close();

	/**********************/
	//Read all rows of transaction
	/**********************/
	// OLD
	ifStream.open(m_transaction_location.toStdString());
	std::vector<std::string> row_vec;
	std::string temp_row_string;

	for (std::string line; std::getline(ifStream, line); ) {
		temp_row_string += line;

		if (!temp_row_string.empty()) {
			if (temp_row_string.back() == '\"' || temp_row_string.back() == ';') {
				//current line ends with " or ;
				row_vec.push_back(temp_row_string);
				temp_row_string.clear();
			}
		}
	}

	// NEW
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

	//OLD
	// Delete first unnescessary rows including "buchungstag" in the first column
	for (auto it_vec = row_vec.cbegin(); it_vec != row_vec.cend(); it_vec++) {
		std::istringstream iss(*it_vec);
		std::vector<std::string> row((std::istream_iterator<WordDelimitedBy<';'>>(iss)), std::istream_iterator<WordDelimitedBy<';'>>());
		if (row.size() < 1) { // Simple format check
			return;
		}
		if (row[0] == "\"Buchungstag\"") {
			row_vec.erase(row_vec.cbegin(), it_vec + 1);
			break;
		}
	}
	// Delete unnescessary rows at the end including "Anfangssaldo" in the nineth column
	for (auto it_vec = row_vec.crbegin(); it_vec != row_vec.crend(); it_vec++) {
		std::istringstream iss(*it_vec);
		std::vector<std::string> row((std::istream_iterator<WordDelimitedBy<';'>>(iss)), std::istream_iterator<WordDelimitedBy<';'>>());
		if (row.size() < 10) { // Simple format check
			return;
		}
		if (row[9] == "\"Anfangssaldo\"") {
			row_vec.erase(--it_vec.base(), row_vec.cend());
			break;
		}
	}


	//NEW
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
	//OLD
	for (auto it_vec = row_vec.cbegin(); it_vec != row_vec.cend(); it_vec++) {
		std::istringstream iss(*it_vec);
		std::vector<std::string> row((std::istream_iterator<WordDelimitedBy<';'>>(iss)), std::istream_iterator<WordDelimitedBy<';'>>());

		Transaction temp_transaction(row);
		for (auto it = aof_vec.begin(); it != aof_vec.end(); it++) {
			if (it->meetsCondition(temp_transaction)) {
				it->addTransaction(temp_transaction);
				break;
			}
		}
	}


	//NEW
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
	//OLD
	std::ofstream outfile(m_output_location.toStdString(), std::ofstream::binary);

	for (auto it = aof_vec.begin(); it != aof_vec.end() - 1; it++) {
		it->print(outfile);
	}
	aof_vec.back().printFullTransaction(outfile);
	outfile.close();

	//NEW
	file.setFileName(m_output_location);
	if (file.open(QIODevice::ReadWrite)) {
		QTextStream stream(&file);
		for (int i = 0; i < ui->treeWidget->topLevelItemCount(); i++) {
			cTransactionGroups* transactionGroupPtr = dynamic_cast<cTransactionGroups*>(ui->treeWidget->topLevelItem(i));
			if (transactionGroupPtr != nullptr) {
				transactionGroupPtr->write(stream);
			}
		}
		//Print all unsorted transactions
		if (!unsorted_transactions.isEmpty()) {
			stream << endl << endl << "Unsortierte Transaktionen" << endl;
			for (auto it = unsorted_transactions.cbegin(); it != unsorted_transactions.cend(); it++) {
				stream << QString::fromStdString(it->data()->getLine()) << endl;
			}
		}
		file.close();
	}
	
}

void MainWindow::addItem()
{
	cTransactionGroups *itm = new cTransactionGroups();
	itm->setText(0, QApplication::translate("MainWindow", "GroupTest", nullptr));
	//itm->setText(1, QApplication::translate("MainWindow", "KeywordTest", nullptr));
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
