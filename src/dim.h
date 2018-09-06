#ifndef DIM_h
#define DIM_h

#include "oba.h"
#include <iterator>
#include <algorithm>

template <class T, class F>
class DynamicIslandModel : public OBA <T>
{
	typedef std::function < void (T &) > Operator;
	typedef std::function < std::vector<T> (const T &, const T &) > Recombinaison;
	private:

		std::vector < std::vector <T> > m_islands;	// population
		Recombinaison m_recombinaison;
		std::function < F (const T &) > m_fitness;
		double m_pm;
		double m_pc;
		size_t m_size;
		std::pair <size_t,size_t>  m_best;
		REPLACEMENT m_replacement;
		SELECTION m_selection;
		AOS m_aos;

		std::vector <std::string> m_output;
		std::vector <std::string> m_regret;

	public:
	
		DynamicIslandModel
		(
			const std::vector < Operator > & omega,
			const std::vector <double> & theta,
			const std::function < std::vector <double> (AOS &, const std::vector <double> &, size_t) > & k,	
			const std::function < size_t (const AOS &, const std::vector<double> &, size_t) > & pi,
			const std::function < bool (size_t) > & terminated,
			const std::vector <std::string> & names,
			const Recombinaison & recombinaison,
			const std::function < F (const T &) > fitness,
			double pm,
			double pc,
			size_t sz,
			REPLACEMENT replacement,
			SELECTION selection
		):
			OBA <T>(omega, theta, k, pi, terminated,names),
			m_islands (),
			m_recombinaison (recombinaison),
			m_fitness (fitness),
			m_pm (pm),
			m_pc (pc),
			m_size (sz),
			m_replacement (replacement),
			m_selection (selection),
			m_aos (omega.size()),
			m_output()

		{

			size_t nbIslands (this->Omega.size());
			for (size_t i = 0; i < nbIslands; ++i)
			{
				m_islands.push_back (std::vector <T>());
				for (size_t j = 0; j < (m_size/nbIslands); ++j)
				{
					m_islands[i].push_back (T());
				}
			}
			//std::cout << "initialization: [OK]" << std::endl;
		}

		
		DynamicIslandModel
		(
			const std::vector < Operator > & omega,
			const std::vector <double> & theta,
			const std::function < std::vector <double> (AOS &, const std::vector <double> &, size_t) > & k,	
			const std::function < size_t (const AOS &, const std::vector<double> &, size_t) > & pi,
			const std::function < bool (size_t) > & terminated,
			const std::vector <std::string> & names,
			const std::vector <T> & population,
			const Recombinaison & recombinaison,
			const std::function < F (const T &) > fitness,
			double pm,
			double pc,
			size_t sz,
			REPLACEMENT replacement,
			SELECTION selection
		):
			OBA <T>(omega, theta, k, pi, terminated,names),
			m_islands (),
			m_recombinaison (recombinaison),
			m_fitness (fitness),
			m_pm (pm),
			m_pc (pc),
			m_size (sz),
			m_replacement (replacement),
			m_selection (selection),
			m_aos (omega.size()),
			m_output()

		{

			
			size_t nbIslands (this->Omega.size());
			for (size_t i = 0; i < nbIslands; ++i)
			{
				m_islands.push_back (std::vector <T>());
			}
			
		
			for (size_t i = 0; i < m_size; ++i)
			{
				
				if ( ( i % nbIslands ) == 0 )
					m_islands[i%nbIslands].push_back (population.at(i));
				
			}
			 

			std::cout << "initialization: [OK]" << std::endl;
		}

	

		// replacing in an island
		std::pair < typename std::vector<T>::iterator, typename std::vector<T>::iterator> replacement (size_t island)	
		{
			typename std::vector<T>::iterator worst1, worst2 ;
			switch (m_replacement)
			{

				case FITNESS:
				{

					worst1 = m_islands[island].begin();
					for (typename std::vector<T>::iterator i = m_islands[island].begin(); i != m_islands[island].end(); ++i)
					{
						if ( m_fitness (*i) < m_fitness (*worst1))
						{
							worst1 = i;
						}
					}
					
					worst2 = m_islands[island].begin();
					for (typename std::vector<T>::iterator i = m_islands[island].begin(); i != m_islands[island].end(); ++i)
					{
						if ( i != worst1 )
						{
							if ( m_fitness (*i) < m_fitness (*worst2))
							{
								worst2 = i;
							}
						}
					}
					break;	
				}
			
				case OLD:
				{

					worst1 = m_islands[island].begin();
					worst2 = std::next (m_islands[island].begin());
				
					break;
				}

		
			}
		
			return std::make_pair (worst1, worst2);
		}	
	
		std::pair < typename std::vector<T>::iterator, typename std::vector<T>::iterator > selection(size_t island)
		{
			typename std::vector<T>::iterator first = m_islands[island].begin();
			typename std::vector<T>::iterator father (first), mother (first);

			unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  			std::default_random_engine gen (seed);
			std::uniform_int_distribution <size_t> distrib (0,m_islands.at(island).size()-1);

			

			switch (m_selection)
			{
				case RANDOM:
				{
					
					std::advance (father, distrib(gen));		
					std::advance (mother, distrib(gen));

					while ( father == mother )
					{
						std::advance (mother, distrib(gen));
					}
					
					
			
					break;
				}

				case BEST:
				{
					size_t best (evaluate_island (island));
					std::advance (father,best);
					size_t imax = 0;

					
					for (size_t i = 1; i < m_islands.at(island).size(); ++i)
					{
						if ( i != best )
						{
							if ( m_fitness (m_islands.at(island).at (i)) > m_fitness (m_islands.at(island).at (imax)) )
							{
								imax = i;
							}
						}
					}
					
					std::advance(mother,imax);
					
					
					break;
				}
				
				case TRIAL:		
				{
					break;
				}
				
				
			}
	
			return std::make_pair (father, mother);
		}		

	
		std::pair <F,F> reproduce (size_t island) 
		{
				

			if ( m_islands.at(island).size() >= 2  )
			{
				unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  				std::default_random_engine gen (seed);

  			
				std::bernoulli_distribution distrib_c(m_pc);

				if (distrib_c(gen)) 
				{
					size_t oldpos (evaluate_island(island));
					F oldfit (m_fitness(m_islands.at(island).at(oldpos)));


					std::pair < typename std::vector<T>::iterator, typename std::vector<T>::iterator > selected (selection(island));

					std::vector <T> child (m_recombinaison (*selected.first, *selected.second));
					
					
					std::bernoulli_distribution distrib_m(m_pm);

					size_t nbimp(0);
					std::vector <double> improvement (child.size());
					
					for (typename std::vector<T>::iterator c = child.begin(); c != child.end(); ++c)
					{
						
						F old1 (m_fitness(*c));
						if ( distrib_m (gen) )	
						{
							
							this->Omega.at(island)(*c);
							
						}
						F new1 (m_fitness(*c));
							
						improvement[nbimp] = std::max (0.0, (double)new1-old1);
					
						++nbimp;
						m_islands[island].push_back (*c);
					}

					std::vector<double>::const_iterator max_imp = std::max_element (improvement.cbegin(), improvement.cend());
					
					std::pair < typename std::vector<T>::iterator, typename std::vector<T>::iterator > removed (replacement(island));
				
					m_islands[island].erase (removed.first);
					m_islands[island].erase (removed.second);
					
					

					size_t newpos (evaluate_island(island));
					F newfit(m_fitness(m_islands.at(island).at(newpos)));
					

					F expected_reward = std::max (0.0,(double)newfit-oldfit);
					
		
					return (std::make_pair (*max_imp, expected_reward));
			
				}
				else
				{
					return (std::make_pair (0.0,0.0));
				}
			}
			else
			{
				return (std::make_pair (0.0,0.0));
			}
			
		}

		

		float get_current_fitness () const
		{
			F sum = 0.0;
			size_t nbIslands = 0;
			
			for (size_t i = 0; i < m_islands.size(); ++i)
			{
				if ( ! m_islands.at(i).empty() )
				{
					sum += m_fitness(m_islands.at(i).at(evaluate_island(i)) );
					++nbIslands;
				}
			}
			return (sum/nbIslands);
		}


		// return pos best individual in island
		size_t evaluate_island (size_t island) const
		{
			if ( !m_islands.at(island).empty() )
			{
				size_t best (0);
			
				for (size_t i = 0; i < m_islands.at(island).size(); ++i)
				{
				
					if ( m_fitness(m_islands.at(island).at(i)) > m_fitness (m_islands.at(island).at(best)) )
					{
						best = i;
					}
				}
				return (best);
			}
			else
				return 0;
			
		}
		
		F evaluate_average (size_t island) const
		{
			if ( !m_islands.at(island).empty() )
			{
				F sum (0.0);
				for (size_t i = 0; i < m_islands.at(island).size(); ++i)
				{
					sum += m_fitness(m_islands.at(island).at(i));
				}

				F avg = sum / (F) m_islands.at(island).size();
				
				return (avg);
			}
			else
				return (0.0);
		}
		
		typename std::vector < std::pair < std::pair <size_t,size_t> , T> > migration_pool (const AOS & aos, size_t island, size_t t)
		{
			//std::cout << "size of the island# " << this->get_names().at(island) << " =" << m_islands.at(island).size() << std::endl;
			std::vector < std::pair < std::pair <size_t,size_t> , T> > pool;

			typename std::vector <T>::iterator first = m_islands[island].begin();
			typename std::vector <T>::iterator last = m_islands[island].end();

		
			m_aos.set_current_island(island);
			for (typename std::vector<T>::iterator i = first; i != last; ++i)
			{
				
				size_t newIsland = this->Pi (m_aos,this->Theta,t);

				if ( newIsland != island )
				{
					pool.push_back( std::make_pair ( std::make_pair (island,newIsland) , *i ) );
				}
			}	
			
			return (pool);
			
		}

		void migrate (const AOS & aos, size_t t)
		{
			std::vector < std::vector < std::pair < std::pair <size_t,size_t> , T> > > pools;
			
			for (size_t j = 0; j < this->Omega.size(); ++j)
			{
				pools.push_back (migration_pool(aos,j,t));
			}

			for (auto p : pools)
			{
				for (auto i : p)
				{
					m_islands[i.first.second].push_back (i.second);
					typename std::vector<T>::iterator found = std::find(m_islands[i.first.first].begin(),m_islands[i.first.first].end(), i.second); 
					if (found != m_islands[i.first.first].end() )
					{
						m_islands[i.first.first].erase (found);
					}
				
				}
			}
		}

		
		void save (const char * filename)
		{
			std::ofstream f (filename);
			f << "name" ;
			for (size_t i = 0; i < this->Omega.size(); ++i)
			{
				f << "," << this->get_names().at(i) ;
			}
			f << std::endl;

			for (auto o : m_output)
				f << o ;
			f.close();
		}

		void save_regret (const char * filename)
		{
			
			std::ofstream f (filename);
			f << "regret" << "," << std::endl ;
			/*for (size_t i = 0; i < this->Omega.size(); ++i)
			{
				f << "," << this->get_names().at(i) ;
			}
			f << std::endl;*/

			for (auto o : m_regret)
				f << o ;
			f.close();
		}

		void set_alpha (double alpha)	
		{
			m_aos.set_alpha (alpha);
		}

		void set_beta (double beta)
		{
			m_aos.set_beta(beta);	
		}

		void set_pmin (double pmin)
		{
			m_aos.set_pmin (pmin);
		}

		void set_tau (double tau)
		{
			m_aos.set_tau(tau);
		}
		
		void set_epsilon (double epsilon)
		{
			m_aos.set_epsilon(epsilon);
		}

		void run (size_t m, const char * out)
		{
			
			
			size_t i (0);


			double cumulative_regret = 0.0;
			while ( !terminated() and (m!=i)  )
			{
				for (size_t j = 0 ; j <  m_islands.size(); ++j)
					m_aos.update_size (j,m_islands.at(j).size());	
				
				

				std::vector <double> reward (this->Omega.size());
				std::vector <double> expected_reward (this->Omega.size());
				std::vector <double> gain (this->Omega.size());				
				
			

				std::ostringstream oss1;
				oss1 << i  << "," ;


				for (size_t j = 0; j < this->Omega.size(); ++j)
				{	
					
					std::pair <double,double> result (reproduce(j));
					reward[j] = result.first;	
					expected_reward[j] = result.second;
					
					gain[j] = 0.0;

					if ( ! m_islands.at(j).empty () )
					{
						gain[j] = m_fitness(m_islands.at(j).at(evaluate_island(j)));
					}
				}

				size_t best = get_best_island();

				//normalize<double> (expected_reward);
				/*std::vector<double>::iterator max_exp = std::max_element (expected_reward.begin(), expected_reward.end());
				size_t argmax_exp = std::distance (expected_reward.begin(), max_exp);*/

				double cumulated_regret (0.0);
				for (size_t j = 0; j < this->Omega.size(); ++j)
					cumulated_regret += std::max (0.0,(double)reward.at(j)-expected_reward.at(j));

				cumulative_regret += cumulated_regret;			

				//oss1  << cumulated_regret <<  std::endl;
				oss1 << cumulative_regret << std::endl;
				m_regret.push_back(oss1.str());
 
				
				

				this->Theta = this->K (m_aos,this->Theta,i);

				normalize <double> (reward);
				m_aos.update_utility(best,reward.at(best));
				
				
				
				migrate (m_aos,i);
				
				std::ostringstream oss;
				oss << get_current_fitness() ;
				for (size_t i = 0; i < m_islands.size(); ++i)
				{
					oss << "," << m_islands.at(i).size()  ;
				}
				oss << std::endl;
				
				m_output.push_back (oss.str());

				++i;
				
			}

			std::ofstream f (out);
			if (f)
			{
				f << best_individual() ;
				f.close();
			}
			std::cout << "best fitness: " << m_fitness(best_individual()) << std::endl;
			
		}

		T best_individual () const
		{
			size_t best_island = get_best_island();
			T best_indiv = m_islands.at(best_island).at(0);
			for (auto ind : m_islands.at(best_island))
			{
				if ( m_fitness(ind) > m_fitness(best_indiv) )
					best_indiv = ind;
			}

			return best_indiv;
		}

		size_t get_best_island () const
		{
			size_t notempty (0);
			for (size_t j = 0; j < this->Omega.size(); ++j)
			{
				if ( ! m_islands.at(j).empty() )
				{
					notempty = j;
					break;
				}
			}

			size_t best (notempty);
			for (size_t j = 0; j < this->Omega.size(); ++j)	
			{
				if ( ! m_islands.at(j).empty()  )
				{ 
					F fit (m_fitness(m_islands.at(j).at(evaluate_island(j)) ));
					F bestfit (m_fitness(m_islands.at(best).at(evaluate_island(best)) ));

					if ( fit > bestfit )
						best = j;
				}

			}

			return (best);
		}
		
		
		bool terminated () const
		{
			for (size_t i = 0; i < this->Omega.size(); ++i)
			{
				if ( ! m_islands.at(i).empty() )
				{
					if ( this->Terminated(m_fitness(m_islands.at(i).at(evaluate_island(i)))) )
					{
						return true;
					}
				}
			}

			return false;
		}

		
				
		~DynamicIslandModel()
		{}

};

#endif
