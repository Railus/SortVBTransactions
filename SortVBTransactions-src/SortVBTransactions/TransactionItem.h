#pragma once
#include <qtreewidget.h>
#include <Transaction.h>

class cTransactionElement :
	public QTreeWidgetItem
{
public:
	cTransactionElement() {};
	~cTransactionElement() {};

	virtual bool insertTransaction(TransactionPtr transaction) = 0;
	virtual double write(QTextStream&) = 0;
};


//// Transaction group
//class Transaction
//{
//public:
//	Transaction() = delete;
//	Transaction(QString infos);	// Todo: Insert some sort of info vector
//	~Transaction();
//
//private:
//	QString m_buchungstag;
//	QString m_valuta;
//	QString m_auftragsgeber;
//	QString m_empfaenger;
//	QString m_kontonr;
//	QString m_iban;
//	QString m_blz;
//	QString m_bic;
//	QString m_verwendungszweck;
//	QString m_kundenref;
//	QString m_waehrung;
//	double m_umsatz;
//	char m_soll_haben;
//};
//
//
//
//Transaction::Transaction(QString infos)
//{
//}
//
//
//Transaction::~Transaction()
//{
//}
