#include "Transaction.h"
#include "main.h"

Transaction::Transaction(QStringList qlist)
{
	if (qlist.size() == TRANSACTION_ROWS) {
		m_buchungstag = qlist[4];
		m_valuta = qlist[5];
		m_sender = "Simon Dwucet"; 
		m_receiver = qlist[6]; 
		m_accNr = "";
		m_iban = qlist[7];
		m_blz = "NA"; 
		m_bic = qlist[8]; 
		m_reference = qlist[9];
		m_customRef = qlist[10];
		m_currency = qlist[12];
		qlist[11].replace(",", ".");
		qlist[11].chop(1);
		//qlist[12] = qlist[12].mid(1);
		// Remove double dot sign
		if (1 < qlist[11].count('.')) { qlist[11].remove(qlist[11].indexOf('.'), 1); };
		//m_soll_haben = static_cast<char>(qlist[13].toStdString()[0]);
		//if (m_soll_haben == 'S') {
			m_volume = qlist[11].toDouble(); //delete "/"" at beginning and end of the string
		//}
		//else if (m_soll_haben == 'H') {
		//	m_volume = -qlist[12].toDouble(); //delete "/"" at beginning and end of the string
		//}
		//else {
		//	qDebug("Missing transaction identifier \'H\'/\'S\'!");
		//}
	}
	else {
		qDebug("Row has not 19 columns!");
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
	else if (m_customRef.contains(keyword, Qt::CaseInsensitive))
	{
		ret_val = true;
	}

	return ret_val;
}
