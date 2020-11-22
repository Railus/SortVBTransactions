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

	/**********************/
	//Read all rows of transaction
	/**********************/
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

	/**********************/
	//Processing row vector
	/**********************/
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
	//Process transactions
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

	/**********************/
	//Output of processed data
	std::ofstream outfile(m_output_location.toStdString(), std::ofstream::binary);

	for (auto it = aof_vec.begin(); it != aof_vec.end() - 1; it++) {
		it->print(outfile);
	}
	aof_vec.back().printFullTransaction(outfile);
	outfile.close();
}

void MainWindow::addItem()
{
	QTreeWidgetItem *itm = new QTreeWidgetItem();
	itm->setText(0, QApplication::translate("MainWindow", "GroupTest", nullptr));
	itm->setText(1, QApplication::translate("MainWindow", "KeywordTest", nullptr));
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
