#pragma once

#include <QString>
#include <QSharedPointer>

class Transaction
{
public:
	Transaction() = delete;
	Transaction(QStringList);
	virtual ~Transaction() {};

	const double& getUmsatz(void) const { return m_umsatz; };
	const QString& getVerwendungszweck(void) const { return m_verwendungszweck; };
	const QString& getEmpfaenger(void) const { return m_empfaenger; };

	QString getLine(void) const;
	bool containsKeyword(const QString& keyword);

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
typedef QSharedPointer<Transaction> TransactionPtr;


