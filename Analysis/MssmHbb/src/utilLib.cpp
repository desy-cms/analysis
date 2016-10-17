#include "Analysis/MssmHbb/interface/utilLib.h"

const bool findStrings(const std::string & input, const std::string & needful)
{
	std::string input1 = input;
	std::string input2 = needful;
	std::transform(input1.begin(),input1.end(),input1.begin(),::tolower);
	std::transform(input2.begin(),input2.end(),input2.begin(),::tolower);
	if(input1.find(input2) != std::string::npos) return true;
	else return false;
}
