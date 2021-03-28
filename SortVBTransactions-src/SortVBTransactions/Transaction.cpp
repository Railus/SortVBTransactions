#include "Transaction.h"
#include "main.h"

Transaction::Transaction(QStringList qlist)
{
	if (qlist.size() == TRANSACTION_ROWS) {
		m_buchungstag = qlist[0];
		m_valuta = qlist[1];
		m_sender = qlist[2];
		m_receiver = qlist[3];
		m_accNr = qlist[4];
		m_iban = qlist[5];
		m_blz = qlist[6];
		m_bic = qlist[7];
		m_reference = qlist[8];
		m_customRef = qlist[9];
		m_currency = qlist[10];
		qlist[11].replace(",", ".");
		qlist[11].chop(1);
		qlist[11] = qlist[11].mid(1);
		// Remove double dot sign
		if (1 < qlist[11].count('.')) { qlist[11].remove(qlist[11].indexOf('.'),1); };
		m_soll_haben = static_cast<char>(qlist[12].toStdString()[1]);
		if (m_soll_haben == 'S') {
			m_volume = qlist[11].toDouble(); //delete "/"" at beginning and end of the string
		}
		else if (m_soll_haben == 'H') {
			m_volume = -qlist[11].toDouble(); //delete "/"" at beginning and end of the string
		}
		else {
			qDebug("Missing transaction identifier \'H\'/\'S\'!");
		}
	}
	else {
		qDebug("Row has not 13 columns!");
	}
}

void Transaction::writeLine(QTextStream& stream) const {
	QString ret_string;
	stream << (this->m_buchungstag + ";");
	stream << (this->m_valuta + ";");
	stream << (this->m_sender + ";");
	stream << (this->m_receiver + ";");
	stream << (this->m_accNr + ";");
	stream << (this->m_iban + ";");
	stream << (this->m_blz + ";");
	stream << (this->m_bic + ";");
	stream << (this->m_reference + ";");
	stream << (this->m_customRef + ";");
	stream << (this->m_currency + ";");
	stream << (QString::number(this->m_volume).replace('.', ',') + ";");
	stream << (QString(&this->m_soll_haben,1));
	stream << endl;
	stream.flush();
}

bool Transaction::containsKeyword(const QString& keyword) {
	bool ret_val = false;

	if (m_receiver.contains(keyword,Qt::CaseInsensitive))
	{
		ret_val = true;
	}
	else if (m_reference.contains(keyword, Qt::CaseInsensitive))
	{
		ret_val = true;
	}

	return ret_val;
}
