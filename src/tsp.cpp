#include "tsp.h"
#include <fstream>
#include "tools.h"

std::ostream & operator << (std::ostream & os, const TSP & tsp)
{
	os << TSP::minimize_tour (tsp) << std::endl;
	for (auto i = tsp.get_tour().begin(); i != tsp.get_tour().end(); ++i)
	{
		os << (*i) << std::endl ;
	}
	return os;
}

TSP * TSP::load (Graph * g, const char * filename) throw (std::exception &)
{
	std::ifstream f (filename);
	if (f)
	{
		std::string line;
		std::getline (f,line);		// first line is fitness

		std::vector <size_t> tour;
		while (std::getline (f, line) )
		{
			size_t town (std::stoll(line));
			tour.push_back (town);
		}

		return new TSP (g,tour);
	}
	else
		throw ("error with "+std::string(filename));

}

std::vector <TSP> TSP::order1_crossover (const TSP & p1, const TSP & p2)
{
	std::vector <TSP> offsprings;
			
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	srand(seed);
  	std::default_random_engine gen (seed);
	std::uniform_int_distribution <size_t> distrib (0,p1.m_tour.size()-1);
			
	size_t pfirst, psecond ;
			
	pfirst = distrib (gen);
	psecond = distrib (gen);
	
	while ( psecond == pfirst)
	{
		psecond = distrib(gen);
				
	}

	if ( psecond < pfirst )
	{	
		size_t tmp = pfirst;
		pfirst = psecond;
		psecond = tmp;
	}
			
	std::pair <size_t,size_t> p (std::make_pair (pfirst,psecond));
		

	return (offsprings);

}


std::vector <TSP> TSP::pmx_crossover (const TSP & p1, const TSP & p2)
{
	std::vector <TSP> offsprings;
			
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	srand(seed);
  	std::default_random_engine gen (seed);
	std::uniform_int_distribution <size_t> distrib (0,p1.m_tour.size()-1);
			
	size_t pfirst, psecond ;
			
	pfirst = distrib (gen);
	psecond = distrib (gen);
	
	while ( psecond == pfirst)
	{
		psecond = distrib(gen);
				
	}

	if ( psecond < pfirst )
	{	
		size_t tmp = pfirst;
		pfirst = psecond;
		psecond = tmp;
	}
			
	std::pair <size_t,size_t> p (std::make_pair (pfirst,psecond));
			
			
	TSP child1 (p1.m_graph,std::vector<size_t>(p1.m_graph->get_size(),0));
	

	std::vector <size_t> all;
	for (size_t i = 0; i < p1.m_graph->get_size(); ++i)
		all.push_back (i);
			
	for (size_t i = p.first ; i <= p.second; ++i)
	{
		size_t e = p1.m_tour.at(i);
		child1.m_tour[i] = e;
		all.erase (std::find (all.begin(), all.end(), i));
	}
			
	TSP::print(p1);
	TSP::print(p2);

	for (size_t i = 0; i < p1.m_tour.size(); ++i)
	{
		if ( i >= p.first and i <= p.second )
		{
			std::cout << p1.m_tour.at(i) << "|" ;
		}
		else
			std::cout << "_|" ;
	}
	std::cout << std::endl;

	std::vector < std::pair <size_t, size_t> > C;
	for (size_t i = p.first; i <= p.second; ++i)
	{
		size_t a = p2.m_tour.at(i);
		auto find = std::find (child1.m_tour.begin(), child1.m_tour.end(), a);
		if ( find == child1.m_tour.end() )
		{ 
			size_t pos = p1.get_element_pos (a);
			C.push_back (std::make_pair (pos,a));
		}	
	}

	for (auto p : C)
	{
		size_t e = child1.m_tour.at(p.first);
		if ( e == 0 )
		{
			child1.m_tour[p.first] = p.second;
			all.erase (std::find (all.begin(), all.end(), p.first) );
		}
		else
		{
			size_t pos = p1.get_element_pos(p.second);
			child1.m_tour[pos] = p.second;
			all.erase (std::find (all.begin(), all.end(), p.first) );
		}
	}

	size_t i = 0;
	while ( i < child1.m_tour.size() )
	{
		size_t e = child1.m_tour.at(i);
		if ( e == 0 )
		{
			size_t a = p2.m_tour.at(i);
			auto find = std::find (child1.m_tour.begin(), child1.m_tour.end(), a);
			if ( find == child1.m_tour.end() )
			{
				child1.m_tour[i]= a;
				all.erase (std::find (all.begin(), all.end(), i) );
			}
		}
		++i;
	}
			
	for (size_t i = 0; i < child1.m_tour.size(); ++i)
	{
		size_t e = child1.m_tour.at(i);
		if ( e == 0)
		{
			size_t a = p1.m_tour.at(i);
			auto find = std::find (child1.m_tour.begin(), child1.m_tour.end(), a);
			if ( find == child1.m_tour.end() )
			{
				child1.m_tour[i]= a;
				all.erase (std::find (all.begin(), all.end(), i) );
			}
		}
	}

	
	TSP::print(child1);
	
	

	exit(EXIT_SUCCESS);
	offsprings.push_back (child1);
	offsprings.push_back (child1);

	return (offsprings);
}
