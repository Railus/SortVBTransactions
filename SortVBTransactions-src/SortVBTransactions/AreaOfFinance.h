#pragma once

#include <stdio.h>
#include <vector>
#include "TransactionGroup.h"

class AreaOfFinance
{
public:
	AreaOfFinance() = delete;
	AreaOfFinance(std::string label) : m_label(label) {};
	virtual ~AreaOfFinance() {};

	void addTransGroup(TransactionGroup trans);
	bool meetsCondition(const Transaction& trans) const;
	bool addTransaction(Transaction& trans);
	virtual void print(std::ofstream& iOfstream);
	virtual void printFullTransaction(std::ofstream& iOfstream);

protected:
	std::vector<TransactionGroup> m_transGroups;
	std::string m_label;
};

class SonstigeAreaOfFinance : public virtual AreaOfFinance
{
public:
	SonstigeAreaOfFinance() = delete;
	SonstigeAreaOfFinance(std::string label) : AreaOfFinance(label) {};
	virtual ~SonstigeAreaOfFinance() {};
	virtual void print(std::ofstream& iOfstream);
};
