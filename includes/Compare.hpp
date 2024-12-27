#pragma once

#include <functional>
#include <string>

struct ci_less : std::binary_function<std::string, std::string, bool>
{
	// case-independent (ci) compare_less binary function
	struct nocase_compare : public std::binary_function<unsigned char,unsigned char,bool> 
	{
		bool operator() (const unsigned char& c1, const unsigned char& c2) const;
	};

	bool operator() (const std::string & s1, const std::string & s2) const;
};
