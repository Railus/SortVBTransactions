#include "TransactionGroup.h"
#include <stdio.h>
#include <sstream>
#include <iostream>
#include <algorithm>
#if 0
double TransactionGroup::getSum(void) const
{
	double ret_val = 0.0;
	if (m_transactions.begin() != m_transactions.end()) {
		for (auto it_transaction = m_transactions.begin(); it_transaction != m_transactions.end(); it_transaction++) {
			ret_val += it_transaction->getUmsatz();
		}
	}
	else {
		std::cout << "Transaction group is empty!" << std::endl;
	}
	return ret_val;
}

double TransactionGroup::getSum(std::string const keyword) const
{
	double ret_val = 0.0;
	if (m_transactions.begin() != m_transactions.end()) {
		for (auto it_transaction = m_transactions.begin(); it_transaction != m_transactions.end(); it_transaction++) {
			if (it_transaction->getVerwendungszweck().find(keyword) != std::string::npos) {
				ret_val += it_transaction->getUmsatz();
				continue;
			}
			else if (it_transaction->getEmpfaenger().find(keyword) != std::string::npos) {
				ret_val += it_transaction->getUmsatz();
				continue;
			}
		}
	}
	else {
		std::cout << "Transaction group is empty!" << std::endl;
	}
	return ret_val;
}

void TransactionGroup::addKeyword(const std::string& iKeyword)
{
	if(!iKeyword.empty()){
		m_keywords.push_back(iKeyword);
	}
}

bool TransactionGroup::meetsCondition(const Transaction& trans) const
{
	bool ret_val = false;

	for (auto it_keyword = m_keywords.begin(); it_keyword != m_keywords.end(); it_keyword++) {
		if (trans.getEmpfaenger().find(*it_keyword) != std::string::npos) {
			ret_val = true;
			break;
		}
		else if(trans.getVerwendungszweck().find(*it_keyword) != std::string::npos){
			ret_val = true;
			break;
		}
	}

	return ret_val;
}

void TransactionGroup::addTransaction(Transaction trans)
{
	m_transactions.push_back(trans);
}

void TransactionGroup::print(std::ofstream& iOfstream)
{
	double ret_val = 0.0;
	if (m_transactions.begin() != m_transactions.end()) {
		for (auto it_transaction = m_transactions.begin(); it_transaction != m_transactions.end(); it_transaction++) {
			ret_val += it_transaction->getUmsatz();
		}
		std::string temp_sum_s(std::to_string(ret_val));
		std::replace(temp_sum_s.begin(), temp_sum_s.end(), '.', ',');

		iOfstream << m_label << ";" << temp_sum_s << std::endl;
	}
	else {
		std::cout << "Transaction group is empty!" << std::endl;
	}

}

void TransactionGroup::printAllTransactions(std::ofstream & iOfstream)
{
	if (m_transactions.begin() != m_transactions.end()) {
		for (auto it_transaction = m_transactions.begin(); it_transaction != m_transactions.end(); it_transaction++) {
			iOfstream << it_transaction->getLine() <<  std::endl;
		}
	}
	else {
		std::cout << "Transaction group is empty!" << std::endl;
	}
}
#endif