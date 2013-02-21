#include "CH_Presorter.h"
#include <math.h>
#include "CH_Tof.h"
#include "CH_Spectrometer.h"
#include "CH_FUN_Lowlevel.h"
//#include "CH_Event.h"

#ifdef CRAP

	presorter_class::presorter_class(int Type, int Flag, CH_event_struct * Event, histo_handler * Hist, spectrometer_class * spect, tof_calc_class * ctof){
#else 
	presorter_class::presorter_class(int Type, int Flag, CH_event_struct * Event, Histo * Hist, spectrometer_class * spect, tof_calc_class * ctof){
#endif
	this->Type = Type;
	this->Flag = Flag;
	this->H = Hist;
	this->evti=Event;
	this->ctof = ctof;
	this->spect = spect;
	
	sprintf(dir,"_presorter check/channel %d",Flag);

	evto = new CH_event_struct();
	for(int i = 0; i<32;i++) {
		evto->e.method.push_back(0.0);
		evto->e.x.push_back(0.0);
		evto->e.y.push_back(0.0);
		evto->e.tof.push_back(0.0);
		evto->e.time.push_back(0.0);
		evto->r.method.push_back(0.0);
		evto->r.x.push_back(0.0);
		evto->r.y.push_back(0.0);
		evto->r.tof.push_back(0.0);
		evto->r.time.push_back(0.0);
		evto->p.method.push_back(0.0);
		evto->p.x.push_back(0.0);
		evto->p.y.push_back(0.0);
		evto->p.tof.push_back(0.0);
		evto->p.time.push_back(0.0);
	}	
	evto->e.num_hits=0;
	evto->r.num_hits=0;
	evto->p.num_hits=0;
}




presorter_class::~presorter_class() {
}

int presorter_class::Get_flag() {
	return Flag;
}

bool presorter_class::Run()
{
	switch (this->Type) {

		case ANY :
			return any();
			break;
		case ETOF :
			return e_tof();
			break;
		case RTOF :
			return r_tof();
			break;
		case ERTOF :
			return er_tof();
			break;
		case PIPICO :
			return pipico();
			break;
		case PRTOF :
			return proj_rec_tof();
			break;
		case PROJ_PIPICO :
			return proj_pipico();
			break;

		default:
			break;
	}
	return false;
}


//////////////////////////			 ANY	   ///////////////////////////////////////
// write all hits to channel FLAG
bool presorter_class::any()
{				
	this->evto = this->evti;
	this->evto->reaction = this->Flag;
	return true;
}

//////////////////////////   S I N G L E   T O F s   ///////////////////////////////////////
// recoil TOF

void presorter_class::init_r_tof(double rtof_center, double rtof_width)
{
	this->rtof_center = rtof_center;
	this->rtof_width  = rtof_width;
}

bool presorter_class::r_tof()
{					
	if(evti->r.num_hits < 2)
		return false;	
	
	this->evto->reaction = this->Flag;
	this->evto->bunchmarker = evti->bunchmarker;

	for (int j=0;j<(evti->r.num_hits);j++) {

		H->fill1(10501+this->Flag*1000,"recoil tof(no prs)",evti->r.tof[j],1.,"recoil TOF",int(rtof_center/2.0),0.,2.0*rtof_center,"TOF [ns]",dir);

		if(fabs(evti->r.tof[j]-rtof_center)<rtof_width)	{
			evto->r.num_hits = 1;
			evto->r.x[0] = evti->r.x[j];
			evto->r.y[0] = evti->r.y[j];
			evto->r.tof[0] = evti->r.tof[j];
			evto->r.time[0] = evti->r.time[j];
			evto->r.method[0] = evti->r.method[j];
			evto->r.num_hits = 1;

			H->fill1(10503+this->Flag*1000,"recoil tof",evti->r.tof[j],1.,"recoil TOF",int(rtof_center/2.0),0.,2.0*rtof_center,"TOF [ns]",dir);

			return true;
		}
	}
	return false;
}	

// electron TOF

void presorter_class::init_e_tof(double etof_center, double etof_width)
{
	this->etof_center = etof_center;
	this->etof_width  = etof_width;
}

bool presorter_class::e_tof()
{					
	if(evti->e.num_hits<1)
		return false;

	this->evto->reaction = this->Flag;
	this->evto->bunchmarker = evti->bunchmarker;

	for (int j=0;j<(evti->e.num_hits);j++) {

		H->fill1(10500+this->Flag*1000,"electron tof(no prs)",evti->e.tof[j],1.,"electron TOF",200,0.,etof_center + 2.0*etof_width,"TOF [ns]",dir);

		if(fabs(evti->e.tof[j]-etof_center)<etof_width)	{
			evto->e.num_hits = 1;
			evto->e.x[0] = evti->e.x[j];
			evto->e.y[0] = evti->e.y[j];
			evto->e.tof[0] = evti->e.tof[j];
			evto->e.time[0] = evti->e.time[j];
			evto->e.method[0] = evti->e.method[j];
			evto->e.num_hits = 1;

			H->fill1(10502+this->Flag*1000,"electron tof",evti->e.tof[j],1.,"electron TOF",200,0.,etof_center + 2.0*etof_width,"TOF [ns]",dir);

			return true;
		}
	}
	return false;
}	


//////////////////////////   E R   T O F s   ///////////////////////////////////////
// electron & recoil TOF

void presorter_class::init_er_tof(double etof_center, double etof_width, double rtof_center, double rtof_width)
{
	this->etof_center = etof_center;
	this->etof_width  = etof_width;
	this->rtof_center = rtof_center;
	this->rtof_width  = rtof_width;
}

bool presorter_class::er_tof()
{				
	if(evti->e.num_hits<1 || evti->r.num_hits < 1)
		return false;

	int Number_Of_Electrons = 1;

	this->evto->reaction = this->Flag;
	this->evto->bunchmarker = evti->bunchmarker;

	for (int i=0;i<(evti->e.num_hits);i++) {
		
		this->ctof->calc(evti,i);
		H->fill1(10500+this->Flag*1000,"electron tof(no prs)",evti->e.tof[i],1.,"electron TOF",200,0.,etof_center + 2.0*etof_width,"TOF [ns]",dir);
		
		for (int j=0;j<(evti->r.num_hits);j++) {

			H->fill1(10501+this->Flag*1000,"recoil tof(no prs)",evti->r.tof[j],1.,"recoil TOF",int(rtof_center/2.0),0.,2.0*rtof_center,"TOF [ns]",dir);
			
			if(fabs(evti->r.tof[j]-rtof_center)<rtof_width)	{
				if(fabs(evti->e.tof[i]-etof_center)<etof_width)	{
				
					evto->r.num_hits = 1;
					evto->r.x[0] = evti->r.x[j];
					evto->r.y[0] = evti->r.y[j];
					evto->r.tof[0] = evti->r.tof[j];
					evto->r.time[0] = evti->r.time[j];
					evto->r.method[0] = evti->r.method[j];

					evto->e.x[0] = evti->e.x[i];
					evto->e.y[0] = evti->e.y[i];
					evto->e.tof[0] = evti->e.tof[i];
					evto->e.time[0] = evti->e.time[i];
					evto->e.method[0] = evti->e.method[i];

					H->fill1(10502+this->Flag*1000,"electron tof",evti->e.tof[i],1.,"electron TOF (1st good electron)",200,0.,etof_center + 2.0*etof_width,"TOF [ns]",dir);
					H->fill1(10503+this->Flag*1000,"recoil tof",evti->r.tof[j],1.,"recoil TOF",int(rtof_center/2.0),0.,2.0*rtof_center,"TOF [ns]",dir);

					// find more electrons
					for(int k=i+1;k<evti->e.num_hits;k++) {
						if(fabs(evti->e.time[i]-evti->e.time[k])<etof_width) {
							evto->e.x[Number_Of_Electrons] = evti->e.x[k];
							evto->e.y[Number_Of_Electrons] = evti->e.y[k];
							evto->e.tof[Number_Of_Electrons] = evti->e.tof[k];
							evto->e.time[Number_Of_Electrons] = evti->e.time[k];
							evto->e.method[Number_Of_Electrons] = evti->e.method[k];						
							Number_Of_Electrons++;
						} else {
							evto->e.num_hits = Number_Of_Electrons;
							return true;
						}
					}
					evto->e.num_hits = Number_Of_Electrons;
					return true;
				}
			}
		}
	}
	return false;
}	

//////////////////////////   P I P I C O   ///////////////////////////////////////
// using spectrometer struct
void presorter_class::init_pipico(double etof_center, double etof_width, double mass1_amu, double charge1_au, double mass2_amu, double charge2_au, double pipico_width) 
{
	this->etof_center = etof_center;
	this->etof_width = etof_width;
	this->pipico_width_ns = pipico_width;
	this->mass1_amu = mass1_amu;
	this->charge1_au = charge1_au;
	this->mass2_amu = mass2_amu;
	this->charge2_au = charge2_au;
	this->pipico_width_ns = pipico_width_ns;
	this->acceleration1_mm = spect->ion_side->lengths[0];
	this->acceleration2_mm = spect->ion_side->lengths[1];
	this->acceleration3_mm = spect->ion_side->lengths[2];
	this->efield1_Vcm = spect->ion_side->Efields[0];
	this->efield2_Vcm = spect->ion_side->Efields[1];
	this->efield3_Vcm = spect->ion_side->Efields[2];
}
void presorter_class::init_pipico(double etof_center, double etof_width, double mass1_amu, double charge1_au, double mass2_amu, double charge2_au, double efield1_Vcm, double acceleration1_mm, double pipico_width)
{
	this->etof_center = etof_center;
	this->etof_width = etof_width;
	this->pipico_width_ns = pipico_width;
	this->mass1_amu = mass1_amu;
	this->charge1_au = charge1_au;
	this->mass2_amu = mass2_amu;
	this->charge2_au = charge2_au;
	this->pipico_width_ns = pipico_width_ns;
	this->acceleration1_mm = acceleration1_mm;
	this->acceleration2_mm = 0.0;
	this->acceleration3_mm = 0.0;
	this->efield1_Vcm = efield1_Vcm;
	this->efield2_Vcm = 0.0;
	this->efield3_Vcm = 0.0;
 }
void presorter_class::init_pipico(double etof_center, double etof_width, double mass1_amu, double charge1_au, double mass2_amu, double charge2_au, double efield1_Vcm, double acceleration1_mm, double efield2_Vcm, double acceleration2_mm, double pipico_width)
{
	this->etof_center = etof_center;
	this->etof_width = etof_width;
	this->pipico_width_ns = pipico_width;
	this->mass1_amu = mass1_amu;
	this->charge1_au = charge1_au;
	this->mass2_amu = mass2_amu;
	this->charge2_au = charge2_au;
	this->pipico_width_ns = pipico_width_ns;
	this->acceleration1_mm = acceleration1_mm;
	this->acceleration2_mm = acceleration2_mm;
	this->acceleration3_mm = 0.0;
	this->efield1_Vcm = efield1_Vcm;
	this->efield2_Vcm = efield2_Vcm;
	this->efield3_Vcm = 0.0;
 }
void presorter_class::init_pipico(double etof_center, double etof_width, double mass1_amu, double charge1_au, double mass2_amu, double charge2_au, double efield1_Vcm, double acceleration1_mm, double efield2_Vcm, double acceleration2_mm, double efield3_Vcm, double acceleration3_mm, double pipico_width)
{
	this->etof_center = etof_center;
	this->etof_width = etof_width;
	this->pipico_width_ns = pipico_width;
	this->mass1_amu = mass1_amu;
	this->charge1_au = charge1_au;
	this->mass2_amu = mass2_amu;
	this->charge2_au = charge2_au;
	this->pipico_width_ns = pipico_width_ns;
	this->acceleration1_mm = acceleration1_mm;
	this->acceleration2_mm = acceleration2_mm;
	this->acceleration3_mm = acceleration3_mm;
	this->efield1_Vcm = efield1_Vcm;
	this->efield2_Vcm = efield2_Vcm;
	this->efield3_Vcm = efield3_Vcm;
 }
bool presorter_class::pipico()
{	

	if(evti->e.num_hits<1 || evti->r.num_hits < 2)
		return false;
	
	int Number_Of_Electrons = 1;
	
	this->evto->reaction = this->Flag;
	this->evto->bunchmarker = evti->bunchmarker;

	for (int i=0;i<(evti->e.num_hits);i++) {
		//if(i!=0)
			this->ctof->calc(evti,i);

			H->fill1(10500+this->Flag*1000,"electron tof(no prs)",evti->e.tof[i],1.,"electron TOF",200,0.,etof_center + 2.0*etof_width,"TOF [ns]",dir);
			H->fill2(10502+this->Flag*1000,"pipico big range(no prs, just first hits)",evti->r.tof[0],evti->r.tof[1],1.,"PIPICO spectrum (coarse)",500,0.,50000.,"rec1 TOF [ns]",500,0.,50000.,"rec2 TOF [ns]",dir);
			H->fill2(10503+this->Flag*1000,"pipico small range(no prs, just first hits)",evti->r.tof[0],evti->r.tof[1],1.,"PIPICO spectrum (fine)",800,80.,20000.,"rec1 TOF [ns]",800,80.,20000.,"rec2 TOF [ns]",dir);


		for (int j=0;j<(evti->r.num_hits);j++) {

			for (int k=j+1;k<(evti->r.num_hits);k++) {
				if(	fabs(evti->r.tof[k] - t2_3accel(evti->r.tof[j], this->acceleration1_mm, this->acceleration2_mm, this->acceleration3_mm, this->efield1_Vcm, this->efield2_Vcm, this->efield3_Vcm, this->charge1_au, this->charge2_au, this->mass1_amu, this->mass2_amu)) < this->pipico_width_ns
					|| 
					fabs(evti->r.tof[k] - t2_3accel(evti->r.tof[j], this->acceleration1_mm, this->acceleration2_mm, this->acceleration3_mm, this->efield1_Vcm, this->efield2_Vcm, this->efield3_Vcm, this->charge2_au, this->charge1_au, this->mass2_amu, this->mass1_amu)) < this->pipico_width_ns) {
					if(fabs(evti->e.tof[i]-etof_center)<etof_width)	{
				
						evto->r.num_hits = 2;
						evto->r.x[0] = evti->r.x[j];
						evto->r.y[0] = evti->r.y[j];
						evto->r.tof[0] = evti->r.tof[j];
						evto->r.time[0] = evti->r.time[j];
						evto->r.method[0] = evti->r.method[j];
						
						evto->r.x[1] = evti->r.x[k];
						evto->r.y[1] = evti->r.y[k];
						evto->r.tof[1] = evti->r.tof[k];
						evto->r.time[1] = evti->r.time[k];
						evto->r.method[1] = evti->r.method[k];

						evto->e.x[0] = evti->e.x[i];
						evto->e.y[0] = evti->e.y[i];
						evto->e.tof[0] = evti->e.tof[i];
						evto->e.time[0] = evti->e.time[i];
						evto->e.method[0] = evti->e.method[i];

						H->fill1(10505+this->Flag*1000,"electron tof",evti->e.tof[i],1.,"electron TOF",200,0.,etof_center + 2.0*etof_width,"TOF [ns]",dir);
						H->fill2(10506+this->Flag*1000,"pipico big range",evti->r.tof[j],evti->r.tof[k],1.,"PIPICO spectrum (coarse)",500,0.,50000.,"rec1 TOF [ns]",500,0.,50000.,"rec2 TOF [ns]",dir);
						H->fill2(10507+this->Flag*1000,"pipico small range",evti->r.tof[j],evti->r.tof[k],1.,"PIPICO spectrum (fine)",800,80.,20000.,"rec1 TOF [ns]",800,80.,20000.,"rec2 TOF [ns]",dir);

						// find more electrons
						for(int l=i+1;l<evti->e.num_hits;l++) {
							if(fabs(evti->e.time[i]-evti->e.time[l])<etof_width) {
								evto->e.x[Number_Of_Electrons] = evti->e.x[l];
								evto->e.y[Number_Of_Electrons] = evti->e.y[l];
								evto->e.tof[Number_Of_Electrons] = evti->e.tof[l];
								evto->e.time[Number_Of_Electrons] = evti->e.time[l];
								evto->e.method[Number_Of_Electrons] = evti->e.method[l];						
								Number_Of_Electrons++;
							} else {
								evto->e.num_hits = Number_Of_Electrons;
								return true;
							}
						}
						evto->e.num_hits = Number_Of_Electrons;
						return true;

					}

				}

			}	

		}		
	
	}

	return false;
}

//////////////////////////   P R O J _ R E C   T O F   ///////////////////////////////////////
// electron & recoil TOF, projectile position

void presorter_class::init_proj_rec_tof(double etof_center, double etof_width, double rtof_center, double rtof_width, double px_center, double py_center, double p_radius)
{
	this->etof_center = etof_center;
	this->etof_width = etof_width;
	this->rtof_center = rtof_center;
	this->rtof_width = rtof_width;
	this->px_center = px_center;
	this->py_center = py_center;
	this->p_radius = p_radius;
}

bool presorter_class::proj_rec_tof()
{	
	if(evti->p.num_hits<1 || evti->r.num_hits < 1)
		return false;

	int Number_Of_Electrons = 0;

	this->evto->reaction = this->Flag;
	this->evto->bunchmarker = evti->bunchmarker;

	for (int i=0;i<(evti->p.num_hits);i++) {
		
		this->ctof->calc(evti,i);
		H->fill1(10501+this->Flag*1000,"recoil tof (no prs)",evti->r.tof[0],1.,"recoil TOF",500,rtof_center-rtof_width-10.,rtof_center+rtof_width+10.,"TOF [ns]",dir);
		H->fill2(10502+this->Flag*1000,"projectile position (no prs)",evti->p.x[0],evti->p.x[0],1.,"projectile position",100,px_center-p_radius-5.,px_center+p_radius+5.,"proj_x [mm]",100,py_center-p_radius-5.,py_center+p_radius+5.,"proj_y [mm]",dir);
		
		for (int j=0;j<(evti->r.num_hits);j++) {
			
			if(fabs(evti->r.tof[j]-rtof_center)<rtof_width)	{

				if(sqrt( (evti->p.x[i] - px_center)*(evti->p.x[i] - px_center) + (evti->p.y[i] - py_center)*(evti->p.y[i] - py_center) ) <p_radius) {  // good projectile position

					evto->r.num_hits = 1;
					evto->r.x[0] = evti->r.x[j];
					evto->r.y[0] = evti->r.y[j];
					evto->r.tof[0] = evti->r.tof[j];
					evto->r.time[0] = evti->r.time[j];
					evto->r.method[0] = evti->r.method[j];

					evto->p.num_hits = 1;
					evto->p.x[0] = evti->p.x[i];
					evto->p.y[0] = evti->p.y[i];
					evto->p.tof[0] = evti->p.tof[i];
					evto->p.time[0] = evti->p.time[i];
					evto->p.method[0] = evti->p.method[i];

					H->fill1(10401+this->Flag*1000,"recoil tof",evti->r.tof[j],1.,"recoil TOF",500,rtof_center-rtof_width-10.,rtof_center+rtof_width+10.,"TOF [ns]",dir);
					H->fill2(10402+this->Flag*1000,"projectile position",evti->p.x[0],evti->p.x[0],1.,"projectile position",100,px_center-p_radius-5.,px_center+p_radius+5.,"proj_x [mm]",100,py_center-p_radius-5.,py_center+p_radius+5.,"proj_y [mm]",dir);

					// find electrons
					for(int k=0;k<evti->e.num_hits;k++) {
						H->fill1(10500+this->Flag*1000,"electron tof (no prs)",evti->e.tof[k],1.,"Electron TOF",500,etof_center-etof_width-10.,etof_center+etof_width+10.,"TOF [ns]",dir);

						if(fabs(evti->e.tof[k]-etof_center)<etof_width)	{
							evto->e.x[Number_Of_Electrons] = evti->e.x[k];
							evto->e.y[Number_Of_Electrons] = evti->e.y[k];
							evto->e.tof[Number_Of_Electrons] = evti->e.tof[k];
							evto->e.time[Number_Of_Electrons] = evti->e.time[k];
							evto->e.method[Number_Of_Electrons] = evti->e.method[k];						
							Number_Of_Electrons++;

							H->fill1(10400+this->Flag*1000,"electron tof",evti->e.tof[k],1.,"Electron TOF",500,etof_center-etof_width-10.,etof_center+etof_width+10.,"TOF [ns]",dir);
						}
					}

					evto->e.num_hits = Number_Of_Electrons;
					return true;
				}
			}
		}
	}
	return false;
}

//////////////////////////   P R O J _ P I P I C O   ///////////////////////////////////////
// 2 recoils (PIPICO curve), projectile position (and additional electron TOFs)

void presorter_class::init_proj_pipico(double etof_center, double etof_width, double mass1_amu, double charge1_au, double mass2_amu, double charge2_au, double pipico_width_ns, double px_center, double py_center, double p_radius)
{
	this->etof_center = etof_center;
	this->etof_width = etof_width;
	this->mass1_amu = mass1_amu;
	this->charge1_au = charge1_au;
	this->mass2_amu = mass2_amu;
	this->charge2_au = charge2_au;
	this->pipico_width_ns = pipico_width_ns;
	this->px_center = px_center;
	this->py_center = py_center;
	this->p_radius = p_radius;
	this->shift_ns = shift_ns;
	this->acceleration1_mm = spect->ion_side->lengths[0];
	this->acceleration2_mm = spect->ion_side->lengths[1];
	this->acceleration3_mm = spect->ion_side->lengths[2];
	this->efield1_Vcm = spect->ion_side->Efields[0];
	this->efield2_Vcm = spect->ion_side->Efields[1];
	this->efield3_Vcm = spect->ion_side->Efields[2];
}

void presorter_class::init_proj_pipico(double etof_center, double etof_width, double mass1_amu, double charge1_au, double mass2_amu, double charge2_au, double efield1_Vcm, double acceleration1_mm, double pipico_width_ns, double px_center, double py_center, double p_radius)
{
	this->etof_center = etof_center;
	this->etof_width = etof_width;
	this->mass1_amu = mass1_amu;
	this->charge1_au = charge1_au;
	this->mass2_amu = mass2_amu;
	this->charge2_au = charge2_au;
	this->pipico_width_ns = pipico_width_ns;
	this->px_center = px_center;
	this->py_center = py_center;
	this->p_radius = p_radius;
	this->shift_ns = shift_ns;
	this->acceleration1_mm = acceleration1_mm;
	this->acceleration2_mm = 0.0;
	this->acceleration3_mm = 0.0;
	this->efield1_Vcm = efield1_Vcm;
	this->efield2_Vcm = 0.0;
	this->efield3_Vcm = 0.0;
}
void presorter_class::init_proj_pipico(double etof_center, double etof_width, double mass1_amu, double charge1_au, double mass2_amu, double charge2_au, double efield1_Vcm, double acceleration1_mm, double efield2_Vcm, double acceleration2_mm, double pipico_width_ns, double px_center, double py_center, double p_radius)
{
	this->etof_center = etof_center;
	this->etof_width = etof_width;
	this->mass1_amu = mass1_amu;
	this->charge1_au = charge1_au;
	this->mass2_amu = mass2_amu;
	this->charge2_au = charge2_au;
	this->pipico_width_ns = pipico_width_ns;
	this->px_center = px_center;
	this->py_center = py_center;
	this->p_radius = p_radius;
	this->shift_ns = shift_ns;
	this->acceleration1_mm = acceleration1_mm;
	this->acceleration2_mm = acceleration2_mm;
	this->acceleration3_mm = 0.0;
	this->efield1_Vcm = efield1_Vcm;
	this->efield2_Vcm = efield2_Vcm;
	this->efield3_Vcm = 0.0;
}
void presorter_class::init_proj_pipico(double etof_center, double etof_width, double mass1_amu, double charge1_au, double mass2_amu, double charge2_au, double efield1_Vcm, double acceleration1_mm, double efield2_Vcm, double acceleration2_mm, double efield3_Vcm, double acceleration3_mm, double pipico_width_ns, double px_center, double py_center, double p_radius)
{
	this->etof_center = etof_center;
	this->etof_width = etof_width;
	this->mass1_amu = mass1_amu;
	this->charge1_au = charge1_au;
	this->mass2_amu = mass2_amu;
	this->charge2_au = charge2_au;
	this->pipico_width_ns = pipico_width_ns;
	this->px_center = px_center;
	this->py_center = py_center;
	this->p_radius = p_radius;
	this->shift_ns = shift_ns;
	this->acceleration1_mm = acceleration1_mm;
	this->acceleration2_mm = acceleration2_mm;
	this->acceleration3_mm = acceleration3_mm;
	this->efield1_Vcm = efield1_Vcm;
	this->efield2_Vcm = efield2_Vcm;
	this->efield3_Vcm = efield3_Vcm;
}

bool presorter_class::proj_pipico()
{	
	if(evti->p.num_hits<1 || evti->r.num_hits < 2)
		return false;
	
	int Number_Of_Electrons = 0;
	
	this->evto->reaction = this->Flag;
	this->evto->bunchmarker = evti->bunchmarker;

	H->fill2(10504+this->Flag*1000,"projectile position (no prs)",evti->p.x[0],evti->p.y[0],1.,"projectile position",100,px_center-p_radius-5.,px_center+p_radius+5.,"proj_x [mm]",100,py_center-p_radius-5.,py_center+p_radius+5.,"proj_y [mm]",dir);

	for (int i=0;i<(evti->p.num_hits);i++) {
		
		this->ctof->calc(evti,i);
		H->fill2(10502+this->Flag*1000,"pipico big range(no prs, just first hits)",evti->r.tof[0],evti->r.tof[1],1.,"PIPICO spectrum (coarse)",500,0.,50000.,"rec1 TOF [ns]",500,0.,50000.,"rec2 TOF [ns]",dir);
		H->fill2(10503+this->Flag*1000,"pipico small range(no prs, just first hits)",evti->r.tof[0],evti->r.tof[1],1.,"PIPICO spectrum (fine)",500,-2500.,7500.,"rec1 TOF [ns]",500,-2500.,7500.,"rec2 TOF [ns]",dir);

		for (int j=0;j<(evti->r.num_hits);j++) {

			for (int k=j+1;k<(evti->r.num_hits);k++) {
				if(fabs(evti->r.tof[k] - t2_3accel(evti->r.tof[j], this->acceleration1_mm, this->acceleration2_mm, this->acceleration3_mm, this->efield1_Vcm, this->efield2_Vcm, this->efield3_Vcm, this->charge1_au, this->charge2_au, this->mass1_amu, this->mass2_amu)) < this->pipico_width_ns
					 || fabs(evti->r.tof[k] - t2_3accel(evti->r.tof[j], this->acceleration1_mm, this->acceleration2_mm, this->acceleration3_mm, this->efield1_Vcm, this->efield2_Vcm, this->efield3_Vcm, this->charge2_au, this->charge1_au, this->mass2_amu, this->mass1_amu)) < this->pipico_width_ns) {
					
					if(sqrt( (evti->p.x[i] - px_center)*(evti->p.x[i] - px_center) + (evti->p.y[i] - py_center)*(evti->p.y[i] - py_center) ) <p_radius) {  // good projectile position

						evto->r.num_hits = 2;
						evto->r.x[0] = evti->r.x[j];
						evto->r.y[0] = evti->r.y[j];
						evto->r.tof[0] = evti->r.tof[j];
						evto->r.time[0] = evti->r.time[j];
						evto->r.method[0] = evti->r.method[j];
						
						evto->r.x[1] = evti->r.x[k];
						evto->r.y[1] = evti->r.y[k];
						evto->r.tof[1] = evti->r.tof[k];
						evto->r.time[1] = evti->r.time[k];
						evto->r.method[1] = evti->r.method[k];

						evto->p.num_hits = 1;
						evto->p.x[0] = evti->p.x[i];
						evto->p.y[0] = evti->p.y[i];
						evto->p.tof[0] = evti->p.tof[i];
						evto->p.time[0] = evti->p.time[i];
						evto->p.method[0] = evti->p.method[i];

						H->fill2(10506+this->Flag*1000,"pipico big range",evti->r.tof[j],evti->r.tof[k],1.,"PIPICO spectrum (coarse)",500,0.,50000.,"rec1 TOF [ns]",500,0.,50000.,"rec2 TOF [ns]",dir);
						H->fill2(10507+this->Flag*1000,"pipico small range",evti->r.tof[j],evti->r.tof[k],1.,"PIPICO spectrum (fine)",800,80.,20000.,"rec1 TOF [ns]",800,80.,20000.,"rec2 TOF [ns]",dir);
						H->fill2(10402+this->Flag*1000,"projectile position",evti->p.x[0],evti->p.x[0],1.,"projectile position",100,px_center-p_radius-5.,px_center+p_radius+5.,"proj_x [mm]",100,py_center-p_radius-5.,py_center+p_radius+5.,"proj_y [mm]",dir);

						// find electrons
						for(int k=0;k<evti->e.num_hits;k++) {
							H->fill1(10500+this->Flag*1000,"electron tof (no prs)",evti->e.tof[k],1.,"Electron TOF",500,etof_center-etof_width-10.,etof_center+etof_width+10.,"TOF [ns]",dir);

							if(fabs(evti->e.tof[k]-etof_center)<etof_width)	{
								evto->e.x[Number_Of_Electrons] = evti->e.x[k];
								evto->e.y[Number_Of_Electrons] = evti->e.y[k];
								evto->e.tof[Number_Of_Electrons] = evti->e.tof[k];
								evto->e.time[Number_Of_Electrons] = evti->e.time[k];
								evto->e.method[Number_Of_Electrons] = evti->e.method[k];						
								Number_Of_Electrons++;

								H->fill1(10400+this->Flag*1000,"electron tof",evti->e.tof[k],1.,"Electron TOF",500,etof_center-etof_width-10.,etof_center+etof_width+10.,"TOF [ns]",dir);
							}
						}

						evto->e.num_hits = Number_Of_Electrons;
						return true;
					}
				}
			}	
		}		
	}

	return false;
}