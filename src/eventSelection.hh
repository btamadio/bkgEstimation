#include "TLatex.h"
#include "TLorentzVector.h"
#include "TMath.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TCanvas.h"
#include "RooPlot.h"
#include "TROOT.h"
#include "THStack.h"
#include "TString.h"
#include "TH1.h"
#include "TH2F.h"
#include "TRandom.h"
#include "TRandom3.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include "../aux/nominalTree.hh"
using namespace std;
#include <sys/stat.h>

nominalTree *p;

map<int,vector<pair<int,int> > >  m_GRL;
string m_GRLFileName;

void get_event(int i) {
  if ( p->LoadTree(i) < 0) { 
    cout<<"\nProblem in LoadTree."
        <<"\nEntry: "<<i<<endl;
    exit(0);
  }
  p->fChain->GetEntry(i);
}


double Nevent( int Run){
  double Nevent = 1;

  //  new_Pythia.txt
  if(Run == 361022) Nevent = 1994600;
  if(Run == 361023) Nevent = 7884500;
  if(Run == 361024) Nevent = 7979800;
  if(Run == 361025) Nevent = 7977600;
  if(Run == 361026) Nevent = 1893400;
  if(Run == 361027) Nevent = 1770200;
  if(Run == 361028) Nevent = 1743200;
  if(Run == 361029) Nevent = 1813200;
  if(Run == 361030) Nevent = 1996000;
  if(Run == 361031) Nevent = 1993200;
  if(Run == 361032) Nevent = 1974600;

//  Sherpa.EXOT3.txt  
if(Run == 426132) Nevent = 1968000.000000;
if(Run == 426133) Nevent = 1886995.000000;
if(Run == 426134) Nevent = 1984000.000000;
if(Run == 426135) Nevent = 1961500.000000;
if(Run == 426136) Nevent = 1963800.000000;
if(Run == 426137) Nevent = 955000.000000;
if(Run == 426138) Nevent = 977400.000000;
if(Run == 426139) Nevent = 465100.000000;
if(Run == 426140) Nevent = 90000.000000;
if(Run == 426141) Nevent = 553150.000000;
if(Run == 426142) Nevent = 436300.000000;

  //  new_Herwig.txt
  if(Run == 426042) Nevent = 1938500;
  if(Run == 426043) Nevent = 1905500;
  if(Run == 426044) Nevent = 1926500;
  if(Run == 426046) Nevent = 1930500;
  if(Run == 426047) Nevent = 1927500;
  if(Run == 426048) Nevent = 1928500;
  if(Run == 426049) Nevent = 387000;
  if(Run == 426050) Nevent = 456000;
  if(Run == 426051) Nevent = 418500;
  if(Run == 426052) Nevent = 402000;


  //  fileList.mc15_13TeV.gammajet.2016_07_13_v1.txt
  if(Run == 423105) Nevent = 967199;
  if(Run == 423106) Nevent = 994200;
  if(Run == 423109) Nevent = 98300;
  if(Run == 423104) Nevent = 996400;
  if(Run == 423107) Nevent = 92000;
  if(Run == 423108) Nevent = 99500;
  if(Run == 423110) Nevent = 94700;
  if(Run == 423103) Nevent = 2883800;

  //AFII Signal
  if (Run == 403550) Nevent = 20000;    
  if (Run == 403551) Nevent = 20000;    
  if (Run == 403552) Nevent = 20000;    
  if (Run == 403553) Nevent = 78000;
  if (Run == 403554) Nevent = 30000; 
  if (Run == 403555) Nevent = 20000;   
  if (Run == 403556) Nevent = 20000;   
  if (Run == 403557) Nevent = 78000;
  if (Run == 403558) Nevent = 28000;   
  if (Run == 403559) Nevent = 20000;  
  if (Run == 403560) Nevent = 20000;  
  if (Run == 403561) Nevent = 20000;   
  if (Run == 403562) Nevent = 20000;  
  if (Run == 403563) Nevent = 20000;
  if (Run == 403564) Nevent = 20000;
  if (Run == 403565) Nevent = 20000;
  if (Run == 403566) Nevent = 20000;
  if (Run == 403567) Nevent = 20000;
  if (Run == 403568) Nevent = 20000;
  if (Run == 403569) Nevent = 20000;
  if (Run == 403570) Nevent = 19000;
  if (Run == 403571) Nevent = 20000;
  if (Run == 403572) Nevent = 20000;
  if (Run == 403573) Nevent = 20000;
  if (Run == 403574) Nevent = 20000;
  if (Run == 403575) Nevent = 20000;
  if (Run == 403576) Nevent = 20000;
  if (Run == 403577) Nevent = 20000;
  if (Run == 403578) Nevent = 20000;
  if (Run == 403579) Nevent = 20000;
  if (Run == 403580) Nevent = 20000;
  if (Run == 403581) Nevent = 20000;
  if (Run == 403582) Nevent = 20000;
  if (Run == 403583) Nevent = 20000;
  if (Run == 403584) Nevent = 20000;
  if (Run == 403585) Nevent = 20000;
  if (Run == 403586) Nevent = 20000;
  if (Run == 403587) Nevent = 20000;
  if (Run == 403588) Nevent = 20000;
  if (Run == 403589) Nevent = 20000;
  if (Run == 403590) Nevent = 20000;
  if (Run == 403591) Nevent = 20000;
  if (Run == 403592) Nevent = 20000;
  if (Run == 403593) Nevent = 20000;
  if (Run == 403594) Nevent = 20000;
  if (Run == 403595) Nevent = 20000;
  if (Run == 403596) Nevent = 20000;
  if (Run == 403597) Nevent = 20000;
  if (Run == 403598) Nevent = 20000;
  if (Run == 403599) Nevent = 19000;
  if (Run == 403600) Nevent = 20000;
  if (Run == 403601) Nevent = 20000;
  if (Run == 403602) Nevent = 20000;
  if (Run == 403603) Nevent = 20000;
  if (Run == 403604) Nevent = 19000;
  if (Run == 403605) Nevent = 80000;
  if (Run == 403606) Nevent = 80000;
  if (Run == 403607) Nevent = 40000;
  if (Run == 403608) Nevent = 20000;
  if (Run == 403609) Nevent = 19000;
  if (Run == 403610) Nevent = 20000;
  if (Run == 403611) Nevent = 20000;
  if (Run == 403612) Nevent = 20000;
  if (Run == 403613) Nevent = 20000;
  if (Run == 403614) Nevent = 20000;
  
  //FullSim Signal
  // if (Run == 403558) Nevent = 30000;
  // if (Run == 403560) Nevent = 20000;
  // if (Run == 403563) Nevent = 20000;
  // if (Run == 403571) Nevent = 19000;
  // if (Run == 403573) Nevent = 19000;
  // if (Run == 403578) Nevent = 20000;
  // if (Run == 403587) Nevent = 20000;
  // if (Run == 403589) Nevent = 19000;
  // if (Run == 403595) Nevent = 20000;
  // if (Run == 403605) Nevent = 77000;
  // if (Run == 403610) Nevent = 20000;

  if(Run == 341271) Nevent = 951800.0;

  if(Run == 403577) Nevent = 20000;
  if(Run == 403578) Nevent = 20000;
  //  ttbar.EXOT3.txt
  if(Run == 410039) Nevent = 7666400.000000;
  if(Run == 410044) Nevent = 7974000.000000;

  return Nevent ;

}

double RPV_xsec(int Run) {

  double xsec = 1;

  //cross sections from twiki page, in pb.
  if (Run == 403103 || Run == 403104 || Run == 403105) xsec = 0.0627027;
  if (Run == 403110 || Run == 403111) xsec = 0.163491;
  if (Run == 403113 || Run == 403114 || Run == 403116) xsec = 0.0856418;
  if (Run == 403117 || Run == 403118 || Run == 403119 || Run == 403120) xsec = 0.0460525;
  if (Run == 403121 || Run == 403122 || Run == 403123 || Run == 403124 || Run == 403125) xsec = 0.0252977;
  if (Run == 403126 || Run == 403127 || Run == 403128 || Run == 403129 || Run == 403130) xsec = 0.0141903;
  if (Run == 403131 || Run == 403134 || Run == 403135) xsec = 0.00810078;
  if (Run == 403137 || Run == 403139 || Run == 403142 || Run == 403143) xsec = 0.00470323;
  if (Run == 403144 || Run == 403145 || Run == 403148 || Run == 403149) xsec = 0.00276133;
  if (Run == 403151 || Run == 403153) xsec = 0.000981077;
  if (Run == 403159 || Run == 403166) xsec = 0.000219049;
  if (Run == 403167) xsec = 0.163491;
  if (Run == 403169) xsec = 0.0460525;
  if (Run == 403170) xsec = 0.0252977;
  if (Run == 403171) xsec = 0.0141903;
  if (Run == 403174) xsec = 0.00276133;
  if (Run == 403175) xsec = 0.000981077;
  if (Run == 403550) xsec = 3.5251;
  if (Run == 403551) xsec = 1.4891;
  if (Run == 403552 || Run == 403605) xsec = 0.677478;
  if (Run == 403553 || Run == 403554 || Run == 403555 || Run == 403556 || Run == 403557) xsec = 0.325388;
  if (Run == 403558 || Run == 403559 || Run == 403560 || Run == 403561 || Run == 403562 || Run == 403563) xsec = 0.0856418;
  if (Run == 403564 || Run == 403565 || Run == 403566 || Run == 403567 || Run == 403568 || Run == 403569 || Run == 403570) xsec = 0.0252977;
  if (Run == 403571 || Run == 403572 || Run == 403573 || Run == 403574 || Run == 403575 || Run == 403576 || Run == 403577 || Run == 403578) xsec = 0.00810078;
  if (Run == 403579 || Run == 403580 || Run == 403581 || Run == 403582 || Run == 403583 || Run == 403584 || Run == 403585 || Run == 403586) xsec = 0.00470323;
  if (Run == 403587 || Run == 403588 || Run == 403589 || Run == 403590 || Run == 403591 || Run == 403592 || Run == 403593 || Run == 403594 || Run == 403595) xsec = 0.00276133;
  if (Run == 403596 || Run == 403597 || Run == 403598 || Run == 403599 || Run == 403600 || Run == 403601 || Run == 403602 || Run == 403603 || Run == 403604) xsec = 0.00163547;
  if (Run == 403605) xsec = 0.677478;
  if (Run == 403606) xsec = 0.325388;
  if (Run == 403607) xsec = 0.163491;
  if (Run == 403608) xsec = 0.0856418;
  if (Run == 403609) xsec = 0.0460525;
  if (Run == 403610) xsec = 0.0252977;
  if (Run == 403611) xsec = 0.0141903;
  if (Run == 403612) xsec = 0.00810078;
  if (Run == 403613) xsec = 0.00470323;
  if (Run == 403614) xsec = 0.00276133;


  if (xsec != 1) {
    xsec = xsec/1000; 
    //change to nb.
  }

  return xsec;

}



void readGRL(){
  Int_t RunNumber1 = 0, MinLum1 = 0, MaxLum1 = 0;
  TString line1;
  ifstream fGRL;
  fGRL.open(m_GRLFileName);
  if(!fGRL.is_open()){
    cout<<"ERROR opening GRL"<<endl;
    abort();
  }
  while(fGRL >> line1){
    if(line1.Contains("</Run>")){
      line1.ReplaceAll("PrescaleRD1=\"8\">", "");
      line1.ReplaceAll("<Run>", "");
      line1.ReplaceAll("</Run>", "");
      RunNumber1 = line1.Atoi();
      while(1){
        fGRL>>line1;
        if( line1.Contains("</LumiBlockCollection>") ) break;
        fGRL>>line1;
        line1.ReplaceAll("Start=\"", "");
        line1.ReplaceAll("\"", "");
        MinLum1 = line1.Atoi();
        fGRL>>line1;
        line1.ReplaceAll("End=\"", "");
        line1.ReplaceAll("\"/>", "");
        MaxLum1 = line1.Atoi();
    pair<int,int> goodLBs;
    goodLBs.first = MinLum1;
    goodLBs.second = MaxLum1;
    vector<pair<int,int> > emptyVec;
    if( m_GRL.find(RunNumber1) == m_GRL.end()){
      m_GRL[RunNumber1]=emptyVec;
    }
    m_GRL[RunNumber1].push_back(goodLBs);
      }
    }
  }
}

bool passGRL(int run, int lb){
  for ( auto& x : m_GRL){
    if ( run == x.first ){
      for ( auto&  p : x.second ){
    if ( lb >= p.first && lb <= p.second ){
      return true;
    }
      }
    }
  }
  return false;
}


bool MistimedEvent (Int_t run, long long event){
  bool result = false;
  long long EventNumber1;
  TString line1, r;
  ifstream fList;
  fList.open("/project/projectdirs/atlas/msilva/RPV/RPV_analyzer/GRL/eventlist.txt");
  if(fList.is_open() == 0 ){
    cout << "ERROR openning eventlist.txt" << endl;
    abort();
  }

  while( fList >> line1 ){
    r = std::to_string(run);
    if( line1.Contains(r) ){
      line1.ReplaceAll(r+"|", "");
      EventNumber1 = line1.Atoll();
      if( event == EventNumber1 ) result = true;
    }
  }
  return result;
}

void reorder(vector<double> &v, vector<int> const &order )  {   
    for (unsigned int s = 1, d; s < order.size(); ++ s ) {
      for ( d = order[s]; d < s; d = order[d] ) ;
      if ( d == s ) while ( d = order[d], d != s ) swap( v[s], v[d] );
    }
}

void reorder(vector<int> &v, vector<int> const &order )  {   
    for (unsigned int s = 1, d; s < order.size(); ++ s ) {
      for ( d = order[s]; d < s; d = order[d] ) ;
      if ( d == s ) while ( d = order[d], d != s ) swap( v[s], v[d] );
    }
}

vector<int> sort_indices(vector<double> const &v) {

  // initialize original index locations
  vector<int> idx(v.size());
  for (unsigned int i = 0; i != idx.size(); ++i) idx[i] = i;

  // sort indexes based on comparing values in v
  sort(idx.begin(), idx.end(),
       [&v](int i1, int i2) {return v[i1] > v[i2];});

  return idx;
}



