//#include "stdafx.h"
#include "Simple_Hist.h"
#include <string>
#include <vector>
#include "console.h"
#include <map>
//#include <thread>
//#include <mutex>

using namespace std;

//Also change in Simple_Hist.h 
#define MAX_NUM_HIST 15000

////////////////////////////////////////////  axis  ////////////////////////////////////////////
axis::axis(char * Name, char * Title,int N_bins, double Min, double Max){
	//std::lock_guard<std::mutex> guard(mutex); //auto lock thread
	n_bins = N_bins;
	max = Max; 
	min = Min;
	title = Title;
	name = Name;
	overflow =0;
	underflow =0;

	return;
 }
axis::~axis(){

}

int axis::get_bin_address(double x){
	//std::lock_guard<std::mutex> guard(mutex); //auto lock thread
	if(x>=max){
		overflow++;
		return -1;
	}
	if(x<min){
		underflow++;
		return -1;
	}
	int bin = int (n_bins*(x-min)/(max-min) );
	//bins[bin] = 1 + bins[bin];
	return bin;
}
////////////////////////////////////////////  end  ////////////////////////////////////////////


////////////////////////////////////////////  H1d  ////////////////////////////////////////////

H1d::H1d(){};

H1d::H1d(char * Name, char * Title,int N_bins, double Min, double Max, char * X_axis_label, char * Dir){
		
	title = Title;
	name = Name;
	dir = Dir;

	Xaxis = new axis("Xaxis", X_axis_label, N_bins, Min, Max );

	//initialise all bins to 0
	for (int i=0; i<N_bins; i++)
	{
		bins.push_back(0.);
		//printf("bin%i:%i   ",i,bins[i]);
	}

	//double * temp_array = new double[N_bins];

	//bins = temp_array;
	//for (int i=0; i<N_bins; i++)
	//{
	//	bins[i] = 0.;
	//}

	//char temp[500] = "";
	//strcat(temp,Name);
	//strcat(temp,Dir);
	//key = temp;

}

H1d::~H1d(){}

void H1d::fill(double x){
	int bin_id = Xaxis->get_bin_address(x);
	// add one to the bin if it isn't overflow are underflow
	
	if (bin_id !=-1) ++bins[bin_id];
}

void H1d::weighted_fill(double x, double weight){
	int bin_id = Xaxis->get_bin_address(x);
	// add one to the bin if it isn't overflow are underflow
	
	if (bin_id !=-1) bins[bin_id]=bins[bin_id]+weight;
}

void H1d::print_bin_contents(){
	//std::lock_guard<std::mutex> guard(mutex); //auto lock thread
	printf("\n");
	//printf("\nnumber of bins=%i\n",bins.size());
	double bin_width = (Xaxis->max-Xaxis->min)/Xaxis->n_bins;
	//printf("bin widh=%f\n", bin_width);
	for (int i=0; i<(Xaxis->n_bins); i++)
	{
		if (bins[i]!=0)	Red(true);
			printf("%2.f=<(bin%i)<%2.f:%i   ",(double)i*bin_width+Xaxis->min,i,((double)i+1.)*bin_width+Xaxis->min,bins[i]);
		if (bins[i]!=0)	White(false);
	
		if((i+1)%10==0 ) printf("\n");
		
	}
	
	printf("\n");
	return;
}

//bool H1d::match(string NAME, string TITLE, int N_BINS, double MIN, double MAX, string X_LABEL, string DIR){
//	return NAME==this->name && TITLE==this->title && N_BINS==this->Xaxis->n_bins && MIN==this->Xaxis->min && MAX==this->Xaxis->max && X_LABEL==this->Xaxis->title && DIR==this->dir; 
//}

void H1d::print_info(){
	cout<<name<< ", "<< title<< ", "<< Xaxis->n_bins << ", "<< Xaxis->min<< ", "<< Xaxis->max<< ", "<< Xaxis->title<< ", "<< dir <<endl;

}
////////////////////////////////////////////  H1d end  ////////////////////////////////////////////

////////////////////////////////////////////  H2d  ////////////////////////////////////////////

H2d::H2d(){};

H2d::H2d(const char * Name, char * Title,int X_N_bins, double X_Min, double X_Max, char * X_axis_label,int Y_N_bins, double Y_Min, double Y_Max, char * Y_axis_label, char * Dir){
		
	title = Title;
	name = Name;
	dir = Dir;

	Xaxis = new axis("Xaxis", X_axis_label, X_N_bins, X_Min, X_Max );
	Yaxis = new axis("Yaxis", Y_axis_label, Y_N_bins, Y_Min, Y_Max );

	//initialise all bins to 0
	//bins[x][y]
	vector<double> col;
	for (int j=0; j<Y_N_bins; j++)
	{
		col.push_back(0);
	}

	for (int i=0; i<X_N_bins; i++)
	{
		bins.push_back(col);
	}
	
	//double ** temp_array = new double*[X_N_bins];
	//for(int i = 0; i < Y_N_bins; ++i){
	//	temp_array[i] = new double[Y_N_bins];
	//}

	//bins = temp_array;
	//for (int i=0; i<X_N_bins; i++)
	//{
	//	for (int j=0; j<Y_N_bins; j++){
	//		bins[i][j] = 0.;
	//	}
	//}

//	char temp[500] = "";
//	strcat(temp,Name);
//	strcat(temp,Dir);
//	key = temp;
}

H2d::~H2d(){}

void H2d::fill(double x, double y){
	int bin_id_x = Xaxis->get_bin_address(x);
	int bin_id_y = Yaxis->get_bin_address(y);
	// add one to the bin if it isn't overflow are underflow
	if (bin_id_x !=-1 && bin_id_y !=-1) ++bins[bin_id_x][bin_id_y];
	return;
}
void H2d::weighted_fill(double x, double y, double weight){
	int bin_id_x = Xaxis->get_bin_address(x);
	int bin_id_y = Yaxis->get_bin_address(y);
	// add one to the bin if it isn't overflow are underflow
	if (bin_id_x !=-1 && bin_id_y !=-1) bins[bin_id_x][bin_id_y]=bins[bin_id_x][bin_id_y]+weight;
	return;
}
//void H2d::print_bin_contents(){
//	//std::lock_guard<std::mutex> guard(mutex); //auto lock thread
//	printf("\n");
//	//printf("\nnumber of bins=%i\n",bins.size());
//	double bin_width = (Xaxis->max-Xaxis->min)/Xaxis->n_bins;
//	//printf("bin widh=%f\n", bin_width);
//	for (int i=0; i<((signed int)bins.size()); i++)
//	{
//		if (bins[i]!=0)	Red(true);
//			printf("%2.f=<(bin%i)<%2.f:%i   ",(double)i*bin_width+Xaxis->min,i,((double)i+1.)*bin_width+Xaxis->min,bins[i]);
//		if (bins[i]!=0)	White(false);
//	
//		if((i+1)%10==0 ) printf("\n");
//		
//	}
//	
//	printf("\n");
//	return;
//}

//bool H2d::match(string NAME, string TITLE, int X_N_BINS, double X_MIN, double X_MAX, string X_LABEL, int Y_N_BINS, double Y_MIN, double Y_MAX, string Y_LABEL,  string DIR){
//	return NAME==this->name && TITLE==this->title && X_N_BINS==this->Xaxis->n_bins && X_MIN==this->Xaxis->min && X_MAX==this->Xaxis->max && X_LABEL==this->Xaxis->title && Y_N_BINS==this->Yaxis->n_bins && Y_MIN==this->Yaxis->min && Y_MAX==this->Yaxis->max && Y_LABEL==this->Yaxis->title && DIR==this->dir; 
//}

void H2d::print_info(){
	cout<<name<< ", "<< title<< ", "<< Xaxis->n_bins << ", "<< Xaxis->min<< ", "<< Xaxis->max<< ", "<< Xaxis->title<< ", ";
	cout<< Yaxis->n_bins << ", "<< Yaxis->min<< ", "<< Yaxis->max<< ", "<< Yaxis->title<< ", "<< dir <<endl;
}
////////////////////////////////////////////  H2d end  ////////////////////////////////////////////


////////////////////////////////////////////  histo_container  ////////////////////////////////////////////

histo_handler::histo_handler(){
	hist_check_counter=0;

	for(int i=0; i<MAX_NUM_HIST; ++i){
		H1d_vector.push_back(0);
		H2d_vector.push_back(0);
	}
}

inline bool compare(const char* s1,const char* s2){
	for(int i=0; i<50; i++){
		if ( *(s1+i) != *(s2+i) )	{
		//	cout << "return false"<< endl;
			return false;
		}
		if ( *(s1+i) == '\0') {
		//	cout << "return true"<< endl;	
			return true;
		}
		//cout << *(s1+i) << ", "<< *(s2+i) << "i=" << i << endl;
	}
}

void histo_handler::fill1(int id, char * NAME, double x, char * TITLE, int N_BINS, double MIN, double MAX, char * X_LABEL, char * DIR){
	if( id > MAX_NUM_HIST) {
		int max_num_hist = MAX_NUM_HIST;
		printf("The current max number of histograms are %d.  Please change the max (in Simple_Hist.cpp -> histo_handler::histo_handler()).\n", max_num_hist);
		return;
	}

	if( H1d_vector[id] == 0){
		H1d * hist = new H1d(NAME, TITLE, N_BINS, MIN, MAX, X_LABEL, DIR);
		H1d_vector[id]=hist;
	}else{
		//checking to see if the histogram matches is slow so we will only do this in the very begining 
		if ( strcmp( H1d_vector[id]->get_name(), NAME)!=0 || H1d_vector[id]->get_X_n_bins() != N_BINS || H1d_vector[id]->get_X_max() != MAX || H1d_vector[id]->get_X_min() != MIN)// !compare( H1d_vector[id]->get_name(),NAME) strcmp( H1d_vector[id]->get_name(), NAME)!=0 )
		{
			Red(true);
			cout<<endl;
			cout<<"ERROR: Histogram is:" << NAME<< ", "<< TITLE<< ", "<< N_BINS<< ", "<< MIN<< ", "<< MAX<< ", "<< X_LABEL<< ", "<< DIR<<endl;
			cout<<"And it should be   :";
			H1d_vector[id]->print_info();
			cout<<endl;
			White(false);
		}
		else {
			H1d_vector[id]->fill(x);
			hist_check_counter++;
		}
	}
	return;

}
void histo_handler::fill1(int id, char * NAME, double x,  double weight, char * TITLE, int N_BINS, double MIN, double MAX, char * X_LABEL, char * DIR){
	if( id > MAX_NUM_HIST) {
		int max_num_hist = MAX_NUM_HIST;
		printf("The current max number of histograms are %d.  Please change the max (in Simple_Hist.cpp -> histo_handler::histo_handler()).\n", max_num_hist);
		return;
	}

	if( H1d_vector[id] == 0){
		H1d * hist = new H1d(NAME, TITLE, N_BINS, MIN, MAX, X_LABEL, DIR);
		H1d_vector[id]=hist;
	}else{
		//checking to see if the histogram matches is slow so we will only do this in the very begining 
		if ( strcmp( H1d_vector[id]->get_name(), NAME)!=0 || H1d_vector[id]->get_X_n_bins() != N_BINS || H1d_vector[id]->get_X_max() != MAX || H1d_vector[id]->get_X_min() != MIN)// !compare( H1d_vector[id]->get_name(),NAME) strcmp( H1d_vector[id]->get_name(), NAME)!=0 )
		{
			Red(true);
			cout<<endl;
			cout<<"ERROR: Histogram is:" << NAME<< ", "<< TITLE<< ", "<< N_BINS<< ", "<< MIN<< ", "<< MAX<< ", "<< X_LABEL<< ", "<< DIR<<endl;
			cout<<"And it should be   :";
			H1d_vector[id]->print_info();
			cout<<endl;
			White(false);
		}
		else {
			H1d_vector[id]->weighted_fill(x,weight);
			hist_check_counter++;
		}
	}
	return;

}
void histo_handler::combine_hist(H1d * hist2, int pos){
	
	if(hist2 == 0) return;

	if(H1d_vector[pos] == 0){ //if it dosen't exit just add it
		H1d_vector[pos]=hist2;
	}
	else{
		//if ( true)//H1d_vector[pos]->match(hist2->get_name(), hist2->get_title(), hist2->get_X_n_bins(), hist2->get_X_min(), hist2->get_X_max(), hist2->get_X_title(), hist2->get_dir() ))
		//{
			for(int i=0; i < hist2->Xaxis->n_bins; ++i){
				H1d_vector[pos]->bins[i] = H1d_vector[pos]->bins[i] + hist2->bins[i];
			}
			H1d_vector[pos]->set_X_overflow(  H1d_vector[pos]->get_X_overflow()  +	hist2->get_X_overflow() );
			H1d_vector[pos]->set_X_underflow( H1d_vector[pos]->get_X_underflow() +	hist2->get_X_underflow() );
		//}
		//else {
		//	Red(true);
		//	cout<<"ERROR in combine_hist()"<<endl;
		//	cout<<"ERROR: Histogram is:";	hist2->print_info();
		//	cout<<"   And it should be:";	H1d_vector[pos]->print_info();
		//		
		//	White(false);
		//}

		}
}


	
void histo_handler::fill2(int id, char * NAME, double x, double y, char * TITLE, int X_N_BINS, double X_MIN, double X_MAX, char * X_LABEL, int Y_N_BINS, double Y_MIN, double Y_MAX, char * Y_LABEL, char * DIR){

	if( id > MAX_NUM_HIST) {
		int max_num_hist = MAX_NUM_HIST;
		printf("The current max number of histograms are %d.  Please change the max (in Simple_Hist.cpp -> histo_handler::histo_handler()).\n", max_num_hist);
		return;
	}

	if( H2d_vector[id] == 0){
		H2d * hist = new H2d(NAME, TITLE, X_N_BINS, X_MIN, X_MAX, X_LABEL, Y_N_BINS, Y_MIN, Y_MAX, Y_LABEL, DIR);
		H2d_vector[id]=hist;
		H2d_vector[id]->fill(x,y);
	}else{
		//checking to see if the histogram matches is slow so we will only do this in the very begining 
		if ( strcmp( H2d_vector[id]->get_name(), NAME)!=0         || H2d_vector[id]->get_X_n_bins() != X_N_BINS || H2d_vector[id]->get_X_max() != X_MAX || H2d_vector[id]->get_X_min() != X_MIN      || H2d_vector[id]->get_Y_n_bins() != Y_N_BINS || H2d_vector[id]->get_Y_max() != Y_MAX || H2d_vector[id]->get_Y_min() != Y_MIN)// !compare( H2d_vector[id]->get_name(),NAME) strcmp( H2d_vector[id]->get_name(), NAME)!=0 )
		{
			Red(true);
			cout<<endl;
			cout<<"ERROR: Histogram is:"<<NAME<< ", "<< TITLE<< ", "<< X_N_BINS<< ", "<< X_MIN<< ", "<< X_MAX<< ", "<< X_LABEL<< ", "<< DIR<<endl;
			cout<<"And it should be   :";
			H2d_vector[id]->print_info();
			cout<<endl;
			White(false);
		}
		else {
			H2d_vector[id]->fill(x,y);
			hist_check_counter++;
		}
	}
	return;

}
void histo_handler::fill2(int id, char * NAME, double x, double y, double weight, char * TITLE, int X_N_BINS, double X_MIN, double X_MAX, char * X_LABEL, int Y_N_BINS, double Y_MIN, double Y_MAX, char * Y_LABEL, char * DIR){

	if( id > MAX_NUM_HIST) {
		int max_num_hist = MAX_NUM_HIST;
		printf("The current max number of histograms are %d.  Please change the max (in Simple_Hist.cpp -> histo_handler::histo_handler()).\n", max_num_hist);
		return;
	}

	if( H2d_vector[id] == 0){
		H2d * hist = new H2d(NAME, TITLE, X_N_BINS, X_MIN, X_MAX, X_LABEL, Y_N_BINS, Y_MIN, Y_MAX, Y_LABEL, DIR);
		H2d_vector[id]=hist;
		H2d_vector[id]->fill(x,y);
	}else{
		//checking to see if the histogram matches is slow so we will only do this in the very begining 
		if ( strcmp( H2d_vector[id]->get_name(), NAME)!=0         || H2d_vector[id]->get_X_n_bins() != X_N_BINS || H2d_vector[id]->get_X_max() != X_MAX || H2d_vector[id]->get_X_min() != X_MIN      || H2d_vector[id]->get_Y_n_bins() != Y_N_BINS || H2d_vector[id]->get_Y_max() != Y_MAX || H2d_vector[id]->get_Y_min() != Y_MIN)// !compare( H2d_vector[id]->get_name(),NAME) strcmp( H2d_vector[id]->get_name(), NAME)!=0 )
		{
			Red(true);
			cout<<endl;
			cout<<"ERROR: Histogram is:"<<NAME<< ", "<< TITLE<< ", "<< X_N_BINS<< ", "<< X_MIN<< ", "<< X_MAX<< ", "<< X_LABEL<< ", "<< DIR<<endl;
			cout<<"And it should be   :";
			H2d_vector[id]->print_info();
			cout<<endl;
			White(false);
		}
		else {
			H2d_vector[id]->weighted_fill(x,y,weight);
			hist_check_counter++;
		}
	}
	return;

}
void histo_handler::combine_hist(H2d * hist2, int pos){
	if(hist2 == 0) return;
	
	if(H2d_vector[pos] == 0){ //if it dosen't exit just add it
		H2d_vector[pos]=hist2;
	}
	else{
		//if (true)// H2d_vector[pos]->match(hist2->get_name(), hist2->get_title(), hist2->get_X_n_bins(), hist2->get_X_min(), hist2->get_X_max(), hist2->get_X_title(), hist2->get_Y_n_bins(), hist2->get_Y_min(), hist2->get_Y_max(), hist2->get_Y_title(), hist2->get_dir() ))
		//{
			for(int i=0; i < (int)hist2->get_X_n_bins(); ++i){
				for(int j=0; j < (int)hist2->get_Y_n_bins(); ++j){
					H2d_vector[pos]->bins[i][j] = H2d_vector[pos]->bins[i][j] + hist2->bins[i][j];
				}
			}

			H2d_vector[pos]->set_X_overflow(  H2d_vector[pos]->get_X_overflow()  +	hist2->get_X_overflow() );
			H2d_vector[pos]->set_X_underflow( H2d_vector[pos]->get_X_underflow() +	hist2->get_X_underflow() );
			
			H2d_vector[pos]->set_Y_overflow(  H2d_vector[pos]->get_Y_overflow()  +	hist2->get_Y_overflow() );
			H2d_vector[pos]->set_Y_underflow( H2d_vector[pos]->get_Y_underflow() +	hist2->get_Y_underflow() );
			
		//}
		//else {
		//	Red(true);
		//	cout<<"ERROR in combine_hist()"<<endl;
		//	cout<<"ERROR: Histogram is:";	hist2->print_info();
		//	cout<<"   And it should be:";	H2d_vector[pos]->print_info();
		//		
		//	White(false);
		//}

		}
}	

																								