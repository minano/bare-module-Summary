#include <TMath.h>
#include <TParameter.h>
#include <TFile.h> 
#include <TTree.h> 
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TLatex.h>


using namespace std;

void comparison(const char *dirName = "",const char *sensor_group) 
{

  gROOT->Reset();


  int np; 
  float Ix[200];// Current (A)
  float Vx[200]; // Voltage (V)
  float I[200]; // Current (uA)
  float V[200]; // Voltage (V)
  char date[200];
  char timestamp[200];
  char fname[200];
  FILE *inputFile;
  int ncol;

if ( !strcmp(sensor_group,"1-10") ) {
  const int ns=10;
   int sensor[ns]={1,2,3,4,5,6,7,8,9,10};
 }
 
 if ( !strcmp(sensor_group,"11-21") ) {
   const int ns=11;
   int sensor[ns]= {11,12,13,14,15,16,17,18,19,20,21};
 }
if ( !strcmp(sensor_group,"22-31") ) {
   const int ns=10;
   int sensor[ns]= {22,23,24,25,26,27,28,29,30,31};
 }
 
 if ( !strcmp(sensor_group,"all") ) {
   const int ns=31;
   int sensor[ns]= {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};
 }

 if ( !strcmp(sensor_group,"S322497-21-1") ) {
   const int ns=4;
   int sensor[ns]= {1,2,3,4};
 }
 
  TCanvas* c1 = new TCanvas("c1","",50,50,600,600);
  TMultiGraph *mg = new TMultiGraph();
  
  TGraph *gr[ns];
 
  printf("\n Leakage Current comparison > Starting ...\n");
 
  for (int i=0;i<ns;i++){

    sprintf(fname, "%s/iv%i.dat", dirName,sensor[i]);
    inputFile = fopen(fname, "r");
    printf("\n ----> Reading %s \n", fname);
    if (!inputFile) { 
      
      printf("\n ----> COULD NOT FIND %s \n", fname);
      printf("----> Continuing with the next sensor ... \n\n");   
      
    }
    
    int k=0;
    while(1){
      
      ncol=fscanf(inputFile,"%f %f %*s %*s",&Vx[k],&Ix[k],&date[k],&timestamp[k]);
     
      if(ncol<0) break;
      I[k]=fabs(Ix[k]/1.0e-6);
      //I[k]=fabs(Ix[k]);
      V[k]=fabs(Vx[k]);
      cout<<"V: "<<V[k]<<", I: "<<I[k]<<endl;
      //cout<<k<<endl;
      k++;
    }
  np=k;
  //cout<<np<<endl;

  gr[i] = new TGraph(np,V,I);
  gr[i]->SetLineColor(i+1);
  if (i>8;){
    gr[i]->SetLineColor(i+20); //skipe white
  }
  gr[i]->SetLineWidth(2);
  gr[i]->SetMarkerSize(1);
  gr[i]->SetMarkerColor(i+2);

  mg->Add(gr[i]);
 
  fclose(inputFile);
}
  
  mg->Draw("AL");
  double xl1=.5, yl1=0.15, xl2=xl1+.2, yl2=yl1+.3;
  TLegend *leg = new TLegend(xl1,yl1,xl2,yl2);
  leg->SetFillColor(0);
  leg->SetTextFont(132);
  for(int j=0;j<ns;j++){
    leg->AddEntry(gr[j],Form("2015_%i",sensor[j]),"l");  
    //leg->SetMarkerStyle(20);
  }
  leg->Draw();
  //mg->SetTitle("IV");
  mg->GetYaxis()->SetTitle("Current [uA]");
  mg->GetXaxis()->SetTitle("Voltage [V]");
  mg->GetYaxis()->SetLabelSize(0.033);
  mg->GetYaxis()->SetTitleOffset(1.5);
  mg->GetXaxis()->SetLabelSize(0.033);
  mg->GetYaxis()->SetTitleSize(0.033);
  mg->GetXaxis()->SetTitleSize(0.033);
  c1->SetLogy();
 

  c1->Print("IV.root");

  printf("\n IV > ................................................ finished\n");
  
}



