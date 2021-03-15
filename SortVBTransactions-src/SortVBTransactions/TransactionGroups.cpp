#include "TransactionGroups.h"

bool cTransactionGroups::insertTransaction(TransactionPtr transaction)
{
	bool ret_b = false;

	if (this->childCount() > 0)
	{
		for (int i = 0; i < this->childCount(); i++)
		{
			if (true == static_cast<cTransactionGroups*>(this->child(i))->insertTransaction(transaction)) {
				ret_b = true;
				break;
			}
		}
	}
	else if (!this->text(1).isEmpty())
	{
		QStringList list1(this->text(1).split(",", QString::KeepEmptyParts));
		for (auto it = list1.cbegin(); it != list1.cend(); it++) {

			if (transaction->containsKeyword(it->trimmed().toUtf8().constData())) {
				m_transactions.push_back(transaction);
				ret_b = true;
				break;
			}
		}
	}
	return ret_b;
};

bool cTransactionGroups::clear()
{
	bool ret_b = false;

	if (this->childCount() > 0)
	{
		for (int i = 0; i < this->childCount(); i++)
		{
			static_cast<cTransactionGroups*>(this->child(i))->clear();
		}
	}
	else if (!this->text(1).isEmpty())
	{
		m_transactions.clear();
	}
	return ret_b;
};

double cTransactionGroups::write(QTextStream& stream) {
	bool ret_b = false;
	double balance = 0.0;
	QString buffer("");
	static uint level = 0;

	if (this->childCount() > 0)
	{
		level++;
		qint64 streamPos = stream.pos();
		for (int i = 0; i < this->childCount(); i++)
		{
			balance += static_cast<cTransactionGroups*>(this->child(i))->write(stream);
		}
		stream.seek(streamPos);
		buffer = stream.readAll() + "\n";
		stream.seek(streamPos);
		level--;
	}
	for (auto it = m_transactions.cbegin(); it != m_transactions.cend(); it++) {
		balance += it->data()->getUmsatz();
	}
	if (balance != 0.0) {
		for (uint i = 0; i < level; i++) {
			stream << ";";
		}
		stream << this->text(0) << ";" << QString::number(balance).replace('.', ',') << endl;
		stream << buffer;
		stream.flush();
	}
	return balance;
};