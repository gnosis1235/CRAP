#ifndef CH_ION_ALREADY_INCLUDED
	#define CH_ION_ALREADY_INCLUDED

	#include "CH_Particle.h"
	#include "CH_Spectrometer.h"
	#include "CH_Basics.h"
	
	class ion_class : public particle_class { 
		
	public:

		ion_class();
		~ion_class();
	
	friend class diatomic_class;
	}; // end ion class

#endif;