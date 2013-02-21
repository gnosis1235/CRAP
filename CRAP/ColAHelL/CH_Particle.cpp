#include "CH_particle.h"
#include "CH_Spectrometer.h"
#include "CH_Basics.h"
#include "CH_FUN_Lowlevel.h"

#include "Math.h"

particle_class::particle_class()
{
	this->raw = new particle_raw;
	this->phy = new particle_phys;

	this->raw->m = 0.0;
	this->raw->q = 0.0;
	this->channel = 0;
	this->valid = false;
	
	this->phy->ang_px.ctheta = -1000.0;
	this->phy->ang_px.phi = -1000.0;
	this->phy->ang_px.theta = -1000.0;

	this->phy->ang_py.ctheta = -1000.0;
	this->phy->ang_pz.phi = -1000.0;
	this->phy->ang_pz.theta = -1000.0;

	this->phy->ang_pz.ctheta = -1000.0;
	this->phy->ang_pz.phi = -1000.0;
	this->phy->ang_pz.theta = -1000.0;

	this->phy->mom.x = -1000.0;
	this->phy->mom.y = -1000.0;
	this->phy->mom.z = -1000.0;
	this->phy->mom.mag = -1000.0;

	this->phy->energy = -1000.0;

	this->cor.dt = 0.0;
	this->cor.dx = 0.0;
	this->cor.dy = 0.0;
	this->cor.overall_stretch = 1.0;
	this->cor.x_stretch = 1.0;
	this->cor.y_stretch = 1.0;

}

particle_class::~particle_class()
{
	delete raw;
	delete phy;
}


void particle_class::set_raw(particle_raw *rawdata) {
	this->raw->data = rawdata->data;
	this->raw->m = rawdata->m;
	this->raw->q = rawdata->q;
	this->raw->method = rawdata->method;
	this->raw->phi = rawdata->phi;
}

void particle_class::set_raw(double x, double y, double t, int method) {
	this->raw->data.x = x;
	this->raw->data.y = y;
	this->raw->data.tof = t;

	this->raw->phi = 0;
	this->raw->method = method;
}

void particle_class::calc_phi_pos() {
	this->raw->phi = atan2(this->raw->data.x, this->raw->data.y)/PI*180.0;
}

void particle_class::set_channel(int channel) {
	this->channel = channel;
}

void particle_class::set_channel(int channel, bool valid) {
	this->channel = channel;
	this->valid = valid;
}
void particle_class::set_valid(bool valid) {
	this->valid = valid;
}

void particle_class::shift_stretch_raw(cor_param * fac) {
	this->raw->data.x += fac->dx;
	this->raw->data.x *= fac->x_stretch;
	this->raw->data.x *= fac->overall_stretch;

	this->raw->data.y += fac->dy;
	this->raw->data.y *= fac->y_stretch;
	this->raw->data.y *= fac->overall_stretch;

	this->raw->data.tof += fac->dt;
}

void particle_class::rotate_raw(double ang) {
	ang = ang/180.*PI;

	double x_temp = cos(ang) * this->raw->data.x - sin(ang) * this->raw->data.y;
	double y_temp = sin(ang) * this->raw->data.x + cos(ang) * this->raw->data.y;
	this->raw->data.x = x_temp;
	this->raw->data.y = y_temp;
}

void particle_class::process(spectrometer_class *spect) {
	double x = this->raw->data.x;
	double y = this->raw->data.y;
	double t = this->raw->data.tof;

	// first correct raw data with individual correction parameters
	x += cor.dx;
	x *= cor.x_stretch;
	x *= cor.overall_stretch;

	y += cor.dy;
	y *= cor.y_stretch;
	y *= cor.overall_stretch;

	t += cor.dt;
	
	// calculate momenta
	this->phy->mom.x = calc_px(t, x, y, this->raw->m, this->raw->q, spect->Bfield_ns, spect->Bfield_clockwise);
	this->phy->mom.y = calc_py(t, x, y, this->raw->m, this->raw->q, spect->Bfield_ns, spect->Bfield_clockwise);

	if(this->raw->m < 1.0) { 
		this->phy->mom.z = tof2mom_3accel(t, spect->electron_side->lengths[0], spect->electron_side->lengths[1], spect->electron_side->lengths[2], spect->electron_side->Efields[0], spect->electron_side->Efields[1], spect->electron_side->Efields[2], fabs(this->raw->q), this->raw->m);
	} else {
		if(spect->ion_side->linear_approximation) {	
			this->phy->mom.z = spect->ion_side->Efields[0] * this->raw->q * t / 124.38;  // according to Markus' diploma thesis
		} else {
			this->phy->mom.z = tof2mom_3accel(t, spect->ion_side->lengths[0], spect->ion_side->lengths[1], spect->ion_side->lengths[2], spect->ion_side->Efields[0], spect->ion_side->Efields[1], spect->ion_side->Efields[2], this->raw->q, this->raw->m);			
		}
	}

	this->phy->mom.mag = sqrt(this->phy->mom.x * this->phy->mom.x + this->phy->mom.y * this->phy->mom.y + this->phy->mom.z * this->phy->mom.z);

	// calculate momentum angles etc.
	this->phy->ang_px.ctheta = this->phy->mom.x / this->phy->mom.mag;
	this->phy->ang_py.ctheta = this->phy->mom.y / this->phy->mom.mag;
	this->phy->ang_pz.ctheta = this->phy->mom.z / this->phy->mom.mag;

	this->phy->ang_px.theta = acos(this->phy->ang_px.ctheta)/PI*180.0;
	this->phy->ang_py.theta = acos(this->phy->ang_py.ctheta)/PI*180.0;
	this->phy->ang_pz.theta = acos(this->phy->ang_pz.ctheta)/PI*180.0;

	this->phy->ang_px.phi = atan2(this->phy->mom.z,this->phy->mom.y)/PI*180.0;
	this->phy->ang_py.phi = atan2(this->phy->mom.x,this->phy->mom.z)/PI*180.0;
	this->phy->ang_pz.phi = atan2(this->phy->mom.y,this->phy->mom.x)/PI*180.0;

	// calculate energy
	this->phy->energy = this->phy->mom.mag * this->phy->mom.mag / (2.0 * this->raw->m * MASSAU) * EVAU;  

}

bool particle_class::check_tof(double tof_from, double tof_to, int channel, bool invalidate)
{
	if((this->raw->data.tof < tof_from) || (raw->data.tof > tof_to))
	{
		if(invalidate)
			this->valid = false;
		return false;
	}
	if(channel>=0)
		this->channel = channel;	
	return true;
}

bool particle_class::check_mom(double px_width, double py_width, double pz_width, int channel, bool invalidate)
{
	if(fabs(phy->mom.x)>px_width)
	{
		if(invalidate)
			this->valid = false;
		return false;
	}
	if(fabs(phy->mom.y)>py_width)
	{
		if(invalidate)
			this->valid = false;
		return false;
	}
	if(fabs(phy->mom.z)>pz_width)
	{
		if(invalidate)
			this->valid = false;
		return false;
	}
	if(channel>=0)
		this->channel = channel;
	return true;
}


bool particle_class::check_energy(double energy_from, double energy_to, int channel, bool invalidate)
{
	if((phy->energy < energy_from) || (phy->energy > energy_to))
	{
		if(invalidate)
			this->valid = false;
		return false;
	}
	if(channel>=0)
		this->channel = channel;
	return true;
}