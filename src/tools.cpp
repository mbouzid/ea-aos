#include "tools.h"
#include <cmath>

template <class T>
void normalize (typename std::vector <T> & v)
{
	T sum = std::accumulate (std::next(v.begin()), v.end(), *v.begin());
	
	for (typename std::vector<T>::iterator i = v.begin(); i != v.end(); ++i)
	{
		if ( *i != 0 )	
		{
			*i = *i / sum;
		}
	}
}



double compute_distance (const std::pair <double, double> & p1, const std::pair <double, double> & p2)
{
	return std::sqrt( (p2.first-p1.first)*(p2.first-p1.first) + (p2.second-p1.second)*(p2.second-p1.second) );
}


std::vector <std::string> split (const std::string & str, const std::string & delim)
{
	 std::vector<std::string> tokens;
   	 std::size_t start = str.find_first_not_of(delim), end = 0;

    	while((end = str.find_first_of(delim, start)) != std::string::npos)
    	{
        	tokens.push_back(str.substr(start, end - start));
        	start = str.find_first_not_of(delim, end);
    	}

    	if (start != std::string::npos)
        {
		tokens.push_back(str.substr(start));
	}


	return (tokens);
}


template void normalize <double> (std::vector <double> &);
