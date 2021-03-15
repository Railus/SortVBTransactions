#include "Transaction.h"
#include "main.h"

Transaction::Transaction(QStringList qlist)
{
	if (qlist.size() == TRANSACTION_ROWS) {
		m_buchungstag = qlist[0];
		m_valuta = qlist[1];
		m_auftragsgeber = qlist[2];
		m_empfaenger = qlist[3];
		m_kontonr = qlist[4];
		m_iban = qlist[5];
		m_blz = qlist[6];
		m_bic = qlist[7];
		m_verwendungszweck = qlist[8];
		m_kundenref = qlist[9];
		m_waehrung = qlist[10];
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
			qDebug("Missing transaction identifier \'H\'/\'S\'!");
		}
	}
	else {
		qDebug("Row has not 13 columns!");
	}
}

QString Transaction::getLine(void) const {
	QString ret_string;
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
	QString temp_substring = QString::number(this->m_umsatz);
	std::replace(temp_substring.begin(), temp_substring.end(), '.',',');
	ret_string += (temp_substring + ";");
	ret_string.push_back(this->m_soll_haben);
	return ret_string;
}

bool Transaction::containsKeyword(const QString& keyword) {
	bool ret_val = false;

	if (m_empfaenger.contains(keyword,Qt::CaseInsensitive)) {
		ret_val = true;
	}
	else if (m_verwendungszweck.contains(keyword, Qt::CaseInsensitive)) {
		ret_val = true;
	}

	return ret_val;
}
