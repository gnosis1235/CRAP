#ifndef CH_BASICS_ALREADY_INCLUDED
	#define CH_BASICS_ALREADY_INCLUDED

	#include <vector>

	#define PAU 1.99285E-24				// Momentum in AU
	#define MASSAU 1822.888				// Mass in AU
	#define VAU 2.1877e+6				// Velocity in AUs
	#define EVAU 27.2114				// eV in AU
	#define EFIELDAU 0.00805			// Electric field (V/cm) in AU
	#define PI 3.14159265				// PI
	#define MEKG 9.1093897E-31			// Electron Mass in Kg
	#define MUKG 1.66053886e-27			// u in Kg
	#define COULOMB	1.60217733E-19		// Elementary charge


	struct ch_vec{

	#ifdef FLOAT_ONLY
		float x, y, z, mag;
	#else
		double x, y, z, mag;
	#endif

	};

	struct ch_angles{

	#ifdef FLOAT_ONLY
		float theta;
		float ctheta;
		float phi;
	#else
		double theta;
		double ctheta;
		double phi;
	#endif
	};

	struct cor_param{ 

	#ifdef FLOAT_ONLY
		float dx;
		float dy;
		float dt;

		float x_stretch;
		float y_stretch;
		float overall_stretch;
	#else
		double dx;
		double dy;
		double dt;

		double x_stretch;
		double y_stretch;
		double overall_stretch;
	#endif
	};

	struct xyt{

	#ifdef FLOAT_ONLY
		float x; // in mm
		float y; // in mm
		float t; // in ns
	#else
		double x; // in mm
		double y; // in mm
		double t; // in ns
	#endif

	};

	struct CH_det_struct{
        int num_hits;
		std::vector<double> method;
        std::vector<double> x;
        std::vector<double> y;
        std::vector<double> time;
        std::vector<double> tof;
	};

	struct CH_data_struct{
		int method;
		double x;
		double y;
		double tof;
		double mcp;
	};

	struct particle_raw{
		int method;

	#ifdef FLOAT_ONLY
		float m; // in a.u.
		float q; // in a.u.
		CH_data_struct data; // tof and position in ns and mm
		
		float phi; // angle on detector
	#else
		double m; // in a.u.
		double q; // in a.u.
		CH_data_struct data; // tof and position in ns and mm
		double phi; // angle on detector
	#endif

	};

	struct particle_phys{
		ch_vec mom;       // in a.u.

	#ifdef FLOAT_ONLY
		float energy; // in eV
	#else
		double energy; // in eV
	#endif

		ch_angles ang_px;
		ch_angles ang_py;
		ch_angles ang_pz;
	};

#endif;

