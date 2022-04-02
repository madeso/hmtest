#include "hm/util.h"

#include <fstream>
#include <cmath>
#include <sstream>
#include <iostream>
#include <cassert>

#include "hm/vfs.h"


std::string
loadFile(const std::string& pFileName)
{
    const auto path = get_base_path() + pFileName;
    std::ifstream file(path.c_str());
    if (file.good())
    {
        std::ostringstream ret;
        ret << file.rdbuf();
        file.close();
        return ret.str();
    }
    else
    {
        std::cerr << "Failed to load " << pFileName << " resolved to " << path
                  << "\n";
    }
    return "";
}


float
randomSign()
{
    return (rand() % 2 == 0) ? -1.0 : 1.0;
}


float
randomRealWithoutSign()
{
    return float(rand()) / float(RAND_MAX);
}


float
randomReal()
{
    const float sign = randomSign();
    return sign * randomRealWithoutSign();
}


void
splitString(char delim, const std::string& s, std::vector<std::string>* numbers)
{
    assert(numbers);
    std::stringstream ss(s);
    std::string item;
    auto result = std::back_inserter(*numbers);
    while (std::getline(ss, item, delim))
    {
        *(result++) = item;
    }
}


std::string
trim(const std::string& s, const std::string& drop)
{
    std::string r = s;
    r = r.erase(s.find_last_not_of(drop) + 1);
    return r.erase(0, r.find_first_not_of(drop));
}


float
toReal(const std::string& str)
{
    std::istringstream s(str);
    float r;
    s >> r;
    return r;
}


int
toInt(const std::string& str)
{
    std::istringstream s(str);
    int r;
    s >> r;
    return r;
}


