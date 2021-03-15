#pragma once
#include <QTreewidget>
#include <QTextStream>
#include <QVector>
#include <QSharedPointer>
#include "Transaction.h"

// Transaction group
class cTransactionGroups :
	public QTreeWidgetItem
{
public:
	cTransactionGroups() {};
	~cTransactionGroups() {};

	bool clear();
	bool insertTransaction(TransactionPtr transaction);
	double write(QTextStream&);

private:
	QVector<TransactionPtr> m_transactions;

};
typedef QSharedPointer<cTransactionGroups> TransactionGroupsPtr;
