
#ifndef MSSMHBB_INTERFACE_UTILLIB_H_
#define MSSMHBB_INTERFACE_UTILLIB_H_

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

const bool findStrings(const std::string & input, const std::string & needful);
//Function to check whether root file isSombie or not
void CheckZombie(const TFile& name);
void CheckZombieObjectInTFile(const TFile& file, const std::string& name);

int returnMassPoint(const std::string& name);

//}  // namespace myUtil



#endif
