#ifndef TSP_H
#define TSP_H

#include <vector>
#include <cstdlib>
#include <iostream>
#include "graph.h"
#include <ctime>
#include <random>
#include <chrono>
#include <algorithm>

class TSP
{
	
	private:
	
		Graph * m_graph;
		std::vector <size_t > m_tour;

		void initialize_random ()
		{
			for (size_t i = 1; i <= m_graph->get_size() ; ++i)
			{
				m_tour.push_back (i);
			}
			
			std::random_shuffle (m_tour.begin(), m_tour.end()) ;
			
		}

	public:

		TSP
		(
			Graph * graph
		):
			m_graph (graph)
		{
			initialize_random ();
		}

		TSP
		(
			Graph * graph,
			const std::vector <size_t> & tour
		):
			m_graph (graph),
			m_tour (tour)
		{}

		TSP (const TSP & tsp):
			m_graph (tsp.m_graph),	
			m_tour (tsp.m_tour)
		{}

		static TSP * load (Graph * g, const char * filename) throw (std::exception &);

		const std::vector <size_t> & get_tour () const
		{
			return (m_tour);
		}
		
		static std::pair <std::vector <size_t>::iterator,std::vector <size_t>::iterator>  get_random_segment (TSP & tsp, bool ordered = true) 
		{
			
			unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
			
  			std::default_random_engine gen (seed);
			std::uniform_int_distribution <size_t> distrib (0,tsp.m_tour.size()-1);
			
			size_t pfirst, psecond ;
			
			pfirst = distrib (gen);
			psecond = distrib (gen);
			while ( psecond == pfirst)
			{
				psecond = distrib(gen);
				
			}

			if (ordered)
			{
				if ( psecond < pfirst )
				{	
					size_t tmp = pfirst;
					pfirst = psecond;
					psecond = tmp;
				}
			}
			
			std::vector<size_t>::iterator itsec = std::find (tsp.m_tour.begin(), tsp.m_tour.end(), tsp.m_tour.at(psecond) );			
			std::vector<size_t>::iterator itfirst = std::find (tsp.m_tour.begin(), tsp.m_tour.end(), tsp.m_tour.at(pfirst) );

			return std::make_pair (itfirst,itsec);
		}

		static std::pair <size_t,size_t>  get_random_segment_pos (const TSP & tsp, bool ordered = true) 
		{
			
			unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
			srand(seed);
  			std::default_random_engine gen (seed);
			std::uniform_int_distribution <size_t> distrib (0,tsp.m_tour.size()-1);
			
			size_t pfirst, psecond ;
			
			pfirst = distrib (gen);
			psecond = distrib (gen);
			while ( psecond == pfirst)
			{
				psecond = distrib(gen);
				
			}

			if (ordered)
			{
				if ( psecond < pfirst )
				{	
					size_t tmp = pfirst;
					pfirst = psecond;
					psecond = tmp;
				}
			}
			
			return std::make_pair (pfirst,psecond);
		}

		static void insertion (TSP & tsp)
		{
			unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  			std::default_random_engine gen (seed);
			std::uniform_int_distribution <size_t> distrib (0,tsp.m_tour.size()-1);
			
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
			
			
			std::vector<size_t>::iterator itsec = std::find (tsp.m_tour.begin(), tsp.m_tour.end(), tsp.m_tour.at(psecond) );			
			std::vector<size_t>::iterator itfirst = std::find (tsp.m_tour.begin(), tsp.m_tour.end(), tsp.m_tour.at(pfirst) );
	
			size_t second = tsp.m_tour.at(psecond);			
			tsp.m_tour.erase (itsec);

			tsp.m_tour.insert(itfirst+1, second); 
			

		}	
		static void print (const TSP & t)
		{
			for (auto i = t.m_tour.begin() ; i != t.m_tour.end(); ++i)
			{
				std::cout << *i << "|";
			}
			std::cout << std::endl;
		}	
		
		static void swap (TSP & tsp)
		{
			unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  			std::default_random_engine gen (seed);
			std::uniform_int_distribution <size_t> distrib (0,tsp.m_tour.size()-1);
			
			size_t pfirst, psecond ;
			
			pfirst = distrib (gen);
			psecond = distrib (gen);
			while ( psecond == pfirst)
			{
				psecond = distrib(gen);
				
			}

			
			std::vector<size_t>::iterator itsec = std::find (tsp.m_tour.begin(), tsp.m_tour.end(), tsp.m_tour.at(psecond) );			
			std::vector<size_t>::iterator itfirst = std::find (tsp.m_tour.begin(), tsp.m_tour.end(), tsp.m_tour.at(pfirst) );
	
			std::iter_swap (itfirst, itsec);
			
		}




		static void inversion (TSP & tsp)
		{
			unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  			std::default_random_engine gen (seed);
			std::uniform_int_distribution <size_t> distrib (0,tsp.m_tour.size()-1);
			
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
			
			std::vector<size_t>::iterator itsec = std::find (tsp.m_tour.begin(), tsp.m_tour.end(), tsp.m_tour.at(psecond) );			
			std::vector<size_t>::iterator itfirst = std::find (tsp.m_tour.begin(), tsp.m_tour.end(), tsp.m_tour.at(pfirst) );

			std::reverse(itfirst, itsec);
		

		}

		static void useless (TSP & tsp)
		{
			std::random_shuffle (tsp.m_tour.begin(), tsp.m_tour.end());
		}

		static void scramble (TSP & tsp)
		{
			unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  			std::default_random_engine gen (seed);
			std::uniform_int_distribution <size_t> distrib (0,tsp.m_tour.size()-1);
			
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
			
			std::vector<size_t>::iterator itsec = std::find (tsp.m_tour.begin(), tsp.m_tour.end(), tsp.m_tour.at(psecond) );			
			std::vector<size_t>::iterator itfirst = std::find (tsp.m_tour.begin(), tsp.m_tour.end(), tsp.m_tour.at(pfirst) );

			std::random_shuffle(itfirst, itsec+1);
		}

		size_t get_element_pos (size_t e) const
		{
			size_t pos = 0;
			for (size_t i = 1; i < m_tour.size(); ++i)
			{
				if ( m_tour.at(i) == e )
				{
					pos = i;
					break;
				}
			}
			return (pos);
		}
		

		
		static std::vector <TSP> pmx_crossover (const TSP & p1, const TSP & p2);
		static std::vector <TSP> order1_crossover (const TSP & p1, const TSP & p2);

		static std::vector <TSP> identity_crossover (const TSP & p1, const TSP & p2)
		{
			std::vector <TSP> offsprings;
			offsprings.push_back (p1);
			offsprings.push_back (p2);
			return (offsprings);
		}

		static float minimize_tour (const TSP & tsp)
		{
			double toursize = 0.0;
			size_t n =  tsp.m_tour.size()-1 ;
			toursize += tsp.m_graph->get_distance(tsp.m_tour.at(0), tsp.m_tour.at(n));
			//std::cout << toursize << std::endl;
			#pragma omp parallel for reduction (+:toursize)
			for (size_t i = 0; i < n ; ++i)
			{
				size_t town1 = tsp.m_tour.at(i);
				size_t town2 = tsp.m_tour.at(i+1);
				toursize =  toursize + tsp.m_graph->get_distance (town1,town2) ;
			}
			toursize +=  tsp.m_graph->get_distance (tsp.m_tour.at(n-1),tsp.m_tour.at(n)) ;
			
			return (-toursize);	
		}

		static bool minimal_tour (float x)
		{
			return (x==0.0);
		}

		static float diversity (const TSP & t1, const TSP & t2)
		{	
			float dist = 0.0;
			size_t n (t1.m_tour.size());

			#pragma omp parallel for reduction (+:dist)
			for (size_t i = 0; i < n ; ++i)
			{
				dist = dist + ( t1.m_tour.at(i) == t2.m_tour.at(i) ? 0 : 1) ;
			} 
			
			dist = dist / (float) n;
			return (dist);
		}
		

		bool operator == (const TSP & tsp) const
		{
			return (tsp.m_tour==m_tour);
		}
			
};

std::ostream & operator << (std::ostream & os, const TSP & tsp);

#endif
