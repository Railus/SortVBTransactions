#include "Transaction.h"
#include <iostream>
#include <sstream>
#include <algorithm>

Transaction::Transaction(std::vector<std::string> row)
{
	if (row.size() == TRANSACTION_ROWS) {
		m_buchungstag = row[0];
		m_valuta = row[1];
		m_auftragsgeber = row[2];
		m_empfaenger = row[3];
		m_kontonr = row[4];
		m_iban = row[5];
		m_blz = row[6];
		m_bic = row[7];
		m_verwendungszweck = row[8];
		m_kundenref = row[9];
		m_waehrung = row[10];
		std::string temp_substring = row[11].substr(1, row[11].size() - 2);
		std::replace(temp_substring.begin(), temp_substring.end(), ',', '.');
		m_soll_haben = static_cast<char>(row[12][1]);
		if (m_soll_haben == 'S') {
			m_umsatz = std::stod(temp_substring); //delete "/"" at beginning and end of the string
		}
		else if(m_soll_haben == 'H'){
			m_umsatz = -std::stod(temp_substring); //delete "/"" at beginning and end of the string
		}
		else {
			std::cout << "Missing transaction identifier \'H\'/\'S\'!" << std::endl;
		}
	}
	else {
		std::cout << "Row has not 13 columns!" << std::endl;
	}
}

Transaction::Transaction(QStringList qlist)
{
	if (qlist.size() == TRANSACTION_ROWS) {
		m_buchungstag = qlist[0].toStdString();
		m_valuta = qlist[1].toStdString();
		m_auftragsgeber = qlist[2].toStdString();
		m_empfaenger = qlist[3].toStdString();
		m_kontonr = qlist[4].toStdString();
		m_iban = qlist[5].toStdString();
		m_blz = qlist[6].toStdString();
		m_bic = qlist[7].toStdString();
		m_verwendungszweck = qlist[8].toStdString();
		m_kundenref = qlist[9].toStdString();
		m_waehrung = qlist[10].toStdString();
		qlist[11].replace(",", ".");
		qlist[11].chop(1);
		qlist[11] = qlist[11].mid(1);
		// Remove double dot sign
		if (1 < qlist[11].count('.')) { qlist[11].remove(qlist[11].indexOf('.'),1); };
		m_soll_haben = static_cast<char>(qlist[12].toStdString()[1]);
		if (m_soll_haben == 'S') {
			m_umsatz = qlist[11].toDouble(); //delete "/"" at beginning and end of the string
		}
		else if (m_soll_haben == 'H') {
			m_umsatz = -qlist[11].toDouble(); //delete "/"" at beginning and end of the string
		}
		else {
			std::cout << "Missing transaction identifier \'H\'/\'S\'!" << std::endl;
		}
	}
	else {
		std::cout << "Row has not 13 columns!" << std::endl;
	}
}

std::string Transaction::getLine(void) const {
	std::string ret_string;
	ret_string += (this->m_buchungstag + ";");
	ret_string += (this->m_valuta + ";");
	ret_string += (this->m_auftragsgeber + ";");
	ret_string += (this->m_empfaenger + ";");
	ret_string += (this->m_kontonr + ";");
	ret_string += (this->m_iban + ";");
	ret_string += (this->m_blz + ";");
	ret_string += (this->m_bic + ";");
	ret_string += (this->m_verwendungszweck + ";");
	ret_string += (this->m_kundenref + ";");
	ret_string += (this->m_waehrung + ";");
	std::string temp_substring = std::to_string(this->m_umsatz);
	std::replace(temp_substring.begin(), temp_substring.end(), '.',',');
	ret_string += (temp_substring + ";");
	ret_string.push_back(this->m_soll_haben);
	return ret_string;
}

bool Transaction::containsKeyword(const std::string& keyword) {
	bool ret_val = false;

	if (m_empfaenger.find(keyword) != std::string::npos) {
		ret_val = true;
	}
	else if (m_verwendungszweck.find(keyword) != std::string::npos) {
		ret_val = true;
	}

	return ret_val;
}

#include <QTextStream>
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
		stream << this->text(0) << ";" << QString::number(balance).replace('.',',') << endl;
		stream << buffer;
		stream.flush();
	}
	return balance;
};