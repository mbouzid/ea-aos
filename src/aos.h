#ifndef aos_h
#define aos_h

#include <set>
#include <ctime>
#include <random>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "tools.h"

class AOS
{
	private:

		size_t m_n;				// nombre d'opérateurs
		std::vector <double> Gain;
		std::vector <double> u;			// vecteur d'utilité
		
		std::vector <size_t> Size;

		double Alpha;				// taux d'apprentissage
		
		double Pmin;
		double Beta;
		double Tau;	
		double Epsilon;

		std::vector <size_t> occ;
		

		std::vector <std::string> output;

		std::vector < std::vector <double> > Matrix;

		size_t current_island;
	public:
		
		AOS (size_t n):	
			m_n (n),
			Gain (),
			u(),
			Size(),
			Alpha (0.80),
			Pmin (1/(double)n),
			Beta (0.01),
			Tau (1.0),
			Epsilon (0.1),
			Matrix ()
		{
			
			
			for (size_t i = 0; i < m_n; ++i)
			{
				u.push_back (0.0);
				Size.push_back (0);
				Gain.push_back (0.0);
				occ.push_back (0);
				
				Matrix.push_back (std::vector <double>(m_n));
				for (size_t j = 0; j < m_n; ++j)
				{
					/*if ( i == j )
						Matrix[i][j] = 1.0;
					else
						Matrix[i][j] = 0.0;*/
					Matrix[i][j] = 0.25;
				}
			}
		}


		void set_alpha (double alpha)
		{
			Alpha = alpha;
		}

		void set_beta (double beta)
		{

			Beta = beta;
		}

		void set_pmin (double pmin)
		{
			Pmin = pmin;
		}

		void set_tau (double tau)
		{
			Tau = tau;
		}
		
		void set_epsilon (double epsilon)
		{
			Epsilon = epsilon;
		}

		void set_current_island (size_t island)
		{
			current_island = island;
		}

		void display_utility () const
		{
			std::cout << "utility:" << std::endl;
			for (auto i : u)
			{
				std::cout << i << " ";
			}
			std::cout << std::endl;
		}	

		
		void display_occ () const
		{
			std::cout << "occ:" << std::endl;
			for (auto i : occ)
			{
				std::cout << i << " ";
			}
			std::cout << std::endl;
		}

		void display_gain () const
		{
			std::cout << "gain:" << std::endl;
			for (auto i : Gain)
			{
				std::cout << i << " ";
			}
			std::cout << std::endl;
		}

		void update_size (size_t i, size_t sz)
		{
			Size[i] = sz;
		}

		// utilité == impact d'un opérateur (gain)
		void update_utility (size_t i, double gain)
		{
			Gain[i] =  gain;
			
			for (size_t j = 0; j < m_n; ++j)
			{
				if ( i != j)
				{
					Gain[j] = 0.0;
				}
			}

			
			u[i] = ((1-Alpha)*u.at(i))+ Alpha*Gain.at(i);
			

			++occ[i] ;
		}


		double get_utility (size_t i) const
		{
			return (u.at(i));
		}

		void record (size_t t, double fitness)
		{
			std::ostringstream oss;
			oss << fitness  ;
			for (size_t i = 0; i < m_n; ++i)
			{
				oss << "," << u.at(i) ;
			}
			oss << std::endl;
			output.push_back (oss.str());
		}

		void save (const char * filename)
		{
			std::ofstream f (filename);
			if ( f )
			{
				f << "operators";
				for (size_t i = 0; i < m_n; ++i)
				{
					f << ", op" << i << " ";
				}
				f << std::endl;
				for (auto l : output)
					f << l;
				f.close();
			}
			
		}

		

		// fixed roulette wheel : ne change pas les probabilités d'utilisation des opérateurs à l'instant t	
		static std::vector <double> fixed_roulette_wheel (AOS & aos, const std::vector <double> & theta, size_t t) 
		{
			return (theta);
		}

		static std::vector <double> adaptive_roulette_wheel (AOS & aos,const std::vector <double> & theta, size_t t) 
		{

			if ( t == 0)
			{
				return fixed_roulette_wheel (aos,theta,t);
			}
			else
			{
				std::vector <double> Theta (theta);
				double sum = std::accumulate (aos.u.begin(), aos.u.end(), 0.0);
			
				for (size_t i = 0; i < aos.m_n; ++i)
				{
					Theta[i] = aos.Pmin + (1- (double)aos.m_n *aos.Pmin)*(aos.u.at(i)/sum);
				}
				
				return (Theta);
				
			}
		}
		
		
		// adaptive pursuit
		static std::vector <double> adaptive_pursuit (AOS & aos, const std::vector <double> & theta, size_t t) 
		{
			std::vector <double> Theta (theta);


			std::vector<double>::iterator umax = std::max_element(aos.u.begin(), aos.u.end());
			size_t posmax = std::distance(aos.u.begin(), umax);	
			
			double pmax (1-((double)(aos.m_n -1)*aos.Pmin));
			
			Theta[posmax] = Theta.at(posmax) + aos.Beta*(pmax - Theta.at(posmax));

			for (size_t i = 0; i < aos.m_n; ++i)
			{
				if ( i != posmax )
					Theta[i] = Theta.at(i) + aos.Beta*(aos.Pmin - Theta.at(i) );
			}
	
			return (Theta);
		}
	
		void AP_DIM (size_t island) 
		{	
			
			std::vector<double>::iterator umax = std::max_element(u.begin(),u.end());
			size_t posmax = std::distance(u.begin(), umax);	
			
			double pmax (1-((double)(m_n -1)*Pmin));
			
			Matrix[island][posmax] = Matrix.at(island).at(posmax) + Beta*(pmax - Matrix.at(island).at(posmax));

			for (size_t i = 0; i < m_n; ++i)
			{
				if ( i != posmax )
					Matrix[island][i] = Matrix.at(island).at(i) + Beta*(Pmin - Matrix.at(island).at(i) );
			}
	
		}


		static std::vector <double> UCB1 (AOS & aos, const std::vector <double> & theta, size_t t) 
		{
			
			if ( t == 0)
			{
				return fixed_roulette_wheel (aos,theta,t);
			}
			else
			{
				
				
				
				auto occ0 = std::find (aos.occ.begin(), aos.occ.end(), 0);
			
				if ( occ0 != aos.occ.end() )
				{
					return (theta);
				}
				else
				{
					std::vector <double> Theta (theta);
					size_t sum_occ = std::accumulate (aos.occ.begin(), aos.occ.end(), 0);
					std::vector <double> UCB1(aos.m_n);
					
					double C = 2.0;

					for (size_t i = 0; i < aos.m_n; ++i)
					{
						double log_i = C*std::log2((double)sum_occ);
					
						double delta_i =  std::sqrt (log_i/(double)aos.occ.at(i));
						
						
						UCB1[i] = aos.u.at(i) + delta_i;
						
					}
					
					
					return (UCB1);
				}
				
				
			}
		}

		void transition (size_t island)
		{
			std::random_device rd;
    			std::mt19937 gen(rd());
			std::uniform_real_distribution<double> distrib(0.0,1000.0);

			double BetaT = 0.01;
			double alpha = 0.95;

			std::vector <double> Noisy (m_n);

			for (size_t i = 0; i < m_n; ++i)
			{
				Noisy[i] = distrib(gen);
			}
			normalize<double>(Noisy);
	
			for (size_t i = 0; i < m_n; ++i)
			{
				Matrix[island][i] = (1-BetaT)*( (alpha*Matrix.at(island).at(i)) + ((1-alpha)*Gain.at(i) ) ) + BetaT*Noisy.at(i) ;
			}

			normalize<double>(Matrix[island]);
		}

		static std::vector <double> migration_policy (AOS & aos, const std::vector <double> & theta, size_t t) 
		{	
			
			for (size_t i = 0; i < aos.m_n; ++i)
			{
				aos.transition(i);
					
				/*for (size_t j = 0; j < aos.m_n; ++j)
				{
					std::cout << aos.Matrix.at(i).at(j) << " ";
				}
				std::cout << std::endl;*/
			}
			//std::cout << std::endl;
			
			return (theta);
		}

		static size_t roulette_selection_wheel (const AOS & aos,const std::vector <double> & theta, size_t t)
		{

		 	std::random_device rd;
    			std::mt19937 gen(rd());
			std::discrete_distribution<> d (theta.begin(), theta.end());
			
			return (d(gen));
		}



		static size_t migration_roulette(const AOS & aos, const std::vector <double> & theta, size_t t)
		{

			return AOS::roulette_selection_wheel(aos,aos.Matrix.at(aos.current_island),t); ;
		}

		static size_t epsilon_greedy (const AOS & aos, const std::vector <double> & theta, size_t t)
		{	

			std::random_device rd;
    			std::mt19937 gen(rd());

			size_t K (theta.size());	

			for (size_t i = 0; i < K ; ++i)
			{
				if ( t == i )
					return i;
			}

			std::vector<double> Theta (theta);
		


			std::vector<double>::iterator max = std::max_element(Theta.begin(),Theta.end());
			size_t argmax = std::distance(Theta.begin(), max);
			
			
			Theta[argmax] = (1-aos.Epsilon)+(aos.Epsilon/(double)K);
			for (size_t j = 0; j < Theta.size(); ++j)
			{
				if ( j != argmax )
					Theta[j] = aos.Epsilon+(aos.Epsilon/(double)K);
			}

			
			std::discrete_distribution<> d (Theta.begin(), Theta.end());
			return (d(gen));
			
		}


		static size_t softmax (const AOS & aos, const std::vector <double> & theta, size_t t)
		{
			std::random_device rd;
    			std::mt19937 gen(rd());

			std::vector<double> Theta (theta);
			
			double sum (0.0);
			for (size_t j = 0; j < Theta.size(); ++j)
			{
				sum += std::exp (Theta.at(j)/aos.Tau);
			}

			for (size_t j = 0; j < Theta.size(); ++j)
			{
				Theta[j] = std::exp(Theta[j]/aos.Tau)/sum;
			}
		
			
			
			std::discrete_distribution<> d (Theta.begin(), Theta.end());
			return (d(gen));
		}

		static size_t UCB (const AOS & aos, const std::vector<double> & theta, size_t t)
		{
			std::vector<double> Theta (theta);

			for (size_t i = 0; i < theta.size(); ++i)
			{
				if ( t == i )
					return i;
			}
			
			
			std::vector<double>::iterator max = std::max_element(Theta.begin(),Theta.end());
			size_t argmax = std::distance(Theta.begin(), max);
			
			return (argmax);
		}

		
		

		

};

#endif
