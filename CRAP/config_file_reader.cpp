//#include "stdafx.h"
#include "config_file_reader.h"
#include "Console.h"


config_file_reader::config_file_reader(string filename){
	outputfilename = "output.root";

	ifstream config (filename);

	string line;
	string par = "parameter";
	string comment = "//";
	string double_forward_slash = comment;
	string semi = ";";
	string read_root = "readROOTfile";
	string write_root = "set_root_output_file_name";
	string back_slash; // = "\\"; 
	back_slash.push_back(92);
	string double_quotes;
	double_quotes.push_back(34);
	string dot_root = ".root";
	string space = " ";
	string tab = "\t";
	string open_comment =  "/*";
	string close_comment = "*/";

	size_t found_par;
	size_t found_comment;
	size_t found_semi;
	size_t found_rroot;
	size_t found_wroot;
	size_t found_bslash;
	size_t found_dqoutes;
	size_t found_dotroot;
	size_t found_pound;
	size_t found_space;
	size_t found_tab;
	size_t found_Open_comment;
	size_t found_Close_comment;

	bool multi_line_comment = false;
	this->num_presorter=0;

	
	
	int par_num = -1000;
	double par_value = -1000;


	settextbackground(0);
	
	if (config.is_open())
	{
		while(!config.eof())
		{
			getline(config, line);
			//cout << line<<endl;
			if(multi_line_comment){
				found_Close_comment = line.find(close_comment);
				if (found_Close_comment!=string::npos){
					multi_line_comment=false;
					line = line.substr(found_Close_comment+2);
					//cout << "found close comment. multi_line_comment= " << multi_line_comment<< endl;
				}
			}
			if(multi_line_comment == false){
				found_Open_comment = line.find(open_comment);
				if (found_Open_comment!=string::npos){
					line = line.substr(0, found_Open_comment);
					multi_line_comment = true;
					//cout << "found open comment. multi_line_comment= " << multi_line_comment<< endl;
				}

				// remove the comments
				found_comment = line.find(comment);
				if (found_comment!=string::npos)	
					line = line.substr(0,found_comment);
				
				// remove the semi colons 
				found_semi = line.find(semi);
				if (found_semi!=string::npos)	
					line = line.erase(found_semi,1);
				
				// replace the tabs with spaces
				found_tab= line.find(tab);
				while(found_tab!=string::npos){
					line = line.replace(found_tab,1," ");
					found_tab= line.find(tab);
				}
				
				//remove the parameter and then fill parameter map
				found_par = line.find(par);
				if (found_par!=string::npos){
					line = line.erase(found_par,9);
		
					stringstream line_stream(line);
					line_stream >> par_num >> par_value;
					parameter[par_num]=par_value;
					//printf("parameter %i	%G \n" , par_num, parameter[par_num]);
					line.erase(line.begin(),line.end());
				}


/*
		/////////////////////////////////// insert presorter commands here /////////////////////////////////////////////////
				string new_presorter="new_presorter";
				size_t found_new_presorter;
				found_new_presorter = line.find(new_presorter); //try to find "new_presorter"
				if (found_new_presorter!=string::npos){
					++this->num_presorter;

					line = line.erase(found_new_presorter, new_presorter.size()); // erase the "new_presorter"
					command temp_command;		// create temp commad object 
					temp_command.presorter=true;
					temp_command.spectrometer=false;
					temp_command.tof=false;
					string presorter;									
					size_t found_presorter;
					int num_arg = 0;
					double temp_arg = 0;
					
					stringstream line_stream(line);
					line_stream >> presorter;
					temp_command.command_str=presorter;	
					//cout << "Persorter found: " <<presorter ;
					while(!line_stream.eof()){
						line_stream >> temp_arg;
						if(!line_stream.fail()){
							temp_command.arg.push_back(temp_arg);
							//cout << ", "<< temp_arg ; 
						}
					}
					cout<< endl;
					config_commands.push_back(temp_command);	
					line = " ";
				}
				
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				string tof="set_TOF";
				size_t found_set_TOF;
				found_set_TOF = line.find(tof); //try to find "set_TOF"

				if (found_set_TOF!=string::npos){
					
					line = line.erase(found_set_TOF, tof.size()); 
					command temp_command;		// create temp commad object 
					temp_command.presorter=false;
					temp_command.spectrometer=false;
					temp_command.tof = true;
					string command;									
					int num_arg = 0;
					double temp_arg = 0;
					
					stringstream line_stream(line);
					line_stream >> command;
					temp_command.command_str=command;	
					//cout << "set_TOF found: " <<command ;
					while(!line_stream.eof()){
						line_stream >> temp_arg;
						if(!line_stream.fail()){
							temp_command.arg.push_back(temp_arg);
							//cout << ", "<< temp_arg ; 
						}
					}
					cout<< endl;
					config_commands.push_back(temp_command);	
					line = " ";
					
	

				}
		/////////////////////////////////// Spectrometer /////////////////////////////////////////////////
				string spectrometer_arm="set_spectrometer";
				size_t found_spectrometer_arm;
				found_spectrometer_arm = line.find(spectrometer_arm);
				if (found_spectrometer_arm!=string::npos){

					line = line.erase(found_spectrometer_arm, spectrometer_arm.size()); // erase the command
					command temp_command;		// create temp commad object 
					temp_command.presorter=false;
					temp_command.spectrometer=true;
					temp_command.tof=false;
					string spec_type;									
					size_t found_spec;
					int num_arg = 0;
					double temp_arg = 0;
					
					
					//electron
					spec_type ="ELEC_ARM";
					
					found_spec = line.find(spec_type);				//try to find "electron"
					if (found_spec!=string::npos){					//if found do something
						temp_command.command_str=spec_type;				// store which command string was found
						line = line.erase(found_spec, spec_type.size()); // erase the "electron"
						
						stringstream line_stream(line);
						
						line_stream >> num_arg;
						temp_command.arg.push_back(num_arg);

						for(int i=0; i < 2*num_arg; ++i){
							line_stream >> temp_arg;				// get the number from the string
							temp_command.arg.push_back(temp_arg);				// store the argument for "electron" in the arguments vector
						}
						
						config_commands.push_back(temp_command);		// store the eTOF command object in the a vector. 
						
					}

					//ion
					spec_type ="ION_ARM";
					
					found_spec = line.find(spec_type);				//try to find "electron"
					if (found_spec!=string::npos){					//if found do something
						temp_command.command_str=spec_type;				// store which command string was found
						line = line.erase(found_spec, spec_type.size()); // erase the "electron"
						
						stringstream line_stream(line);
						
						line_stream >> num_arg;
						temp_command.arg.push_back(num_arg);

						for(int i=0; i < 2*num_arg; ++i){
							line_stream >> temp_arg;				// get the number from the string
							temp_command.arg.push_back(temp_arg);				// store the argument for "electron" in the arguments vector
						}
						
						config_commands.push_back(temp_command);		// store the eTOF command object in the a vector. 
						
					}

					//ION_ARM_LINEAR
					spec_type ="ION_ARM_LINEAR";
					found_spec = line.find(spec_type);			
					if (found_spec!=string::npos){					//if found do something
						temp_command.command_str=spec_type;				// store which command string was found
						line = line.erase(found_spec, spec_type.size()); // erase the "electron"
						
						stringstream line_stream(line);
						line_stream >> temp_arg;				// get the number from the string
						temp_command.arg.push_back(temp_arg);				// store the argument for "electron" in the arguments vector
						config_commands.push_back(temp_command);		// store the eTOF command object in the a vector. 
					}

					//B_ns
					spec_type ="B_ns";
					found_spec = line.find(spec_type);			
					if (found_spec!=string::npos){					//if found do something
						temp_command.command_str=spec_type;				// store which command string was found
						line = line.erase(found_spec, spec_type.size()); // erase the "electron"
						
						stringstream line_stream(line);
						line_stream >> temp_arg;				// get the number from the string
						temp_command.arg.push_back(temp_arg);				// store the argument for "electron" in the arguments vector
						line_stream >> temp_arg;
						temp_command.arg.push_back(temp_arg);
						config_commands.push_back(temp_command);		// store the eTOF command object in the a vector. 
					}

					//B_Gauss
					spec_type ="B_Gauss";
					found_spec = line.find(spec_type);			
					if (found_spec!=string::npos){							//if found do something
						temp_command.command_str=spec_type;					// store which command string was found
						line = line.erase(found_spec, spec_type.size());	// erase the "electron"
						
						stringstream line_stream(line);
						line_stream >> temp_arg;							// get the number from the string
						temp_command.arg.push_back(temp_arg);				// store the argument for "electron" in the arguments vector
						line_stream >> temp_arg;
						temp_command.arg.push_back(temp_arg);
						config_commands.push_back(temp_command);			// store the eTOF command object in the a vector. 
					}
				}
*/
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////				
				//else{
					//look for readROOTfile
					found_rroot = line.find(read_root);
					if (found_rroot!=string::npos){
						line = line.erase(found_rroot, read_root.size());

						// find and remove anything before and after the double quotes
						found_dqoutes = line.find(double_quotes);
						if (found_dqoutes!=string::npos){
							line.erase(found_dqoutes, 1);
							line = line.substr(found_dqoutes, line.find(double_quotes)-found_dqoutes);
						}
						//cout << line << endl;
						inputfilename.push_back(line);
						line.erase(line.begin(),line.end());
					}

					//look for set_root_output_file_name
					found_wroot = line.find(write_root);
					if (found_wroot!=string::npos){
						line = line.erase(found_wroot,write_root.size());
						// find and remove anything before and after the double quotes
						found_dqoutes = line.find(double_quotes);
						if (found_dqoutes!=string::npos){
							line.erase(found_dqoutes, 1);
							line = line.substr(found_dqoutes, line.find(double_quotes)-found_dqoutes);
						}
						outputfilename = line;

						// add .root to the end of the file name if it isn't there
						found_dotroot = outputfilename.find(dot_root);
						if (found_dotroot==string::npos){
							outputfilename.append(dot_root);
						}

						line.erase(line.begin(),line.end());
						//cout << write_root << outputfilename << endl;
					}

				//}
					if ((int)line.length() > 0){
						//cout <<"(int)line.length()="<<(int)line.length()<<endl;
						double temp_arg = -123456789.;

						command temp_command;		// create temp commad object 
						//temp_command.command_str=spec_type;					// store which command string was found
						//line = line.erase(found_spec, spec_type.size());	// erase the "electron"
						
						stringstream line_stream(line);
						line_stream >> temp_command.command_str_main;
						//test to see if it is a number
						stringstream temp_stream(temp_command.command_str_main);
						temp_stream >> temp_arg;
						if (!temp_stream.fail()){
							Red(true);
							cout << "Error in config file:";
							White(true);
							cout << line << endl;
							Red(true);
							cout << "The first thing on the line should be a string." << endl;
							White(false);
						}else{
							temp_arg = -123456789.;
						}

						//cout <<temp_command.command_str_main <<endl;

						line_stream >> temp_command.command_str;
						//test to see if it is a number
						stringstream temp_stream2(temp_command.command_str);
						temp_stream2 >> temp_arg;
						cout << "test out: " << temp_command.command_str << " " << temp_arg<<endl;
						if (!temp_stream2.fail()){
							cout << "Deleted second string in favor of storing the value in a double.\n"<< temp_command.command_str<<endl;
							cout << "On line:"<<line<<endl;
							temp_command.command_str = "";
						}else{
							temp_arg = -123456789.;
						}


						//	cout <<temp_command.command_str <<endl;

						while(!line_stream.eof()){
							line_stream >> temp_arg;
							if(!line_stream.fail()){
								temp_command.arg.push_back(temp_arg);
							//	cout << "," << temp_arg;
							}
						}
						config_commands.push_back(temp_command);


					}
			}
		}//end of while(!config.eof())
	}

	config.close();
	settextbackground(0);
	settextcolor(7);
}

	
config_file_reader::~config_file_reader(){}


