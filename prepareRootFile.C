#include "TROOT.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TKey.h"
#include "TObject.h"
#include <algorithm>
#include "TStyle.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TMacro.h"
#include "TList.h"
#include "TString.h"
#include "TObject.h"
#include "TH1D.h"
#include "TProfile2D.h"
#include "TLine.h"
#include "string.h"
#include <iostream> // cout
#include <iomanip> //  setw
#include <fstream> // ofstream
#include <sstream> // combine string with integer


//
// Nov. 2014 A.V 
// Merge bare module root files into a single root 
// file for MoReWeb processing
// Use:
// root -b -q prepareRootFile.C("4010")


void prepareRootFile(const char *dirName = "")
{

  //TFile * _fileSave = TFile::Open( Form("BM%ir.root", module ), "new" );
  //TFile * _fileSave = TFile::Open( "pxar.root", "new" );
  TFile * _fileSave = TFile::Open( Form("%s/pxar.root",dirName), "new" );
  //TDirectory *cdPixelAlive = 
  _fileSave->mkdir("PixelAlive");
  _fileSave->mkdir("BumpBonding");

  for( int roc = 0; roc < 16; ++roc ) {

    cout << "----------ROC: " << roc << endl;
    // D4004c15.root

    TFile * F1 = TFile::Open( Form("%s/roc%i.root",dirName, roc ), "read" );
    
    if( ! F1 ) continue;

    TH2D * h20 = NULL;
    int found = 0;

    TIter nextkey( gDirectory->GetListOfKeys() );
    TKey * key;

    while( ( key = (TKey*)nextkey() ) ) {

      TObject * obj = key->ReadObj();
      
      //cout << obj->GetName() << endl;

      if( obj->IsA()->InheritsFrom( "TDirectory" )) {

	TObject * objDir = key->ReadObj();
	//cout << "directory?" << objDir->GetName() << endl;	
	
	string dirNameactual = objDir->GetName();
	//cout << dirNameactual << endl;

	if ((dirNameactual == "PixelAlive") || (dirNameactual == "BumpBonding")){
	  cout << "directory?" << objDir->GetName() << endl;
	  F1->cd(objDir->GetName());

	  TIter nextkey2( gDirectory->GetListOfKeys() );
	  TKey * key2;

	  while( ( key2 = (TKey*)nextkey2() ) ) { //looping over histograms
	    TObject * obj2 = key2->ReadObj();
	    //cout << obj2->GetName() << endl;	    

	    // --------------------------------------------------
	    if( obj2->IsA()->InheritsFrom( "TH2D" )) {

	      TH2D * h2 = (TH2D*)obj2;
	      string hn = h2->GetName();
	      std::ostringstream listName_stream; 
	      listName_stream << "C" << roc;    
	      const std::string valVar = listName_stream.str();

	      ReplaceStringInPlace(hn, "C0",valVar);

	      cout << h2->GetName() << " and " << hn.c_str() << endl;
	      h2->SetName(hn.c_str());
	      _fileSave->cd(objDir->GetName());
	      //_fileSave->cd();
	      h2->Write();

	    }
	    else if ( obj2->IsA()->InheritsFrom( "TH1D" )) {
	      
	      TH1D * h1 = (TH1D*)obj2;
	      string hn1 = h1->GetName();
	      std::ostringstream listName_stream1; 
	      listName_stream1 << "C" << roc;    
	      const std::string valVar1 = listName_stream1.str();

	      ReplaceStringInPlace(hn1, "C0",valVar1);

	      cout << h1->GetName() << " and " << hn1.c_str() << endl;
	      h1->SetName(hn1.c_str());
	      _fileSave->cd(objDir->GetName());
	      //_fileSave->cd();
	      h1->Write();


	    }
	    //-------------------------------------------

	  } // end histogram loop
	} // Pixel alive 
		
      } // end directory loop
    }
    F1->Close();
  } // end loop over roc files

  _fileSave->Close();
 
}

void ReplaceStringInPlace(std::string& subject, const std::string& search,
                          const std::string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
}


