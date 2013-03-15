// CRAP.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
//#include <stdio.h>
//#include <string>


//
#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TNtupleD.h"
#include "TSystem.h"
//
//#include "rootstuff.h"
//#include "event_data.h"
#include "Root_file_handler.h"
#include "console.h"
#include "Simple_Hist.h"
//#include "config_file_parser.h"
#include "config_file_reader.h"
#include "CH_Presorter.h"
#include "CH_Event.h"

#include <thread>
#include <mutex>
#include <math.h>
#include <time.h>   
#include <unordered_map> 
#include <tuple>
#include <sys/stat.h> //needed for FileExists
#include <sstream> //for string_to_double

#define black		0
#define blue1		1
#define green1		2	
#define cyan1		3
#define red1		4
#define magenta1	5
#define yellow1		6
#define white1		7
#define gray1		8
#define blue2		9
#define green2		10
#define cyan2		11
#define red2		12
#define magenta2	13
#define yellow2		14
#define white2		15


using namespace std;
bool stop_reading_input_files = false;

void analysis(Root_file_handler * input_root_file, Root_file_handler * output_root_file, histo_handler * Hist, config_file_reader * config_file, __int64 num_events);

//kbhit get keyboard input
#ifndef LINUX

	__int32 my_kbhit()
	{
		if (!_kbhit()) return 0;
		return _getch();
	}

#else

	__int32 my_kbhit(void)
	{
		struct termios term, oterm;
		__int32 fd = 0;
		__int32 c = 0;
		tcgetattr(fd, &oterm);
		memcpy(&term, &oterm, sizeof(term));
		term.c_lflag = term.c_lflag & (!ICANON);
		term.c_cc[VMIN] = 0;
		term.c_cc[VTIME] = 1;
		tcsetattr(fd, TCSANOW, &term);
		c = getchar();
		tcsetattr(fd, TCSANOW, &oterm);
		return ((c != -1) ? c : 0);
	}

#endif



 
/////////////////////////////////////////////////////////////////////////////
bool FileExists(const char * strFilename) {
/////////////////////////////////////////////////////////////////////////////
	#ifdef LINUX

		if ( access(strFilename,F_OK) )
			return(false);
		else if ( access(strFilename,R_OK) ) {
			return(false);
		} else	return(true);

	#else

		FILE * fi;
		fi = fopen(strFilename,"rb");
		if (!fi) return false;
		if (ferror(fi)) {
			clearerr(fi); fclose(fi);	fi = 0;
			return false;
		}
		char dummy_char;
		fread(&dummy_char,1,1,fi);
		if (ferror(fi)) {
			clearerr(fi); fclose(fi);	fi = 0;			
			return false;
		}
		if (fi) {
			fclose(fi);	fi = 0;
		}
		return true;
	#endif
}




 void GUI(Root_file_handler * input_root_file, Root_file_handler * output_root_file ){


	printf("Number of events in file: %.0lf\n", double(input_root_file->get_Total_Events_inputfile()));
	printf("\n0%%           25%%          50%%          75%%        100%%\n");
	printf("|------------|------------|------------|-----------|\n");
	COORD start_COORD= getXY();
	//printf("\n\n\n\n\n\n");
	__int64 total_events = input_root_file->get_Total_Events_inputfile();
	__int64 prev_position=0;
	__int64 current_position=0;
	double rate=0;
	double sum_rate = 0.;
	int count = 0;
	string p_bar;

	double percentage_complete = 0;
	int num_star =0;
	__int32 c = 0;
	//time_t timer1;
	//time_t timer2;

	SYSTEMTIME file_start_time;
	GetSystemTime(&file_start_time);
	SYSTEMTIME timer1;
	GetSystemTime(&timer1);

	
	//SYSTEMTIME timer2;
	//GetSystemTime(&timer2);
	ULARGE_INTEGER temptime1;
	ULARGE_INTEGER temptime2;
	FILETIME Ftimer1;
//	FILETIME Ftimer2;

	SystemTimeToFileTime(&timer1, &Ftimer1);
	temptime2.LowPart = Ftimer1.dwLowDateTime;
	temptime2.HighPart = Ftimer1.dwHighDateTime;
	
	while(input_root_file->get_current_entry_inputfile() < total_events && !input_root_file->stop_reading){
		prev_position = input_root_file->get_current_entry_inputfile();
		temptime1=temptime2;


		std::chrono::milliseconds sleepDuration(250);
		std::this_thread::sleep_for(sleepDuration);


		current_position =input_root_file->get_current_entry_inputfile();

		GetSystemTime(&timer1);
		SystemTimeToFileTime(&timer1, &Ftimer1);
		temptime2.LowPart = Ftimer1.dwLowDateTime;
		temptime2.HighPart = Ftimer1.dwHighDateTime;
		double dif = (temptime2.QuadPart - temptime1.QuadPart) / 10000000.0;

		rate = ((double)(current_position - prev_position)) / dif;
		sum_rate += rate;
		count++;

		gotoXY(start_COORD.X, start_COORD.Y);
		
		//double dif = timer2.wMilliseconds - timer1.wMilliseconds;
		//printf( "\n t1=%f, t2=%f diff: %f \n",temptime1.QuadPart/ 10000000.0, temptime2.QuadPart/ 10000000.0, dif );
		
		percentage_complete = (double)current_position / (double)total_events ;
		num_star = (int)(51. * percentage_complete);
		p_bar = "*";
		for(int i=0; i < num_star; ++i){
			p_bar += "*";
		}

		printf("%s \nrate = %G /s							\n", p_bar.c_str(), rate);
		printf("Average rate = 	%G /s			            \n", sum_rate/((double)count));
		printf("Percentage complete = %2.0f %%              \n", percentage_complete * 100.);
		printf("Estimated time remaining = %2.1f mins		\n", (((double)total_events - (double)current_position ) / (sum_rate/((double)count)))/60. );
		printf("Current event in file: %I64i of %I64i               \n", current_position, total_events);
		printf("Total events written to output file:  %I64i         \n",output_root_file->GetEventsWrittenCounter());

		


		c = my_kbhit();
		if (c) {
			while (my_kbhit());
			if(c=='q') {
				printf("\nq was pressed -> skipping this file. \n"); 
				input_root_file->stop_reading = true;
			//	printf("average rate = 	%G /s			            \n", sum_rate/((double)count));
			}
			if(c=='Q') {
				printf("\nQ was pressed -> skipping all files.\n"); 
				input_root_file->stop_reading = true;
				stop_reading_input_files = true;
			
			}
		}

	}
 }


 



int main(__int32 argc, char* argv[], char* envp[])
{
	SYSTEMTIME start_time;
	GetSystemTime(&start_time);
	ULARGE_INTEGER U_start_time;
	FILETIME F_start_time;
	SystemTimeToFileTime(&start_time, &F_start_time);
	U_start_time.LowPart = F_start_time.dwLowDateTime;
	U_start_time.HighPart = F_start_time.dwHighDateTime;

#ifdef _DEBUG
	Red(true);
	printf("\n***********************\n    SLOW DEBUG VERSION !\n***********************\n");
	Red(false);
#endif
	string tabs = "\t\t";
	White(true);
	cout <<endl;
	settextcolor(red2);
	settextbackground(blue1);
	cout <<tabs <<"     __________  ___    ____	    " << tabs;
	settextbackground(black);
	cout << endl;
	settextbackground(blue1);
	cout <<tabs <<"    / ____/ __ \\/   |  / __ \\   " << tabs;
	settextbackground(black);
	cout << endl;
	settextbackground(blue1);
	cout <<tabs <<"   / /   / /_/ / /| | / /_/ /	" << tabs;
	settextbackground(black);
	cout << endl;
	settextbackground(blue1);
	cout <<tabs <<"  / /___/ _, _/ ___ |/ ____/	    " << tabs;
	settextbackground(black);
	cout << endl;
	settextbackground(blue1);
	cout <<tabs <<"  \\____/_/ |_/_/  |_/_/		    " << tabs;
	settextbackground(black);
	cout << endl;
	settextbackground(blue1);
	cout <<tabs <<"                                 " << tabs;
	settextbackground(black);
	cout << endl;
	settextbackground(blue1);
	settextcolor(white2);
	cout <<tabs <<"(Coltrims Root Analysis Program)" << tabs;
	settextbackground(black);
	cout << endl;
	settextbackground(blue1);
	cout <<tabs <<"          Verison 0.95          " << tabs;
	settextbackground(black);
	cout << endl;
	White(true);
	//printf("Verison 0.85\n");
//	printf("        By Joshua Williams");
	printf("By Joshua Williams");
	printf(", Achim Czasch");
	printf(", Till Jahnke");
	printf(", Markus Schoeffler\n");
	settextcolor(yellow2);
	cout <<tabs <<"       Featuring ColAHelL          " << tabs << endl;
	//printf(", and ROOT\n");
	White(false);
	Red(true);
	cout << endl <<"\t      " <<"Don't panic! Everything will be fine."<<"\t"<<endl;
	//printf("Don't panic! Everything will be fine.\n");
	White(false);


	settextcolor(white1);
	settextbackground(black);

	config_file_reader * config_file;// = new config_file_reader("config.txt");

	//////////////////////////////// read config file //////////////////////////////////////////

	FILE * ascii_file = 0;
	if (argc>1) {
		if (!strcmp(argv[1],"help") || !strcmp(argv[1],"-help")) {
		}
		else{
			ascii_file = fopen(argv[1],"rt");
			if (!ascii_file || !FileExists(argv[1])) {
				printf("Error:Problem with config file.");
			}
			else if (ascii_file) {
				fclose(ascii_file); 
				ascii_file = 0;
			}
				config_file = new config_file_reader(argv[1]);
		}
	}else {
		cout << "\nPlease give a config file.\nExample: \"CRAP config.txt\"" << endl;
		cout << "\nTrying \"config.txt\"" << endl;

		ascii_file = fopen("config.txt","rt");
		if (!ascii_file || !FileExists("config.txt")) {
			printf("Error:Problem with config file.");
		}
		else if (ascii_file) {
			fclose(ascii_file); 
			ascii_file = 0;
		}
			config_file = new config_file_reader("config.txt");
		
	}
	
	//testing: Display the config file commands
	cout << "Commands found in config file"<<endl;
	cout << "--------------------------------------------------------------------------------"<<endl;
	for(int i=0; i<(int)config_file->config_commands.size();++i){
//		if(config_file->config_commands[i].presorter)
//			cout << "new_presorter: ";
//		if(config_file->config_commands[i].spectrometer)
//			cout << "set_spectrometer: ";
//		if(config_file->config_commands[i].tof)
//			cout << "set_Tof: ";
		cout<< config_file->config_commands[i].command_str_main ;
		cout<< " " << config_file->config_commands[i].command_str ;
		for(int j=0; j<(int)config_file->config_commands[i].arg.size(); j++){
			cout<< ", "<<config_file->config_commands[i].arg[j];
		}
		cout << endl;
	}
	cout << endl;

	

	
	///////////////////////////// end read config file /////////////////////////////////////////
	//roughly find how many events per thread
	__int64 num_events_per_thread;
	__int64 extras;

	std::vector<std::thread> threads;

	cout << "--------------------------------------------------------------------------------"<<endl;
	//start output file handlers
	string outputfilename = config_file->outputfilename;
	Root_file_handler * output_root_file = new Root_file_handler(outputfilename, "write");
	if ( output_root_file->IsZombie()){
		cout << "Error opening the output root files"<< endl;
		return 0;
	}



	cout << "--------------------------------------------------------------------------------"<<endl;
	//set the number of theads
	int number_of_threads=(int) config_file->parameter[1000];
	int numCPU;
	if(number_of_threads <= 0){
#ifdef LINUX
	numCPU = sysconf( _SC_NPROCESSORS_ONLN );
#else
	SYSTEM_INFO sysinfo;
	GetSystemInfo( &sysinfo );
	numCPU = sysinfo.dwNumberOfProcessors;
#endif
	 number_of_threads=numCPU;
	}
		
	White(true);
	cout << "Using ";
	Green(true);
	cout << number_of_threads;
	White(true);
	cout << " threads for analysis." << endl;
	White(false);
	 //Start the histogram_handlers (one for each thread)
	std::vector<histo_handler*> Histogram_Handler_vector;
	histo_handler * temp;
	for(int i = 0; i < number_of_threads; ++i){
		temp= new histo_handler();
		Histogram_Handler_vector.push_back(temp);
	}



	vector<Root_file_handler*> input_root_file_vector;
	Root_file_handler * temp_root_file;
	for(vector<string>::iterator it=config_file->inputfilename.begin(); it != config_file->inputfilename.end(); ++it){
		if( !FileExists( it->c_str() ) ){
			cout << "File: " <<  *it << " does not exist. Skipping file." << endl;
			config_file->inputfilename.erase(it);		// erase the none existant filename from the vector
			--it;		//now move the vector back by one, so that you don't skip a filename
		}
	}
	
	//start input file handlers
	int input_file_counter = 0;
	for(vector<string>::iterator it=config_file->inputfilename.begin(); it != config_file->inputfilename.end(); ++it){	

		temp_root_file = new Root_file_handler(*it, "read");
		input_root_file_vector.push_back(temp_root_file);
		if (input_root_file_vector[input_file_counter]->IsZombie() ){
			cout << "Error opening the input root files"<< endl;
			return 0;
		}
		input_file_counter++;
	}
	
	if ((int)config_file->inputfilename.size() > 1 ){
		cout << "Multifile mode. The following files will be processed." << endl;
		for(vector<string>::iterator it=config_file->inputfilename.begin(); it != config_file->inputfilename.end(); ++it){
			cout << "File: " <<  *it << endl;
		}
	}


	//loop over all of the input files
	for(int j=0; j < (int)config_file->inputfilename.size(); ++j){
		if(!stop_reading_input_files){
			
			//roughly find how many events per thread
			num_events_per_thread = input_root_file_vector[j]->get_Total_Events_inputfile()/((__int64)number_of_threads) ; 
			//find the left overs
			extras = input_root_file_vector[j]->get_Total_Events_inputfile() - (num_events_per_thread * number_of_threads);
	
			
			//lanuch threads
			//start thread 0 (this one has the extra event)
			threads.push_back(std::thread(analysis, input_root_file_vector[j], output_root_file, Histogram_Handler_vector[0], config_file, num_events_per_thread + extras));
			//start the rest of the threads 
			for(int i = 1; i < number_of_threads; ++i){
				threads.push_back(std::thread(analysis, input_root_file_vector[j], output_root_file,  Histogram_Handler_vector[i], config_file, num_events_per_thread));
			}
			std::thread so_called_gui(GUI, input_root_file_vector[j], output_root_file);
	
	
			for(auto& thread : threads){
				thread.join();
			}
	
			so_called_gui.join();
			// kill old threads
			for(auto& thread : threads){
				thread.~thread();
			}
			so_called_gui.~thread();

			//remove the analysis threads from the threads vector
			threads.erase(threads.begin(), threads.end());

			cout<< endl ;
			cout << "--------------------------------------------------------------------------------"<<endl;
			input_root_file_vector[j]->close_file();



		}
	}


	//combine the historams form each thread
	// iterate over the Histogram_Handler_vector
	// store the results in Histogram_Handler_vector[0]
	if (number_of_threads > 1){
		printf("Combining histgrams from each thread ...\n");
	
		//threads.push_back(std::thread([&number_of_threads, &Histogram_Handler_vector](){
		//	for(int i=0; i<number_of_threads;++i){
		//		//the 1d histograms
		//		for ( int j=0;j < (int)(Histogram_Handler_vector[i]->H1d_vector.size()); j++ ) {
		//				Histogram_Handler_vector[0]->combine_hist( Histogram_Handler_vector[i]->H1d_vector[j], j);
		//			}
		//	}
		//}));
		//threads.push_back(std::thread([&number_of_threads, &Histogram_Handler_vector](){
		//	for(int i=0; i<number_of_threads;++i){
		//		//the 2d histograms
		//		for ( int j=0;j < (int)(Histogram_Handler_vector[i]->H2d_vector.size()); j++ ) {
		//				Histogram_Handler_vector[0]->combine_hist(Histogram_Handler_vector[i]->H2d_vector[j], j);
		//			}
		//	}
		//}));
		//for(auto& thread : threads){
		//	thread.join();
		//}

		for(int i=0; i<number_of_threads;++i){
			//the 1d histograms
			for ( int j=0;j < (int)(Histogram_Handler_vector[i]->H1d_vector.size()); j++ ) {
				Histogram_Handler_vector[0]->combine_hist( Histogram_Handler_vector[i]->H1d_vector[j], j);
			}
			//the 2d histograms
			for ( int j=0;j < (int)(Histogram_Handler_vector[i]->H2d_vector.size()); j++ ) {

				Histogram_Handler_vector[0]->combine_hist(Histogram_Handler_vector[i]->H2d_vector[j], j);
			}
		}

	}


	printf("Writing histgrams to the root file ...\n");
	//write 1D histograms to the root file
	for ( int j=0;j < (int)(Histogram_Handler_vector[0]->H1d_vector.size()); j++ ) {
		if(Histogram_Handler_vector[0]->H1d_vector[j] != 0)
			output_root_file->add_hist( Histogram_Handler_vector[0]->H1d_vector[j] );
	}

	//write 2D histograms to the root file
	for (	int j=0;j < (int)(Histogram_Handler_vector[0]->H2d_vector.size()); j++ ){
		if(Histogram_Handler_vector[0]->H2d_vector[j] != 0)
			output_root_file->add_hist( Histogram_Handler_vector[0]->H2d_vector[j] );
	}




	output_root_file->write_TNtupleD();
	output_root_file->close_file();

	SYSTEMTIME stop_time;
	GetSystemTime(&stop_time);
	ULARGE_INTEGER U_stop_time;
	FILETIME F_stop_time;
	SystemTimeToFileTime(&stop_time, &F_stop_time);
	U_stop_time.LowPart = F_stop_time.dwLowDateTime;
	U_stop_time.HighPart = F_stop_time.dwHighDateTime;
	double dif = (U_stop_time.QuadPart - U_start_time.QuadPart) / 10000000.0;
	
	printf("\nCRAP execution time: %1.2f mins\n",dif/60.);
	
	return 0;
}


