#pragma once

#include <QString>
#include <QSharedPointer>
#include <QTextStream>

class Transaction
{
public:
	Transaction() = delete;
	Transaction(QStringList);
	virtual ~Transaction() {};

	const double& getUmsatz(void) const { return m_volume; };

	void writeLine(QTextStream& stream) const;
	bool containsKeyword(const QString& keyword);

private:
	QString m_buchungstag;
	QString m_valuta;
	QString m_sender;
	QString m_receiver;
	QString m_accNr;
	QString m_iban;
	QString m_blz;
	QString m_bic;
	QString m_reference;
	QString m_customRef;
	QString m_currency;
	double m_volume;
	QChar m_soll_haben;
};
typedef QSharedPointer<Transaction> TransactionPtr;


