#ifndef ea_h
#define ea_h


#include <functional>
#include <vector>
#include <cmath>
#include <map>
#include "aos.h"

#include <iostream>
template <class T>
class OBA
{
	typedef std::function < void (T &) > Operator;
	

	protected:
		
		/** 
		 *	Ω = {o_1, ..., o_n} 
		 *	a set of operators 
		 **/
		std::vector < Operator > Omega;	
		
		/**
		 *	θ = (σ_1,...,σ_n)
		 * 	parameter vector associates a probability of selecting 
		 **/
		std::vector <double> Theta;
		
		/** 
		 *	K : θ x ℕ -> θ 
		 *	the control function 
		 **/
		std::function < std::vector <double> (AOS &,const std::vector <double> &, size_t) > K;
		
		/**
		 *	π : θ x ℕ ->  ℕ
		 *	OS policy (e.g. selection wheel...)
		 **/
		std::function < size_t (const AOS &, const std::vector <double> &, size_t) > Pi;

		std::function < bool (size_t) > Terminated;

		std::vector <std::string> Names;


	public:

		OBA
		(
			const std::vector < Operator > & omega,
			const std::vector <double> & theta,
			const std::function < std::vector <double> (AOS &, const std::vector <double> &, size_t) > & k,	
			const std::function < size_t (const AOS &, const std::vector<double> &, size_t) > & pi,
			const std::function < bool (size_t) > & terminated,
			const std::vector < std::string > & names
		):
			Omega (omega),
			Theta (theta),
			K (k),
			Pi (pi),
			Terminated (terminated),
			Names (names)
		{}


		virtual const std::vector<std::string> & get_names () const
		{
			return (Names);
		}

		virtual ~OBA()
		{}
	
};

#endif
