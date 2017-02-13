
#ifndef MSSMHBB_INTERFACE_UTILLIB_H_
#define MSSMHBB_INTERFACE_UTILLIB_H_

#include <sys/stat.h>

#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <cctype>
#include <algorithm>
#include "TFile.h"

//namespace myUtil {

template <typename T1, typename T2>
struct less_second {
    typedef std::pair<T1, T2> type;
    bool operator ()(type const& a, type const& b) const {
        return a.second < b.second;
    }
};

template <typename T1, typename T2>
struct great_second {
    typedef std::pair<T1, T2> type;
    bool operator ()(type const& a, type const& b) const {
        return a.second > b.second;
    }
};

template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 6)
{
    std::ostringstream out;
    out << std::setprecision(n) << a_value;
    return out.str();
}

// Function to check whether file exists or not
inline bool file_exists(const std::string&name);

const bool findStrings(const std::string & input, const std::string & needful);
//Function to check whether root file isSombie or not
void CheckZombie(const TFile& name);
//Function to get TFile:
const TFile& OpenTFile(const std::string& name);
void CheckZombieObjectInTFile(const TFile& file, const std::string& name);

int returnMassPoint(const std::string& name);

//}  // namespace myUtil

inline bool file_exists(const std::string&name){
	struct stat buffer;
	return (stat (name.c_str(), &buffer) == 0);
}


#endif
