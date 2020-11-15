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
