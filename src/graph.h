#ifndef GRAPH_H
#define GRAPH_H

#include <map>
#include <cmath>
class Graph
{
	private:

		std::map <size_t, std::pair <double,double> > m_vertices;
		std::map < size_t , std::map < size_t, double > > m_distance;
		void compute_distance_matrix ();
	public:

		Graph (const std::map <size_t, std::pair <double, double> > & vertices):
			m_vertices (vertices)
		{
			compute_distance_matrix();
		}	

		static Graph * load (const char * filename) throw (std::exception &);

		const std::pair <double,double>  & get_coordinates (size_t i)  const throw (std::exception &);
		double get_distance (size_t i, size_t j) const throw (std::exception &);
		size_t get_size() const
		{
			return (m_vertices.size() );
		}
};

#endif
