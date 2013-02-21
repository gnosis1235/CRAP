#ifndef CH_PARTICLE_ALREADY_INCLUDED
	#define CH_PARTICLE_ALREADY_INCLUDED

	#include "CH_Basics.h"
	#include "CH_Spectrometer.h"

	struct particle_struct{
		bool valid;
		int channel;
		
		particle_raw * raw;
		particle_phys * phy;
	};
	
	class particle_class { 

	public:
		bool valid;
		int channel;
		
		particle_raw * raw;
		particle_phys * phy;

		cor_param cor;

	public:

		particle_class();
		~particle_class();
		
		void set_raw(particle_raw *rawdata);
		void set_raw(double x, double y, double t, int method = 0);
	    void calc_phi_pos();

		void set_channel(int channel);
		void set_channel(int channel, bool valid);
		void set_valid(bool valid);

		void shift_stretch_raw(cor_param * fac);
		void rotate_raw(double ang);

		void process(spectrometer_class *spect);
	
		bool check_tof(double tof_from, double tof_to, int channel, bool invalidate);
		bool check_mom(double px_width, double py_width, double pz_width, int channel, bool invalidate);
		bool check_energy(double energy_from, double energy_to, int channel, bool invalidate);

		friend class electron_class;
		friend class ion_class;

	}; // end particle class

#endif;