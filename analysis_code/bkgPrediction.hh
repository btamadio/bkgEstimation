#include <cmath>
#include <iostream>
#include <fstream>
#include <cstdlib>

double GetMedian(TH1F* h){
  //const int nq = 1; double xq[nq] = {0.5}; double yq[nq];
  //h->GetQuantiles(nq,yq,xq);
  //return yq[0];
  return h->GetMean();
}

double calcMedian(TH1F* h){
  const int nq = 1; double xq[nq] = {0.5}; double yq[nq];
  h->GetQuantiles(nq,yq,xq);
  return yq[0];
}

bool isFile(const char *fileName){
  ifstream infile(fileName);
  return infile.good();
}

class bkgPrediction{
public:
  bkgPrediction();
  bkgPrediction(string,double,bool);
	void runPredictionAndHist(bool,int,string,int);
  void runPredictionRange(double,double,bool,int,string,int);
  void getHistMJ(bool,int,string,int);
  TH1F* getNonClosure();
  void getHistMass(int,bool,int,string,int,int);
  void getHistMass_wErrors(int,bool,int,string,int,int);
  TH1F* getNonClosureMass(int);

  void getHistJetMass(bool,int,string,int,int);
  void getHistJetMass_wErrors(bool,int,string,int,int);
  TH1F* getNonClosureJetMass();

	void setSR_cut(double);
	void setLumi(double);
  void setPEs(int);
  void setBlind(int);

	double getObs();
  double getObsError();
	double getPred();
	double getPESpread();
	double getPtRes();
  double getLumi();
  string getName();
  TH1F* getYield();
	TH1F* kinMJ();
	TH1F* dressedMJ();
  TH1F* dressedMJ_up();
  TH1F* dressedMJ_down();
  TH1F* kinMass(int);
  TH1F* dressedMass(int);
  TH1F* kinJetMass();
  TH1F* dressedJetMass();

	//variables
	double obs, obs_error,pred, pred_error, pred_error2,pred_error3;
  TH1F *yield;
	TH1F *h_kinMJ;
	TH1F *h_dressMJ;
  TH1F *h_dressMJ_up;
  TH1F *h_dressMJ_down;
  TH1F *h_kinMass[4];
  TH1F *h_dressMass[4];
  TH1F *h_kinJetMass;
  TH1F *h_dressJetMass;
	double lumi = 3.2, SR_cut;
  string source;

private:
	char SR_cut_str[100];
  bool isMC;
  bool isBlinded = false;
  int PE_number;
  double dEta_cut;
  char hist_locations[200];
  char f_kinematic[300];
  char f_dressed[300];
};


bkgPrediction::bkgPrediction(){}
bkgPrediction::bkgPrediction(string src, double sr,bool MC = false){
  source = src;
  SR_cut = sr;
  sprintf(SR_cut_str,"SR_cut_%igev",(int)(SR_cut*1e3));
  isMC = MC;
  dEta_cut = 1.4;

  sprintf(hist_locations,"/project/projectdirs/atlas/btamadio/RPV_SUSY/bkgEstimation");
  sprintf(f_kinematic,"%s/kinematic_hists/%s/main_kinematic_%s.root",hist_locations, source.c_str(), source.c_str());
  //check folders for kinematic and dressed hists
  if(isFile(f_kinematic)){
    cout << "Found kinematic histos in: " << endl; cout << f_kinematic << endl;
    sprintf(f_dressed,"%s/dressed_hists/%s",hist_locations,source.c_str());
    TSystemDirectory dir(f_dressed,f_dressed);
    if(dir.GetListOfFiles()) PE_number = dir.GetListOfFiles()->GetSize()-2;
    if(PE_number){
      cout << "Found " << PE_number << " dressed histos in: " << endl; cout << f_dressed << endl;
      cout << "Setting PE_number to " << PE_number << "." << endl;
    }
    else{
      cout << "Could not find dressed histos in: " << endl;
      cout << f_dressed << endl;
      throw invalid_argument(Form("Invalid name of dressed sample: %s",source.c_str()));
    }

    cout << "SR Cut set to " << SR_cut << " TeV" << endl;
    if(isMC){
      cout << "Kinematic Sample is MC, going to scale by luminosity." << endl;
    }
    cout << "Current lumi is: " << Form("%3.2f",lumi) << " fb-1." << endl;
    cout << endl;  
    cout << "Successfully initialized background prediction for " << source.c_str() << endl;
    cout << endl;  
    //check for dressed hists and correct number of PEs --> Hard?
  }
  else{
    cout << "Could not find kinematic histos in " << endl;
    cout << f_kinematic << endl;
    throw invalid_argument(Form("Invalid name of kinematic sample: %s",source.c_str()));
  }
}

void bkgPrediction::runPredictionAndHist(bool incl, int njets, string reg,int btag){
  char nom[100];
  char dress[100];
  char dress_up[100];
  char dress_down[100];
  int SRbin = (int)(100*SR_cut+1);
  const int N_MJ_bins = 14;
  double MJ_bins[N_MJ_bins+1] = {0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.40,.45,0.5,0.6,0.8,1.0,2.0};

  bool blindPlot = (isBlinded && reg =="SR" && njets >= 4);

  double MJ_width[N_MJ_bins];
  for(int i = 0; i < N_MJ_bins; i++) MJ_width[i] = MJ_bins[i+1]-MJ_bins[i];

  string excStr = "n";
  if(incl) excStr = "m";

  char* region = Form("%s%ij%s_b%i",excStr.c_str(),njets,reg.c_str(),btag);
  cout << source << " running prediction for " << region << endl;

  sprintf(nom,"MJ_%s",region);
  sprintf(dress,"MJ_%s",region);
  sprintf(dress_up,"MJ_%s_shiftUp",region);
  sprintf(dress_down,"MJ_%s_shiftDown",region);

  TFile *f_kin = TFile::Open(f_kinematic);
  TH1F* h_kin = (TH1F*)f_kin->Get(nom);
  h_kin->SetDirectory(0);
  f_kin->Close();
  if(isMC)h_kin->Scale(lumi);

  if(blindPlot){
    for(int i = 61; i < 201; i++){
      h_kin->SetBinContent(i,0);
      h_kin->SetBinError(i,0);}
  }

  //Truth values
  double obs_NR_error, obs_NR;
  obs = h_kin->IntegralAndError(SRbin,200,obs_error);
  obs_NR = h_kin->IntegralAndError(21,60,obs_NR_error);

  //cout << "Calculating yields..." << endl;
  //Open all files and calculate yield distributions
  TH1F* temp_MJs[PE_number+1];
  TH1F* temp_MJs_up[PE_number+1];
  TH1F* temp_MJs_down[PE_number+1];
  TH1F* yield_SR = new TH1F("yield_SR","yield_SR",1e4,0,1e4);
  TH1F* yield_SR_up = new TH1F("yield_SR_up","yield_SR_up",1e4,0,1e4);
  TH1F* yield_SR_down = new TH1F("yield_SR_down","yield_SR_down",1e4,0,1e4);
  TH1F* yield_NR = new TH1F("yield_NR","yield_NR",1e6,0,1e6);
  TH1F* yield_NR_up = new TH1F("yield_NR_up","yield_NR_up",1e6,0,1e6);
  TH1F* yield_NR_down = new TH1F("yield_NR_down","yield_NR_down",1e6,0,1e6);
  for(int PE = 1; PE <= PE_number; PE++){
    sprintf(f_dressed,"%s/dressed_hists/%s/main_dressed_%s_%d.root",hist_locations, source.c_str(), source.c_str(), PE);
    TFile *f_dress = TFile::Open(f_dressed);
    if(!f_dress){
      cout<<"Could not find file "<<f_dressed<<endl;
      exit(1);
    }      
    temp_MJs[PE] = (TH1F*)f_dress->Get(dress);
    temp_MJs_up[PE] = (TH1F*)f_dress->Get(dress_up);
    temp_MJs_down[PE] = (TH1F*)f_dress->Get(dress_down);
    temp_MJs[PE]->SetDirectory(0);
    temp_MJs_up[PE]->SetDirectory(0);
    temp_MJs_down[PE]->SetDirectory(0);
    f_dress->Close();

    yield_SR->Fill(temp_MJs[PE]->Integral(SRbin,200));
    yield_SR_up->Fill(temp_MJs_up[PE]->Integral(SRbin,200));
    yield_SR_down->Fill(temp_MJs_down[PE]->Integral(SRbin,200));
    yield_NR->Fill(temp_MJs[PE]->Integral(21,60));
    yield_NR_up->Fill(temp_MJs_up[PE]->Integral(21,60));
    yield_NR_down->Fill(temp_MJs_down[PE]->Integral(21,60));

  }

  double med_SR = GetMedian(yield_SR);
  double med_SR_up = GetMedian(yield_SR_up);
  double med_SR_down = GetMedian(yield_SR_down);
  double rms_SR = yield_SR->GetRMS();
  double med_NR = GetMedian(yield_NR);
  double med_NR_up = GetMedian(yield_NR_up);
  double med_NR_down = GetMedian(yield_NR_down);
  double rms_NR = yield_NR->GetRMS();
  double norm = obs_NR/med_NR;

  delete yield_SR; delete yield_SR_up; delete yield_SR_down; delete yield_NR; delete yield_NR_up; delete yield_NR_down;


  pred = med_SR*norm;
  double pred_up = med_SR_up*obs_NR/med_NR_up;
  double pred_down = med_SR_down*obs_NR/med_NR_down;
  pred_error = pred*sqrt(pow(rms_SR/med_SR,2)+pow(rms_NR/med_NR,2));
  pred_error2 = (fabs(pred_up-pred) + fabs(pred_down-pred))/2;
  double dev = (pred-obs)/obs;

  //cout << "Rebinning histograms..." << endl;
  //Switch to variable binning, then get errors bin by bin. 
  h_kinMJ = (TH1F*)h_kin->Rebin(N_MJ_bins,Form("h_kin_%s",source.c_str()),MJ_bins);
  TH1F* temp_MJs_var[PE_number+1];
  TH1F* temp_MJs_up_var[PE_number+1];
  TH1F* temp_MJs_down_var[PE_number+1];

  for(int PE = 1; PE <=PE_number; PE++){
    temp_MJs_var[PE] = (TH1F*)temp_MJs[PE]->Rebin(N_MJ_bins,Form("%s_PE%i",temp_MJs[PE]->GetName(),PE),MJ_bins);
    temp_MJs_up_var[PE] = (TH1F*)temp_MJs_up[PE]->Rebin(N_MJ_bins,Form("%s_PE%i",temp_MJs_up[PE]->GetName(),PE),MJ_bins);
    temp_MJs_down_var[PE] = (TH1F*)temp_MJs_down[PE]->Rebin(N_MJ_bins,Form("%s_PE%i",temp_MJs_down[PE]->GetName(),PE),MJ_bins);
  }

  //cout << "Combnining PEs..." << endl;
  //now calculate errors and bands
  h_dressMJ = new TH1F(Form("h_dress_%s",source.c_str()),"h_dress_nom",N_MJ_bins,MJ_bins);
  h_dressMJ_up = new TH1F(Form("h_dress_up_%s",source.c_str()),"h_dress_syst_up",N_MJ_bins,MJ_bins);
  h_dressMJ_down = new TH1F(Form("h_dress_down_%s",source.c_str()),"h_dress_syst_down",N_MJ_bins,MJ_bins);

  // TCanvas c_nom("c_nom");
  //  c_nom.Print("result_nom.pdf[");
//  TCanvas c_up("c_up");
//  c_up.Print("result_up.pdf[");
//  TCanvas c_down("c_down");
//  c_down.Print("result_down.pdf[");
  for(int bin = 1; bin <= N_MJ_bins; bin++){
    int xMax = 10;
    int nBins = 100;
    if (bin==1 || N_MJ_bins-bin < 2){
      xMax = 1;
    }
    TH1F* temp_bin = new TH1F("temp_bin","temp_bin",1e7,0,1e7);
    TH1F* temp_bin_up = new TH1F("temp_bin_up","temp_bin_up",1e7,0,1e7);
    TH1F* temp_bin_down = new TH1F("temp_bin_down","temp_bin_down",1e7,0,1e7);
    //    TH1F* temp_bin = new TH1F("temp_bin","temp_bin",nBins,0,xMax);
    //    TH1F* temp_bin_up = new TH1F("temp_bin_up","temp_bin_up",nBins,0,xMax);
    //    TH1F* temp_bin_down = new TH1F("temp_bin_down","temp_bin_down",nBins,0,xMax);
    for(int PE = 1; PE <= PE_number; PE++){
      temp_bin->Fill(temp_MJs_var[PE]->GetBinContent(bin));
      temp_bin_up->Fill(temp_MJs_up_var[PE]->GetBinContent(bin));
      temp_bin_down->Fill(temp_MJs_down_var[PE]->GetBinContent(bin));
    }

    double rms = temp_bin->GetRMS();
    double med    = GetMedian(temp_bin);
    double med_up = GetMedian(temp_bin_up);
    double med_down = GetMedian(temp_bin_down);
    double syst_uncert_up = fabs( med_up - med);
    double syst_uncert_down = fabs(med_down - med);
    double syst_uncert_avg = 0.5*(syst_uncert_up + syst_uncert_down);
    double total_uncert_up = pow( pow(syst_uncert_up, 2) + pow(rms, 2), 0.5);
    double total_uncert_down = pow( pow(syst_uncert_down, 2) + pow(rms, 2), 0.5);

    double total_uncert = pow(pow(syst_uncert_avg,2)+pow(rms,2),0.5);

    h_dressMJ->SetBinContent(bin, med);
    h_dressMJ_up->SetBinContent(bin, med_up);
    h_dressMJ_down->SetBinContent(bin, med_down);

    h_dressMJ->SetBinError(bin, total_uncert);
    h_dressMJ_up->SetBinError(bin, temp_bin_up->GetRMS());
    h_dressMJ_down->SetBinError(bin, temp_bin_down->GetRMS());
    
    //cout<<"\t nom mean = "<<temp_bin->GetMean()<<"\t nom median = "<<calcMedian(temp_bin)<<endl;
    //    cout<<"\t up mean = "<<temp_bin_up->GetMean()<<"\t up median = "<<calcMedian(temp_bin_up)<<endl;
    //    cout<<"\t down mean = "<<temp_bin_down->GetMean()<<"\t down median = "<<calcMedian(temp_bin_down)<<endl;
    // c_nom.cd();
    
    // temp_bin->Draw();
    // c_nom.Print("result_nom.pdf");

    // c_up.cd();
    // temp_bin_up->Draw();
    // c_up.Print("result_up.pdf");

    // c_down.cd();
    // temp_bin_down->Draw();
    // c_down.Print("result_down.pdf");
    delete temp_bin; delete temp_bin_up; delete temp_bin_down; 
  }
  //  c_nom.Print("result_nom.pdf]");
  //  c_up.Print("result_up.pdf]");
  //  c_down.Print("result_down.pdf]");
  for(int PE = 1; PE <= PE_number; PE++) {
    delete temp_MJs_var[PE];
    delete temp_MJs_up_var[PE];
    delete temp_MJs_down_var[PE];
  }

  h_dressMJ->Scale(norm);
  h_dressMJ_up->Scale(obs_NR/med_NR_up);
  h_dressMJ_down->Scale(obs_NR/med_NR_down);
}

void bkgPrediction::runPredictionRange(double low, double high,bool incl, int njets, string reg,int btag){
  char nom[100];
  char dress[100];
  char dress_up[100];
  char dress_down[100];
  const int N_MJ_bins = 14;
  double MJ_bins[N_MJ_bins+1] = {0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.40,.45,0.5,0.6,0.8,1.0,2.0};

  bool blindPlot = (isBlinded && reg =="SR" && njets >= 4);
  double MJ_width[N_MJ_bins];
  for(int i = 0; i < N_MJ_bins; i++) MJ_width[i] = MJ_bins[i+1]-MJ_bins[i];

  string excStr = "n";
  if(incl) excStr = "m";

  char* region = Form("%s%ij%s_b%i",excStr.c_str(),njets,reg.c_str(),btag);
  cout << source << " running prediction for " << region << " and " <<  Form("%3.2f",low) <<  " TeV < MJ < " << Form("%3.2f",high) << " TeV." << endl;

  sprintf(nom,"MJ_%s",region);
  sprintf(dress,"MJ_%s",region);
  sprintf(dress_up,"MJ_%s_shiftUp",region);
  sprintf(dress_down,"MJ_%s_shiftDown",region);

  TFile *f_kin = TFile::Open(f_kinematic);
  TH1F* h_kin = (TH1F*)f_kin->Get(nom);
  h_kin->SetDirectory(0);
  f_kin->Close();
  if(isMC)h_kin->Scale(lumi);

  if(blindPlot){
    for(int i = 61; i < 201; i++){
      h_kin->SetBinContent(i,0);
      h_kin->SetBinError(i,0);}
  }

  int lowBin = (int)(1e2*low)+1;
  int highBin = min(200,(int)(1e2*high));

  /*
  int bigBin;
  if(low == 0 && high == 0.2) bigBin = 1;
  if(low == 0.2 && high == 0.6) bigBin = 2;
  if(low == 0.6) bigBin = 3;
  */

  //Truth values
  double obs_NR_error, obs_NR;
  obs = h_kin->IntegralAndError(lowBin,highBin,obs_error);
  obs_NR = h_kin->IntegralAndError(21,60,obs_NR_error);

  //Open all files and calculate yield distributions
  TH1F* temp_MJs[PE_number+1];
  TH1F* temp_MJs_up[PE_number+1];
  TH1F* temp_MJs_down[PE_number+1];
  yield = new TH1F("yield","yield",1e6,0,1e6);
  TH1F* yield_up = new TH1F("yield_up","yield_up",1e6,0,1e6);
  TH1F* yield_down = new TH1F("yield_down","yield_down",1e6,0,1e6);
  TH1F* yield_NR = new TH1F("yield_NR","yield_NR",1e6,0,1e6);
  TH1F* yield_NR_up = new TH1F("yield_NR_up","yield_NR_up",1e6,0,1e6);
  TH1F* yield_NR_down = new TH1F("yield_NR_down","yield_NR_down",1e6,0,1e6);
  for(int PE = 1; PE <= PE_number; PE++){
    sprintf(f_dressed,"%s/dressed_hists/%s/main_dressed_%s_%d.root",hist_locations, source.c_str(), source.c_str(), PE);
    TFile *f_dress = TFile::Open(f_dressed);

    temp_MJs[PE] = (TH1F*)f_dress->Get(dress);
    temp_MJs_up[PE] = (TH1F*)f_dress->Get(dress_up);
    temp_MJs_down[PE] = (TH1F*)f_dress->Get(dress_down);
    temp_MJs[PE]->SetDirectory(0);
    temp_MJs_up[PE]->SetDirectory(0);
    temp_MJs_down[PE]->SetDirectory(0);
    f_dress->Close();

    yield->Fill(temp_MJs[PE]->Integral(lowBin,highBin));
    //yield->Fill(temp_MJs[PE]->GetBinContent(bigBin));
    yield_up->Fill(temp_MJs_up[PE]->Integral(lowBin,highBin));
    yield_down->Fill(temp_MJs_down[PE]->Integral(lowBin,highBin));
    yield_NR->Fill(temp_MJs[PE]->Integral(21,60));
    //yield_NR->Fill(temp_MJs[PE]->GetBinContent(2));
    yield_NR_up->Fill(temp_MJs_up[PE]->Integral(21,60));
    yield_NR_down->Fill(temp_MJs_down[PE]->Integral(21,60));

  }

  double med = GetMedian(yield);
  double med_up = GetMedian(yield_up);
  double med_down = GetMedian(yield_down);
  double rms = yield->GetRMS();
  double med_NR = GetMedian(yield_NR);
  double med_NR_up = GetMedian(yield_NR_up);
  double med_NR_down = GetMedian(yield_NR_down);
  double rms_NR = yield_NR->GetRMS();
  double norm = obs_NR/med_NR;

  delete yield_up; delete yield_down; delete yield_NR; delete yield_NR_up; delete yield_NR_down;

  pred = med*norm;
  double pred_up = med_up*obs_NR/med_NR_up;
  double pred_down = med_down*obs_NR/med_NR_down;
  pred_error = pred*sqrt(pow(rms/med,2)+pow(rms_NR/med_NR,2));
  pred_error2 = (fabs(pred_up-pred) + fabs(pred_down-pred))/2;
}

void bkgPrediction::getHistMJ(bool incl, int njets, string reg,int btag){
  char nom[100];
  char dress[100];
  int SRbin = (int)(100*SR_cut+1);
  const int N_MJ_bins = 14;
  double MJ_bins[N_MJ_bins+1] = {0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.40,.45,0.5,0.6,0.8,1.0,2.0};

  bool blindPlot = (isBlinded && reg =="SR" && njets >= 4);

  double MJ_width[N_MJ_bins];
  for(int i = 0; i < N_MJ_bins; i++) MJ_width[i] = MJ_bins[i+1]-MJ_bins[i];

  string excStr = "n";
  if(incl) excStr = "m";

  char* region = Form("%s%ij%s_b%i",excStr.c_str(),njets,reg.c_str(),btag);
  cout << source << " getting nominal histos for " << region << endl;

  sprintf(nom,"MJ_%s",region);
  sprintf(dress,"MJ_%s",region);

  TFile *f_kin = new TFile(f_kinematic);
  TH1F* h_kin = (TH1F*)f_kin->Get(nom);
  h_kin->SetDirectory(0);
  if(isMC)h_kin->Scale(lumi);
  f_kin->Close();

  if(blindPlot){
    for(int i = 61; i < 201; i++){
      h_kin->SetBinContent(i,0);
      h_kin->SetBinError(i,0);}
  }

  //Truth values
  double obs_error;
  double obs_NR_error, obs_NR;
  obs_NR = h_kin->IntegralAndError(21,60,obs_NR_error);

  //Open all files and calculate yield distributions
  TH1F* temp_MJs[PE_number+1];
  TH1F* yield_NR = new TH1F("yield_NR","yield_NR",1e6,0,1e6);
  for(int PE = 1; PE <= PE_number; PE++){
    sprintf(f_dressed,"%s/dressed_hists/%s/main_dressed_%s_%d.root",hist_locations, source.c_str(), source.c_str(), PE);
    TFile *f_dress = new TFile(f_dressed);
    temp_MJs[PE] = (TH1F*)f_dress->Get(dress);
    temp_MJs[PE]->SetDirectory(0);
    f_dress->Close();
    
    yield_NR->Fill(temp_MJs[PE]->Integral(21,60));
  }
  double med_NR = GetMedian(yield_NR);
  delete yield_NR;
  double norm = obs_NR/med_NR;

  //cout << "Rebinning histograms..." << endl;
  //Switch to variable binning, then get errors bin by bin. 
  h_kinMJ = (TH1F*)h_kin->Rebin(N_MJ_bins,Form("h_kin_%s",source.c_str()),MJ_bins);
  TH1F* temp_MJs_var[PE_number+1];

  for(int PE = 1; PE <=PE_number; PE++){
    temp_MJs_var[PE] = (TH1F*)temp_MJs[PE]->Rebin(N_MJ_bins,Form("%s_PE%i",temp_MJs[PE]->GetName(),PE),MJ_bins);
  }

  //cout << "Combnining PEs..." << endl;
  //now calculate errors and bands
  h_dressMJ = new TH1F(Form("h_dress_%s",source.c_str()),"h_dress",N_MJ_bins,MJ_bins);
  for(int bin = 1; bin <= N_MJ_bins; bin++){
    TH1F* temp_bin = new TH1F("temp_bin","temp_bin",1e7,0,1e7);
    for(int PE = 1; PE <= PE_number; PE++){
      temp_bin->Fill(temp_MJs_var[PE]->GetBinContent(bin));
    }

    double med = GetMedian(temp_bin);
    h_dressMJ->SetBinContent(bin, med);
    h_dressMJ->SetBinError(bin, 0);

    delete temp_bin;
  }
  for(int PE = 1; PE <= PE_number; PE++) delete temp_MJs_var[PE];

  h_dressMJ->Scale(norm);
  //cout << "Done." << endl;
  return;
}


TH1F* bkgPrediction::getNonClosure(){
  TH1F* dev;
  if(!h_kinMJ || !h_dressMJ){
    cout << "No kinematic or dressed histogram not found, run getHists()!" << endl;
    return dev;
  }
  dev = (TH1F*)h_dressMJ->Clone("devMJ");
  dev->Add(h_kinMJ,-1);
  dev->Divide(h_kinMJ);

  return dev;
}

void bkgPrediction::getHistMass(int jet,bool incl,int njets, string reg, int btag,int etaCut = 0){
  char nom[100];
  char dress[100];
  int SRbin = (int)(100*SR_cut+1);
  const int N_Mass_bins = 6;
  double Mass_bins[N_Mass_bins+1] = {0,0.1,0.2,0.3,0.6,1,2.0};
  double Mass_width[N_Mass_bins];
  for(int i = 0; i < N_Mass_bins; i++) Mass_width[i] = Mass_bins[i+1]-Mass_bins[i];

  string excStr = "n";
  if(incl) excStr = "m"; 

  char* region = Form("%s%ij%s_b%i",excStr.c_str(),njets,reg.c_str(),btag);
  cout << source << " getting nominal histos for Jet " << jet << " in " << region << endl;

  sprintf(hist_locations,"/project/projectdirs/atlas/btamadio/RPV_SUSY/bkgEstimation");
  sprintf(f_kinematic,"%s/kinematic_hists/%s/main_kinematic_%s.root",hist_locations, source.c_str(), source.c_str());

  char* etaStr = (char*)"";
  if(etaCut == 1) etaStr = (char*)"central_";
  if(etaCut == 2) etaStr = (char*)"forward_";

  sprintf(nom,"M%i_%s%s",jet,etaStr,region);
  sprintf(dress,"M%i_%s%s",jet,etaStr,region);

  TFile *f_kin = TFile::Open(f_kinematic);
  TH1F* h_kin = (TH1F*)f_kin->Get(nom);
  h_kin->SetDirectory(0);
  f_kin->Close();
  if(isMC)h_kin->Scale(lumi);
  
  //first open all files and calculate ratio
  TH1F* temp_MJs[PE_number+1];
  for(int PE = 1; PE <= PE_number; PE++){
    sprintf(f_dressed,"%s/dressed_hists/%s/main_dressed_%s_%d.root",hist_locations, source.c_str(), source.c_str(), PE);
    TFile *f_dress = TFile::Open(f_dressed);
    temp_MJs[PE] = (TH1F*)f_dress->Get(dress);
    temp_MJs[PE]->SetDirectory(0);
    f_dress->Close();
  }
  //Switch to variable binning, then get errors bin by bin. 
  h_kinMass[jet-1] = (TH1F*)h_kin->Rebin(N_Mass_bins,Form("h_kin_m%i_%s",jet,source.c_str()),Mass_bins);
  TH1F* temp_MJs_var[PE_number+1];
  for(int PE = 1; PE <=PE_number; PE++){
    temp_MJs_var[PE] = (TH1F*)temp_MJs[PE]->Rebin(N_Mass_bins,Form("%s_PE%i",temp_MJs[PE]->GetName(),PE),Mass_bins);
  }
  //now calculate errors and bands
  h_dressMass[jet-1] = new TH1F(Form("h_dress_m%i_%s",jet,source.c_str()),"h_dress_nom",N_Mass_bins,Mass_bins);

  for(int bin = 1; bin <= N_Mass_bins; bin++){
    TH1F* temp_bin = new TH1F("temp_bin","temp_bin",1e7,0,1e7);
    for(int PE = 1; PE <= PE_number; PE++){
      temp_bin->Fill(temp_MJs_var[PE]->GetBinContent(bin));
    }
    double med    = GetMedian(temp_bin);
    h_dressMass[jet-1]->SetBinContent(bin, med);
    h_dressMass[jet-1]->SetBinError(bin, 0);
    delete temp_bin;
  }  
  for(int PE = 1; PE <= PE_number; PE++) delete temp_MJs_var[PE];

  h_dressMass[jet-1]->Scale(h_kinMass[jet-1]->Integral()/h_dressMass[jet-1]->Integral());
}

void bkgPrediction::getHistMass_wErrors(int jet,bool incl,int njets, string reg, int btag,int etaCut = 0){
  char nom[100];
  char dress[100];
  char dress_up[100];
  char dress_down[100];
  const int N_Mass_bins = 6;
  double Mass_bins[N_Mass_bins+1] = {0,0.1,0.2,0.3,0.6,1,2.0};
  double Mass_width[N_Mass_bins];
  for(int i = 0; i < N_Mass_bins; i++) Mass_width[i] = Mass_bins[i+1]-Mass_bins[i];

  string excStr = "n";
  if(incl) excStr = "m"; 

  char* region = Form("%s%ij%s_b%i",excStr.c_str(),njets,reg.c_str(),btag);
  cout << source << " getting histos for Jet " << jet << " in " << region << endl;

  sprintf(hist_locations,"/project/projectdirs/atlas/btamadio/RPV_SUSY/bkgEstimation");
  sprintf(f_kinematic,"%s/kinematic_hists/%s/main_kinematic_%s.root",hist_locations, source.c_str(), source.c_str());

  char* etaStr = (char*)"";
  if(etaCut == 1) etaStr = (char*)"central_";
  if(etaCut == 2) etaStr = (char*)"forward_";

  sprintf(nom,"M%i_%s%s",jet,etaStr,region);
  sprintf(dress,"M%i_%s%s",jet,etaStr,region);
  sprintf(dress_up,"M%i_%s%s_shiftUp",jet,etaStr,region);
  sprintf(dress_down,"M%i_%s%s_shiftDown",jet,etaStr,region);

  TFile *f_kin = TFile::Open(f_kinematic);
  TH1F* h_kin = (TH1F*)f_kin->Get(nom);
  h_kin->SetDirectory(0);
  f_kin->Close();
  if(isMC)h_kin->Scale(lumi);
  
  //first open all files and calculate ratio
  TH1F* temp_MJs[PE_number+1];
  TH1F* temp_MJs_up[PE_number+1];
  TH1F* temp_MJs_down[PE_number+1];
  for(int PE = 1; PE <= PE_number; PE++){
    sprintf(f_dressed,"%s/dressed_hists/%s/main_dressed_%s_%d.root",hist_locations, source.c_str(), source.c_str(), PE);
    TFile *f_dress = TFile::Open(f_dressed);

    temp_MJs[PE] = (TH1F*)f_dress->Get(dress);
    temp_MJs_up[PE] = (TH1F*)f_dress->Get(dress_up);
    temp_MJs_down[PE] = (TH1F*)f_dress->Get(dress_down);
    temp_MJs[PE]->SetDirectory(0);
    temp_MJs_up[PE]->SetDirectory(0);
    temp_MJs_down[PE]->SetDirectory(0);
    f_dress->Close();

  }
  //Switch to variable binning, then get errors bin by bin. 
  h_kinMass[jet-1] = (TH1F*)h_kin->Rebin(N_Mass_bins,Form("h_kin_m%i_%s",jet,source.c_str()),Mass_bins);
  TH1F* temp_MJs_var[PE_number+1];
  TH1F* temp_MJs_up_var[PE_number+1];
  TH1F* temp_MJs_down_var[PE_number+1];

  for(int PE = 1; PE <=PE_number; PE++){
    temp_MJs_var[PE] = (TH1F*)temp_MJs[PE]->Rebin(N_Mass_bins,Form("%s_PE%i",temp_MJs[PE]->GetName(),PE),Mass_bins);
    temp_MJs_up_var[PE] = (TH1F*)temp_MJs_up[PE]->Rebin(N_Mass_bins,Form("%s_PE%i",temp_MJs_up[PE]->GetName(),PE),Mass_bins);
    temp_MJs_down_var[PE] = (TH1F*)temp_MJs_down[PE]->Rebin(N_Mass_bins,Form("%s_PE%i",temp_MJs_down[PE]->GetName(),PE),Mass_bins);
  }

  //now calculate errors and bands
  h_dressMass[jet-1] = new TH1F(Form("h_dress_m%i_%s",jet,source.c_str()),"h_dress_nom",N_Mass_bins,Mass_bins);
  //TH1F* h_dress_syst_up = new TH1F("h_dress_syst_up","h_dress_syst_up",N_Mass_bins,Mass_bins);
  //TH1F* h_dress_syst_down = new TH1F("h_dress_syst_down","h_dress_syst_down",N_Mass_bins,Mass_bins);
  
  for(int bin = 1; bin <= N_Mass_bins; bin++){
    TH1F* temp_bin = new TH1F("temp_bin","temp_bin",1e7,0,1e7);
    TH1F* temp_bin_up = new TH1F("temp_bin_up","temp_bin_up",1e7,0,1e7);
    TH1F* temp_bin_down = new TH1F("temp_bin_down","temp_bin_down",1e7,0,1e7);
    for(int PE = 1; PE <= PE_number; PE++){
      temp_bin->Fill(temp_MJs_var[PE]->GetBinContent(bin));
      temp_bin_up->Fill(temp_MJs_up_var[PE]->GetBinContent(bin));
      temp_bin_down->Fill(temp_MJs_down_var[PE]->GetBinContent(bin));
    }
    double rms = temp_bin->GetRMS();
    double med    = GetMedian(temp_bin);
    double med_up = GetMedian(temp_bin_up);
    double med_down = GetMedian(temp_bin_down);
    double syst_uncert_up = fabs( med_up - med);
    double syst_uncert_down = fabs(med_down - med);
    double syst_uncert_avg = 0.5*(syst_uncert_up + syst_uncert_down);
    double total_uncert_up = pow( pow(syst_uncert_up, 2) + pow(rms, 2), 0.5);
    double total_uncert_down = pow( pow(syst_uncert_down, 2) + pow(rms, 2), 0.5);

    double total_uncert = pow(pow(syst_uncert_avg,2)+pow(rms,2),0.5);

    h_dressMass[jet-1]->SetBinContent(bin, med);
    //h_dress_syst_up->SetBinContent(bin, med_up);
    //h_dress_syst_down->SetBinContent(bin, med_down);

    h_dressMass[jet-1]->SetBinError(bin, total_uncert);
    //h_dress_syst_up->SetBinError(bin, syst_uncert_up);
    //h_dress_syst_down->SetBinError(bin, syst_uncert_down);

    delete temp_bin; delete temp_bin_up; delete temp_bin_down;
  }  

  for(int PE = 1; PE <= PE_number; PE++) {
    delete temp_MJs_var[PE];
    delete temp_MJs_up_var[PE];
    delete temp_MJs_down_var[PE];
  }

  h_dressMass[jet-1]->Scale(h_kinMass[jet-1]->Integral()/h_dressMass[jet-1]->Integral());
}

TH1F* bkgPrediction::getNonClosureMass(int jet){
  TH1F* dev;
  if(!h_kinMass[jet-1] || !h_dressMass[jet-1]){
    cout << "No kinematic or dressed histogram not found, run getHists()!" << endl;
    return dev;
  }
  
  dev = (TH1F*)h_dressMass[jet-1]->Clone("devMass");
  dev->Add(h_kinMass[jet-1],-1);
  dev->Divide(h_kinMass[jet-1]);

  return dev;
}

void bkgPrediction::getHistJetMass(bool incl,int njets, string reg, int btag,int etaCut = 0){
  char nom[100];
  char dress[100];
  const int N_Mass_bins = 6;
  double Mass_bins[N_Mass_bins+1] = {0,0.1,0.2,0.3,0.6,1,2.0};
  double Mass_width[N_Mass_bins];
  for(int i = 0; i < N_Mass_bins; i++) Mass_width[i] = Mass_bins[i+1]-Mass_bins[i];

  string excStr = "n";
  if(incl) excStr = "m"; 

  char* region = Form("%s%ij%s_b%i",excStr.c_str(),njets,reg.c_str(),btag);
  cout << source << " getting nominal histos for Jet Mass in " << region << endl;

  sprintf(hist_locations,"/project/projectdirs/atlas/btamadio/RPV_SUSY/bkgEstimation");
  sprintf(f_kinematic,"%s/kinematic_hists/%s/main_kinematic_%s.root",hist_locations, source.c_str(), source.c_str());

  char* etaStr = (char*)"";
  if(etaCut == 1) etaStr = (char*)"central_";
  if(etaCut == 2) etaStr = (char*)"forward_";

  TFile *f_kin = TFile::Open(f_kinematic);
  TH1F* h_kin = new TH1F("h_kin","h_kin",200,0,2);
  h_kin->SetDirectory(0);

  for(int jet = 1; jet < 5; jet++){
    region = Form("%s%ij%s_b%i",excStr.c_str(),njets,reg.c_str(),btag);
    sprintf(nom,"M%i_%s%s",jet,etaStr,region);
    TH1F* h_temp = (TH1F*)f_kin->Get(nom);
    h_kin->Add(h_temp);
    delete h_temp;
  }
  f_kin->Close();
  
  //first open all files
  TH1F* temp_MJs[PE_number+1];
  for(int PE = 1; PE <= PE_number; PE++){
    region = Form("%s%ij%s_b%i",excStr.c_str(),njets,reg.c_str(),btag);
    sprintf(f_dressed,"%s/dressed_hists/%s/main_dressed_%s_%d.root",hist_locations, source.c_str(), source.c_str(), PE);
    TFile *f_dress = TFile::Open(f_dressed);
    temp_MJs[PE] = new TH1F(Form("PE_%i",PE),Form("PE_%i",PE),200,0,2);
    temp_MJs[PE]->SetDirectory(0);

    for(int jet = 1; jet < 5; jet++){
      sprintf(dress,"M%i_%s%s",jet,etaStr,region);
      TH1F* h_dress_tmp = (TH1F*)f_dress->Get(dress);
      temp_MJs[PE]->Add(h_dress_tmp);
      delete h_dress_tmp;
    }
    f_dress->Close();
  }

  if(isMC)h_kin->Scale(lumi);
  
  //Switch to variable binning, then get errors bin by bin. 
  h_kinJetMass = (TH1F*)h_kin->Rebin(N_Mass_bins,Form("h_kin_jet_m_%s",source.c_str()),Mass_bins);
  TH1F* temp_MJs_var[PE_number+1];

  for(int PE = 1; PE <=PE_number; PE++){
    temp_MJs_var[PE] = (TH1F*)temp_MJs[PE]->Rebin(N_Mass_bins,Form("%s_PE%i",temp_MJs[PE]->GetName(),PE),Mass_bins);
  }

  //now calculate errors and bands
  h_dressJetMass = new TH1F(Form("h_dress_jet_m_%s",source.c_str()),"h_dress_nom",N_Mass_bins,Mass_bins);
  for(int bin = 1; bin <= N_Mass_bins; bin++){
    TH1F* temp_bin = new TH1F("temp_bin","temp_bin",1e7,0,1e7);
    for(int PE = 1; PE <= PE_number; PE++){
      temp_bin->Fill(temp_MJs_var[PE]->GetBinContent(bin));
    }
    double med = GetMedian(temp_bin);

    h_dressJetMass->SetBinContent(bin, med);
    h_dressJetMass->SetBinError(bin, 0);

    delete temp_bin;
  }  

  for(int PE = 1; PE <= PE_number; PE++) {
    delete temp_MJs_var[PE];
  }

  h_dressJetMass->Scale(h_kinJetMass->Integral()/h_dressJetMass->Integral());
}

void bkgPrediction::getHistJetMass_wErrors(bool incl,int njets, string reg, int btag,int etaCut = 0){
  char nom[100];
  char dress[100];
  char dress_up[100];
  char dress_down[100];
  const int N_Mass_bins = 6;
  double Mass_bins[N_Mass_bins+1] = {0,0.1,0.2,0.3,0.6,1,2.0};
  double Mass_width[N_Mass_bins];
  for(int i = 0; i < N_Mass_bins; i++) Mass_width[i] = Mass_bins[i+1]-Mass_bins[i];

  string excStr = "n";
  if(incl) excStr = "m"; 

  char* region = Form("%s%ij%s_b%i",excStr.c_str(),njets,reg.c_str(),btag);
  cout << source << " getting histos for Jet Mass in " << region << endl;

  sprintf(hist_locations,"/project/projectdirs/atlas/btamadio/RPV_SUSY/bkgEstimation");
  sprintf(f_kinematic,"%s/kinematic_hists/%s/main_kinematic_%s.root",hist_locations, source.c_str(), source.c_str());

  char* etaStr = (char*)"";
  if(etaCut == 1) etaStr = (char*)"central_";
  if(etaCut == 2) etaStr = (char*)"forward_";


  TFile *f_kin = TFile::Open(f_kinematic);
  TH1F* h_kin = new TH1F("h_kin","h_kin",200,0,2);
  h_kin->SetDirectory(0);

  for(int jet = 1; jet < 5; jet++){
    region = Form("%s%ij%s_b%i",excStr.c_str(),njets,reg.c_str(),btag);
    sprintf(nom,"M%i_%s%s",jet,etaStr,region);
    TH1F* h_temp = (TH1F*)f_kin->Get(nom);
    h_kin->Add(h_temp);
    delete h_temp;
  }
  f_kin->Close();
  
  //first open all files
  TH1F* temp_MJs[PE_number+1];
  TH1F* temp_MJs_up[PE_number+1];
  TH1F* temp_MJs_down[PE_number+1];
  for(int PE = 1; PE <= PE_number; PE++){
    region = Form("%s%ij%s_b%i",excStr.c_str(),njets,reg.c_str(),btag);
    sprintf(f_dressed,"%s/dressed_hists/%s/main_dressed_%s_%d.root",hist_locations, source.c_str(), source.c_str(), PE);
    TFile *f_dress = TFile::Open(f_dressed);
    temp_MJs[PE] = new TH1F(Form("PE_%i",PE),Form("PE_%i",PE),200,0,2);
    temp_MJs_up[PE] = new TH1F(Form("PE_%i_up",PE),Form("PE_%i_up",PE),200,0,2);
    temp_MJs_down[PE] = new TH1F(Form("PE_%i_down",PE),Form("PE_%i_down",PE),200,0,2);
    temp_MJs[PE]->SetDirectory(0);
    temp_MJs_up[PE]->SetDirectory(0);
    temp_MJs_down[PE]->SetDirectory(0);

    for(int jet = 1; jet < 5; jet++){
      sprintf(dress,"M%i_%s%s",jet,etaStr,region);
      sprintf(dress_up,"M%i_%s%s_shiftUp",jet,etaStr,region);
      sprintf(dress_down,"M%i_%s%s_shiftDown",jet,etaStr,region);

      TH1F* h_dress_tmp = (TH1F*)f_dress->Get(dress);
      TH1F* h_dress_tmp_up = (TH1F*)f_dress->Get(dress_up);
      TH1F* h_dress_tmp_down = (TH1F*)f_dress->Get(dress_down);

      temp_MJs[PE]->Add(h_dress_tmp);
      temp_MJs_up[PE]->Add(h_dress_tmp_up);
      temp_MJs_down[PE]->Add(h_dress_tmp_down);

      delete h_dress_tmp;
      delete h_dress_tmp_up;
      delete h_dress_tmp_down;
    }
    f_dress->Close();
  }

  if(isMC)h_kin->Scale(lumi);
  
  //Switch to variable binning, then get errors bin by bin. 
  h_kinJetMass = (TH1F*)h_kin->Rebin(N_Mass_bins,Form("h_kin_jet_m_%s",source.c_str()),Mass_bins);
  TH1F* temp_MJs_var[PE_number+1];
  TH1F* temp_MJs_up_var[PE_number+1];
  TH1F* temp_MJs_down_var[PE_number+1];

  for(int PE = 1; PE <=PE_number; PE++){
    temp_MJs_var[PE] = (TH1F*)temp_MJs[PE]->Rebin(N_Mass_bins,Form("%s_PE%i",temp_MJs[PE]->GetName(),PE),Mass_bins);
    temp_MJs_up_var[PE] = (TH1F*)temp_MJs_up[PE]->Rebin(N_Mass_bins,Form("%s_PE%i",temp_MJs_up[PE]->GetName(),PE),Mass_bins);
    temp_MJs_down_var[PE] = (TH1F*)temp_MJs_down[PE]->Rebin(N_Mass_bins,Form("%s_PE%i",temp_MJs_down[PE]->GetName(),PE),Mass_bins);
  }

  //now calculate errors and bands
  h_dressJetMass = new TH1F(Form("h_dress_jet_m_%s",source.c_str()),"h_dress_nom",N_Mass_bins,Mass_bins);
  //TH1F* h_dress_syst_up = new TH1F("h_dress_syst_up","h_dress_syst_up",N_Mass_bins,Mass_bins);
  //TH1F* h_dress_syst_down = new TH1F("h_dress_syst_down","h_dress_syst_down",N_Mass_bins,Mass_bins);
  
  for(int bin = 1; bin <= N_Mass_bins; bin++){
    TH1F* temp_bin = new TH1F("temp_bin","temp_bin",1e7,0,1e7);
    TH1F* temp_bin_up = new TH1F("temp_bin_up","temp_bin_up",1e7,0,1e7);
    TH1F* temp_bin_down = new TH1F("temp_bin_down","temp_bin_down",1e7,0,1e7);
    for(int PE = 1; PE <= PE_number; PE++){
      temp_bin->Fill(temp_MJs_var[PE]->GetBinContent(bin));
      temp_bin_up->Fill(temp_MJs_up_var[PE]->GetBinContent(bin));
      temp_bin_down->Fill(temp_MJs_down_var[PE]->GetBinContent(bin));
    }
    double rms = temp_bin->GetRMS();
    double med    = GetMedian(temp_bin);
    double med_up = GetMedian(temp_bin_up);
    double med_down = GetMedian(temp_bin_down);
    double syst_uncert_up = fabs( med_up - med);
    double syst_uncert_down = fabs(med_down - med);
    double syst_uncert_avg = 0.5*(syst_uncert_up + syst_uncert_down);
    double total_uncert_up = pow( pow(syst_uncert_up, 2) + pow(rms, 2), 0.5);
    double total_uncert_down = pow( pow(syst_uncert_down, 2) + pow(rms, 2), 0.5);

    double total_uncert = pow(pow(syst_uncert_avg,2)+pow(rms,2),0.5);

    h_dressJetMass->SetBinContent(bin, med);
    //h_dress_syst_up->SetBinContent(bin, med_up);
    //h_dress_syst_down->SetBinContent(bin, med_down);

    h_dressJetMass->SetBinError(bin, total_uncert);
    //h_dress_syst_up->SetBinError(bin, syst_uncert_up);
    //h_dress_syst_down->SetBinError(bin, syst_uncert_down);

    delete temp_bin; delete temp_bin_up; delete temp_bin_down;
  }  

  for(int PE = 1; PE <= PE_number; PE++) {
    delete temp_MJs_var[PE];
    delete temp_MJs_up_var[PE];
    delete temp_MJs_down_var[PE];
  }

  h_dressJetMass->Scale(h_kinJetMass->Integral()/h_dressJetMass->Integral());
}

TH1F* bkgPrediction::getNonClosureJetMass(){
  TH1F* dev;
  if(!h_kinJetMass || !h_dressJetMass){
    cout << "No kinematic or dressed histogram not found, run getHists()!" << endl;
    return dev;
  }
  dev = (TH1F*)h_dressJetMass->Clone("devMass");
  dev->Add(h_kinJetMass,-1);
  dev->Divide(h_kinJetMass);

  return dev;
}

void bkgPrediction::setSR_cut(double sr){
	SR_cut = sr;
	sprintf(SR_cut_str,"SR_cut_%igev",(int)(SR_cut*1e3));
	cout << "SR Cut set to " << SR_cut << " TeV" << endl;
}

void bkgPrediction::setBlind(int blind){
  isBlinded = (bool)blind;
  if(isBlinded){
    cout << "blinding beyond MJ > 0.6 TeV turned on." << endl;
  }
  else{
     cout << "blinding beyond MJ > 0.6 TeV turned off." << endl; 
  }
}

void bkgPrediction::setLumi(double lum){
  lumi = lum;
  cout << "Luminosity Set to " << Form("%3.2f",lumi) << " fb-1." << endl;
}

void bkgPrediction::setPEs(int n){
  PE_number = n;
  cout << "Set PE_number to " << PE_number << endl;
  cout << "Going to use first " << PE_number << " files in:" << endl;
  cout << hist_locations << "/dressed_hists/" << source.c_str() << "/" << endl;
}

double bkgPrediction::getObs(){return obs;}
double bkgPrediction::getObsError(){return obs_error;}
double bkgPrediction::getPred(){return pred;}
double bkgPrediction::getPESpread(){return pred_error;}
double bkgPrediction::getPtRes(){return pred_error2;}
double bkgPrediction::getLumi(){return lumi;}
string bkgPrediction::getName(){return source;}
TH1F* bkgPrediction::getYield(){return yield;}
TH1F* bkgPrediction::kinMJ(){return h_kinMJ;}
TH1F* bkgPrediction::dressedMJ(){return h_dressMJ;}
TH1F* bkgPrediction::dressedMJ_up(){return h_dressMJ_up;}
TH1F* bkgPrediction::dressedMJ_down(){return h_dressMJ_down;}
TH1F* bkgPrediction::kinMass(int jet){return h_kinMass[jet-1];}
TH1F* bkgPrediction::dressedMass(int jet){return h_dressMass[jet-1];}
TH1F* bkgPrediction::kinJetMass(){return h_kinJetMass;}
TH1F* bkgPrediction::dressedJetMass(){return h_dressJetMass;}

