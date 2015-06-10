#include <TMath.h>
#include <TParameter.h>
#include <TFile.h> 
#include <TTree.h> 
#include <Riostream.h> 
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <string>
#include <vector>
#include <TSpectrum.h>
#include <stdio.h>
#include <algorithm>
#include <cstdlib>
#include <errno.h>

using namespace std;


void sensorSummary(const char *dirName = "",const float T=21.) 
{
 static char *grade;
 float slope(0.);
 float V[300],I[300];
 float x_V[300],y_I[300];
 float iv100(0.);
 float iv150(0.);
 float ivn150(0.);
 char date[300];
 char timestamp[400];

 //log file to store the parameter values
 char logname[70];
 ofstream outlog;
 sprintf(logname,"%s/logfile.dat" ,dirName);
 outlog.open(logname);
 
 //ivCurve.log: input file
 char ifname[40];
 sprintf(ifname, "%s/ivCurve.log", dirName);
 outlog <<ifname<<endl;
 ifstream ivf(ifname);
 string row;
 
 //iv.dat: output file with only the IV data. Rejecting the comments
 char ofname[40];
 sprintf(ofname,"%s/iv.dat", dirName);
 ofstream tmp(ofname);
    while (!ivf.eof()) {
	getline(ivf,row);
	if (row.length() == 0 || row[0] == '#')
	  continue;
	else{
	  tmp<<row<<endl;
	}  
    }

ivf.close(); 

//Using iv.dat to manage the data
 char ivname[40];
 sprintf(ivname,"%s/iv.dat", dirName);
 FILE *ivFile; 
 ivFile = fopen(ivname, "r"); 
 int i = 0;
 int ncol=0;
  
   while(1){
       ncol=fscanf(ivFile,"%f %f %*s %*s",&x_V[i],&y_I[i],&date[i],&timestamp[i]);
       
       if(ncol<0) break;
       //I[i]=fabs(y_I[i]/1.0e-6);
       I[i]=fabs(y_I[i]);
       V[i]=fabs(x_V[i]);
       if(i>0){	
	 // check that voltage is increasing & find current at 150 V 
	 if ( V[i] < V[i-1] ) { cout<<"voltage is decreasing"<<endl; }
	 if ( V[i] == 100. ) { iv100 = I[i];
	   outlog<<"V="<<V[i]<<"V: "<<"I(A)= \n"<<iv100<<endl;  
	 }
	 if ( V[i] == 150.) { iv150 = I[i];
	   outlog<<"V="<<V[i]<<"V: "<<"I(A)= \n"<<iv150<<endl; 
	 } 
	 
       } 
     
     i++;
   }
   
   if ( iv100 != 0. ) { slope = iv150/iv100; }
   else               { slope = 0; }
   
   fclose(ivFile); 

   //Normalizing the current at 150V
   const float Tn=293.15;//20C in Kelvin
   float Tk=T+273.15;//Temp in Kelvin
   const float Eg=1.2;//eV
   const float kb=8.62E-5;//eV/K
   ivn150=iv150*pow((Tn/Tk),2)*exp((-Eg/(2*kb))*((1/Tn)-(1/Tk)));
   cout<<"T="<<Tk<<"K ("<<T<<"C)"<<", Tn="<<Tn<<"K (20C)"<<endl;
   cout<<"I (150V)="<<iv150/1.e-6<<" uA"<<endl;
   cout<<"Inorm (150V)="<<ivn150/1.e-6<<" uA"<<endl;
       
  
 //GRADING
 if((ivn150/1.e-6) < 2. && slope < 2.) grade="A";
 else {
   if((ivn150/1.e-6) < 10.) grade="B";
     else grade="C";
 }
 cout<<"slope: "<<slope<<endl;
 cout<<"grade: "<<grade<<endl;
 
 outlog.precision(3);
 outlog<<"slope: \n"<<slope<<"\ngrade: \n"<<grade<<endl;
 outlog<<"Temperature: "<<T<<" C"<<endl;
 outlog<<"Inorm (150V): "<<ivn150/1.e-6<<" uA (20C)"<<endl;
}

