#pragma once

#include <stdio.h>
#include <vector>
#include "main.h"
#include <QSharedPointer>

class Transaction
{
public:
	Transaction() = delete;
	Transaction(std::vector<std::string>);
	Transaction(QStringList);
	virtual ~Transaction() {};

	const double& getUmsatz(void) const { return m_umsatz; };
	const std::string& getVerwendungszweck(void) const { return m_verwendungszweck; };
	const std::string& getEmpfaenger(void) const { return m_empfaenger; };

	std::string getLine(void) const;
	bool containsKeyword(const std::string& keyword);

private:
	std::string m_buchungstag;
	std::string m_valuta;
	std::string m_auftragsgeber;
	std::string m_empfaenger;
	std::string m_kontonr;
	std::string m_iban;
	std::string m_blz;
	std::string m_bic;
	std::string m_verwendungszweck;
	std::string m_kundenref;
	std::string m_waehrung;
	double m_umsatz;
	char m_soll_haben;
};
typedef QSharedPointer<Transaction> TransactionPtr;


#include <TransactionItem.h>

// Transaction group
class cTransactionGroups :
	public cTransactionElement
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
