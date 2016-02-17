#include <ostream>
#include <istream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
struct Range
{
    friend std::istream& operator>>(std::istream& is, Range& p);
    friend std::ostream& operator<<(std::ostream& os, const Range& p);

    int lower;
    int bigger;
    std::vector <int> allruns;
};

std::istream& operator>>(std::istream& is, Range& p)
{
    is.ignore(1, '[');
    std::string s;
    std::getline(is, s, ']');

    auto commaPos = std::find(s.begin(), s.end(), ',');

    p.lower = std::stoi(std::string{s.begin(), commaPos});
    p.bigger = std::stoi(std::string{std::next(commaPos, 2), s.end()});

    return is;
}

std::ostream& operator<<(std::ostream& os, const Range& p)
{
    return os << "[" << p.lower << ", " << p.bigger << "]";
}

struct Period
{
    friend std::istream& operator>>(std::istream& is, Period& p);
    friend std::ostream& operator<<(std::ostream& os, const Period& p);

    string name;
    std::vector<Range> ranges;
};

std::istream& operator>>(std::istream& is, Period& p)
{
    std::getline(is, p.name, ':');

    is.ignore(std::numeric_limits<std::streamsize>::max(), '[');
    do
    {
        Range r;
        is >> r;
        p.ranges.push_back(r);
    } while(!(is.ignore(std::numeric_limits<std::streamsize>::max(), ' ').eof()));

    return is;
}

std::ostream& operator<<(std::ostream& os, const Period& p)
{
    os << p.name << ": [";
    for(auto a = p.ranges.begin(); a != std::prev(p.ranges.end()); ++a)
        os << *a << ", ";
    os << *(std::prev(p.ranges.end())) << "],";

    return os;
}



