#include "graph.h"
#include "tools.h"
#include <fstream>
#include <cmath>
#include <iostream>

Graph * Graph::load (const char * filename) throw (std::exception &)
{
	std::ifstream f (filename);

	if (f)	
	{
		std::string line;
		do 
		{
			std::getline (f,line);
		}
		while (line.find("NODE_COORD_SECTION") == std::string::npos);
			
		std::map <size_t, std::pair <double,double> > vertices;
		
		while ( std::getline (f,line) and line.find("EOF") == std::string::npos )
		{
			
			std::vector <std::string> tokens (split (line));
			size_t node_id (std::stoll(tokens.at(0)));
			double x (std::stod(tokens.at(1)));
			double y (std::stod(tokens.at(2)));			
			vertices.emplace (node_id, std::make_pair (x,y));
			
		}
		

		return (new Graph (vertices));		
		
	}
	else
		throw ("error with "+std::string(filename));
}

const std::pair <double,double>  & Graph::get_coordinates (size_t i)  const throw (std::exception &)
{
	if ( m_vertices.find (i) != m_vertices.end() )
	{
		return (m_vertices.at(i));
	}
	else
		throw ("bad index ");	
}


void Graph::compute_distance_matrix ()
{
	size_t n (m_vertices.size());

	
	for (size_t i = 1; i <= n; ++i)
	{
		
		m_distance.emplace (i, std::map <size_t,double> () );
		
	}

	#pragma omp parallel for simd
	for (size_t i = 1; i <= n ; ++i)
	{
		for (size_t j = 1; j <= n; ++j)
		{
			m_distance[i][j] = ( i == j ) ? 0.0 : std::sqrt (  (m_vertices.at(j).first - m_vertices.at(i).first)*(m_vertices.at(j).first - m_vertices.at(i).first) + (m_vertices.at(j).second - m_vertices.at(i).second)*(m_vertices.at(j).second - m_vertices.at(i).second) ) ;
			
		}
	}

}

double Graph::get_distance (size_t i, size_t j) const throw (std::exception &)
{
	if ( m_distance.find(i) != m_distance.end() )
	{
		if ( m_distance.at(i).find(j) != m_distance.at(i).end() )
		{
			return (m_distance.at(i).at(j));
		}
		else
			throw ("error second index");
	}
	else
		throw ("error first index"); 
}
