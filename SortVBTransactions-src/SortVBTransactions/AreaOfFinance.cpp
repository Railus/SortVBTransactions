#include "AreaOfFinance.h"
#include <stdio.h>
#include <iostream>
#include <sstream>

void AreaOfFinance::addTransGroup(TransactionGroup trans)
{
	m_transGroups.push_back(trans);
}

bool AreaOfFinance::meetsCondition(const Transaction & trans) const
{
	for (auto it = m_transGroups.begin(); it != m_transGroups.end(); it++) {
		if (it->meetsCondition(trans)) {
			return true;
		}
	}
	return false;
}

bool AreaOfFinance::addTransaction(Transaction & trans)
{
	for (auto it = m_transGroups.begin(); it != m_transGroups.end(); it++) {
		if (it->meetsCondition(trans)) {
			it->addTransaction(trans);
			return true;
		}
	}
	return false;
}

void AreaOfFinance::print(std::ofstream& iOfstream)
{
	iOfstream << m_label << std::endl;
	for (auto it = m_transGroups.begin(); it != m_transGroups.end(); it++) {
		if (!it->empty()) {
			iOfstream << ";"; //add column
			it->print(iOfstream);
		}
		
	}
}

void AreaOfFinance::printFullTransaction(std::ofstream& iOfstream)
{
	iOfstream << m_label << std::endl;
	for (auto it = m_transGroups.begin(); it != m_transGroups.end(); it++) {
		if (!it->empty()) {
			it->printAllTransactions(iOfstream);
		}
	}
}

void SonstigeAreaOfFinance::print(std::ofstream & iOfstream)
{
	iOfstream << m_label << std::endl;
	for (auto it = m_transGroups.begin(); it != m_transGroups.end(); it++) {
		if (!it->empty()) {
			it->printAllTransactions(iOfstream);
		}
	}
}
