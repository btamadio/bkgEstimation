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
using namespace std;
#include <sys/stat.h>


miniTree *p;
vector<int> b_tag = {0,1,9};
vector<char*> reg = {(char*)"SR",(char*)"VR"};
char* hname;

TH1F* h_MJ[4][2][3];
TH1F* h_MJ_shiftUp[4][2][3];
TH1F* h_MJ_shiftDown[4][2][3];

TH1F* h_mass[4][4][2][3];
TH1F* h_mass_shiftUp[4][4][2][3];
TH1F* h_mass_shiftDown[4][4][2][3];

TH1F* h_mass_central[4][4][2][3];
TH1F* h_mass_central_shiftUp[4][4][2][3];
TH1F* h_mass_central_shiftDown[4][4][2][3];

TH1F* h_mass_forward[4][4][2][3];
TH1F* h_mass_forward_shiftUp[4][4][2][3];
TH1F* h_mass_forward_shiftDown[4][4][2][3];


void get_event(int i) {
  if ( p->LoadTree(i) < 0) { 
    cout<<"\nProblem in LoadTree."
        <<"\nEntry: "<<i<<endl;
    exit(0);
  }
  p->fChain->GetEntry(i);
}

struct Config {
    TString templates;
    int PE;
    int tmpType;
    vector<double> jetMassSF;
};

void loadConfig(Config& config, char* configFile) {
    ifstream fin(configFile);
    string line;
    string s;
    while (getline(fin, line)) {
        istringstream iss(line.substr(line.find(":") + 1));
        if (line.find("templates") != string::npos)
            iss >> config.templates;
        else if (line.find("tmpType") != string::npos)
            iss >> config.tmpType;
        else if (line.find("PE") != string::npos)
            iss >> config.PE;
        else if (line.find("jetMassSF") != string::npos){
          getline(iss,s,'{');
          getline(iss,s,'}');
          istringstream iss2(s);
          while(iss2){
            if(!getline(iss2,s,',')) break;
            config.jetMassSF.push_back(stod(s));
          }
        }
    }
}

void nameHistos(){
cout << "initializing histograms" << endl;
 char* excStr = (char*)"n";
  int nj;

  for(int i = 0; i < 3; i++){
    //i --> b_tag[i]
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
      h_MJ[j][l][i] = new TH1F(hname,hname,200,0,2);
      hname = Form("MJ_%s%ij%s_b%i_shiftUp",excStr,nj,reg[l],b_tag[i]);
      h_MJ_shiftUp[j][l][i] = new TH1F(hname,hname,200,0,2);
      hname = Form("MJ_%s%ij%s_b%i_shiftDown",excStr,nj,reg[l],b_tag[i]);
      h_MJ_shiftDown[j][l][i] = new TH1F(hname,hname,200,0,2);

      for(int k = 0; k < 4; k++){
        //individual jet masses
        hname = Form("M%i_%s%ij%s_b%i",k+1,excStr,nj,reg[l],b_tag[i]);
        h_mass[k][j][l][i] = new TH1F(hname,hname,200,0,2);
        hname = Form("M%i_%s%ij%s_b%i_shiftUp",k+1,excStr,nj,reg[l],b_tag[i]);
        h_mass_shiftUp[k][j][l][i] = new TH1F(hname,hname,200,0,2);
        hname = Form("M%i_%s%ij%s_b%i_shiftDown",k+1,excStr,nj,reg[l],b_tag[i]);
        h_mass_shiftDown[k][j][l][i] = new TH1F(hname,hname,200,0,2);

        hname = Form("M%i_central_%s%ij%s_b%i",k+1,excStr,nj,reg[l],b_tag[i]);
        h_mass_central[k][j][l][i] = new TH1F(hname,hname,200,0,2);
        hname = Form("M%i_central_%s%ij%s_b%i_shiftUp",k+1,excStr,nj,reg[l],b_tag[i]);
        h_mass_central_shiftUp[k][j][l][i] = new TH1F(hname,hname,200,0,2);
        hname = Form("M%i_central_%s%ij%s_b%i_shiftDown",k+1,excStr,nj,reg[l],b_tag[i]);
        h_mass_central_shiftDown[k][j][l][i] = new TH1F(hname,hname,200,0,2);

        hname = Form("M%i_forward_%s%ij%s_b%i",k+1,excStr,nj,reg[l],b_tag[i]);
        h_mass_forward[k][j][l][i] = new TH1F(hname,hname,200,0,2);
        hname = Form("M%i_forward_%s%ij%s_b%i_shiftUp",k+1,excStr,nj,reg[l],b_tag[i]);
        h_mass_forward_shiftUp[k][j][l][i] = new TH1F(hname,hname,200,0,2);
        hname = Form("M%i_forward_%s%ij%s_b%i_shiftDown",k+1,excStr,nj,reg[l],b_tag[i]);
        h_mass_forward_shiftDown[k][j][l][i] = new TH1F(hname,hname,200,0,2);
      }
  	}
    }
  }

}

//use to calculate mass given log(pt/m) template
void calc_mass(double & final_mass, TH1F * hist, double pT){
  double temp_value = hist->GetRandom();
  temp_value = exp(temp_value);
  //temp_value = 1/temp_value;
  final_mass = temp_value * pT;
  return;

}

void fluctuateMCTemp(int PE_number, TH1F* h){
  double val, sig, new_val;
  for(int i = 1; i < h->GetNbinsX()+1; i++){
    if(gRandom) delete gRandom;
    gRandom = new TRandom3(1000*PE_number + i);
    val = h->GetBinContent(i);
    sig = h->GetBinError(i);
    new_val = max(0.,gRandom->Gaus(val,sig));
    h->SetBinContent(i,new_val);
    delete gRandom;
  }
}

void fluctuateDataTemp(int PE_number, TH1F* h){
  double val, new_val;
  for(int i = 1; i < h->GetNbinsX()+1; i++){
    if(gRandom) delete gRandom;
    gRandom = new TRandom3(1000*PE_number + i);
    val = h->GetBinContent(i);
    new_val = max(0.,gRandom->PoissonD(val));
    h->SetBinContent(i,new_val);
    delete gRandom;
  }
}


bool isFile(const char *fileName){
	ifstream infile(fileName);
	return infile.good();
}
