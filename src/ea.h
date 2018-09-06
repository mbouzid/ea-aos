#ifndef EA_H
#define EA_H

#include "oba.h"
#include <chrono>
#include <algorithm>
#include "types.h"


template <typename T>
void remove( typename std::vector<T>& vec, size_t pos)
{
    typename std::vector<T>::iterator it = vec.begin();
    std::advance(it, pos);
    vec.erase(it);
}

template <class T>
class EA : public OBA <T>
{
	typedef std::function < void (T &) > Operator;
	typedef std::function < std::vector <T> (const T &, const T &) > Recombinaison;

	private:

		
		std::vector < T > m_population;
		Recombinaison m_recombinaison;
		std::function < size_t (const T &) > m_fitness;
		double m_pm;
		double m_pc;
		size_t m_size;
		size_t m_best;
		REPLACEMENT m_replacement;
		SELECTION m_selection;
		std::vector <size_t> m_occ;
		std::vector <std::string> m_output;
		size_t m_nb_trials;

			
	public:
		
		EA
		(
			const std::vector < Operator > & omega,
			const std::vector <double> & theta,
			const std::function < std::vector <double> (AOS &, const std::vector <double> &, size_t) > & k,	
			const std::function < size_t (const AOS &, const std::vector<double> &, size_t) > & pi,
			const std::function < bool (size_t) > & terminated,
			const std::vector <std::string> & names,
			const Recombinaison & recombinaison,
			const std::function < size_t (const T &) > & fitness,
			double pm,
			double pc,
			size_t sz,
			REPLACEMENT replacement,
			SELECTION selection
		):
			OBA <T>(omega,theta,k,pi, terminated,names),
			m_recombinaison (recombinaison),
			m_fitness (fitness),
			m_pm (pm),
			m_pc (pc),
			m_size (sz),
			m_replacement (replacement),
			m_selection (selection),
			m_occ(),
			m_nb_trials (2)
		{
			for (size_t i = 0; i < m_size; ++i)	
			{
				m_population.push_back (T());
			}

			for (size_t i = 0; i < omega.size(); ++i)
			{
				m_occ.push_back (0);
			}
		}

		void set_nb_trials (size_t nb_trials)
		{
			m_nb_trials = nb_trials;
		}
		
		std::pair <size_t, size_t> selection () 
		{
			size_t father, mother;
			unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  			std::default_random_engine gen (seed);
			std::uniform_int_distribution <size_t> distrib (0,m_size-1);

			switch (m_selection)
			{
				case RANDOM:
				{
					
					
					
					father = distrib(gen);		
					mother = distrib(gen);

					while ( father == mother )
					{
						mother = distrib (gen);
					}
					
					
			
					break;
				}

				case TRIAL:
				{
				
					std::vector <size_t> subpop;
				
							
					while (subpop.size() != m_nb_trials)
					{
						size_t r (distrib(gen));
						auto found = std::find (subpop.begin(), subpop.end(), r);

						
						if ( found == subpop.end() )	
						{		
							subpop.push_back (r) ;
						}
						
						
					}

										

					std::vector <double> subpopfit (subpop.size());
					for (size_t i = 0; i < subpop.size(); ++i)
						subpopfit[i] = m_fitness(m_population.at(subpop.at(i)));

					std::vector<double>::iterator max1 = std::max_element (subpopfit.begin(), subpopfit.end());
					father = std::distance(subpopfit.begin(),max1);

					subpopfit.erase (max1);
					
					std::vector<double>::iterator max2 = std::max_element (subpopfit.begin(), subpopfit.end());
					mother = std::distance(subpopfit.begin(),max2);				

	
					break;
				}



				case BEST:
				{
					
					father = m_best;
					size_t imax = 0;
					for (size_t i = 1; i < m_size; ++i)
					{
						if ( i != m_best )
						{
							if ( m_fitness (m_population.at (i)) > m_fitness (m_population.at (imax)) )
							{
								imax = i;
							
							}
						}
					}
					
					mother = imax;
					
	
					break;
				}
				
				
			}
	
			return std::make_pair (father, mother);
		}		

	

		std::pair <size_t, size_t> replacement ()	
		{
			size_t worst1 , worst2 ;
			switch (m_replacement)
			{

				case FITNESS:
				{
					std::vector <size_t> pop;
					
					for (size_t i = 0; i < m_size; ++i)	
					{
						pop.push_back (i);
					}

					worst1 = *pop.begin();
					for (auto i = pop.begin(); i != pop.end(); ++i)
					{
						if ( m_fitness (m_population.at (*i)) < m_fitness (m_population.at (worst1) ))
						{
							worst1 = *i;
						}
					}

					pop.erase (std::find(pop.begin(), pop.end(), pop.at(worst1)));

					worst2 = *pop.begin();
					for (auto i = pop.begin(); i != pop.end(); ++i)
					{
						if ( m_fitness (m_population.at (*i)) < m_fitness (m_population.at (worst2) ))
						{
							worst2 = *i;
						}
					}
					break;	
				}
			
				case OLD:
				{

					worst1 = 0;
					worst2 = 1;

				
					break;
				}

		
			}
		
			return std::make_pair (worst1, worst2);
		}	
		
		
		void evaluate ()
		{
			size_t imax (0) ;
			for (size_t i = 1; i < m_size; ++i)
			{
				if ( m_fitness (m_population.at(i)) > m_fitness (m_population.at(imax)) )
				{
					imax = i;
				}
			}
			m_best = imax;
		}

		void record (double fitness)
		{
			std::ostringstream oss;
			oss << fitness;
			for (size_t i = 0; i < this->Omega.size(); ++i)
			{
				oss << "," << m_occ.at(i) ;
			}	
			oss << std::endl;
			m_output.push_back (oss.str());
		}
		
		

		void save (const char * filename)
		{
			std::ofstream f (filename);	
			if ( f )
			{
				f << "name";
				for (size_t i = 0; i < this->Omega.size(); ++i)
				{
					f << "," << "op" << i ;
				}
				f << std::endl;
				for (auto l : m_output)
					f << l;
			
				f.close();
			}
		}

		/*void run (const char * filename, size_t m, double alpha, double pmin, double beta)
		{
			std::vector <double> cumulated_rewards (this->Omega.size());
			for (size_t i = 0; i < this->Omega.size(); ++i)
				cumulated_rewards[i] = 0.0;
		
			size_t i (0);
			AOS a (this->Omega.size());
			a.set_alpha (alpha);
			a.set_pmin (pmin);
			a.set_beta (beta);
	
			evaluate();
			while ( ! this->Terminated(m_fitness(m_population.at(m_best))) )
			{	
				
				
				this->Theta = this->K (a,this->Theta,i);
				
				evaluate();
				double oldfit (m_fitness(m_population.at(m_best)));
				a.record (i,oldfit);
				record (oldfit);
	

				size_t n (this->Pi (a,this->Theta,i) );
				++m_occ[n];
				
				unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  				std::default_random_engine gen (seed);

  			
				std::bernoulli_distribution distrib_c(m_pc);	
				double d (0.0), oldf (0.0), newf (0.0);
				if (distrib_c(gen)) 
				{
					std::pair <size_t, size_t> selected (selection());
				
					std::pair <size_t, size_t> removed (replacement());

					std::vector <T> child (m_recombinaison (m_population.at(selected.first), m_population.at (selected.second) ));
					
					
					remove(m_population,removed.first);
					remove(m_population,removed.second);
					
					std::bernoulli_distribution distrib_m(m_pm);
					
					 
					for (typename std::vector<T>::iterator c = child.begin(); c != child.end(); ++c)
					{
						oldf += m_fitness(*c);
						if ( distrib_m (gen) )	
						{
							this->Omega.at(n)(*c);
						}
						newf += m_fitness(*c);
						m_population.push_back (*c);
					}
					
				}
				
				oldf = oldf/2;
				newf = newf/2;

				d = std::max (0.0,newf-oldf); 		// real reward
				evaluate();
				//std::cout << "d=" << d << std::endl;
			
				double newfit (m_fitness(m_population.at(m_best)));
				
				double delta (std::max(0.0,newfit-oldfit));		// expected reward
				cumulated_rewards[n] += delta;
				//std::cout << "delta=" << delta << std::endl;
				double regret (std::max (0.0, d-delta ));
				
				

				std::cout << "regret=" << regret << std::endl;
				
				a.update_utility(n,d);

				for (size_t i = 0; i < this->Omega.size(); ++i)
					std::cout << cumulated_rewards.at(i) << " ";
				std::cout << std::endl;
					
				//std::cout << "best fitness so far:" << m_fitness(m_population.at(m_best)) << std::endl;

				++i;
			}
			
			std::cout << i << std::endl;

			a.save ("utility.csv");
			save(filename);
		}*/

		void run (const char * filename, size_t m, size_t op)
		{
	
			size_t i (0);
		
			evaluate();
			while ( ! this->Terminated(m_fitness(m_population.at(m_best))) )
			{	
				
				evaluate();
				double oldfit (m_fitness(m_population.at(m_best)));
				
				record (oldfit);
	

				size_t n (op);
				++m_occ[n];
				
				unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  				std::default_random_engine gen (seed);

  			
				std::bernoulli_distribution distrib_c(m_pc);	
				
				if (distrib_c(gen)) 
				{
					std::pair <size_t, size_t> selected (selection());
				
					

					std::vector <T> child (m_recombinaison (m_population.at(selected.first), m_population.at (selected.second) ));
					
					
					
					std::bernoulli_distribution distrib_m(m_pm);
					
					 
					for (typename std::vector<T>::iterator c = child.begin(); c != child.end(); ++c)
					{
						if ( distrib_m (gen) )	
						{
							this->Omega.at(n)(*c);
						}
						m_population.push_back (*c);
					}

					std::pair <size_t, size_t> removed (replacement());
					remove(m_population,removed.first);
					remove(m_population,removed.second);
					
					
				}
				

				evaluate();
			
				double newfit (m_fitness(m_population.at(m_best)));
				
				std::ostringstream oss ;
				oss << i << newfit << std::endl;
				m_output.push_back(oss.str());
					
				//std::cout << "best fitness so far:" << m_fitness(m_population.at(m_best)) << std::endl;

				++i;
			}
			
			std::cout << i << std::endl;

			save(filename);
		}

		~EA()
		{}
};		

#endif
