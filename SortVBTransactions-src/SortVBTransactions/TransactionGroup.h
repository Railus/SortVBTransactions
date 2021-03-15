#pragma once

#include <stdio.h>
#include <vector>
#include "Transaction.h"
#include <fstream>
#if 0
class TransactionGroup
{
public:
	TransactionGroup() = delete;
	TransactionGroup(std::string label) : m_label(label) {};
	virtual ~TransactionGroup() {};

	bool empty() const { return m_transactions.empty(); };
	double getSum(void) const;
	double getSum(std::string const keyword) const;
	std::vector<Transaction>& getTransactions(void) { return m_transactions; };
	std::string& getLabel(void) { return m_label; };
	void addKeyword(const std::string& iKeyword);
	bool meetsCondition(const Transaction& trans) const;
	void addTransaction(Transaction trans);
	void print(std::ofstream& iOfstream);
	void printAllTransactions(std::ofstream& iOfstream);

private:
	std::vector<Transaction> m_transactions;
	std::string m_label;
	std::vector<std::string> m_keywords;
};
#endif