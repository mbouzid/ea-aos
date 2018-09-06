#ifndef bitfield_h
#define bitfield_h

#include <bitset>
#include <ctime>
#include <random>
#include <chrono>

template <size_t N>
class Bitfield : public std::bitset <N>
{

	public:
	
		Bitfield():	
			std::bitset<N>()
		{}


		template <size_t K>
			static void flipping (Bitfield & b)
		{
			srand(time(NULL));
	   		std::random_device rd ;
	    		std::mt19937 gen(rd());
	    		std::uniform_int_distribution<> dis(0, N-1);

			std::set <size_t> pos;
			while ( pos.size() != K)
			{
				pos.insert (dis(gen));
			}

			for (size_t i : pos)
			{
				b.flip (i);
			}
		}

		static std::vector <Bitfield> monopoint_crossover (const Bitfield & b1, const Bitfield & b2)
		{
			std::vector <Bitfield> child;

			Bitfield <N>children1 , children2;

			unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  			std::default_random_engine gen (seed);

			std::uniform_int_distribution <size_t> distrib (0,N-1);	
			
			
			
			size_t monopoint (distrib(gen));
			
			for (size_t i = 0; i < monopoint; ++i)
			{
				children1[i] = b1[i];
				children2[i] = b2[i];
			}

			
			for (size_t i = monopoint; i < N; ++i)
			{
				
				children1[i] = b2[i];
				children2[i] = b1[i];
			}	
	
			child.push_back (children1);
			child.push_back (children2);

			return (child);
		}

		template <size_t K>
		static std::vector <Bitfield> point_crossover (const Bitfield & b1, const Bitfield & b2)
		{
			std::vector <Bitfield> child;

			Bitfield <N>children1 , children2;

			unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  			std::default_random_engine gen (seed);
			std::uniform_int_distribution <size_t> distrib (0,N-1);	


			std::set <size_t> points;

			while (points.size() != K)
			{
				size_t point = distrib(gen);
				points.insert(point);
			}
			points.insert(N);
			size_t first = 0;
			for (size_t point : points)
			{
				for (size_t i = first ; i < point; ++i)
				{
					children1[i] = b1[i];
					children2[i] = b2[i];
				}
				first = point+1;
			}
	
			child.push_back (children1);
			child.push_back (children2);

			return (child);
		}


		static std::vector <Bitfield>  uniform_crossover (const Bitfield & b1, const Bitfield & b2)
		{	
			std::vector <Bitfield <N>> child;
			unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  			std::default_random_engine gen (seed);

			double prob (0.5);

  			std::bernoulli_distribution distrib (prob);
			Bitfield <N> children1, children2;

			
			for (size_t i = 0; i < N; ++i)
			{
				if (distrib (gen))
				{
					children1[i] = b1[i];
					children2[i] = b2[i];
				}
				else
				{
					children1[i] = b2[i];
					children2[i] = b1[i];
				}
			}

	
			child.push_back (children1);
			child.push_back (children2);
			return (child);	
		}



		static void bitflip (Bitfield & b)
		{
			unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  			std::default_random_engine gen (seed);

			double prob (1/(double)N);

  			std::bernoulli_distribution distrib (prob);

			for (size_t i = 0; i < N; ++i)	
			{
				if (distrib (gen)) 
				{
					b.flip(i);
				}
			}	
		}

		static size_t maximize_bits (const Bitfield & b) 
		{
			return (b.count());
		}

		
		static bool max_bits (size_t f)
		{
			return (f==N);
		}
};

#endif
