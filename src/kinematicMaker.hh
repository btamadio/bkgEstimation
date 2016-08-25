#include "TLatex.h"
#include "TMath.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "THStack.h"
#include "TString.h"
#include "TH1.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TRandom.h"
#include "TRandom3.h"
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include "../aux/miniTree.hh"
//#include "header.hh"
#include <sys/stat.h>

using namespace std;

miniTree *p;
vector<int> b_tag = {0,1,9};
//b0 - b-veto
//b1 - b-tag
//b9 - b-inclusive
vector<char*> reg = {(char*)"SR",(char*)"VR"};

char* hname;
TH1F* MJ[4][2][3];
TH1F* h_pt[2][4];
TH1F* mass[5][4][2][3];
TH1F* mass_central[5][4][2][3];
TH1F* mass_forward[5][4][2][3];


void get_event(int i) {
  if ( p->LoadTree(i) < 0) { 
    cout<<"\nProblem in LoadTree."
        <<"\nEntry: "<<i<<endl;
    exit(0);
  }
  p->fChain->GetEntry(i);
}

struct Config {
    vector<double> etaBins;
    vector<double> ptBins;
    int CRdef;
    bool useVR;
};

void loadConfig(Config& config, char* configFile) {
    ifstream fin(configFile);
    string line;
    string s;
    while (getline(fin, line)) {
        istringstream iss(line.substr(line.find(":") + 1));
        if (line.find("etaBins") != string::npos){
          getline(iss,s,'{');
          getline(iss,s,'}');
          istringstream iss2(s);
          while(iss2){
            if(!getline(iss2,s,',')) break;
            config.etaBins.push_back(stod(s));
          }
        }
        else if (line.find("ptBins ") != string::npos){
        	getline(iss,s,'{');
        	getline(iss,s,'}');
        	istringstream iss2(s);
        	while(iss2){
        		if(!getline(iss2,s,',')) break;
        		config.ptBins.push_back(stod(s));
        	}
        }

        else if (line.find("CRdef") != string::npos)
            iss >> config.CRdef;
        else if (line.find("useVR") != string::npos)
            iss >> config.useVR;
    }
}

void nameHistos(){
	cout << "initializing histograms" << endl;
  	for(int j = 0; j<4; j++){
    	for(int k = 0; k <2; k++){
      	hname = Form("jet%i_pt_b%i",j+1,b_tag[k]);
      	h_pt[k][j] = new TH1F(hname,hname,700,0,7);
    	}
  	}

 char* excStr = (char*)"n";
  int nj;
  for(int i = 0; i < 3; i++){
    //b_tag[i+1]
    for(int j = 0; j < 4; j++){
      //j = n3,n4,m4,m5
      if(j < 2){
        excStr = (char*)"n";
        nj = j+3;
      }
      if(j > 1){
        excStr = (char*)"m";
        nj = j+2;
      } 
      for(int l = 0; l < 2; l++){
      	hname = Form("MJ_%s%ij%s_b%i",excStr,nj,reg[l],b_tag[i]);
      	MJ[j][l][i] = new TH1F(hname,hname,200,0,2);
      	MJ[j][l][i]->Sumw2();
      for(int k = 0; k < 5; k++){
        //individual jet masses
        hname = Form("M%i_%s%ij%s_b%i",k+1,excStr,nj,reg[l],b_tag[i]);
        mass[k][j][l][i] = new TH1F(hname,hname,200,0,2);
        mass[k][j][l][i]->Sumw2();

        hname = Form("M%i_central_%s%ij%s_b%i",k+1,excStr,nj,reg[l],b_tag[i]);
        mass_central[k][j][l][i] = new TH1F(hname,hname,200,0,2);
        mass_central[k][j][l][i]->Sumw2();

        hname = Form("M%i_forward_%s%ij%s_b%i",k+1,excStr,nj,reg[l],b_tag[i]);
        mass_forward[k][j][l][i] = new TH1F(hname,hname,200,0,2);
        mass_forward[k][j][l][i]->Sumw2();
      }
  	}
    }
  }

}


bool isFile(const char *fileName){
	ifstream infile(fileName);
	return infile.good();
}


