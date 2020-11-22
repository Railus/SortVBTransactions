#pragma once

#include <stdio.h>
#include <vector>

#define VERSION_MAJOR 1
#define VERSION_MINOR 2

#define TRANSACTION_ROWS (unsigned short) 13

template<char delimiter>
class WordDelimitedBy : public std::string
{};

template<char delimiter>
std::istream& operator>>(std::istream& is, WordDelimitedBy<delimiter>& output)
{
	std::getline(is, output, delimiter);
	return is;
}
