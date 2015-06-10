#include <TMath.h>


//------------------------------------------
// Bare Module Test for single ROC summary page producer
// updated by M. Miñano 2014
// 
// Usage:
//.L BMT_chipSummary.C
//chipSummary("<folder>","<root file>","ChipINFO",0)
//  
//--------------------------------------------

// -- data files ----------------------------------------------------------------------------------
//const char* fileName = "BMTest.root";

float defectsB, defectsC, maskdefB,  maskdefC;
float currentB, currentC, slopeivB,  slopeivC;
float noiseB,   noiseC,   trimmingB, trimmingC;
float gainB,    gainC,    pedestalB, pedestalC;
float pedDistr, gainDistr, trmDistr;

int nChips(16);
int startChip(0);
int readVerbose(1);

char fname[200];
FILE *inputFile, *critFile;

TFile *f, *f1;

TCanvas* c1 = NULL;
TCanvas* c11= NULL; 
TCanvas* c2 = NULL; 
TCanvas* c3 = NULL; 
TLatex *tl;
TLatex *ts;
TLine *line;
TBox *box;


Double_t g(Double_t *v, Double_t *par)
	  {
	    Double_t arg = 0;
	    if(par[2]!=0) arg = (v[0] - par[1]) / par[2];
	    
	    Double_t area = par[0]/(par[2]*sqrt(2*3.1415926));
	    Double_t gaus = area*TMath::Exp(-0.5*arg*arg);
	    
	    //if (gaus<=0) gaus=1e-10;
	    return gaus;
	  }



void chipSummary(const char *dirName,const char *fileName, int chipId)
{

  
 
	if (f && f->IsOpen()) f->Close();
	//if (f1 && f1->IsOpen()) f1->Close();

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
	gStyle->SetNdivisions(6, "X");
	gStyle->SetNdivisions(8, "Y");
	gStyle->SetTitleFont(132);

	gROOT->ForceStyle();

	tl = new TLatex;
	tl->SetNDC(kTRUE);
	tl->SetTextSize(0.09);

	ts = new TLatex;
	ts->SetNDC(kTRUE);
	ts->SetTextSize(0.08);

	line = new TLine;
	line->SetLineColor(kRed);
	line->SetLineStyle(kSolid);
	
	box = new TBox;
	box->SetFillColor(kRed);
	box->SetFillStyle(3002);

	f = new TFile(Form("%s/%s", dirName, fileName), "READ");
	
       	TH1D *h1;
	TH2D *h2;
	//const int size = startChip+nChips;
	//float bblimit[size]=0;
       

	c1 = new TCanvas("c1", "", 700, 500);
	c1->Clear();
	c1->Divide(2,2, 0.01, 0.04);
	c11 = new TCanvas("c11", "", 500, 300);

        TString noslash(dirName);
        noslash.ReplaceAll("/", " ");
        noslash.ReplaceAll(".. ", "");
	
	
	// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	// Row 1
	// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

	// -- Dead pixels
	TH2D *hpm = new TH2D("hpm", "", 80, 0., 80., 52, 0., 52.);

	int nDeadPixel(0.);
	int nMaskDefect(0.);
	int nNoisyPixel(0.);
	int nRootFileProblems(0);
	double noise[53][81];

	c1->cd(1);
	f->cd("PixelAlive;1");
	h2 = (TH2D*)gDirectory->Get(Form("PixelAlive_C%i_V0;1", chipId));
	ofstream OUT(Form("Final/Noise_%s-%s.txt", dirName,fileName));
        if (h2) {
	  for (int icol = 0; icol < 52; ++icol)
	  {
		for (int irow = 0; irow < 80; ++irow)
		{
		        hpm->SetBinContent(irow+1, icol+1, h2->GetBinContent(icol+1, irow+1));

			if (h2->GetBinContent(icol+1, irow+1)  == 0)
			{
			  OUT << "Dead Pixel: "        << nDeadPixel << endl;
			  OUT << "Pixel ["<<icol+1<<"]["<<irow+1<<"] "<<"Noise= "<<noise[icol+1][irow+1]<<endl;
				++nDeadPixel;
			}
			if (h2->GetBinContent(icol+1, irow+1)  > 10)
			{
			  noise[icol+1][irow+1]=h2->GetBinContent(icol+1,irow+1);
			  cout<<noise[icol+1][irow+1]<<endl;
			 

			  OUT << "nNoisyPixel: "        << nNoisyPixel << endl;
			  OUT << "Pixel ["<<icol+1<<"]["<<irow+1<<"] "<<"Noise= "<<noise[icol+1][irow+1]<<endl;
				++nNoisyPixel;
				
			}
			if (h2->GetBinContent(icol+1, irow+1)  < 0)
			{
				++nMaskDefect;
			}
		}
	  }
	  h2->SetTitle("");
	  h2->Draw("colz");
	  tl->DrawLatex(0.1, 0.92, "Pixel Map");
	  OUT.close();
	}

	else { ++nRootFileProblems; }

	// -- Tornado Test
	c1->cd(2);
	if (!f->IsZombie())
	{
	      f->cd("Pretest;1");
	      h1 = (TH1D*)gDirectory->Get(Form("pretestVthrCompCalDel_c11_r20_C%i_V0;1", chipId));
	      if(!h1){h1 = (TH1D*)gDirectory->Get(Form("pretestVthrCompCalDel_c12_r22_C%i_V0;1", chipId));}
              if (h1) {
		h1->SetTitle("");
		//h1->SetAxisRange(0., 100.);
		h1->Draw("colz");
		tl->DrawLatex(0.1, 0.92, "VthrComp vs CalDel");
		
              }
              else {

	        ++nRootFileProblems;
       
                       }
	     
	     	}

	
	// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	// Row 2
	// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

	// -- Bump Bonding Distribution
	c1->cd(3);
	TF1 *fung = new TF1("fung",g,0.,100.,3.);
	gStyle->SetOptFit(00111);//Para que te salgan los parametros en el canvas
	gStyle->SetOptFit(0);
	fung->SetLineWidth(2);
	gPad->SetLogy(1);
	if (!f->IsZombie())
	  {
	    
	    f->cd("BumpBonding;1");
	    h1 = (TH1D*)gDirectory->Get(Form("dist_thr_calSMap_VthrComp_C%i_V0;1", chipId));
	    if (h1) {
	      h1->SetTitle("");
	      h1->SetFillColor(kYellow);
	      //h1->SetAxisRange(5., 100.);
	      
	    fung->SetParameters(1,h1->GetMean(),h1->GetRMS());
	    fung->SetParNames("Constant","Mean_value","Sigma");
	    //h1->Fit(fung,"0q"); FIXME
	    h1->Draw();
	    //line->DrawLine(fung->GetParameter(1)+fung->GetParameter(2)*5, 0, fung->GetParameter(1)+fung->GetParameter(2)*5, 300.);
	    tl->DrawLatex(0.1, 0.92, "BumpBond Distribution");
	    

	    c3 = new TCanvas("c3", "", 700, 500);
	    c3->cd();
	    gPad->SetLogy(1);
	    h1->DrawCopy();
	    //line->DrawLine(fung->GetParameter(1)+fung->GetParameter(2)*5, 0, fung->GetParameter(1)+fung->GetParameter(2)*5, 300.);
	    //line->DrawLine(89., 0, 89., 200.);
	    tl->DrawLatex(0.1, 0.92, "BumpBond Distribution");
		
              }
              else {

	        ++nRootFileProblems;
       
                       }
	     
	     	}

	     
	// -- Bump Map
	TH2D *hbm = new TH2D("hbm", "", 80, 0., 80., 52, 0., 52.);

	int nDeadBumps(0);

	c1->cd(4);
	//f->cd("BumpBonding;1");
	gStyle->SetOptStat(0);
	h2 = (TH2D*)gDirectory->Get(Form("thr_calSMap_VthrComp_C%i_V0;1", chipId));


        double minThrDiff(fung->GetParameter(1)+fung->GetParameter(2)*5);
	double maxThrDiff(0.);
        if (h2) {

	  for (int icol = 0; icol < 52; ++icol)
	  {
		for (int irow = 0; irow < 80; ++irow)
		{
		        hbm->SetBinContent(irow+1, icol+1, h2->GetBinContent(icol+1, irow+1));

			if ( h2->GetBinContent(icol+1, irow+1)  >= minThrDiff )
			{
			  cout << Form("BMT-Summary> missing bumps: %3d %3d, VthrCom: %7.5f", icol, irow, h2->GetBinContent(icol, irow)) << endl;
				++nDeadBumps;
			}
		}
	  }

	  h2->SetTitle("");
	  //h2->GetZaxis()->SetRangeUser(maxThrDiff, minThrDiff);
	  h2->SetMinimum(0);
	  h2->Draw("colz");
	  tl->DrawLatex(0.1, 0.92, "Bump Bonding Map");
	  c2 = new TCanvas("c2", "", 700, 500);
	  c2->cd();	  
	  h2->Draw("colz");
	  tl->DrawLatex(0.1, 0.92, "Bump Bonding Map");
	}

	else { ++nRootFileProblems; }

      
	
	// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	// Numerics and Titles
	// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

	// -- Compute the final verdict on this chip  //?? FIXME (below is pure randomness)
	//char finalVerdict(0);
		/*
	if (nDeadPixel > 40) finalVerdict += 10;
	if (nNoisyPixel > 40) finalVerdict += 10;
	if (nDeadBumps > 40) finalVerdict += 100;
		*/

	// -- Defects
	c11->cd();
	tl->SetTextSize(0.10);
	tl->SetTextFont(22);
	double y = 0.92;
	y -= 0.11;
	tl->DrawLatex(0.1, y, "Summary");
	//tl->DrawLatex(0.7, y, Form("%d", finalVerdict));

	tl->SetTextFont(132);
	tl->SetTextSize(0.09);
	y -= 0.11;
	tl->DrawLatex(0.1, y, Form("Dead Pixels: "));
	tl->DrawLatex(0.7, y, Form("%4d", nDeadPixel));

	y -= 0.10;
	tl->DrawLatex(0.1, y, Form("Noisy Pixels: "));
	tl->DrawLatex(0.7, y, Form("%4d", nNoisyPixel));
	
	y -= 0.10;
	tl->DrawLatex(0.1, y, "Mask defects: ");
	tl->DrawLatex(0.7, y, Form("%4d", nMaskDefect));

	y -= 0.10;
	tl->DrawLatex(0.1, y, "Dead Bumps: ");
	tl->DrawLatex(0.7, y, Form("%4d", nDeadBumps));



     	// -- Operation Parameters
	/*
	c1->cd(3);
		
	y = 0.92;
	tl->SetTextSize(0.10);
	tl->SetTextFont(22);
	y -= 0.11;
	tl->DrawLatex(0.1, y, Form("Op. Parameters"));

	tl->SetTextFont(132);
	tl->SetTextSize(0.09);
	
	y -= 0.11;
	int vana(-1.);
        vana = dac_findParameter(dirName, "Vana", chipId);
	tl->DrawLatex(0.1, y, "VANA: ");
	if (vana >= 0.) tl->DrawLatex(0.6, y, Form("%3i DAC", vana));
	else tl->DrawLatex(0.7, y, "N/A");

	y -= 0.10;
	int caldel(-1.);
        caldel = dac_findParameter(dirName, "CalDel", chipId);
	tl->DrawLatex(0.1, y, "CALDEL: ");
	if (vana >= 0.) tl->DrawLatex(0.6, y, Form("%3d DAC", caldel));
	else tl->DrawLatex(0.7, y, "N/A");

	y -= 0.10;
	int vthrcomp(-1.);
        vthrcomp = dac_findParameter(dirName, "VthrComp", chipId);
	tl->DrawLatex(0.1, y, "VTHR: ");
	if (vana >= 0.) tl->DrawLatex(0.6, y, Form("%3d DAC", vthrcomp));
	else tl->DrawLatex(0.7, y, "N/A");
	*/
	
	// -- Page title
	c1->cd(0);
	tl->SetTextSize(0.035);
	tl->SetTextFont(22);
	tl->DrawLatex(0.02, 0.97, Form("%s ", noslash.Data()));

	TDatime date;
	tl->SetTextSize(0.035);
	tl->DrawLatex(0.75, 0.97, Form("%s", date.AsString()));

	//c1->SaveAs(Form("Final/BMT-Summary%s_C%s.gif", dirName, fileName));
	//c2->SaveAs(Form("%s/%s_%s.gif", dirName, fileName,"BBMap"));
	//c3->SaveAs(Form("%s/%s_%s.gif", dirName, fileName,"BBdist"));
	//c1->SaveAs(Form("%s/C%i_%i.pdf", dirName, fileName));
	
	/*
	// -- Dump into logfile
	ofstream OUT(Form("%s/summary_C%i.txt", dirName,chipId));
	OUT << "nDeadPixel: "         << nDeadPixel << endl;
	OUT << "nNoisyPixel: "        << nNoisyPixel << endl;
	OUT << "nDeadBumps: "         << nDeadBumps << endl;
	OUT << "nMaskDefect: "        << nMaskDefect << endl;
        OUT << "nRootFileProblems: "  << nRootFileProblems << endl;
	OUT.close();
	*/
}

//-------------------------------------------------------------------------
int dac_findParameter(const char *dir, const char *dacPar, int chipId) {

  FILE *File, *File50, *File60;
  char fname[1000];
  char string[2000]; int a;
  int prm(-1);
   
  sprintf(fname, "%s/dacParameters_C%i.dat", dir, chipId);
  File = fopen(fname, "r");
  
  sprintf(fname, "%s/dacParameters50_C%i.dat", dir, chipId);
  File40 = fopen(fname, "r");
  
  sprintf(fname, "%s/dacParameters60_C%i.dat", dir, chipId);
  File60 = fopen(fname, "r");
  
  if ( File )
  {
    if ( !strcmp(dacPar,"Vana") ) {
      printf("\nBMT-Summary> Reading dac Parameters for chip %i ...\n", chipId);
    }
    for (int i = 0; i < 29; i++) {
  
      fscanf(File, "%i %s %i", &a, string, &prm);
      if ( strcmp(dacPar,string) == 0 )  break;

    }
  }
  
  if ( File60 && !File )
  {
    if ( !strcmp(dacPar,"Vana") ) {
      printf("\nBMT-Summary> Reading dac Parameters Vcal = 60 for chip %i ...\n", chipId);
    }
    for (int i = 0; i < 29; i++) {
  
      fscanf(File60, "%i %s %i", &a, string, &prm);
      if ( strcmp(dacPar,string) == 0 )  break;

    }
  }
  
  if (!File60 && !File)
  { 
    
    for (int i = 0; i < 29; i++) {
  
      fscanf(File40, "%i %s %i", &a, string, &prm);
      if ( strcmp(dacPar,string) == 0 )  break;

    }
    
    if ( !File40 )
    {
      printf("\nBMT-Summary> !!!!!!!!!  ----> DAC Parameters: Could not find a file to read DAC parameter\n\n");
      return 0;
    }
    else
    {
    printf("\nBMT-Summary> No DAC Parameters after trimming available. Reading %s ...\n\n", dacPar);
    }

  }
  
  if (File)   fclose(File);
  if (File40) fclose(File40);
  if (File60) fclose(File60);
  
  return prm;
}

