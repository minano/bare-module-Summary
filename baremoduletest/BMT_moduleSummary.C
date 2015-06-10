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

char fname[400];
FILE *inputFile;

TCanvas *c1 = NULL;
TCanvas *c2 = NULL;
TLatex *tl;     
TLatex *ts;
TLatex *ti;
TLine *line;
TLine *Rocline;
TLine *volt;
TLine *curr;
TBox *box;
int nRootFileProblems(0);

int nChips(16);
int startChip(0);
int readVerbose(1);


void moduleSummary(const char *dirName = "", const char *module_type) 
{
  
 printf("\nmoduleSummary> Starting ...\n");
 
 if ( !strcmp(module_type,"a") ) {
   
   nChips = 8; 
   startChip = 0; 
  }
 
 if ( !strcmp(module_type,"b") ) {
   
   nChips = 8; 
   startChip = 8; 
 }
 
 if ( !strcmp(module_type,"c") ) {
    
   nChips = 16; 
   startChip = 0; 
 }
  
 
 //LOG FILE
 char logname[70];
 ofstream outstream;
 cout << "writing log file <.txt> -> Identification of dead pixels & missing bumps ... "<<endl;
 sprintf(logname,"Final/logfile_%s.txt" ,dirName);
 outstream.open(logname);
 //outstream << Lo que sea;
 
 
 //CHECK IF ROC<n>.root EXISTS
 for (int i=startChip;i<nChips;i++){
    
   sprintf(fname, "%s/roc%i.root", dirName,i);
   inputFile = fopen(fname, "r");
   if (!inputFile) { 
      
     printf("\nmoduleSummary> ----> COULD NOT FIND %s IN DIRECTORY %s\n", fname, dirName);
     
   }
 }
 
 gROOT->SetStyle("Plain");
 gStyle->SetPalette(1);
 gStyle->SetOptStat(0);
 gStyle->SetTitle(0);
 gStyle->SetStatFont(132);
 gStyle->SetTextFont(132);
 gStyle->SetLabelFont(132, "X");
 gStyle->SetLabelFont(132, "Y");
 gStyle->SetLabelSize(0.08, "X");
 gStyle->SetLabelSize(0.08, "Y");
 gStyle->SetTitleSize(0.08, "X");
 gStyle->SetTitleSize(0.08, "Y");
 gStyle->SetNdivisions(10, "X");
 gStyle->SetNdivisions(8, "Y");
 gStyle->SetTitleFont(132);
 gROOT->ForceStyle();
 
 tl = new TLatex;
 tl->SetNDC(kTRUE);
 tl->SetTextSize(0.1);
 
 ts = new TLatex;
 ts->SetNDC(kTRUE);
 ts->SetTextSize(0.05);
 
 ti = new TLatex;
 ti->SetNDC(kTRUE);
 ti->SetTextSize(0.07);
 
 line = new TLine;
 line->SetLineColor(kRed);
 line->SetLineStyle(kDashed);
 
 Rocline = new TLine;
 Rocline->SetLineColor(kBlack);
 Rocline->SetLineStyle(kDashed);
 Rocline->SetLineWidth(1);
 
 
 box = new TBox;
 box->SetFillColor(3);
 box->SetFillStyle(3004);
 
 
 //PIXEL ALIVE MAP
 c1 = new TCanvas("c1", "", 900, 500);
 c1->Clear();
 c1->Divide(1,2);
 
 TH2D *mAlive     = new TH2D("mAlive","", 416, 0., 416., 160, 0., 160.); 
 
 c1->cd(1);
 mAlive->SetMaximum(10.);
 mAlive->SetMinimum(0.);  
 
 for (int i = startChip; i < startChip+nChips; i++) { 
    
   TFile *f = new TFile(Form("%s/roc%i.root", dirName, i));
   if(f->IsOpen()){
     cout<<"... file: "<<gFile->GetName()<<endl;
     printf("...opened successfully\n");
     f->cd("PixelAlive;1");
     addChip("PixelAlive",i, mAlive); 
     f->Close();
   }
   else
     {
       cout<<"Roc not tested yet!!"<<endl;
     }
 }
  
 if ( mAlive ) {   
   mAlive->DrawCopy("colz");
   Rocline->DrawLine(0., 80., 416., 80.);
   Rocline->DrawLine(52., 0., 52., 160.);
   Rocline->DrawLine(104., 0., 104., 160.);
   Rocline->DrawLine(156., 0., 156., 160.);
   Rocline->DrawLine(208., 0., 208., 160.);
   Rocline->DrawLine(260., 0., 260., 160.);
   Rocline->DrawLine(312., 0., 312., 160.);
   Rocline->DrawLine(364., 0., 364., 160.);
   ////////////////////////////////////////
   if ( !strcmp(module_type,"c") ) {
     float px=0.12;
     float py=0.015;
     float ppx=px+(7*0.1);
     float ppy=py+0.9;
     for(int p=0; p < nChips;p++){
       if(p<8){
	 ts->DrawLatex(px, py, Form("ROC %i",p));
	 px=px+0.1;
       }
       if(p>7){
	 ts->DrawLatex(ppx,ppy, Form("ROC %i",p));
	ppx=ppx-0.1;
       }
       
     }
     
   }
    
 }
 cout<<"Pixel Map data taken :-)"<<endl;
  
 
 //BUMP BONDING MAP
 c1->cd(2);
 TH2D *mBumpBond     = new TH2D("mBumpBond","", 416, 0., 416., 160, 0., 160.); 
 mBumpBond->SetMaximum(100.);
 mBumpBond->SetMinimum(0.);  
 
 for (int i = startChip; i < startChip+nChips; i++) { 
   
   TFile *f = new TFile(Form("%s/roc%i.root", dirName, i));
   if(f->IsOpen()){
     
     f->cd("BumpBonding;1");
     addChip("thr_calSMap_VthrComp",i, mBumpBond); 
      
     f->Close();
   }
   else{  
     cout<<"Roc not tested yet!!"<<endl;
    }
 }
 
 if ( mBumpBond ) {   
   mBumpBond->DrawCopy("colz");
   Rocline->DrawLine(0., 80., 416., 80.);
   Rocline->DrawLine(52., 0., 52., 160.);
   Rocline->DrawLine(104., 0., 104., 160.);
   Rocline->DrawLine(156., 0., 156., 160.);
   Rocline->DrawLine(208., 0., 208., 160.);
   Rocline->DrawLine(260., 0., 260., 160.);
   Rocline->DrawLine(312., 0., 312., 160.);
   Rocline->DrawLine(364., 0., 364., 160.);
   ////////////////////////////////////////
   if ( !strcmp(module_type,"c") ) {
     float px=0.12;
     float py=0.015;
     float ppx=px+(7*0.1);
     float ppy=py+0.9;
    for(int p=0; p < nChips;p++){
      if(p<8){
	ts->DrawLatex(px, py, Form("ROC %i",p));
	px=px+0.1;
      }
      if(p>7){
	ts->DrawLatex(ppx,ppy, Form("ROC %i",p));
	ppx=ppx-0.1;
      }
      
    }
    
   }
 }
 
 
 cout<<"Bump Bonding data taken :-)"<<endl;
 cout<<"... files closed."<<endl;
 cout<<"---------------------------------------------------------------"<<endl;

 
 ////////////--> Bump Bonding Distributions ////////////
 c2 = new TCanvas("c2","",1200,500);
 c2->Clear();
 c2->Divide(8,2);
 char cname[20];
 const int size = startChip+nChips;
 
 int j=0;
 if(nChips==8) j=0;
 else j=0;
 outstream<<"Bump Bonding Test Distributions:"<<endl;
 for (int i = startChip; i < startChip+nChips; i++) { 
   
   if(i>7 && nChips==8){
     c2->cd(i+j);
     j--;
     j--;
     gPad->SetLogy(1);
   } 
   if(i>7 && nChips==16){
	    c2->cd(i+j);
	    j-=2;
	    gPad->SetLogy(1);
	    
	  }
   if (i<8){ c2->cd(i+9);
     gPad->SetLogy(1);
   }
   
   TFile *f = new TFile(Form("%s/roc%i.root", dirName, i));
	  
   gStyle->SetOptFit(00111);//Para que te salgan los parametros en el canvas
   gStyle->SetOptFit(0);
   
   if ( f->IsOpen() ) { 
     cout<<"... Next roc"<<endl;
     printf("...opened successfully\n");
     cout<<"... file: "<<gFile->GetName()<<endl;
     
     f->cd("BumpBonding;1");
     TH1D *hb = (TH1D*)gDirectory->Get("dist_thr_calSMap_VthrComp_C0_V0;1");
     
     if ( hb ) {
       
       
       //******NEW CUT*************
       vector<TH1D*> dlist;
       dlist.push_back(hb);
       int nPeaks(0);
       int bbprob(0);
       int cutDead(0);
       string bbString(""),bbCuts("");
       TSpectrum s;
       for (unsigned int l = 0; l < dlist.size(); ++l) {
	 hb = (TH1D*)dlist[l];
	 nPeaks = s.Search(hb, 5, "", 0.01); 
	 cout<<"For roc: "<<i<<endl; 
	 cout<< "found " << nPeaks << " peaks in " << hb->GetName()<<endl;
	 cutDead = fitPeaks(hb, s, nPeaks); 
	 
	 bbprob = static_cast<int>(hb->Integral(cutDead, hb->FindBin(255)));
		bbString += Form(" %4d", bbprob); 
		bbCuts   += Form(" %4d", cutDead);
		cout<<"*********************************"<<endl; 
		cout<<"Roc: "<<i<<endl; 
		cout<<"Missing Bonds: "<<bbString<<endl;
		cout<<"Separation cut:  "<<cutDead<<endl;
		cout<<"*********************************"<<endl;
		outstream<<"*********************************"<<endl;
		outstream<<"Roc: "<<i<<endl; 
		outstream<<"Missing Bonds: "<<bbString<<endl;
		outstream<<"Separation cut:  "<<cutDead<<endl;
		
       }
       
       //**************************
       
       
       hb->SetTitle("");
       hb->SetFillColor(kYellow);
       hb->GetXaxis()->SetRangeUser(0,150.);
       gPad->SetLogy(1);
       hb->DrawCopy();
       
       line->DrawLine(cutDead, 0,cutDead, 300.);
       tl->DrawLatex(0.7, 0.92, Form("Chip %i", i));
     }
     f->Close(); 
   }
   else{
     cout<<"Roc not tested yet!!!"<<endl;
   }
   
 }
 /////////////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////////////
 TCanvas *c3 = new TCanvas("c3","",1100,300);
 c3->Clear();
 c3->Divide(2,1);
 c3->cd(1);
 char ifname[40];
 //IV
 cout<<"------------------- I-V data analysis --------------------"<<endl;
 gPad->SetBottomMargin(0.2);
 gPad->SetLogy(1);
 gPad->SetLeftMargin(0.20);
 gPad->SetRightMargin(0.01);
 
 sprintf(ifname, "%s/ivCurve.log", dirName);
 cout<<ifname<<endl;
 addIV(dirName, ifname);
 ///////////////////////////////////////////////////////////////////////////////////////////// 
 /////////////////////////////////////////////////////////////////////////////////////////////
 c3->cd(2);
 cout<<"----------------Bare Module Test Summary-----------------------"<<endl;
 //Summary 
 int nDeadPixel(0);
 int nMaskDefect(0);
 int nNoisyPixel(0);
 int nDeadBumps(0);
 
 for (int i = startChip; i < startChip+nChips; i++) { 
   
   TFile *f = new TFile(Form("%s/roc%i.root", dirName, i));
    if(f->IsOpen()){
      f->cd("PixelAlive;1");
      TH2D *h2 = (TH2D*)gDirectory->Get("PixelAlive_C0_V0;1");
      if (h2) {
	for (int icol = 0; icol < 52; ++icol)
	  {
	    for (int irow = 0; irow < 80; ++irow)
	      {
		if (h2->GetBinContent(icol+1, irow+1)  == 0)
			{
			  ++nDeadPixel;
			  outstream<<"Dead Pixel at Roc: "<<i<<", Pixel:("<<icol+1<<", "<<irow+1<<")"<<endl;
			  //cout<<"Dead Pixel at Roc: "<<i<<", Pixel:("<<icol+1<<", "<<irow+1<<")"<<endl;
			}
		if (h2->GetBinContent(icol+1, irow+1)  > 10)
		  {
		    ++nNoisyPixel;	
		  }
		if (h2->GetBinContent(icol+1, irow+1)  < 0)
		  {
		    ++nMaskDefect;
		  }
	      }
	  }
      }     
      f->cd("BumpBonding;1");
      h2 = (TH2D*)gDirectory->Get("thr_calSMap_VthrComp_C0_V0;1");
      
      
      if (h2) {
	
      for (int icol = 0; icol < 52; ++icol)
	{
	  for (int irow = 0; irow < 80; ++irow)
	    {  
	      if ( h2->GetBinContent(icol+1, irow+1)  >= cutDead )
		{
		  ++nDeadBumps;
		  //cout<<"Dead bump at Roc: "<<i<<", Pixel:("<<icol+1<<", "<<irow+1<<")"<<endl;
		  
		  
		}
	    }
	  
	  
	} 
      
      }
      
      
    }
    
 }
 
 // -- Defects
 tl->SetTextSize(0.10);
 tl->SetTextFont(22);
 double y = 0.96;
 y -= 0.11;
 TString noslash(dirName);
 TSubString sub=noslash(0, 15);
 TString sdirName(sub);
 sdirName.Data();
 cout<<sdirName<<endl;
 tl->DrawLatex(0.3, y, "Summary:");
 y-=0.11;
 tl->DrawLatex(0.3, y, sdirName);
 
 tl->SetTextFont(132);
 tl->SetTextSize(0.09);
 y -= 0.11;
 tl->DrawLatex(0.3, y, Form("Dead Pixels (=0): "));
 tl->DrawLatex(0.65, y, Form("%4d", nDeadPixel));

 y -= 0.10;
 tl->DrawLatex(0.3, y, Form("Noisy Pixels (>10): "));
 tl->DrawLatex(0.65, y, Form("%4d", nNoisyPixel));
 
 y -= 0.10;
 tl->DrawLatex(0.3, y, "Mask defects (<0): ");
 tl->DrawLatex(0.65, y, Form("%4d", nMaskDefect));
 
 y -= 0.10;
 tl->DrawLatex(0.3, y, "Missing Bumps: ");
 //tl->DrawLatex(0.3, y, "Dead Bumps (nn): ");
 tl->DrawLatex(0.65, y, Form("%4d", nDeadBumps));
 //tl->DrawLatex(0.65, y, Form("%4d", 34));

 ///////////////////////////////////////////////////
 //  c1->SaveAs(Form("plots/%s_%s.gif", dirName, "maps"));
 //  c2->SaveAs(Form("plots/%s_%s.gif", dirName, "BBDist"));
 //  c3->SaveAs(Form("plots/%s_%s.gif", dirName, "IV"));
 //////////////////////////////////////////////////
 
 outstream.close();
 cout << "closing log file... "<<endl;
 
 printf("\nmoduleSummary> ................................................ finished\n");

}


// ----------------------------------------------------------------------
void addChip(const char *hist, int chip, TH2D *h3) {
  
  int icol, irow, value;
  
  TH2D *h2 = (TH2D*)gDirectory->Get(Form("%s_C0_V0;1", hist));
  
  
  if ( h2 ) {
    
    for (icol = 0; icol < 52; icol++) {
      for (irow = 0; irow < 80; irow++)  {
	
	value = h2->GetBinContent(icol+1, irow+1);
	if (chip < 8) {h3->SetBinContent(chip*52+icol+1, irow+1, value);
	}
	if (chip > 7) {h3->SetBinContent(415-((chip-8)*52+icol)+1, 160-irow, value);
	  
	}
      }
    }
 } 
  
  
  
}
//----------------------------------------------------------
int fitPeaks(TH1D *h, TSpectrum &s, int npeaks) {
  
  Float_t *xpeaks = s.GetPositionX();
  string name; 
  double lcuts[3]; lcuts[0] = lcuts[1] = lcuts[2] = 255.;
  TF1 *fg(0); 
  double peak, sigma;
  int fittedPeaks(0); 
  
  for (int p = 0; p < npeaks; ++p) {
    double xp = xpeaks[p];
  //cout<< "xpeaks ["<<p<<"]: "<<xp<<endl;
    if (p > 1) continue;
    if (xp > 200) {
      continue;
    }
    name = Form("gauss_%d", p); 
    fg = new TF1(name.c_str(), "gaus(0)", 0., 256.);
    int bin = h->GetXaxis()->FindBin(xp);
    double yp = h->GetBinContent(bin);
    fg->SetParameters(yp, xp, 2.);
    h->Fit(fg, "Q+"); 
    cout<<"xp: "<<xp<<", yp: "<<yp<<endl;
    ++fittedPeaks;
    peak = h->GetFunction(name.c_str())->GetParameter(1); 
    sigma = h->GetFunction(name.c_str())->GetParameter(2); 
    if (0 == p) {
      lcuts[0] = peak + 3*sigma; 
      if (h->Integral(h->FindBin(peak + 10.*sigma), 250) > 10.) {
  	lcuts[1] = peak + 5*sigma;
      } else {
  	lcuts[1] = peak + 10*sigma;
      }
    } else {
      lcuts[1] = peak - 3*sigma; 
      lcuts[2] = peak - sigma; 
    }
    delete fg;
  }
  
  int startbin = (int)(0.5*(lcuts[0] + lcuts[1])); 
  int endbin = (int)(lcuts[1]); 
  if (endbin <= startbin) {
    endbin = (int)(lcuts[2]); 
    if (endbin < startbin) {
      endbin = 255.;
    }
  }
  
  int minbin(0); 
  double minval(999.); 
  
  for (int i = startbin; i <= endbin; ++i) {
    if (h->GetBinContent(i) < minval) {
      if (1 == fittedPeaks) {
  	if (0 == h->Integral(i, i+4)) {
  	  minval = h->GetBinContent(i); 
  	  minbin = i; 
  	} else {
  	  minbin = endbin;
  	}
      } else {
  	minval = h->GetBinContent(i); 
  	minbin = i; 
      }
    }
  }
  
  cout << "cut for dead bump bonds: " << minbin << " (obtained for minval = " << minval << ")" << endl;
  cout << " start: " << startbin << " .. " << endbin << endl;
  cout << " last peak: " << peak << " last sigma: " << sigma << endl; 
  cout << " lcuts[0] = " << lcuts[0] << " lcuts[1] = " << lcuts[1]<<endl;
  return minbin+1; 
  
}

void addIV(const char*dirName, const char *ifname){

 float V[300],I[300];
 float x_V[300],y_I[300]; 
 float iv100(0.);
 float iv150(0.);
 char date[300];
 char timestamp[400];
 float variation(0.);
 
 ifstream ivf(ifname);
 string row;

 if (!ivf.is_open()) {
   printf("moduleSummary> !!!!!!!!!  ----> Could not open file to read data\n");
   perror("Why?");
 }
 
 else {
   printf("File opened!!\n");
 }
 char ofname[40];
 sprintf(ofname,"%s/iv.dat", dirName);
 ofstream tmp(ofname);
    while (!ivf.eof()) {
	getline(ivf,row);
	if (row.length() == 0 || row[0] == '#')
	  cout<<"ignoring line #"<<endl;
	else{
	  //cout << row << "\n";
	  tmp<<row<<endl;
	}  
    }

ivf.close(); 

 char ivname[300];
 sprintf(ivname,"%s/iv.dat", dirName);
 FILE *ivFile; 
 ivFile = fopen(ivname, "r"); 
 int i = 0;
 int ncol=0;
  
   while(1){
       ncol=fscanf(ivFile,"%f %f %*s %*s",&x_V[i],&y_I[i],&date[i],&timestamp[i]);
       
       if(ncol<0) break;
       I[i]=fabs(y_I[i]/1.0e-6);
       V[i]=fabs(x_V[i]);
       //cout<<i<<endl;
       //std::cout<<V[i]<<"  ,   "<<I[i]<<std::endl;
       if(i>0){	
	 // check that voltage is increasing & find current at 150 V 
	 if ( V[i] < V[i-1] ) { cout<<"voltage is decreasing"<<endl; }
	 if ( V[i] == 100. ) { iv100 = I[i];
	   cout<<"V="<<V[i]<<": "<<"I_leak="<<iv100<<endl;  
	 }
	 if ( V[i] == 150.) { iv150 = I[i];
	   cout<<"V="<<V[i]<<": "<<"I_leak="<<iv150<<endl; 
	 } 
	 
       } 
     
     i++;
   }
   
   int n=i;	
   std::cout<<"There are "<<n<<" measures"<<std::endl;
   std::cout<<"Done."<<std::endl;
   
   
   if ( iv100 != 0. ) { variation = iv150/iv100; }
   else               { variation = 0; }
   
   
   //Normalizing the current at 150V --> USE BMT_sensor.C
   //const float Tn=293.15;//20C in Kelvin
   //float Tk=T+273.15;//Temp in Kelvin
   //const float Eg=1.2;//eV
   //const float kb=8.62E-5;//eV/K
   //ivn150=iv150*pow((Tn/Tk),2)*exp((Eg/(2*kb))*((1/Tn)-(1/Tk)));
   //cout<<"T="<<Tk<<"K ("<<T<<"C)"<<", Tn="<<Tn<<"K (20C)"<<endl;
   //cout<<"I (150V)="<<iv150<<"A"<<endl;
   //cout<<"Inorm (150V)="<<ivn150<<"A"<<endl;
       
   cout<<"I_leak(100V)= "<<iv100<<", I_leak(150V)= "<<iv150<<endl;
   cout<<"i(150V)/i(100V)= "<<variation<<endl;
   
   if(iv150 < 2.) {
     cout<<"good! The leakage current at 150V is less than 2"<<endl; 
   }
    else {
      cout<<"what are you doing?!!!!!"<<endl; 
    }
   
   fclose(ivFile);  
   
   
   volt = new TLine;
   curr = new TLine;
   volt->SetLineColor(kRed);
   volt->SetLineStyle(kDashed);
   curr->SetLineColor(kRed);
   curr->SetLineStyle(kDashed);
   TGraph *g1 = new TGraph(n,V,I);
   
   
   TF1 *func = new TF1("func","pol1",50.,200);
   gStyle->SetOptFit(0);
   func->SetParameters(0,1);
   
   g1->SetTitle("");
   g1->SetLineColor(4);
   g1->SetLineWidth(2);
   
   g1->GetXaxis()->SetTitle("Voltage [V]");
   g1->GetYaxis()->SetTitle("Current [#muA]");
   g1->GetYaxis()->SetDecimals();
   g1->GetYaxis()->SetTitleOffset(0.6);
   g1->GetYaxis()->CenterTitle();
   g1->Draw("aC");
   
   tl->DrawLatex(0.2, 0.92, "I-V Curve");
   ti->DrawLatex(0.55, 0.41, Form("I(150V) = %.2f #muA (< 2#muA)", iv150));
   ti->DrawLatex(0.55, 0.28, Form("I_{150}/I_{100} =  %.2f (< 2)", variation));
   if (I[n-1]>2.){
     curr->DrawLine(0., 2., 600., 2.);
   }
   volt->DrawLine(150., 0., 150., I[n-1]);
  
   
 }


