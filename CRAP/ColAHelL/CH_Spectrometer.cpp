#include "CH_Spectrometer.h"


spectrometer_class::spectrometer_class(){
	this->ion_side = new spec_arm;

	this->ion_side->linear_approximation = false;
	this->ion_side->number_of_regions = 0;
	
	for(int i=0;i<MAX_NUM_REGIONS;i++) {
		this->ion_side->Efields[i] = 0.0;
		this->ion_side->lengths[i] = 0.0;
	}

	this->electron_side = new spec_arm;

	this->electron_side->linear_approximation = false;
	this->electron_side->number_of_regions = 0;

	for(int i=0;i<MAX_NUM_REGIONS;i++) {
		this->electron_side->Efields[i] = 0.0;
		this->electron_side->lengths[i] = 0.0;
	}

	this->Bfield_G = 0;
	this->Bfield_clockwise = false;
}

spectrometer_class::~spectrometer_class(){
	delete this->ion_side;
	delete this->electron_side;
}

void spectrometer_class::set_Bfield(double B_val, bool B_clockwise, bool B_in_ns) {
	this->Bfield_clockwise = B_clockwise;
	if(B_in_ns) {
		this->Bfield_ns = B_val;
		this->Bfield_G = 357.238755341 / B_val; 
	} else {
		this->Bfield_ns = 357.238755341 / B_val ;
		this->Bfield_G = B_val;
	}
}

void spectrometer_class::set_ion_arm_lin(double E_val) {	
	this->ion_side->linear_approximation = true;
	this->ion_side->Efields[0] = E_val;
}

void spectrometer_class::set_ion_arm(unsigned short num_regions, double *lengths, double *E_val) {
	this->ion_side->linear_approximation = false;
	this->ion_side->number_of_regions = num_regions;

	for(int i=0;i<num_regions;i++) {
		this->ion_side->Efields[i] = E_val[i];
		this->ion_side->lengths[i] = lengths[i];
	}
}

void spectrometer_class::set_electron_arm(unsigned short num_regions, double *lengths, double *E_val) {
	this->electron_side->linear_approximation = false;
	this->electron_side->number_of_regions = num_regions;

	for(int i=0;i<num_regions;i++) {
		this->electron_side->Efields[i] = E_val[i];
		this->electron_side->lengths[i] = lengths[i];
	}
}