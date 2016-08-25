char plot_locations[200];
char hist_locations[200];
int PE_number = 100;
ofstream texfile;
ofstream texfile_limits;
ofstream texfile_bkg;
ofstream textfile_bkg;
ofstream texfile_pred;
ofstream texfile_predSig;
ofstream texfile_summary;
ofstream htmlfile;
ofstream pyList3Sig;
ofstream pyList95CL;
 double SR_cut =0.6; 
 char SR_cut_str[100];
 double dEta_cut = 1.4;
 bool blinded;
 bool isMC;
 vector<int> btag = {0,1,9};
 double lumi = 3.2;
 char dateStr[100];

 //for new baseline [softJetTempl], from pythia!
 double mcNonClosure0p6[4] = {0.272,0.1662,0.165,0.127};
 double mcNonClosure0p8[4] = {0.267,0.165,0.205,0.134};


 double getMCnonClosure(string reg, int btag, double SR_cut){
  double nonClose = 0;
   if(SR_cut == 0.6){
      if(reg == "VR" && btag == 1) nonClose = mcNonClosure0p6[0];
      if(reg == "VR" && btag == 9) nonClose = mcNonClosure0p6[1];
      if(reg == "SR" && btag == 1) nonClose = mcNonClosure0p6[2];
      if(reg == "SR" && btag == 9) nonClose = mcNonClosure0p6[3];
  }
    if(SR_cut == 0.8){
      if(reg == "VR" && btag == 1) nonClose = mcNonClosure0p8[0];
      if(reg == "VR" && btag == 9) nonClose = mcNonClosure0p8[1];
      if(reg == "SR" && btag == 1) nonClose = mcNonClosure0p8[2];
      if(reg == "SR" && btag == 9) nonClose = mcNonClosure0p8[3];
  }
  return nonClose;
 }


double Z(  double eS , double B, double eB ,double eBres,double L = 3.2, bool useGaus=true)
{
  double S;
  L = L/lumi;
  if(useGaus){ //gaussian assumption fine
  for( int i = 0 ; i < 50000 ; i ++ )
    {
      S = i*0.01;
        double denominator = 0;
        denominator =  B*L;
        denominator += eB*eB*L; // assume uncertainty scale with sqrt(L)
        denominator += pow(eBres*B,2);
        denominator = sqrt(denominator);
        if(S/denominator > 3) {
          return S;
        }
      }
  }
  else{
  //otherwise, toys.
  int nToys = 10000;
  double B_toy;
  TH1F* h_bkg = new TH1F("h_bkg","h_bkg",100,0,max(B*3,5.));
  for(int iToy = 0; iToy < nToys; iToy++){
    if(gRandom) delete gRandom;
    gRandom = new TRandom3(0);
    B_toy = gRandom->PoissonD(B);
    B_toy = gRandom->Gaus(B_toy,pow(eB*eB*L+pow(eBres,2),0.5));
    h_bkg->Fill(B_toy);
  }
    h_bkg->Scale(1/h_bkg->Integral());
    h_bkg->Draw("hist");
    for(int i = 1; i <=100; i++){
      if(h_bkg->Integral(i,100) < 0.0027){
        S = h_bkg->GetBinLowEdge(i+1)-B;
        return S;
      }
    }
  }
    return S;
}

double limit(  double eS , double B, double eB , double eBres,double L = 3.2)
{ 
  double S;
  L = L/lumi;
  for( int i = 0 ; i < 50000 ; i ++ )
    {
      S = i*0.01;
      double denominator = 0;
      denominator = S + B*L;
      denominator += pow(eS*S,2);
      denominator += eB*eB*L; // assume uncertainty scale with sqrt(L)
      denominator += pow(eBres*B,2);
      denominator = sqrt(denominator);
      if(S/denominator > 1.96) {
        return S;
      }
    }
    return S;
}




