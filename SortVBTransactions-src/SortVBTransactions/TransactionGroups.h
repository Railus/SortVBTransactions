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
	virtual ~cTransactionGroups() {};

	void clear();
	bool insertTransaction(TransactionPtr transaction);
	double outputVolume(QTextStream&);

private:
	QVector<TransactionPtr> m_transactions;

};
typedef QSharedPointer<cTransactionGroups> TransactionGroupsPtr;
