#include "TLatex.h"
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

void get_event(int i) {
  if ( p->LoadTree(i) < 0) { 
    cout<<"\nProblem in LoadTree."
        <<"\nEntry: "<<i<<endl;
    exit(0);
  }
  p->fChain->GetEntry(i);
}

double getLumi(int run){
double lumi = 1;

//2015
if(run == 276262) lumi = 6.166897;
if(run == 276329) lumi = 11.7065;
if(run == 276336) lumi = 0.673924;
if(run == 276416) lumi = 4.21279;
if(run == 276511) lumi = 8.76604;
if(run == 276689) lumi = 10.656495;
if(run == 276778) lumi = 0.657529;
if(run == 276790) lumi = 1.59535;
if(run == 276952) lumi = 6.47731;
if(run == 276954) lumi = 0.73329;
if(run == 278880) lumi = 21.41921;
if(run == 278912) lumi = 19.633223;
if(run == 278968) lumi = 9.883003;
if(run == 279169) lumi = 55.00551;
if(run == 279259) lumi = 7.31293;
if(run == 279279) lumi = 16.897099;
if(run == 279284) lumi = 31.8164;
if(run == 279345) lumi = 52.683661;
if(run == 279515) lumi = 0.347662;
if(run == 279598) lumi = 67.85899;
if(run == 279685) lumi = 74.470953;
if(run == 279813) lumi = 47.616706;
if(run == 279867) lumi = 29.5636;
if(run == 279928) lumi = 1.09603;
if(run == 279932) lumi = 44.64153;
if(run == 279984) lumi = 65.27506;
if(run == 280231) lumi = 88.284052;
if(run == 280273) lumi = 1.62778;
if(run == 280319) lumi = 93.6371;
if(run == 280368) lumi = 8.227303;
if(run == 280423) lumi = 68.57333;
if(run == 280464) lumi = 58.3428;
if(run == 280500) lumi = 7.27397;
if(run == 280520) lumi = 12.118483;
if(run == 280614) lumi = 24.23566;
if(run == 280673) lumi = 149.0631;
if(run == 280753) lumi = 45.214597;
if(run == 280853) lumi = 7.21388;
if(run == 280862) lumi = 131.153913;
if(run == 280950) lumi = 125.749586;
if(run == 280977) lumi = 34.93267;
if(run == 281070) lumi = 12.79458;
if(run == 281074) lumi = 46.66011;
if(run == 281075) lumi = 1.07213;
if(run == 281317) lumi = 24.36288;
if(run == 281385) lumi = 73.975948;
if(run == 281411) lumi = 146.741527;
if(run == 282625) lumi = 10.6752;
if(run == 282631) lumi = 26.76917;
if(run == 282712) lumi = 93.93239;
if(run == 282784) lumi = 2.88326;
if(run == 282992) lumi = 106.77136;
if(run == 283074) lumi = 55.80083;
if(run == 283155) lumi = 28.7327;
if(run == 283270) lumi = 13.2713;
if(run == 283429) lumi = 234.56794;
if(run == 283608) lumi = 23.80896;
if(run == 283780) lumi = 145.089802;
if(run == 284006) lumi = 38.91892;
if(run == 284154) lumi = 9.99866;
if(run == 284213) lumi = 199.99072;
if(run == 284285) lumi = 247.88597;
if(run == 284420) lumi = 57.500046;
if(run == 284427) lumi = 35.2553;
if(run == 284484) lumi = 103.396857;


//2016
if(run == 297730) lumi = 3.539916;
if(run == 298595) lumi = 0.306561;
if(run == 298609) lumi = 2.038514;
if(run == 298633) lumi = 6.126455;
if(run == 298687) lumi = 21.478222;
if(run == 298690) lumi = 7446;
if(run == 298771) lumi = 42217;
if(run == 298773) lumi = 18302;
if(run == 298862) lumi = 24.791675;
if(run == 298967) lumi = .9486;
if(run == 299055) lumi = 46.393232;
if(run == 299144) lumi = .9584;
if(run == 299147) lumi = 24.286768;
if(run == 299184) lumi = 95.300343;
if(run == 299243) lumi = 3;
if(run == 299584) lumi = 227.640391;
if(run == 300279) lumi = 5.494332;
if(run == 300345) lumi = 5.038334;
if(run == 300415) lumi = 92.319154;
if(run == 300418) lumi = 5.758711;
if(run == 300487) lumi = 124.55881;
if(run == 300540) lumi = 182.557558;
if(run == 300571) lumi = 147.807207;
if(run == 300600) lumi = 50.88581;
if(run == 300655) lumi = 239.96066;
if(run == 300687) lumi = 337.658438;
if(run == 300784) lumi = 7.560546;
if(run == 300800) lumi = 287.574325;
if(run == 300863) lumi = 341.348278;
if(run == 300908) lumi = 222.7633;
if(run == 301912) lumi = 6.669922;
if(run == 301918) lumi = 20.4846;
if(run == 301932) lumi = 369.227912;
if(run == 301973) lumi = 482.915902;
if(run == 302053) lumi = 397.097848;
if(run == 302137) lumi = 207.7135;
if(run == 302265) lumi = 107.711258;
if(run == 302269) lumi = 59.02594;
if(run == 302300) lumi = 401.839103;
if(run == 302347) lumi = 206.088924;
if(run == 302380) lumi = 110.98889;
if(run == 302391) lumi = 134.34099;
if(run == 302393) lumi = 526.239298;
if(run == 302737) lumi = 84.953042;
if(run == 302831) lumi = 16.28424;

return lumi;
}