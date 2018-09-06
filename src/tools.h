#ifndef TOOLS_H
#define TOOLS_H

#include <algorithm>
#include <vector>
#include <string>

template <class T>
void normalize (typename std::vector <T> & v);



double compute_distance (const std::pair <double, double> & p1, const std::pair <double, double> & p2);
std::vector <std::string> split (const std::string & str, const std::string & delim = " ");



#endif
