#pragma once
#include <QString>
#include <QTreeWidget>
#include "Transaction.h"

namespace HelperFunctions {
	bool createTemplateTree(const QString& dir, QTreeWidget& treeWidget_pst);

	/**********************/
	//Read all rows of transaction
	bool getTransactionRowsFromFile(const QString& dir, QStringList& transactionList);

	bool fillTransactionsIntoTreeWidget(QTreeWidget& treeWidget_pst, QStringList& transactionList, QVector<TransactionPtr>& unsorted_transactions);

	bool createSortedFile(QTreeWidget& treeWidget_pst, const QString& transactionList, QVector<TransactionPtr>& unsorted_transactions);
}

