#ifndef CH_EVENT_ALREADY_INCLUDED
	#define CH_EVENT_ALREADY_INCLUDED

	#include "CH_Basics.h"

	struct CH_event_struct{
	public:	
		double reaction;
		double bunchmarker;
		CH_det_struct r;
		CH_det_struct e;
		CH_det_struct p;
	};

#endif;