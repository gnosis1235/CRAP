#ifndef CH_DIATOMIC_ALREADY_INCLUDED
	#define CH_DIATOMIC_ALREADY_INCLUDED

	#include "CH_Basics.h"
	#include "CH_Ion.h"
	
	struct diatomic_struct{
		bool valid;
		int channel;

		ion_class * ion1;
		ion_class * ion2;

		particle_phys * rel;
		particle_phys * cm;
	};	

	class diatomic_class : diatomic_struct{  // find out how to inherit from diatomic struct!

	private:

		double reduced_mass;
		ion_class * tmp_ion;

	public:

		diatomic_class();
		~diatomic_class();
		
		void set_ions(ion_class *ion1, ion_class *ion2);

		void set_channel(int channel);
		void set_channel(int channel, bool valid);
		void set_valid(bool valid);

		void shift_stretch_rel(cor_param * fac);

		void sort_ions_and_process(spectrometer_class *spect);
		void process(spectrometer_class *spect);
		void process_diatomic_only();
	
		bool check_mom_cm_x(double psumx_width, int channel, bool invalidate);
		bool check_mom_cm_y(double psumy_width, int channel, bool invalidate);
		bool check_mom_cm_z(double psumz_width, int channel, bool invalidate);
		bool check_mom_cm(double psumx_width, double psumy_width, double psumz_width, int channel, bool invalidate);

		bool check_KER(double energy_from, double energy_to, int channel, bool invalidate);

	}; // end diatomic class

#endif;


