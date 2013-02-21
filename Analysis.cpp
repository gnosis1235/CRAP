//#include "stdafx.h"
//#include "event_data.h"
#include "Root_file_handler.h"
#include "console.h"
#include "Simple_Hist.h"
#include "config_file_reader.h"
#include "CH_Presorter.h"
#include "CH_Event.h"

#define ANY				0
#define ETOF			1
#define RTOF			2
#define ERTOF			3
#define PIPICO			4
#define PRTOF			5
#define PROJ_PIPICO		6

	void set_branches(CH_event_struct * single_event, double_hits * hits, TTree * inputfileRootTree){

			char ntuple_identifier[500];
			bool Check = 0;

			// find out how many recoils are in Ntuple
			int MaxRec = 0;
			do {
				sprintf(ntuple_identifier,"r%ix",++MaxRec);
			} while(inputfileRootTree->GetBranch(ntuple_identifier));
			
			//cout << "The MaxRec are: " << MaxRec-1 << endl;
			

			// find out how many electrons are in Ntuple
			int MaxElec = 0;
			do {
				sprintf(ntuple_identifier,"e%ix",++MaxElec);
			} while(inputfileRootTree->GetBranch(ntuple_identifier));
			
			//cout << "The MaxElec are: " << MaxElec-1 << endl;
			

			// find out how many projectyles are in Ntuple
			int MaxProj = 0;
			do {
				sprintf(ntuple_identifier,"p%ix",++MaxProj);
			} while(inputfileRootTree->GetBranch(ntuple_identifier));
			
			//cout << "The MaxProj are: " << MaxProj-1 << endl;
			
			//prepare the data sturturce 
			for(int i=0; i<MaxRec; i++){
				single_event->r.method.push_back(-500); 
				single_event->r.time.push_back(-500); 
				single_event->r.tof.push_back(-500); 
				single_event->r.x.push_back(-500); 
				single_event->r.y.push_back(-500); 
			}
			for(int i=0; i<MaxElec; i++){
				single_event->e.method.push_back(-500); 
				single_event->e.time.push_back(-500); 
				single_event->e.tof.push_back(-500); 
				single_event->e.x.push_back(-500); 
				single_event->e.y.push_back(-500); 
			}
			for(int i=0; i<MaxProj; i++){
				single_event->p.method.push_back(-500); 
				single_event->p.time.push_back(-500); 
				single_event->p.tof.push_back(-500); 
				single_event->p.x.push_back(-500); 
				single_event->p.y.push_back(-500); 
			}



			if(inputfileRootTree->GetBranch("reaction"))
				inputfileRootTree->SetBranchAddress("reaction",&single_event->reaction);
			
			if(inputfileRootTree->GetBranch("ehit"))
				inputfileRootTree->SetBranchAddress("ehit",&hits->ehit);
			
			if(inputfileRootTree->GetBranch("rhit"))
				inputfileRootTree->SetBranchAddress("rhit",&hits->rhit);
			
			if(inputfileRootTree->GetBranch("phit"))
				inputfileRootTree->SetBranchAddress("phit",&hits->phit);
			
			if(inputfileRootTree->GetBranch("bunchmarker"))
				inputfileRootTree->SetBranchAddress("bunchmarker",&single_event->bunchmarker);
			


			for(int i=0;i<MaxRec-1;i++) {
				sprintf(ntuple_identifier,"r%ix",i+1);
				if(inputfileRootTree->GetBranch(ntuple_identifier))
					inputfileRootTree->SetBranchAddress(ntuple_identifier,&single_event->r.x[i]);
				sprintf(ntuple_identifier,"r%iy",i+1);
				if(inputfileRootTree->GetBranch(ntuple_identifier))
					inputfileRootTree->SetBranchAddress(ntuple_identifier,&single_event->r.y[i]);
				sprintf(ntuple_identifier,"r%imcp",i+1);
				if(inputfileRootTree->GetBranch(ntuple_identifier))
					inputfileRootTree->SetBranchAddress(ntuple_identifier,&single_event->r.time[i]);
				sprintf(ntuple_identifier,"r%itof",i+1);
				if(inputfileRootTree->GetBranch(ntuple_identifier))
					inputfileRootTree->SetBranchAddress(ntuple_identifier,&single_event->r.tof[i]);
				//sprintf(ntuple_identifier,"r%iflag",i+1);
				//if(inputfileRootTree->GetBranch(ntuple_identifier))
				//	inputfileRootTree->SetBranchAddress(ntuple_identifier,&single_event->rflag[i]);	
			}




			for(int i=0;i<MaxElec-1;i++) {
				sprintf(ntuple_identifier,"e%ix",i+1);
				if(inputfileRootTree->GetBranch(ntuple_identifier))
					inputfileRootTree->SetBranchAddress(ntuple_identifier,&single_event->e.x[i]);

				sprintf(ntuple_identifier,"e%iy",i+1);
				if(inputfileRootTree->GetBranch(ntuple_identifier))
					inputfileRootTree->SetBranchAddress(ntuple_identifier,&single_event->e.y[i]);

				sprintf(ntuple_identifier,"e%imcp",i+1);
				if(inputfileRootTree->GetBranch(ntuple_identifier))
					inputfileRootTree->SetBranchAddress(ntuple_identifier,&single_event->e.time[i]);

				sprintf(ntuple_identifier,"e%itof",i+1);
				if(inputfileRootTree->GetBranch(ntuple_identifier))
					inputfileRootTree->SetBranchAddress(ntuple_identifier,&single_event->e.tof[i]);

				//sprintf(ntuple_identifier,"e%iflag",i+1);
				//if(inputfileRootTree->GetBranch(ntuple_identifier))
				//	inputfileRootTree->SetBranchAddress(ntuple_identifier,&single_event->eflag[i]);		
			}




			for(int i=0;i<MaxProj-1;i++) {
				sprintf(ntuple_identifier,"p%ix",i+1);
				if(inputfileRootTree->GetBranch(ntuple_identifier))
					inputfileRootTree->SetBranchAddress(ntuple_identifier,&single_event->p.x[i]);

				sprintf(ntuple_identifier,"p%iy",i+1);
				if(inputfileRootTree->GetBranch(ntuple_identifier))						 
					inputfileRootTree->SetBranchAddress(ntuple_identifier,&single_event->p.y[i]);
				
				sprintf(ntuple_identifier,"p%imcp",i+1);								 
				if(inputfileRootTree->GetBranch(ntuple_identifier))
					inputfileRootTree->SetBranchAddress(ntuple_identifier,&single_event->p.time[i]);
																						 
				sprintf(ntuple_identifier,"p%itof",i+1);
				if(inputfileRootTree->GetBranch(ntuple_identifier))
					inputfileRootTree->SetBranchAddress(ntuple_identifier,&single_event->p.tof[i]);
				
				//sprintf(ntuple_identifier,"p%iflag",i+1);
				//if(inputfileRootTree->GetBranch(ntuple_identifier))
				//	inputfileRootTree->SetBranchAddress(ntuple_identifier,&single_event->pflag[i]);		
			}
}


void analysis(Root_file_handler * input_root_file, Root_file_handler * output_root_file, histo_handler * Hist, config_file_reader * config_file, __int64 num_events){
	
	unordered_map<int, double> parameter = config_file->parameter;
	vector<command> config_commands = config_file->config_commands;

	int NRmax = (int)(parameter[61]);
	int NEmax = (int)(parameter[62]);
	int NPmax = (int)(parameter[60]);

	
	CH_event_struct * event = new CH_event_struct();
	spectrometer_class * spect = new spectrometer_class();
	tof_calc_class * ctof;

	//create spectrometer sturcts
	for(int i=0;i<(int)config_commands.size();++i){
		if(config_commands[i].spectrometer){

			if(config_commands[i].command_str =="ELEC_ARM"){
				if( 2*config_commands[i].arg[0] == (int)config_commands[i].arg.size()-1){
					std::vector<double> lenghts;
					std::vector<double> E_val;
					for(int j=1; j <= 2*config_commands[i].arg[0]; j=j+2){
						lenghts.push_back(config_commands[i].arg[j]);
						E_val.push_back(config_commands[i].arg[j+1]);
					}
					spect->set_electron_arm(config_commands[i].arg[0] , lenghts.data(), E_val.data());
				}else{
					cout<< "Error wrong number of arguments: ELEC_ARM."<<endl;
				}
			}
			
			if(config_commands[i].command_str =="ION_ARM"){
				if( 2*config_commands[i].arg[0] == (int)config_commands[i].arg.size()-1){
					std::vector<double> lenghts;
					std::vector<double> E_val;
					for(int j=1; j <= 2*config_commands[i].arg[0]; j=j+2){
						lenghts.push_back(config_commands[i].arg[j]);
						E_val.push_back(config_commands[i].arg[j+1]);
					}
					spect->set_ion_arm(config_commands[i].arg[0] , lenghts.data(), E_val.data());
				}else{
					cout<< "Error wrong number of arguments: ION_ARM."<<endl;
				}
			}

			if(config_commands[i].command_str =="ION_ARM_LINEAR"){
				if((int)config_commands[i].arg.size()==1  ){
					spect->set_ion_arm_lin(config_commands[i].arg[0]);
				}else{
					cout<< "Error wrong number of arguments: ION_ARM_LINEAR."<<endl;
				}
			}

			if(config_commands[i].command_str =="B_ns"){
				if((int)config_commands[i].arg.size()==2 ){
					spect->set_Bfield(config_commands[i].arg[0], config_commands[i].arg[0], true);
				}else{
					cout<< "Error wrong number of arguments: B_ns."<<endl;
				}
			}
			
			if(config_commands[i].command_str =="B_Gauss"){
				if((int)config_commands[i].arg.size()==2 ){
					spect->set_Bfield(config_commands[i].arg[0], config_commands[i].arg[0], false);
				}else{
					cout<< "Error wrong number of arguments: B_Gauss."<<endl;
				}
			}
		}
	}
	//create ctof
	for(int i=0;i<(int)config_commands.size();++i){
		if(config_commands[i].tof){

			//printf("create ctof:\n");
			//for(int j =0; j<(int)config_commands[i].arg.size(); ++j)
			//	printf("config_commands[i].arg[%d]=%f\n",j , config_commands[i].arg[j]);

			if(config_commands[i].command_str =="BM"){
				if((int)config_commands[i].arg.size()==1 ){
					ctof = new tof_calc_class(0, config_commands[i].arg[0]);
				}
				else if((int)config_commands[i].arg.size()==2 ){
					ctof = new tof_calc_class(0, config_commands[i].arg[0], config_commands[i].arg[1]);
				}
				else if((int)config_commands[i].arg.size()==3 ){
					ctof = new tof_calc_class(0, config_commands[i].arg[0], config_commands[i].arg[1]);
					ctof->set_mod_shift_e(config_commands[i].arg[2]);
				}
				else{
					cout<< "Error wrong number of arguments: get_tof_from"<<endl;
				}
				//printf("ctof: BMspacing=%f, BM=%d \n", ctof->GetBMspacing(), ctof->usesBM() );
			}
			if(config_commands[i].command_str =="PROJ"){
				if((int)config_commands[i].arg.size()==1 ){
					ctof = new tof_calc_class(1);
				}
				else if((int)config_commands[i].arg.size()==2 ){
					ctof = new tof_calc_class(1, config_commands[i].arg[0]);
				}
				else{
					cout<< "Error wrong number of arguments: get_tof_from"<<endl;
				}
			}
			if(config_commands[i].command_str =="ELEC"){
				if((int)config_commands[i].arg.size()==1 ){
					ctof = new tof_calc_class(2);
				}
				else{
					cout<< "Error wrong number of arguments: get_tof_from"<<endl;
				}
			}
		}
	}
	//create new presorter
	int current_pre = 0;
	std::vector<presorter_class*> presorter_list;
	for(int i=0;i<(int)config_commands.size();++i){ 
		if(config_commands[i].presorter){

			if(config_commands[i].command_str=="ANY"){
				presorter_list.push_back( new presorter_class(ANY, (int)config_commands[i].arg[0], event, Hist, spect, ctof) );
				if((int)config_commands[i].arg.size() != 1)
					printf("Presorter Error: %s wrong number of arguments", config_commands[i].command_str.c_str());
				current_pre++;
			}

			if(config_commands[i].command_str=="ETOF"){
				presorter_list.push_back( new presorter_class(ETOF, (int)config_commands[i].arg[0], event, Hist, spect, ctof) );
				if((int)config_commands[i].arg.size() == 2+1)
					presorter_list[current_pre]->init_e_tof(config_commands[i].arg[1], config_commands[i].arg[2]);
				else
					printf("Presorter Error: %s wrong number of arguments", config_commands[i].command_str.c_str());
				current_pre++;
			}
			if(config_commands[i].command_str=="RTOF"){
				presorter_list.push_back( new presorter_class(RTOF, (int)config_commands[i].arg[0], event, Hist, spect, ctof) );
				if((int)config_commands[i].arg.size() == 2+1)
					presorter_list[current_pre]->init_r_tof(config_commands[i].arg[1], config_commands[i].arg[2]);
				else
					printf("Presorter Error: %s wrong number of arguments", config_commands[i].command_str.c_str());
				current_pre++;
			}			
			if(config_commands[i].command_str=="ERTOF"){
				presorter_list.push_back( new presorter_class(ERTOF, (int)config_commands[i].arg[0], event, Hist, spect, ctof) );
				if((int)config_commands[i].arg.size() == 4+1)
					presorter_list[current_pre]->init_er_tof(config_commands[i].arg[1], config_commands[i].arg[2], config_commands[i].arg[3], config_commands[i].arg[4]);
				else
					printf("Presorter Error: %s wrong number of arguments", config_commands[i].command_str.c_str());
				current_pre++;
			}
			if(config_commands[i].command_str=="PIPICO"){
				presorter_list.push_back( new presorter_class(PIPICO, (int)config_commands[i].arg[0], event, Hist, spect, ctof) );
				//for(int j =1; j<(int)config_commands[i].arg.size(); ++j)
					//printf("config_commands[i].arg[%d]=%f\n",j , config_commands[i].arg[j]);
				if((int)config_commands[i].arg.size() == 7+1)
					presorter_list[current_pre]->init_pipico(config_commands[i].arg[1], config_commands[i].arg[2], config_commands[i].arg[3], config_commands[i].arg[4], config_commands[i].arg[5], config_commands[i].arg[6],config_commands[i].arg[7]);
				else if((int)config_commands[i].arg.size() == 9+1)
					presorter_list[current_pre]->init_pipico(config_commands[i].arg[1], config_commands[i].arg[2], config_commands[i].arg[3], config_commands[i].arg[4], config_commands[i].arg[5], config_commands[i].arg[6],config_commands[i].arg[7],config_commands[i].arg[8],config_commands[i].arg[9]);
				else
					printf("Presorter Error: %s wrong number of arguments", config_commands[i].command_str.c_str());
				current_pre++;
			}
			
			if(config_commands[i].command_str=="PRTOF"){
				presorter_list.push_back( new presorter_class(PRTOF, (int)config_commands[i].arg[0], event, Hist, spect, ctof) );
				if((int)config_commands[i].arg.size() == 7+1)
					presorter_list[current_pre]->init_proj_rec_tof(config_commands[i].arg[1], config_commands[i].arg[2], config_commands[i].arg[3], config_commands[i].arg[4], config_commands[i].arg[5], config_commands[i].arg[6],config_commands[i].arg[7]);
				else
					printf("Presorter Error: %s wrong number of arguments", config_commands[i].command_str.c_str());
				current_pre++;
			}
			if(config_commands[i].command_str=="PROJ_PIPICO"){
				presorter_list.push_back( new presorter_class(PROJ_PIPICO, (int)config_commands[i].arg[0], event, Hist, spect, ctof) );
				if((int)config_commands[i].arg.size() == 7+1)
					presorter_list[current_pre]->init_proj_pipico(config_commands[i].arg[1], config_commands[i].arg[2], config_commands[i].arg[3], config_commands[i].arg[4], config_commands[i].arg[5], config_commands[i].arg[6],config_commands[i].arg[7],config_commands[i].arg[8],config_commands[i].arg[9],config_commands[i].arg[10]);
				else
					printf("Presorter Error: %s wrong number of arguments", config_commands[i].command_str.c_str());
				current_pre++;
			}
		}
	}
	//
	//printf("\npresorter=%d\n",(int)presorter_list.size());


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	bool writeNTuple = false;
	char * sub_name = "Basic";
	int num = 0;
	for(__int64 idx=0;idx<num_events;idx++){
		////////////////////////////////////

		// this should always be first
		input_root_file->get_next_event(event);	
		
		int NRec  = (int)event->r.x.size();
		int NElec = (int)event->e.x.size();
		int NPro  = (int)event->p.x.size();

		double pdet_size	= 5. + parameter[122];				// size of projectile detector (for displaying spectra properly)
		double rdet_size	= 5. + parameter[222];				// size of recoil detector (for displaying spectra properly)
		double edet_size	= 5. + parameter[322];	

		char ntuple_identifier[500];
		double NTupleData[25];
		//////////////////////////////////
		


		for(int i=0; i<1; ++i){

			Hist->fill2(1,"pipico1 spectrum ",event->r.tof[0],event->r.tof[1],1.,"PIPICO spectrum " ,750,0.,6000.,"rec1 TOF [ns]",750,0.,6000.,"rec2 TOF [ns]",sub_name);
			Hist->fill1(2," tof hit 1 ",event->r.tof[0],1.,"r1tof ",500,0.,5000.,"rec1 TOF [ns]",sub_name);
			Hist->fill1(3," tof hit 2 ",event->r.tof[1],1.,"r2tof ",500,0.,5000.,"rec2 TOF [ns]",sub_name);
			Hist->fill1(4,"e1_tof",event->e.tof[0],1.,"e1_tof",500,-10.,350.,"e1tof [ns]",sub_name);
			Hist->fill1(5,"e2_tof",event->e.tof[1],1.,"e2_tof",500,-10.,350.,"e1tof [ns]",sub_name);
			Hist->fill2(6,"e1 X fish",event->e.tof[0],event->e.x[0],1.,"e1 X fish",500,-10.,100.,"e1tof [ns]",500,-50.,50.,"e1z [mm]",sub_name);
			Hist->fill2(7,"e1 Y fish",event->e.tof[0],event->e.y[0],1.,"e1 Y fish",500,-10.,100.,"e1tof [ns]",500,-50.,50.,"e1y [mm]",sub_name);
			Hist->fill2(8,"e2 X fish",event->e.tof[1],event->e.x[1],1.,"e2 X fish",500,-10.,100.,"e2tof [ns]",500,-50.,50.,"e2z [mm]",sub_name);
			Hist->fill2(9,"e2 Y fish",event->e.tof[1],event->e.y[1],1.,"e2 Y fish",500,-10.,100.,"e2tof [ns]",500,-50.,50.,"e2y [mm]",sub_name);
			Hist->fill2(11,"e1 Wiggles",event->e.tof[0],sqrt(event->e.x[0]*event->e.x[0] + event->e.y[0]*event->e.y[0]),1.,"e1 Wiggles",500,-10.,150.,"e1tof [ns]",500,0.,50.,"Radius [mm]",sub_name);
			Hist->fill2(12,"e2 Wiggles",event->e.tof[1],sqrt(event->e.x[1]*event->e.x[1] + event->e.y[1]*event->e.y[1]),1.,"e2 Wiggles",500,-10.,150.,"e2tof [ns]",500,0.,50.,"Radius [mm]",sub_name);
			Hist->fill2(13,"r1x vs r1y",event->r.x[0],event->r.y[0],1.,"Recoil Position",500,-75.,75.,"r1z [mm]",500,-75.,75.,"r1y [mm]",sub_name);
			Hist->fill2(14,"r2x vs r2y",event->r.x[1],event->r.y[1],1.,"Recoil Position",500,-75.,75.,"r2z [mm]",500,-75.,75.,"r2y [mm]",sub_name);
			Hist->fill2(15,"e1z vs e1y",event->e.x[0],event->e.y[0],1.,"Recoil Position",500,-50.,50.,"e1z [mm]",500,-50.,50.,"e1y [mm]",sub_name);
			Hist->fill2(16,"e2x vs e2y",event->e.x[1],event->e.y[1],1.,"Recoil Position",500,-75.,75.,"e2z [mm]",500,-75.,75.,"e2y [mm]",sub_name);
			Hist->fill1(17," r1y " ,event->r.y[0],1.,"r1y ",500,-70.,70.,"rec1 y [mm]",sub_name);
			Hist->fill1(18," r1x " ,event->r.x[0],1.,"r1z ",500,-70.,70.,"rec1 z [mm]",sub_name);
			Hist->fill1(19,"reaction",event->reaction,1.,"Reaction Flag",400,-1.,15.,"Reaction Number",sub_name);

			//gated to be in the center of y (low y momentum)
			if( fabs(event->e.y[0]) <= 5. ){
				Hist->fill2(20,"e1 X fish ygated",event->e.tof[0],event->e.x[0],1.,"e1 X fish ygated",150,30.,85.,"e1tof [ns]",150,-50.,50.,"e1z [mm]",sub_name);
			}
			//gated to be in the center of z (low z momentum)
			if( fabs(event->e.x[0]) <= 5. ){
				Hist->fill2(21,"e1 Y fish zgated",event->e.tof[0],event->e.y[0],1.,"e1 Y fish zgated",150,30.,85.,"e1tof [ns]",150,-50.,50.,"e1y [mm]",sub_name);
			}

			Hist->fill2(22,"r1 X fish",event->r.tof[0],event->r.x[0],1.,"r1 X fish",500,-10.,8000.,"r1tof [ns]",500,-70.,70.,"r1z [mm]",sub_name);
			Hist->fill2(23,"r1 Y fish",event->r.tof[0],event->r.y[0],1.,"r1 Y fish",500,-10.,8000.,"r1tof [ns]",500,-70.,70.,"r1y [mm]",sub_name);
		}



/*
		for(int i=999; i<1000; ++i){
			Hist->fill1(101,"ehit_ntuple0",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
			Hist->fill1(102,"ehit_ntuple1",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
			Hist->fill1(103,"ehit_ntuple2",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
			Hist->fill1(104,"ehit_ntuple3",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
			Hist->fill1(105,"ehit_ntuple4",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
			Hist->fill1(106,"ehit_ntuple5",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
			Hist->fill1(107,"ehit_ntuple6",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
			Hist->fill1(108,"ehit_ntuple7",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
			Hist->fill1(109,"ehit_ntuple8",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
			Hist->fill1(110,"ehit_ntuple9",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
			Hist->fill2(111,"elec1xy_ntuple0",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
			Hist->fill2(112,"elec1xy_ntuple1",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
			Hist->fill2(113,"elec1xy_ntuple2",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
			Hist->fill2(114,"elec1xy_ntuple3",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
			Hist->fill2(115,"elec1xy_ntuple4",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
			Hist->fill2(116,"elec1xy_ntuple5",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
			Hist->fill2(117,"elec1xy_ntuple6",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
			Hist->fill2(118,"elec1xy_ntuple7",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
			Hist->fill2(119,"elec1xy_ntuple8",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
			Hist->fill2(120,"elec1xy_ntuple9",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");


			Hist->fill1(201,"ehit_ntuple10",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
			Hist->fill1(202,"ehit_ntuple11",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
			Hist->fill1(203,"ehit_ntuple12",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
			Hist->fill1(204,"ehit_ntuple13",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
			Hist->fill1(205,"ehit_ntuple14",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
			Hist->fill1(206,"ehit_ntuple15",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
			Hist->fill1(207,"ehit_ntuple16",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
			Hist->fill1(208,"ehit_ntuple17",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
			Hist->fill1(209,"ehit_ntuple18",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
			Hist->fill1(210,"ehit_ntuple19",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
			//Hist->fill2(211,"elec1xy_ntuple10",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
			//Hist->fill2(212,"elec1xy_ntuple11",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
			//Hist->fill2(213,"elec1xy_ntuple12",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
			//Hist->fill2(214,"elec1xy_ntuple13",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
			//Hist->fill2(215,"elec1xy_ntuple14",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
			//Hist->fill2(216,"elec1xy_ntuple15",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
			//Hist->fill2(217,"elec1xy_ntuple16",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
			//Hist->fill2(218,"elec1xy_ntuple17",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
			//Hist->fill2(219,"elec1xy_ntuple18",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
			//Hist->fill2(220,"elec1xy_ntuple19",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");

			Hist->fill1(301,"ehit_ntuple20",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
			Hist->fill1(302,"ehit_ntuple21",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
			Hist->fill1(303,"ehit_ntuple22",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
			Hist->fill1(304,"ehit_ntuple23",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
			Hist->fill1(305,"ehit_ntuple24",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
			Hist->fill1(306,"ehit_ntuple25",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
			Hist->fill1(307,"ehit_ntuple26",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
			Hist->fill1(308,"ehit_ntuple27",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
			Hist->fill1(309,"ehit_ntuple28",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
			Hist->fill1(310,"ehit_ntuple29",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
			//Hist->fill2(311,"elec1xy_ntuple20",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
			//Hist->fill2(312,"elec1xy_ntuple21",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
			//Hist->fill2(313,"elec1xy_ntuple22",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
			//Hist->fill2(314,"elec1xy_ntuple23",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
			//Hist->fill2(315,"elec1xy_ntuple24",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
			//Hist->fill2(316,"elec1xy_ntuple25",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
			//Hist->fill2(317,"elec1xy_ntuple26",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
			//Hist->fill2(318,"elec1xy_ntuple27",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
			//Hist->fill2(319,"elec1xy_ntuple28",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
			//Hist->fill2(320,"elec1xy_ntuple29",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");


			Hist->fill1(401,"ehit_ntuple30",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
			Hist->fill1(402,"ehit_ntuple31",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
			Hist->fill1(403,"ehit_ntuple32",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
			Hist->fill1(404,"ehit_ntuple33",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
			Hist->fill1(405,"ehit_ntuple34",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
			Hist->fill1(406,"ehit_ntuple35",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
			Hist->fill1(407,"ehit_ntuple36",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
			Hist->fill1(408,"ehit_ntuple37",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
			Hist->fill1(409,"ehit_ntuple38",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
			Hist->fill1(410,"ehit_ntuple39",event->ehit,1.,"Electron hits (ntuple)",43,-1.25,20.25,"ehit","ntuple");
			//Hist->fill2(411,"elec1xy_ntuple30",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
			//Hist->fill2(412,"elec1xy_ntuple31",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
			//Hist->fill2(413,"elec1xy_ntuple32",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
			//Hist->fill2(414,"elec1xy_ntuple33",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
			//Hist->fill2(415,"elec1xy_ntuple34",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
			//Hist->fill2(416,"elec1xy_ntuple35",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
			//Hist->fill2(417,"elec1xy_ntuple36",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
			//Hist->fill2(418,"elec1xy_ntuple37",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
			//Hist->fill2(419,"elec1xy_ntuple38",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");
			//Hist->fill2(420,"elec1xy_ntuple39",event->ex[0],event->ey[0],1.,"Electron position #1",400,-50,50,"e1x [mm]",400,-50,50,"e1y [mm]","ntuple");

		}

*/


//---------------------------------------------------------------------------------------------------------------------------
//	                            P R E S O R T E R                                      
//---------------------------------------------------------------------------------------------------------------------------

	double bunchmarkertime = 0.0;

	bool dublicate = true;

	bool found = false;
	int prs;

	// execute all presorters
	for(prs=0; prs < (int)presorter_list.size() ;prs++)
	{

		found = presorter_list[prs]->Run();
				
		if(found)
		{	
			// a presorter found a valid combination..

			NRec = presorter_list[prs]->evto->r.num_hits;
			if(NRec > NRmax)
				NRec = NRmax;
			
			NElec = presorter_list[prs]->evto->e.num_hits;
			if(NElec > NEmax)
				NElec = NEmax;

			NPro = presorter_list[prs]->evto->p.num_hits;
			if(NPro > NPmax)
				NPro = NPmax;

			writeNTuple = true;

			// we need to recalculate the tofs as the reference particle might have changed!
			//Ueber->CTof->calc(presorter_list[prs]->evto);

			NTupleData[0] = double(presorter_list[prs]->Get_flag());
			NTupleData[1] = double(NElec);
			NTupleData[2] = double(NRec);
			NTupleData[3] = double(NPro);
			NTupleData[4] = presorter_list[prs]->evto->bunchmarker;
			
			int absind = 5;

			for(int i=0;i<NRec;i++)
			{	// begin for: write all Recoils to NTuple
				NTupleData[absind+i*5] = double(presorter_list[prs]->evto->r.x[i]);
				NTupleData[absind+i*5+1] = double(presorter_list[prs]->evto->r.y[i]);
				NTupleData[absind+i*5+2] = double(presorter_list[prs]->evto->r.time[i]);
				NTupleData[absind+i*5+3] = double(presorter_list[prs]->evto->r.tof[i]);
				NTupleData[absind+i*5+4] = double(presorter_list[prs]->evto->r.method[i]);
			}	// end for: write all Recoils to NTuple

			absind += NRmax*5;

			for(int i=0;i<NElec;i++)
			{	// begin for: write all Electrons to NTuple
				NTupleData[absind+i*5] = double(presorter_list[prs]->evto->e.x[i]);
				NTupleData[absind+i*5+1] = double(presorter_list[prs]->evto->e.y[i]);
				NTupleData[absind+i*5+2] = double(presorter_list[prs]->evto->e.time[i]);
				NTupleData[absind+i*5+3] = double(presorter_list[prs]->evto->e.tof[i]);
				NTupleData[absind+i*5+4] = double(presorter_list[prs]->evto->e.method[i]);
			}	// end for: write all Electrons to NTuple

			absind += NEmax*5;

			for(int i=0;i<NPro;i++)
			{	// begin for: write all Projectiles to NTuple
				NTupleData[absind+i*5] = double(presorter_list[prs]->evto->p.x[i]);
				NTupleData[absind+i*5+1] = double(presorter_list[prs]->evto->p.y[i]);
				NTupleData[absind+i*5+2] = double(presorter_list[prs]->evto->p.time[i]);
				NTupleData[absind+i*5+3] = double(presorter_list[prs]->evto->p.tof[i]);
				NTupleData[absind+i*5+4] = double(presorter_list[prs]->evto->p.method[i]);
			}	// end for: write all Projectiles to NTuple

			//////////////////////////////////////////////////////////////////////////
			//	write data to ntuple
			//////////////////////////////////////////////////////////////////////////
			char tmp[100];

			sprintf(ntuple_identifier,"reaction:ehit:rhit:phit:bunchmarker");

			for(int i=1;i<=NRmax;i++)
			{
				sprintf(tmp,":r%dx:r%dy:r%dmcp:r%dtof:r%dflag",i,i,i,i,i);
				strcat(ntuple_identifier,tmp);
			}
			for(int i=1;i<=NEmax;i++)
			{
				sprintf(tmp,":e%dx:e%dy:e%dmcp:e%dtof:e%dflag",i,i,i,i,i);
				strcat(ntuple_identifier,tmp);
			}
			for(int i=1;i<=NPmax;i++)
			{
				sprintf(tmp,":p%dx:p%dy:p%dmcp:p%dtof:p%dflag",i,i,i,i,i);
				strcat(ntuple_identifier,tmp);
			}

			output_root_file->NTupleD("Data","BESSY2012",ntuple_identifier, 32000, NTupleData);


			// plot histogramms for found hits
			Hist->fill1(200,"reaction_ntuple",presorter_list[prs]->Get_flag(),1.,"Reaction channel flag",203,-1.25,100.25,"reaction_flag","ntuple");
		
			if (NElec>0)
			{	// begin if: at least 1 electron
				Hist->fill2(210,"elec1xy_ntuple",presorter_list[prs]->evto->e.x[0],presorter_list[prs]->evto->e.y[0],1.,"Electron position #1 (ntuple)",400,-1.*edet_size,edet_size,"presorter_list[prs]->evto->e.x[0] [mm]",400,-1.*edet_size,edet_size,"presorter_list[prs]->evto->e.y[0] [mm]","ntuple");
				Hist->fill1(215,"elec1tof_ntuple",presorter_list[prs]->evto->e.tof[0],1.,"Electron TOF #1 (ntuple)",1000,-500.0,500.0,"presorter_list[prs]->evto->e.tof[0] [ns]","ntuple");
				Hist->fill2(216,"elec1wiggle_ntuple",presorter_list[prs]->evto->e.tof[0],(sqrt(presorter_list[prs]->evto->e.x[0]*presorter_list[prs]->evto->e.x[0]+presorter_list[prs]->evto->e.y[0]*presorter_list[prs]->evto->e.y[0])),1.,"Electron wiggle #1 (ntuple)",250,0.,250.,"presorter_list[prs]->evto->e.tof[0] [ns]",200,-1.,edet_size,"elec_radius [mm]","ntuple");
				Hist->fill2(217,"elec1xfish_ntuple",presorter_list[prs]->evto->e.tof[0],presorter_list[prs]->evto->e.x[0],1.,"Electron x-fish #1 (ntuple)",250,0.,250.,"presorter_list[prs]->evto->e.tof[0] [ns]",200,-1.*edet_size,edet_size,"presorter_list[prs]->evto->e.x[0] [mm]","ntuple");
				Hist->fill2(218,"elec1yfish_ntuple",presorter_list[prs]->evto->e.tof[0],presorter_list[prs]->evto->e.y[0],1.,"Electron y-fish #1 (ntuple)",250,0.,250.,"presorter_list[prs]->evto->e.tof[0] [ns]",200,-1.*edet_size,edet_size,"presorter_list[prs]->evto->e.y[0] [mm]","ntuple");
				if ( fabs(presorter_list[prs]->evto->e.x[0])<10. ) Hist->fill2(219,"elec1yfishcut_ntuple",presorter_list[prs]->evto->e.tof[0],presorter_list[prs]->evto->e.y[0],1.,"Electron y-fish (x<10) #1 (ntuple)",250,0.,250.,"presorter_list[prs]->evto->e.tof[0] [ns]",20,-1.*edet_size,edet_size,"presorter_list[prs]->evto->e.y[0] [mm]","ntuple");
			}	// end if: at least 1 electron

			if (NElec>1)
			{	// begin if: at least 2 electrons
				Hist->fill2(220,"elec2xy_ntuple",presorter_list[prs]->evto->e.x[1],presorter_list[prs]->evto->e.y[1],1.,"Electron position #2 (ntuple)",400,-1.*edet_size,edet_size,"presorter_list[prs]->evto->e.x[1] [mm]",400,-1.*edet_size,edet_size,"presorter_list[prs]->evto->e.y[1] [mm]","ntuple");
				Hist->fill1(225,"elec2tof_ntuple",presorter_list[prs]->evto->e.tof[1],1.,"Electron TOF #2 (ntuple)",1000,-500.,500.,"presorter_list[prs]->evto->e.tof[1] [ns]","ntuple");
				Hist->fill2(226,"elec2wiggle_ntuple",presorter_list[prs]->evto->e.tof[1],(sqrt(presorter_list[prs]->evto->e.x[1]*presorter_list[prs]->evto->e.x[1]+presorter_list[prs]->evto->e.y[1]*presorter_list[prs]->evto->e.y[1])),1.,"Electron wiggle #2 (ntuple)",250,0.,250.,"presorter_list[prs]->evto->e.tof[1] [ns]",200,-1.,edet_size,"elec2_radius [mm]","ntuple");
				Hist->fill2(227,"elec2xfish_ntuple",presorter_list[prs]->evto->e.tof[1],presorter_list[prs]->evto->e.x[1],1.,"Electron x-fish #2 (ntuple)",250,0.,250.,"presorter_list[prs]->evto->e.tof[1] [ns]",200,-1.*edet_size,edet_size,"presorter_list[prs]->evto->e.x[1] [mm]","ntuple");
				Hist->fill2(228,"elec2yfish_ntuple",presorter_list[prs]->evto->e.tof[1],presorter_list[prs]->evto->e.y[1],1.,"Electron y-fish #2 (ntuple)",250,0.,250.,"presorter_list[prs]->evto->e.tof[1] [ns]",200,-1.*edet_size,edet_size,"presorter_list[prs]->evto->e.y[1] [mm]","ntuple");
			}	// end if: at least 2 electrons

			if (NRec>0)
			{	// begin if: at least 1 recoil
				Hist->fill2(250,"rec1xy_ntuple",presorter_list[prs]->evto->r.x[0],presorter_list[prs]->evto->r.y[0],1.,"Recoil position #1 (ntuple)",400,-1.*rdet_size,rdet_size,"presorter_list[prs]->evto->r.x[0] [mm]",400,-1.*rdet_size,rdet_size,"presorter_list[prs]->evto->r.y[0] [mm]","ntuple");
				Hist->fill1(255,"rec1tof_ntuple",presorter_list[prs]->evto->r.tof[0],1.,"Recoil TOF #1 (ntuple)",30100,-100.,30000.,"presorter_list[prs]->evto->r.tof[0] [ns]","ntuple");
				Hist->fill2(256,"rec1wiggle_ntuple",presorter_list[prs]->evto->r.tof[0],(sqrt(presorter_list[prs]->evto->r.x[0]*presorter_list[prs]->evto->r.x[0]+presorter_list[prs]->evto->r.y[0]*presorter_list[prs]->evto->r.y[0])),1.,"Recoil wiggle #1 (ntuple)",500,10000.,30000.,"presorter_list[prs]->evto->r.tof[0] [ns]",200,-1.,rdet_size,"rec1_radius [mm]","ntuple");
				Hist->fill2(257,"rec1xfish_ntuple",presorter_list[prs]->evto->r.tof[0],presorter_list[prs]->evto->r.x[0],1.,"Recoil x-fish #1 (ntuple)",5000,100.,30000.,"presorter_list[prs]->evto->r.tof[0] [ns]",200,-1.*rdet_size,rdet_size,"presorter_list[prs]->evto->r.x[0] [mm]","ntuple");
				Hist->fill2(258,"rec1yfish_ntuple",presorter_list[prs]->evto->r.tof[0],presorter_list[prs]->evto->r.y[0],1.,"Recoil y-fish #1 (ntuple)",5000,100.,30000.,"presorter_list[prs]->evto->r.tof[0] [ns]",200,-1.*rdet_size,rdet_size,"presorter_list[prs]->evto->r.y[0] [mm]","ntuple");
				Hist->fill2(259,"rec1tof_flag_ntuple",presorter_list[prs]->evto->r.tof[0],presorter_list[prs]->Get_flag(),1.,"Recoil TOF Reaction channel #1 (ntuple)",100,5000.,30000.,"presorter_list[prs]->evto->r.tof[0] [ns]",405,-1.125,100.125,"reaction_flag","ntuple");
			}	// end if: at least 1 recoil

			if (NRec>1)
			{	// begin if: at least 2 recoils
				Hist->fill2(260,"rec2xy_ntuple",presorter_list[prs]->evto->r.x[1],presorter_list[prs]->evto->r.y[1],1.,"Recoil position #2 (ntuple)",400,-1.*rdet_size,rdet_size,"presorter_list[prs]->evto->r.x[1] [mm]",400,-1.*rdet_size,rdet_size,"presorter_list[prs]->evto->r.y[1] [mm]","ntuple");
				Hist->fill1(265,"rec2tof_ntuple",presorter_list[prs]->evto->r.tof[1],1.,"Recoil TOF #2 (ntuple)",30100,-100.,30000.,"presorter_list[prs]->evto->r.tof[1] [ns]","ntuple");
				Hist->fill2(266,"rec2wiggle_ntuple",presorter_list[prs]->evto->r.tof[1],(sqrt(presorter_list[prs]->evto->r.x[1]*presorter_list[prs]->evto->r.x[1]+presorter_list[prs]->evto->r.y[1]*presorter_list[prs]->evto->r.y[1])),1.,"Recoil wiggle #2 (ntuple)",500,10000.,30000.,"presorter_list[prs]->evto->r.tof[1] [ns]",200,-1.,rdet_size,"rec2_radius [mm]","ntuple");
				Hist->fill2(267,"rec2xfish_ntuple",presorter_list[prs]->evto->r.tof[1],presorter_list[prs]->evto->r.x[1],1.,"Recoil x-fish #2 (ntuple)",5000,100.,30000.,"presorter_list[prs]->evto->r.tof[1] [ns]",200,-1.*rdet_size,rdet_size,"presorter_list[prs]->evto->r.x[1] [mm]","ntuple");
				Hist->fill2(268,"rec2yfish_ntuple",presorter_list[prs]->evto->r.tof[1],presorter_list[prs]->evto->r.y[1],1.,"Recoil y-fish #2 (ntuple)",5000,100.,30000.,"presorter_list[prs]->evto->r.tof[1] [ns]",200,-1.*rdet_size,rdet_size,"presorter_list[prs]->evto->r.y[1] [mm]","ntuple");
				Hist->fill2(270,"pipico_ntuple",presorter_list[prs]->evto->r.tof[0],presorter_list[prs]->evto->r.tof[1],1.,"PIPICO spectrum (ntuple)",500,0.,50000.,"presorter_list[prs]->evto->r.tof[0] [ns]",500,0.,50000.,"presorter_list[prs]->evto->r.tof[1] [ns]","ntuple");
				Hist->fill2(271,"pipico_short_ntuple",presorter_list[prs]->evto->r.tof[0],presorter_list[prs]->evto->r.tof[1],1.,"PIPICO spectrum fine (ntuple)",800,-2500.,7500.,"presorter_list[prs]->evto->r.tof[0] [ns]",800,-2500.,7500.,"presorter_list[prs]->evto->r.tof[1] [ns]","ntuple");
			}	// end if: at least 2 recoils	

			if (NPro>0)
			{	// begin if: at least 1 projectile
				Hist->fill2(290,"proj1xy_ntuple",presorter_list[prs]->evto->p.x[0],presorter_list[prs]->evto->p.y[0],1.,"Projectile position #1 (ntuple)",400,-1.*pdet_size,pdet_size,"presorter_list[prs]->evto->p.x[0] [mm]",400,-1.*pdet_size,pdet_size,"presorter_list[prs]->evto->p.y[0] [mm]","ntuple");
			}	// end if: at least 1 projectile	

			if(dublicate == false)
				break;
		
		}
			
	}
		//writeNTuple = false;
		//
		//if(writeNTuple) {
		//	NTupleData[0]= 0;
		//	NTupleData[1]= 0;
		//	NTupleData[2]=event->reaction;
		//	//NTupleData[3]=event->rx[0];


		//	output_root_file->NTupleD("Data","test_file","cputime",32000,NTupleData);
		//	output_root_file->EventsWrittenCounter();
		//}
		
		if(	input_root_file->stop_reading) idx=num_events+1;  //exit
	}	

}