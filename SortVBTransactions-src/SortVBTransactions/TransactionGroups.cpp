#include "TransactionGroups.h"

bool cTransactionGroups::insertTransaction(TransactionPtr transaction){
	bool ret_b = false;

	if (this->childCount() > 0)
	{
		for (int i = 0; i < this->childCount(); i++)
		{
			if (true == static_cast<cTransactionGroups*>(this->child(i))->insertTransaction(transaction))
			{
				ret_b = true;
				break;
			}
		}
	}
	else if (!this->text(1).isEmpty())
	{
		QStringList list1(this->text(1).split(",", QString::KeepEmptyParts));
		for (auto it = list1.cbegin(); it != list1.cend(); it++)
		{
			if (transaction->containsKeyword(it->trimmed().toUtf8().constData()))
			{
				m_transactions.push_back(transaction);
				ret_b = true;
				break;
			}
		}
	}
	return ret_b;
};

void cTransactionGroups::clear(){
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
};

double cTransactionGroups::outputVolume(QTextStream& stream) {
	bool ret_b(false);
	double umsatz(0.0);
	QString buffer("");
	static uint level(0);

	if (this->childCount() > 0)
	{
		level++;
		qint64 streamPos = stream.pos();
		for (int i = 0; i < this->childCount(); i++)
		{
			umsatz += static_cast<cTransactionGroups*>(this->child(i))->outputVolume(stream);
		}
		stream.seek(streamPos);
		buffer = stream.readAll() + "\n";
		stream.seek(streamPos);
		level--;
	}
	for (auto it = m_transactions.cbegin(); it != m_transactions.cend(); it++)
	{
		umsatz += it->data()->getUmsatz();
	}
	if (umsatz != 0.0)
	{
		for (uint i = 0; i < level; i++)
		{
			stream << ";";
		}
		stream << this->text(0) << ";" << QString::number(umsatz).replace('.', ',') << endl;
		stream << buffer;
		stream.flush();
	}
	return umsatz;
};