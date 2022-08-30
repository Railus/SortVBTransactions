#include "HelperFunctions.h"
#include <QFile>
#include <QTextStream>
#include <QApplication>
#include "TransactionGroups.h"

namespace HelperFunctions {

	bool createTemplateTree(const QString& dir, QTreeWidget& treeWidget_pst) {
		bool error_b = false;

		QFile file(dir);
		if (file.open(QIODevice::ReadOnly)) {
			QTextStream stream(&file);
			QString temp_line;

			//Read as long as there are lines
			while (stream.readLineInto(&temp_line)) {
				QStringList list1(temp_line.split(";", QString::KeepEmptyParts));

				// Check if line has 3 strings seperated by ';'
				if (list1.length() == 3)
				{
					if (!list1[0].isEmpty()) {
						cTransactionGroups *itm = new cTransactionGroups();
						itm->setText(0, QApplication::translate("MainWindow", list1[0].toStdString().c_str(), nullptr));
						itm->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
						treeWidget_pst.addTopLevelItem(itm);
					}
					else if (!list1[1].isEmpty()) {
						cTransactionGroups *itm = new cTransactionGroups();
						itm->setText(0, QApplication::translate("MainWindow", list1[1].toStdString().c_str(), nullptr));
						itm->setText(1, QApplication::translate("MainWindow", list1[2].toStdString().c_str(), nullptr));
						itm->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
						treeWidget_pst.findItems("", Qt::MatchContains).last()->addChild(itm);
					}
				}
				else
				{
					error_b = true;
					break;
				}
			}
		}
		else {
			//Error File could not be opened
		}
		return error_b;
	}


	bool getTransactionRowsFromFile(const QString& dir, QStringList& transactionList) {
		bool error_b = false;
		QFile file(dir);
		if (file.open(QIODevice::ReadOnly)) {
			QTextStream stream(&file);
			QString line;
			QString temp_line;
			while (stream.readLineInto(&temp_line)) {
				if (!temp_line.isEmpty()) {
					line += temp_line;
					/*if (line.back() == '\"' ||
						line.back() == ';' ||
						line.back() == '\n')
					{*/
						//current line ends
						transactionList.append(line);
						line.clear();
					//}
				}
			}
			file.close();


			// Delete first unnescessary rows including "Buchungstag" in the first column
			// and last unnescessary rows from "Anfangssaldo" in the first column
			bool foundFirstTransaction_b = false;
			bool foundLastTransaction = false;
			for (auto it_vec = transactionList.begin(); it_vec != transactionList.end(); it_vec++) {
				QStringList list1 = it_vec->split(";", QString::SkipEmptyParts);
				if (list1.size() < 1)
				{
					if (foundFirstTransaction_b)
					{
						transactionList.erase(it_vec, transactionList.end());
						foundFirstTransaction_b = true;
						break;
					}
				}
				else if (list1[0] == "Bezeichnung Auftragskonto")
				{
					transactionList.erase(transactionList.begin(), it_vec + 1);
					foundFirstTransaction_b = true;
					break;
				}
			}
			if (true != foundFirstTransaction_b)
			{
				error_b = true;
			}
		}
		else {
			error_b = true;
		}

		return error_b;
	}

	bool fillTransactionsIntoTreeWidget(QTreeWidget& treeWidget_pst, QStringList& transactionList, QVector<TransactionPtr>& unsorted_transactions) {
		bool error_b = false;

		//Clear all TransactionGroups
		for (int i = 0; i < treeWidget_pst.topLevelItemCount(); i++) {
			cTransactionGroups* transactionGroupPtr = dynamic_cast<cTransactionGroups*>(treeWidget_pst.topLevelItem(i));
			if (transactionGroupPtr != nullptr) {
				transactionGroupPtr->clear();
			}
		}

		//Fill Transaction into TransactionGroups
		for (auto it_vec = transactionList.cbegin(); it_vec != transactionList.cend(); it_vec++) {
			QStringList list1 = it_vec->split(";");
			bool inserted_b = false;

			TransactionPtr transPtr(new Transaction(list1));
			for (int i = 0; i < treeWidget_pst.topLevelItemCount(); i++) {
				cTransactionGroups* transactionGroupPtr = dynamic_cast<cTransactionGroups*>(treeWidget_pst.topLevelItem(i));
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
		return error_b;
	}

	bool createSortedFile(QTreeWidget& treeWidget_pst, const QString& transactionList, QVector<TransactionPtr>& unsorted_transactions) {
		QFile file(transactionList);
		if (file.open(QIODevice::ReadWrite)) {
			file.resize(0);
			QTextStream stream(&file);
			for (int i = 0; i < treeWidget_pst.topLevelItemCount(); i++) {
				cTransactionGroups* transactionGroupPtr = dynamic_cast<cTransactionGroups*>(treeWidget_pst.topLevelItem(i));
				if (transactionGroupPtr != nullptr) {
					transactionGroupPtr->outputVolume(stream);
				}
			}
			//Print all unsorted transactions
			if (!unsorted_transactions.isEmpty()) {
				stream << endl << "Unsortierte Transaktionen" << endl;
				for (auto it = unsorted_transactions.cbegin(); it != unsorted_transactions.cend(); it++) {
					it->data()->writeLine(stream);
				}
			}
			file.close();
		}
		return false;
	}
}

