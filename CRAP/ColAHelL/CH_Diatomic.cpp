#include "CH_Diatomic.h"
#include "CH_Basics.h"
#include "CH_Ion.h"

#include "Math.h"

diatomic_class::diatomic_class()
{
	this->ion1 = new ion_class;
	this->ion2 = new ion_class;

	this->rel = new particle_phys;
	this->cm = new particle_phys;

	this->tmp_ion = new ion_class;
}

diatomic_class::~diatomic_class()
{
	delete ion1;
	delete ion2;

	delete rel;
	delete cm;

	delete tmp_ion;
}

void diatomic_class::set_ions(ion_class *ion1, ion_class *ion2) {
	this->ion1 = ion1;
	this->ion2 = ion2;
	this->reduced_mass = this->ion1->raw->m * this->ion2->raw->m / (this->ion1->raw->m + this->ion2->raw->m);  
}


void diatomic_class::set_channel(int channel) {
	this->channel = channel;
}

void diatomic_class::set_channel(int channel, bool valid) {
	this->channel = channel;
	this->valid = valid;
}

void diatomic_class::set_valid(bool valid) {
	this->valid = valid;
}

void diatomic_class::sort_ions_and_process(spectrometer_class *spect) {
	// find right m/q by checking back to back emission (i.e. cm_mom = 0)

	double temp_m1 = this->ion1->raw->m;
	double temp_q1 = this->ion1->raw->q;
	double temp_m2 = this->ion2->raw->m;
	double temp_q2 = this->ion2->raw->q;

	// process first combination
	process(spect);
	double cm_mom_A = this->cm->mom.mag;

	// swap particle properties

	this->ion1->raw->m = temp_m2;
	this->ion1->raw->q = temp_q2;
	this->ion2->raw->m = temp_m1;
	this->ion2->raw->q = temp_q1;

	// process second combination
	process(spect);
	double cm_mom_B = this->cm->mom.mag;

	if(cm_mom_A < cm_mom_B) {
		// restore initial parameter set, inital asignment was ok
		this->ion1->raw->m = temp_m2;
		this->ion1->raw->q = temp_q2;
		this->ion2->raw->m = temp_m1;
		this->ion2->raw->q = temp_q1;
		process(spect);
	
	} else {
		// swap ions, as we want the ion with inital mass m1 to be ion1
		tmp_ion = ion2;
		ion2 = ion1;
		ion1 = tmp_ion;
	}
}
void diatomic_class::process(spectrometer_class *spect) {
	this->ion1->process(spect);
	this->ion2->process(spect);

	process_diatomic_only();
}

void diatomic_class::process_diatomic_only (){

	// relative momenta
	this->rel->mom.x = (this->ion1->phy->mom.x - this->ion2->phy->mom.x) / 2.0;
	this->rel->mom.y = (this->ion1->phy->mom.y - this->ion2->phy->mom.y) / 2.0;
	this->rel->mom.z = (this->ion1->phy->mom.z - this->ion2->phy->mom.z) / 2.0;
	this->rel->mom.mag = sqrt(this->rel->mom.x * this->rel->mom.x + this->rel->mom.y * this->rel->mom.y + this->rel->mom.z * this->rel->mom.z);

	this->rel->ang_px.ctheta = this->rel->mom.x / this->rel->mom.mag;
	this->rel->ang_py.ctheta = this->rel->mom.y / this->rel->mom.mag;
	this->rel->ang_pz.ctheta = this->rel->mom.z / this->rel->mom.mag;

	this->rel->ang_px.theta = acos(this->rel->ang_px.ctheta)/PI*180.0;
	this->rel->ang_py.theta = acos(this->rel->ang_py.ctheta)/PI*180.0;
	this->rel->ang_pz.theta = acos(this->rel->ang_pz.ctheta)/PI*180.0;

	this->rel->ang_px.phi = atan2(this->rel->mom.z,this->rel->mom.y)/PI*180.0;
	this->rel->ang_py.phi = atan2(this->rel->mom.x,this->rel->mom.z)/PI*180.0;
	this->rel->ang_pz.phi = atan2(this->rel->mom.y,this->rel->mom.x)/PI*180.0;

	this->rel->energy = this->rel->mom.mag * this->rel->mom.mag / (2.0 * this->reduced_mass) * EVAU;

	// cm momenta
	this->cm->mom.x = (this->ion1->phy->mom.x + this->ion2->phy->mom.x);
	this->cm->mom.y = (this->ion1->phy->mom.y + this->ion2->phy->mom.y);
	this->cm->mom.z = (this->ion1->phy->mom.z + this->ion2->phy->mom.z);
	this->cm->mom.mag = sqrt(this->cm->mom.x * this->cm->mom.x + this->cm->mom.y * this->cm->mom.y + this->cm->mom.z * this->cm->mom.z);

	this->cm->ang_px.ctheta = this->cm->mom.x / this->cm->mom.mag;
	this->cm->ang_py.ctheta = this->cm->mom.y / this->cm->mom.mag;
	this->cm->ang_pz.ctheta = this->cm->mom.z / this->cm->mom.mag;

	this->cm->ang_px.theta = acos(this->cm->ang_px.ctheta)/PI*180.0;
	this->cm->ang_py.theta = acos(this->cm->ang_py.ctheta)/PI*180.0;
	this->cm->ang_pz.theta = acos(this->cm->ang_pz.ctheta)/PI*180.0;

	this->cm->ang_px.phi = atan2(this->cm->mom.z,this->cm->mom.y)/PI*180.0;
	this->cm->ang_py.phi = atan2(this->cm->mom.x,this->cm->mom.z)/PI*180.0;
	this->cm->ang_pz.phi = atan2(this->cm->mom.y,this->cm->mom.x)/PI*180.0;

	this->cm->energy = this->cm->mom.mag * this->cm->mom.mag / (2.0 * (this->ion1->raw->m + this->ion2->raw->m) * MASSAU) * EVAU;

}



bool diatomic_class::check_mom_cm_x(double psumx_width, int channel, bool invalidate)
{
	if(fabs(this->cm->mom.x)>psumx_width)
	{
		if(invalidate)
			this->valid = false;
		return false;
	}
	return true;
}

bool diatomic_class::check_mom_cm_y(double psumy_width, int channel, bool invalidate)
{
	if(fabs(this->cm->mom.y)>psumy_width)
	{
		if(invalidate)
			this->valid = false;
		return false;
	}
	return true;
}

bool diatomic_class::check_mom_cm_z(double psumz_width, int channel, bool invalidate)
{
	if(fabs(this->cm->mom.z)>psumz_width)
	{
		if(invalidate)
			this->valid = false;
		return false;
	}
	return true;
}

bool diatomic_class::check_mom_cm(double psumx_width, double psumy_width, double psumz_width, int channel, bool invalidate)
{	
	if(!(check_mom_cm_x(psumx_width,channel,invalidate)))
		return false;

	if(!(check_mom_cm_y(psumy_width,channel,invalidate)))
		return false;

	if(!(check_mom_cm_z(psumz_width,channel,invalidate)))
		return false;

	if(channel>=0)
		this->channel = channel;

	return true;
}


bool diatomic_class::check_KER(double energy_from, double energy_to, int channel, bool invalidate)
{
	if((this->rel->energy < energy_from) || (this->rel->energy > energy_to))
	{
		if(invalidate)
			this->valid = false;
		return false;
	}
	if(channel>=0)
		this->channel = channel;

	return true;
}