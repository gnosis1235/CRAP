#ifndef config_file_reader_ALREADY_INCLUDED
	#define config_file_readerR_ALREADY_INCLUDED

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <sstream>

#include <string>
#include <vector>
#include <unordered_map> 

//#include <thread>
//#include <mutex>
using namespace std;




struct command
{
	//bool presorter;
	//bool spectrometer;
	//bool tof;

	string command_str_main;
	string command_str;

	vector<double> arg;
};

class config_file_reader
{
	//std::mutex mutex; //thread locking
public:

	config_file_reader(string filename);
	~config_file_reader();

	unordered_map<int, double> parameter;
	vector<string> inputfilename;
	string outputfilename;

	vector<command> config_commands;
	int num_presorter;
};

#endif