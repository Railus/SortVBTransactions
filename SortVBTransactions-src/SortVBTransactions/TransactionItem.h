#pragma once
#include <qtreewidget.h>

class Transaction;

class cTransactionElement :
	public QTreeWidgetItem
{
public:
	cTransactionElement();
	~cTransactionElement();

	virtual bool insertTransaction(Transaction transaction) = 0;
	virtual bool printTransaction(void) = 0;
};



cTransactionElement::cTransactionElement()
{
}


cTransactionElement::~cTransactionElement()
{
}


// Transaction group
class cTransactionGroup :
	public cTransactionElement
{
public:
	cTransactionGroup();
	~cTransactionGroup();

};



cTransactionGroup::cTransactionGroup()
{
}


cTransactionGroup::~cTransactionGroup()
{
}


// Transaction group
class Transaction :
	public cTransactionElement
{
public:
	Transaction() = delete;
	Transaction(QString infos);
	~Transaction();

private:
	QString m_buchungstag;
	QString m_valuta;
	QString m_auftragsgeber;
	QString m_empfaenger;
	QString m_kontonr;
	QString m_iban;
	QString m_blz;
	QString m_bic;
	QString m_verwendungszweck;
	QString m_kundenref;
	QString m_waehrung;
	double m_umsatz;
	char m_soll_haben;
};



Transaction::Transaction(QString infos)
{
}


Transaction::~Transaction()
{
}
