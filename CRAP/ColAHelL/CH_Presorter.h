#ifndef CH_PRESORTER_ALREADY_INCLUDED
	#define CH_PRESORTER_ALREADY_INCLUDED

	#define PRESORTERVERSION (0.1)

	#include "OS_Version.h"

	#include "CH_Event.h"
	#include "CH_Basics.h"
	#include "CH_Spectrometer.h"
	#include "CH_Tof.h"

	#include <math.h>

	#ifdef CRAP
		#include "Simple_Hist.h"
	#else
			#include "Histo.h"
	#endif;

	

	#define ANY				0
	#define ETOF			1
	#define RTOF			2
	#define ERTOF			3
	#define PIPICO			4
	#define PRTOF			5
	#define PROJ_PIPICO		6

	class presorter_class
	{	
		private:

			int Type;
			int Flag;
			
			#ifdef CRAP
				histo_handler * H;
			#else
				Histo * H;
			#endif

			spectrometer_class * spect;
			tof_calc_class * ctof;

			CH_event_struct * evti;

			char dir[80];

			//single tof variables
			double etof_center;
			double etof_width;
			double rtof_center;
			double rtof_width;

			//additional pipico variables
			double mass1_amu;
			double charge1_au;
			double mass2_amu;
			double charge2_au;
			double efield1_Vcm;
			double acceleration1_mm;
			double efield2_Vcm;
			double acceleration2_mm;
			double efield3_Vcm;
			double acceleration3_mm;
			double pipico_width_ns;
			double shift_ns;

			//additional three ions variables
			double mass3_amu;
			double charge3_au;

			//if projectile detector is used
			double px_center;
			double py_center;
			double p_radius;


		public:
			//presorter_class();
			#ifdef CRAP
				presorter_class(int Type, int Flag, CH_event_struct * Event, histo_handler * Hist, spectrometer_class * spect, tof_calc_class * ctof);
			#else
				presorter_class(int Type, int Flag, CH_event_struct * Event, Histo * Hist, spectrometer_class * spect, tof_calc_class * ctof);
			#endif
			
			~presorter_class();

			CH_event_struct * evto;

			int Get_flag();

			bool Run();

			//////////////////////////			 ANY	   ///////////////////////////////////////
			// write all hits to channel FLAG
			bool any();

			//////////////////////////   S I N G L E   T O F S  ///////////////////////////////////////
			// electron & recoil TOF
			void init_r_tof(double rtof_center, double rtof_width);
			bool r_tof();
			
			void init_e_tof(double etof_center, double etof_width);
			bool e_tof();

			//////////////////////////   E R   T O F s   ///////////////////////////////////////
			// electron & recoil TOF
			void init_er_tof(double etof_center, double etof_width, double rtof_center, double rtof_width);
			bool er_tof();

			//////////////////////////   P I P I C O   ///////////////////////////////////////
			// using spectrometer struct
			void init_pipico(double etof_center, double etof_width, double mass1_amu, double charge1_au, double mass2_amu, double charge2_au, double pipico_width);
			void init_pipico(double etof_center, double etof_width, double mass1_amu, double charge1_au, double mass2_amu, double charge2_au, double efield1_Vcm, double acceleration1_mm, double pipico_width);
			void init_pipico(double etof_center, double etof_width, double mass1_amu, double charge1_au, double mass2_amu, double charge2_au, double efield1_Vcm, double acceleration1_mm, double efield2_Vcm, double acceleration2_mm, double pipico_width);
			void init_pipico(double etof_center, double etof_width, double mass1_amu, double charge1_au, double mass2_amu, double charge2_au, double efield1_Vcm, double acceleration1_mm, double efield2_Vcm, double acceleration2_mm, double efield3_Vcm, double acceleration3_mm, double pipico_width);
			bool pipico();

			//////////////////////////   P R O J _ R E C   T O F   ///////////////////////////////////////
			// electron & recoil TOF, projectile position
			void init_proj_rec_tof(double etof_center, double etof_width, double rtof_center, double rtof_width, double px_center, double py_center, double p_radius);
			bool proj_rec_tof();

			//////////////////////////   P R O J _ P I P I C O   ///////////////////////////////////////
			// 2 recoils (PIPICO curve), projectile position (and additional electron TOFs)
			void presorter_class::init_proj_pipico(double etof_center, double etof_width, double mass1_amu, double charge1_au, double mass2_amu, double charge2_au, double pipico_width_ns, double px_center, double py_center, double p_radius);
			void presorter_class::init_proj_pipico(double etof_center, double etof_width, double mass1_amu, double charge1_au, double mass2_amu, double charge2_au, double efield1_Vcm, double acceleration1_mm, double pipico_width_ns, double px_center, double py_center, double p_radius);
			void presorter_class::init_proj_pipico(double etof_center, double etof_width, double mass1_amu, double charge1_au, double mass2_amu, double charge2_au, double efield1_Vcm, double acceleration1_mm, double efield2_Vcm, double acceleration2_mm, double pipico_width_ns, double px_center, double py_center, double p_radius);
			void presorter_class::init_proj_pipico(double etof_center, double etof_width, double mass1_amu, double charge1_au, double mass2_amu, double charge2_au, double efield1_Vcm, double acceleration1_mm, double efield2_Vcm, double acceleration2_mm, double efield3_Vcm, double acceleration3_mm, double pipico_width_ns, double px_center, double py_center, double p_radius);
			bool proj_pipico();

	};	// end presorter_class



#endif;