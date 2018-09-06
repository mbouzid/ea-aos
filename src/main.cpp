#include <cstdlib>
#include "ea.h"
#include "bitfield.h"

#include "aos.h"
#include "dim.h"
#include "tsp.h"

int main (int argc, char * argv [])
{
	srand(time(NULL));
	try
	{
		
		char * tsp = nullptr;
		char * out = nullptr;
		size_t sz (10);
		double alpha (0.85);
		double beta (0.01);
		double pmin (0.10);
		double pc (1.0);
		double pm (1.0);
		size_t it (100);
		bool help (false);


		for (size_t i = 0; i < (size_t)argc; ++i)
		{
			if (std::string(argv[i]) == "--tsp")
			{
				tsp = argv[i+1];
			}
		
			if (std::string(argv[i]) == "--out")
			{
				out = argv[i+1];
			}

			if (std::string(argv[i]) == "--sz")
			{
				sz = std::stoll(argv[i+1]);
			}

			if (std::string(argv[i]) == "--alpha")
			{
				alpha = std::stoi(argv[i+1]);
			}

			if (std::string(argv[i]) == "--beta")
			{
				beta = std::stoi(argv[i+1]);
			}

			if (std::string(argv[i]) == "--pmin")
			{
				pmin = std::stoi(argv[i+1]);
			}

			if (std::string(argv[i]) == "--it")
			{
				it = std::stoll(argv[i+1]);
			}

			if (std::string(argv[i]) == "--pc")
			{
				pc = std::stoi(argv[i+1]);
			}
	
			if (std::string(argv[i]) == "--pm")
			{
				pm = std::stoi(argv[i+1]);
			}

			if (std::string(argv[i]) == "--help")
			{
				help = true;
			}
		}
		
		if ( tsp and out and !help)
		{

			Graph * g = Graph::load(tsp);
		
			std::vector < std::function < void ( TSP &) >> O = { TSP::swap, TSP::inversion, TSP::insertion, TSP::scramble }  ;

		
		

			std::vector < std::string > names = {"swap" , "inversion", "insertion", "scramble"};

			std::vector <double> proba = 	// probabilité des opérateurs de O
			{ 
				0.25,
				0.25,		
				0.25, 
				0.25		
			};	
	
				
			std::vector < TSP > instances ;
		
			for (size_t i = 0; i < sz; ++i)
			{
				instances.push_back (TSP(g));
			}	
	


			DynamicIslandModel < TSP , float > dim 
			(
				O,
				proba,
				AOS::migration_policy,				// politique de contrôle
				AOS::migration_roulette, 		// politique de sélection	// politique de sélection
				TSP::minimal_tour,		// fonction de terminaison
				names,
				instances,
				TSP::identity_crossover,
				TSP::minimize_tour, 		// fonction objective
				pm, 					// probabilité de mutation
				pc, 					// probabilité de recombinaison
				sz, 					// taille de la population
				FITNESS, 				// heuristique de remplacement
				BEST
			);

		

			dim.set_alpha (alpha);
			dim.set_pmin (pmin);
			dim.set_beta (beta);

			dim.run (it,out);
	
	
			dim.save ("fitness_fct_size.csv");

		}
		else
		{
			std::cerr << "Utilisation: " << argv[0] << "--tsp <instance tsp> " << " --out <fichier output tsp> " << std::endl;
			std::cout << "Autres options:" << std::endl;
			std::cerr << "--sz <taille population> [defaut: 10] " << std::endl;
			std::cerr << "--it <nombre itérations> [defaut: 100]" << std::endl ;
			std::cerr << "--pmin <proba min> [defaut: 0.10]" << std::endl;
			std::cerr << "--pc <proba croisement>  [defaut: 1.0]" << std::endl;
			std::cerr << "--pm <proba mutation> [defaut: 1.0]" << std::endl;
			std::cerr << "--alpha <facteur apprentissage>  [defaut: 0.85] " << std::endl;
			std::cerr << "--beta <bruit> [defaut: 0.01] " << std::endl; 
			std::cerr << "--help <affiche l'aide>" << std::endl;
			
			exit(EXIT_FAILURE);
		}
	}
	catch (const std::exception & e)
	{
		std::cerr << e.what() << std::endl;
	}

	return (EXIT_SUCCESS);
};
