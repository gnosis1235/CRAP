#include "CH_Ion.h"
#include "CH_Basics.h"

#include "Math.h"

ion_class::ion_class()
{
	this->raw->m = 1.0; // all masses are in AMU, default ion is  proton.
	this->raw->q = 1.0;
}

ion_class::~ion_class()
{
	delete raw;
	delete phy;
}
