#include "Math.h"
#include "float.h"
#include "CH_Basics.h"

double NEW_v(double acc1, double acc2, double acc3, double acc1_length, double acc2_length, double acc3_length, double v, double t){

			double t_function = 0.;
			double Dt_function_dv = 0.;
			
			
			double a2_term = 0.0;
			double a3_term = 0.0;
			double v1 = 0.0;
			
				// Newton's method x1 = x0 - f(x0)/f'(x0)
				/*
				t_function =	
								-t + (-v + sqrt(2.0*acc1*acc1_length + v*v))/acc1 + 
								( (-sqrt(2.0*acc1*acc1_length + v*v) + sqrt(2.0*acc1*acc1_length + 2.0*acc2*acc2_length + v*v))/acc2 )+ 
								( (-sqrt(2.0*acc1*acc1_length + 2.0*acc2*acc2_length + v*v) + sqrt(2.0*acc1*acc1_length + 2.0*acc2*acc2_length + 2.0*acc3*acc3_length + v*v))/acc3 );

				Dt_function_dv = 
								(-1.0 + v/sqrt(2.0*acc1*acc1_length + v*v))/acc1 + 
								( (v*(-(1.0/sqrt(2.0*acc1*acc1_length + v*v)) + 1.0/sqrt(2.0*acc1*acc1_length + 2.0*acc2*acc2_length + v*v)))/acc2 )+ 
								( (v*(-(1.0/sqrt(2.0*acc1*acc1_length + 2.0*acc2*acc2_length + v*v)) + 1.0/sqrt(2.0*acc1*acc1_length + 2.0*acc2*acc2_length + 2.0*acc3*acc3_length + v*v)))/acc3 );		
				*/
				//I must do it in parts because otherwise it will blow up.
				
				if (acc2 != 0. &&  acc2_length != 0.){
					a2_term = (-sqrt(2.0*acc1*acc1_length + v*v) + sqrt(2.0*acc1*acc1_length + 2.0*acc2*acc2_length + v*v))/acc2;
				}
				else if (acc2_length != 0. && acc2 == 0.){
					a2_term = acc2_length/sqrt(2.*acc1*acc1_length + v*v); 
				}
				else{
					a2_term = 0.0;
				}
				if (acc3 != 0. &&  acc3_length != 0.){
					a3_term = (-sqrt(2.0*acc1*acc1_length + 2.0*acc2*acc2_length + v*v) + sqrt(2.0*acc1*acc1_length + 2.0*acc2*acc2_length + 2.0*acc3*acc3_length + v*v))/acc3;
				}
				else if (acc3_length != 0. && acc3 == 0.) {
					a3_term = acc3_length/sqrt(2.*acc1*acc1_length + 2.*acc2*acc2_length + v*v);
				}
				else{
					a3_term = 0.0;
				}
				
				//printf("a2_term = %5.5e | a3_term = %5.5e \n", a2_term, a3_term);

				t_function =	-t + (-v + sqrt(2.0*acc1*acc1_length + v*v))/acc1 + a2_term + a3_term; 
				
				
				v1 = v*(1.01);
				
				
				if (acc2 != 0. &&  acc2_length != 0.){
					a2_term = (-sqrt(2.0*acc1*acc1_length + v1*v1) + sqrt(2.0*acc1*acc1_length + 2.0*acc2*acc2_length + v1*v1))/acc2;
				}
				else if (acc2_length != 0. && acc2 == 0.){
					a2_term =acc2_length/sqrt(2.*acc1*acc1_length + v1*v1); 
				}
				else{
					a2_term = 0.0;
				}
				
				if (acc3 != 0. &&  acc3_length != 0.){
					a3_term = (-sqrt(2.0*acc1*acc1_length + 2.0*acc2*acc2_length + v1*v1) + sqrt(2.0*acc1*acc1_length + 2.0*acc2*acc2_length + 2.0*acc3*acc3_length + v1*v1))/acc3;
				}
				else if (acc3_length != 0. && acc3 == 0.) {
					a3_term = acc3_length/sqrt(2.*acc1*acc1_length + 2.*acc2*acc2_length + v1*v1);
				}
				else{
					a3_term = 0.0;
				}
				
				
				Dt_function_dv =( (-t + (-v1 + sqrt(2.0*acc1*acc1_length + v1*v1))/acc1 + a2_term + a3_term) - t_function )/ (v1 - v); 

				
				
	
				return  v -  t_function / Dt_function_dv;;


}

//// **************************************************************
//// **
//// ** momentum calculation for time of flight direction for 3 
//// ** accel. regions of any size.  This function uses newton's
//// ** method to solve the problem.  Returns P in Au.  
//// ** 
//// ** By Joshua Williams
//// **************************************************************
double tof2mom_3accel(double tof_ns, double acc1mm, double acc2mm, double acc3mm, 
					  double Efield1_Vpcm, double Efield2_Vpcm, double Efield3_Vpcm, double charge_au, double mass_amu){

		
		//don't try to find P if the time of flight is negative
		if (tof_ns > 0.1){ 

			
			double E_Field_au_over_si =  1. / 5.1421e11; // 									au * m / V
			double length_au_over_m =  1. / 5.2917720859e-11;  // 						au / m
			double time_au_over_s = 1. / ( 2.418884326505e-17 ); // 						au / s
			double AUmass_over_AMU = 1.660538782e-27 / 9.10938215e-31 ;//		(au / kg) * (kg / amu)		
			double MperS_over_auVelocity = 2.1876912633e6;//								m / ( s * au )
			double Kg_over_AMU = 1.660538782e-27;			
			//double Psi_to_Pau = 1. / 1.992851565e-24;
			
			
			//convert to AU units
			double t = (tof_ns * 1e-9) * time_au_over_s  ;
			double mass = mass_amu *  AUmass_over_AMU;
			double q = charge_au;
			double acc1_length = acc1mm * 0.001 * length_au_over_m;
			double acc2_length = acc2mm * 0.001 * length_au_over_m;
			double acc3_length = acc3mm * 0.001 * length_au_over_m;
			double Efield1_Vau = Efield1_Vpcm * 100.0 * E_Field_au_over_si;
			double Efield2_Vau = Efield2_Vpcm * 100.0 * E_Field_au_over_si;
			double Efield3_Vau = Efield3_Vpcm * 100.0 * E_Field_au_over_si;
			double v=1;


			//find accel. for each region
			double acc1 = (Efield1_Vau * q) / mass;
			double acc2 = (Efield2_Vau * q) / mass;
			double acc3 = (Efield3_Vau * q) / mass;
			
			//find the an initial value for v (this is done through the a first order seires expansion of the t(v) about zero)
			double acc1_temp = 0.001, acc2_temp = 0.001, acc3_temp = 0.001;
			if(acc1 != 0.) acc1_temp = acc1;
			if(acc2 != 0.) acc2_temp = acc2;
			if(acc3 != 0.) acc3_temp = acc3;
			//YOU SHOULD CHANGE V IF YOU KNOW A BETTER GUESS 
			v = sqrt(2.)*sqrt(acc1_temp*acc1_length) - (sqrt(2.)*acc1_temp*sqrt(acc1_temp*acc1_length))/acc2_temp + (sqrt(2.)*acc1_temp*sqrt(acc1_temp*acc1_length + acc2_temp*acc2_length))/acc2_temp - (sqrt(2.)*acc1_temp*sqrt(acc1_temp*acc1_length + acc2_temp*acc2_length))/acc3_temp + (sqrt(2.)*acc1_temp*sqrt(acc1_temp*acc1_length + acc2_temp*acc2_length + acc3_temp*acc3_length))/acc3_temp - acc1_temp*t;
			
			if ( _finite(v) == 0.0 ) {
				//**********
				v=5;		//if you get here this proabably means you are using a decceration field, so I will guess a large value
				//*********
			}

			double new_v = 0.;
			
			
			//int limit = 10;
			int shift_counter = 1;
			int loop_counter = 0;
			for( int i=0;i<50;i++){
				loop_counter++;
				new_v = NEW_v( acc1,  acc2,  acc3,  acc1_length,  acc2_length,  acc3_length,  v, t);
				
				if ( _finite(new_v) == 0.0 ) {
					new_v = v * (1. +  pow(2., (double)shift_counter) / 1000.);   //move it, at first just a little bit but increase the size of the shift until you get a finite value for the new_v
					shift_counter++;
				}
				else{
					shift_counter = 1;  // reset the shift counter when you get a finite value for new_v
				}
				
				if ( fabs((new_v * mass  - v * mass )/ (v * mass )) < 1e-6) {
					i = 500; //thus exiting the loop
				}  
				v = new_v;
			}
			
			if ( loop_counter >= 50) { return (1e50 ); } // the function did not converge (but the value might still be pretty to the correct value) 
			return v * mass;
						  
			}

		else{
			return -100000.; 
			//if negative tof then the data point is crap so return crap
		}

}

// **************************************************************
// **
// ** calculate momenta for electron x,y direction (magnetic field)
// ** using Mirko's functions.
// **
// **************************************************************

double calc_px(double tof_ns, double x_mm, double y_mm, double mass_amu, double charge_au, double BField_ns, bool BField_clockwise = true) {

	double px,w,a,b;

	double m = mass_amu / MASSAU * MEKG;
	double q = charge_au * COULOMB;
	double pau = m*300.e6/137.;

	double fieldB = 2.*m*3.14152 / (q * BField_ns * 1e-9);

	if(BField_clockwise)
		fieldB = -fieldB;

	w = q / m * fieldB;
	a = (1. - cos(w * tof_ns*1.e-9)) / w;
	b = (sin(w * tof_ns*1.e-9)) / w;

	px = m * (x_mm/1000. * b - a*y_mm/1000.) / (a*a + b*b);
	px = px / pau;

	return px;
}

double calc_py(double tof_ns, double x_mm, double y_mm, double mass_amu, double charge_au, double BField_ns, bool BField_clockwise = true) {

	double py,w,a,b;

	double m = mass_amu / MASSAU * MEKG;
	double q = charge_au * COULOMB;
	double pau = m*300.e6/137.;

	double fieldB = 2.*m*3.14152 / (q * BField_ns * 1e-9);

	if(BField_clockwise)
		fieldB = -fieldB;

	w = q / m * fieldB;
	a = (1. - cos(w * tof_ns*1.e-9)) / w;
	b = (sin(w * tof_ns*1.e-9)) / w;

	py = m * (-x_mm/1000. * a - b*y_mm/1000.) / (a*a + b*b);
	py = py / pau;
	
	return py;
}

// **************************************************************
// **
// ** This function can calculate the time of flight for the
// ** second recoil.  This function uses tof2mom_3accel
// ** and assumes that it is a two body break up. 
// **
// ** 
// ** By Joshua Williams
// **************************************************************
double t2_3accel(double tof_ns, double acc1mm, double acc2mm, double acc3mm, 
					  double Efield1_Vpcm, double Efield2_Vpcm, double Efield3_Vpcm, double charge1_au, double charge2_au, double mass1_amu, double mass2_amu){
		double t = tof_ns * 1e-9;
		double mass = mass2_amu * 1.660538782e-27;
		double q = charge2_au * 1.602176487e-19;;
		double acc1m = acc1mm * .001;
		double acc2m = acc2mm * .001;
		double acc3m = acc3mm * .001;
		double Efield1_Vpm = Efield1_Vpcm * 100.0;
		double Efield2_Vpm = Efield2_Vpcm * 100.0;
		double Efield3_Vpm = Efield3_Vpcm * 100.0;

		//find accel. for each region
		double acc1 = (Efield1_Vpm * q)/ mass;
		double acc2 = (Efield2_Vpm * q)/ mass;
		double acc3 = (Efield3_Vpm * q)/ mass;
		
		double  acc2_zero_bool = 0.;
		double  acc3_zero_bool = 0.;
			

			
		double a2_term = 0.0;
		double a3_term = 0.0;


		double r1_px = - tof2mom_3accel(tof_ns, acc1mm, acc2mm, acc3mm, 
						  Efield1_Vpcm, Efield2_Vpcm, Efield3_Vpcm, charge1_au, mass1_amu);
		
		double v = r1_px / (  mass / (9.1093826e-31*2.1876912633e6) );
		

		if (acc2 != 0. &&  acc2m != 0.){
			a2_term = (-sqrt(2.0*acc1*acc1m + v*v) + sqrt(2.0*acc1*acc1m + 2.0*acc2*acc2m + v*v))/acc2;
		}
		else if (acc2m != 0. && acc2 == 0.){
			a2_term = acc2m/sqrt(2.*acc1*acc1m + v*v); 
		}
		else{
			a2_term = 0.0;
		}
		
		if (acc3 != 0. &&  acc3m != 0.){
			a3_term = (-sqrt(2.0*acc1*acc1m + 2.0*acc2*acc2m + v*v) + sqrt(2.0*acc1*acc1m + 2.0*acc2*acc2m + 2.0*acc3*acc3m + v*v))/acc3;
		}
		else if (acc3m != 0. && acc3 == 0.) {
			a3_term = acc3m/sqrt(2.*acc1*acc1m + 2.*acc2*acc2m + v*v);
		}
		else{
			a3_term = 0.0;
		}
		
		/*if (counter < 10){
			printf("a2_term = %e, a3_term = %e, " , a2_term, a3_term);
		}*/	
		double t2 = (-v + sqrt(2.0*acc1*acc1m + v*v))/acc1 + a2_term + a3_term; 
		
		
		
		return (t2 * 1.e+9);
	
}