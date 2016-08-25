#include "Style/AtlasStyle.C"
#include "Style/AtlasLabels.C"
#include "analysis.h"
#include <cmath>
#include <iostream>
#include <fstream>

/*************************************************************************************************/
/**************************** MC NON ClOSURE SCRIPTS *********************************************/
/*************************************************************************************************/

TH1F* getMCnonClosureHist(string reg,int btag){
  string source="sherpa";
  int njets = 4;
  bool incl=true;
  PE_number = 100;

    char hist_locations[200];
    char f_kinematic[300];
    char f_dressed[300];
    char nom[100];
    char dress[100];
    int SRbin = (int)(100*SR_cut+1);
  const int N_MJ_bins = 14;
    //double MJ_bins[N_MJ_bins+1] = {0,0.1,0.2,0.3,0.6,0.8,1,2.0};
    double MJ_bins[N_MJ_bins+1] = {0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.40,.45,0.5,0.6,0.8,1.0,2.0};

    double MJ_width[N_MJ_bins];
    for(int i = 0; i < N_MJ_bins; i++) MJ_width[i] = MJ_bins[i+1]-MJ_bins[i];

    string excStr = "n";
    if(incl) excStr = "m";

    char* region = Form("%s%ij%s_b%i",excStr.c_str(),njets,reg.c_str(),btag);

    sprintf(hist_locations,"/project/projectdirs/atlas/ndshal/RPV_SUSY/bkgEstimation");
    sprintf(f_kinematic,"%s/kinematic_hists/%s/main_kinematic_%s.root",hist_locations, source.c_str(), source.c_str());

    sprintf(nom,"MJ_%s",region);
    sprintf(dress,"MJ_%s",region);

    TFile *f_kin = TFile::Open(f_kinematic);
    TH1F* h_kin = (TH1F*)f_kin->Get(nom);
    h_kin->Scale(lumi);

  //Truth values
  double obs_error, obs;
  double obs_NR_error, obs_NR;
  obs = h_kin->IntegralAndError(SRbin,200,obs_error);
  obs_NR = h_kin->IntegralAndError(21,60,obs_NR_error);

  //Open all files and calculate yield distributions
  TH1F* temp_MJs[PE_number+1];
  TH1F* yield_NR = new TH1F("yield_NR","yield_NR",1e6,0,1e6);
  for(int PE = 1; PE <= PE_number; PE++){
    sprintf(f_dressed,"%s/dressed_hists/%s/main_dressed_%s_%d.root",hist_locations, source.c_str(), source.c_str(), PE);
    TFile *f_dress = TFile::Open(f_dressed);

    temp_MJs[PE] = (TH1F*)f_dress->Get(dress);

    yield_NR->Fill(temp_MJs[PE]->Integral(21,60));
  }

  double med_NR = GetMedian(yield_NR);
  double rms_NR = yield_NR->GetRMS();
  double norm = obs_NR/med_NR;

  //Switch to variable binning, then get errors bin by bin. 
  
  TH1F* h_kin_var = (TH1F*)h_kin->Rebin(N_MJ_bins,"h_kin_var",MJ_bins);
  TH1F* temp_MJs_var[PE_number+1];

  for(int PE = 1; PE <=PE_number; PE++){
    temp_MJs_var[PE] = (TH1F*)temp_MJs[PE]->Rebin(N_MJ_bins,Form("%s_PE%i",temp_MJs[PE]->GetName(),PE),MJ_bins);
  }

  //now calculate errors and bands
  TH1F* h_dress_stat = new TH1F("h_dress_stat","h_dress_stat",N_MJ_bins,MJ_bins);
  for(int bin = 1; bin <= N_MJ_bins; bin++){
    TH1F* temp_bin = new TH1F("temp_bin","temp_bin",1e7,0,1e7);
    for(int PE = 1; PE <= PE_number; PE++){
      temp_bin->Fill(temp_MJs_var[PE]->GetBinContent(bin));
    }
    double med = GetMedian(temp_bin);

    h_dress_stat->SetBinContent(bin, med);
    h_dress_stat->SetBinError(bin,0);

    delete temp_bin;
  }
  h_dress_stat->Scale(norm);

  TH1F *ratio_nom = (TH1F*)h_dress_stat->Clone("ratio_nom");
  ratio_nom->Add(h_kin_var,-1);
  ratio_nom->Divide(h_kin_var);

  return ratio_nom;
}


TH1F* getMCnonClosureHistMass(string reg,int jet, int btag){
  string source="pythia_softJetTempl";
  int njets = 4;
  bool incl=true;
  PE_number = 100;

    char hist_locations[200];
    char f_kinematic[300];
    char f_dressed[300];
    char nom[100];
    char dress[100];
  const int N_Mass_bins = 6;

  double Mass_bins[N_Mass_bins+1] = {0,0.1,0.2,0.3,0.6,1,2.0};
  double Mass_width[N_Mass_bins];
  for(int i = 0; i < N_Mass_bins; i++) Mass_width[i] = Mass_bins[i+1]-Mass_bins[i];

    string excStr = "n";
    if(incl) excStr = "m";

    char* region = Form("%s%ij%s_b%i",excStr.c_str(),njets,reg.c_str(),btag);

    sprintf(hist_locations,"/project/projectdirs/atlas/ndshal/RPV_SUSY/bkgEstimation");
    sprintf(f_kinematic,"%s/kinematic_hists/%s/main_kinematic_%s.root",hist_locations, source.c_str(), source.c_str());

  sprintf(nom,"M%i_%s",jet,region);
  sprintf(dress,"M%i_%s",jet,region);

    TFile *f_kin = TFile::Open(f_kinematic);
    TH1F* h_kin = (TH1F*)f_kin->Get(nom);
    h_kin->Scale(lumi);

  //Open all files and calculate yield distributions
  TH1F* temp_MJs[PE_number+1];
  for(int PE = 1; PE <= PE_number; PE++){
    sprintf(f_dressed,"%s/dressed_hists/%s/main_dressed_%s_%d.root",hist_locations, source.c_str(), source.c_str(), PE);
    TFile *f_dress = TFile::Open(f_dressed);

    temp_MJs[PE] = (TH1F*)f_dress->Get(dress);
  }

  //Switch to variable binning, then get errors bin by bin. 
  
  TH1F* h_kin_var = (TH1F*)h_kin->Rebin(N_Mass_bins,"h_kin_var",Mass_bins);
  TH1F* temp_MJs_var[PE_number+1];

  for(int PE = 1; PE <=PE_number; PE++){
    temp_MJs_var[PE] = (TH1F*)temp_MJs[PE]->Rebin(N_Mass_bins,Form("%s_PE%i",temp_MJs[PE]->GetName(),PE),Mass_bins);
  }

  //now calculate errors and bands
  TH1F* h_dress_stat = new TH1F("h_dress_stat","h_dress_stat",N_Mass_bins,Mass_bins);
  for(int bin = 1; bin <= N_Mass_bins; bin++){
    TH1F* temp_bin = new TH1F("temp_bin","temp_bin",1e7,0,1e7);
    for(int PE = 1; PE <= PE_number; PE++){
      temp_bin->Fill(temp_MJs_var[PE]->GetBinContent(bin));
    }
    double med = GetMedian(temp_bin);

    h_dress_stat->SetBinContent(bin, med);
    h_dress_stat->SetBinError(bin,0);

    delete temp_bin;
  }
  double kin_area = h_kin_var->Integral();
  h_dress_stat->Scale(kin_area/h_dress_stat->Integral());

  TH1F *ratio_nom = (TH1F*)h_dress_stat->Clone("ratio_nom");
  ratio_nom->Add(h_kin_var,-1);
  ratio_nom->Divide(h_kin_var);

  return ratio_nom;
}

/*************************************************************************************************/
/**************************** PLOTTING MACROS IN A REGION ****************************************/
/*************************************************************************************************/

void compare_MJs(string reg,int njets, int btag,bool incl,string source){
  //compares MJ in dressed and kinematic samples "source"

  SetAtlasStyle();
  char hist_locations[200];
  char f_kinematic[300];
  char f_dressed[300];
  char nom[100];
  char dress[100];
  char dress_up[100];
  char dress_down[100];
  int SRbin = (int)(100*SR_cut+1);
  const int N_MJ_bins = 14;
  //double MJ_bins[N_MJ_bins+1] = {0,0.1,0.2,0.3,0.6,0.8,1,2.0};
  double MJ_bins[N_MJ_bins+1] = {0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.40,.45,0.5,0.6,0.8,1.0,2.0};

  bool blindPlot = (blinded && reg == "SR" && njets >= 4);

  double MJ_width[N_MJ_bins];
  for(int i = 0; i < N_MJ_bins; i++) MJ_width[i] = MJ_bins[i+1]-MJ_bins[i];

  string excStr = "n";
  if(incl) excStr = "m";

  char* region = Form("%s%ij%s_b%i",excStr.c_str(),njets,reg.c_str(),btag);
  cout << region << " Prediction for " << source << endl;
  gROOT->ProcessLine(Form(".! mkdir %s/%s",plot_locations,region));

  sprintf(hist_locations,"/project/projectdirs/atlas/ndshal/RPV_SUSY/bkgEstimation");
  sprintf(f_kinematic,"%s/kinematic_hists/%s/main_kinematic_%s.root",hist_locations, source.c_str(), source.c_str());

  sprintf(nom,"MJ_%s",region);
  sprintf(dress,"MJ_%s",region);
  sprintf(dress_up,"MJ_%s_shiftUp",region);
  sprintf(dress_down,"MJ_%s_shiftDown",region);

  TFile *f_kin = TFile::Open(f_kinematic);
  TH1F* h_kin = (TH1F*)f_kin->Get(nom);
  if(isMC)h_kin->Scale(lumi);

  if(blindPlot){
    for(int i = 61; i < 201; i++){
      h_kin->SetBinContent(i,0);
      h_kin->SetBinError(i,0);}
  }

  //Truth values
  double obs_error, obs;
  double obs_NR_error, obs_NR;
  obs = h_kin->IntegralAndError(SRbin,200,obs_error);
  obs_NR = h_kin->IntegralAndError(21,60,obs_NR_error);

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

    temp_MJs[PE] = (TH1F*)f_dress->Get(dress);
    temp_MJs_up[PE] = (TH1F*)f_dress->Get(dress_up);
    temp_MJs_down[PE] = (TH1F*)f_dress->Get(dress_down);

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

  double pred = med_SR*norm;
  double pred_up = med_SR_up*obs_NR/med_NR_up;
  double pred_down = med_SR_down*obs_NR/med_NR_down;
  double pred_error = pred*sqrt(pow(rms_SR/med_SR,2)+pow(rms_NR/med_NR,2));
  double pred_error2 = (fabs(pred_up-pred) + fabs(pred_down-pred))/2;
  double pred_error3 = pred*getMCnonClosure(reg,btag,SR_cut);
  double dev = (pred-obs)/obs;

  //Switch to variable binning, then get errors bin by bin. 
  
  TH1F* h_kin_var = (TH1F*)h_kin->Rebin(N_MJ_bins,"h_kin_var",MJ_bins);
  TH1F* temp_MJs_var[PE_number+1];
  TH1F* temp_MJs_up_var[PE_number+1];
  TH1F* temp_MJs_down_var[PE_number+1];

  for(int PE = 1; PE <=PE_number; PE++){
    temp_MJs_var[PE] = (TH1F*)temp_MJs[PE]->Rebin(N_MJ_bins,Form("%s_PE%i",temp_MJs[PE]->GetName(),PE),MJ_bins);
    temp_MJs_up_var[PE] = (TH1F*)temp_MJs_up[PE]->Rebin(N_MJ_bins,Form("%s_PE%i",temp_MJs_up[PE]->GetName(),PE),MJ_bins);
    temp_MJs_down_var[PE] = (TH1F*)temp_MJs_down[PE]->Rebin(N_MJ_bins,Form("%s_PE%i",temp_MJs_down[PE]->GetName(),PE),MJ_bins);
  }

  //now calculate errors and bands
  TH1F* h_dress_nom = new TH1F("h_dress_nom","h_dress_nom",N_MJ_bins,MJ_bins);
  TH1F* h_dress_stat = new TH1F("h_dress_stat","h_dress_stat",N_MJ_bins,MJ_bins);
  TH1F* h_dress_syst_up = new TH1F("h_dress_syst_up","h_dress_syst_up",N_MJ_bins,MJ_bins);
  TH1F* h_dress_syst_down = new TH1F("h_dress_syst_down","h_dress_syst_down",N_MJ_bins,MJ_bins);
  for(int bin = 1; bin <= N_MJ_bins; bin++){
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

    h_dress_nom->SetBinContent(bin, med);
    h_dress_stat->SetBinContent(bin, med);
    h_dress_syst_up->SetBinContent(bin, med_up);
    h_dress_syst_down->SetBinContent(bin, med_down);

    h_dress_nom->SetBinError(bin, total_uncert);
    h_dress_stat->SetBinError(bin, 0);
    h_dress_syst_up->SetBinError(bin, syst_uncert_up);
    h_dress_syst_down->SetBinError(bin, syst_uncert_down);

    delete temp_bin; delete temp_bin_up; delete temp_bin_down;
  }
  h_dress_nom->Scale(norm);
  h_dress_stat->Scale(norm);

  /*
  //Add mc non closure
  TH1F* mcHist;
  if(!isMC){
  mcHist = getMCnonClosureHist(reg,btag);
  for(int bin =1; bin <= N_MJ_bins; bin++){
    double mc_err = mcHist->GetBinContent(bin)*h_dress_nom->GetBinContent(bin);
    double data_err = h_dress_nom->GetBinError(bin);
    double err = pow(pow(mc_err,2)+pow(data_err,2),0.5);

    h_dress_nom->SetBinError(bin,err);
  }
}
*/


  //Scale bin contents by bin width
  for(int bin = 1; bin <= N_MJ_bins; bin ++){
    h_kin_var->SetBinContent(bin, h_kin_var->GetBinContent(bin)/MJ_width[bin-1]);
    h_kin_var->SetBinError(bin, h_kin_var->GetBinError(bin)/MJ_width[bin-1]);

    h_dress_stat->SetBinContent(bin, h_dress_stat->GetBinContent(bin)/MJ_width[bin-1]);
    h_dress_nom->SetBinContent(bin, h_dress_nom->GetBinContent(bin)/MJ_width[bin-1]);
    h_dress_nom->SetBinError(bin, h_dress_nom->GetBinError(bin)/MJ_width[bin-1]);
  }

  TH1F *ratio_nom = (TH1F*)h_kin_var->Clone("ratio_nom");
  ratio_nom->Divide(h_dress_stat);
  ratio_nom->GetXaxis()->SetTitle("M_{J}^{#Sigma} [TeV]");

  TH1F *ratio_band = (TH1F*)h_dress_nom->Clone("ratio_band");
  ratio_band->Divide(h_dress_stat);
  ratio_band->SetLineColor(0);
  ratio_band->SetMarkerSize(0);
  ratio_band->SetFillColor(kRed);
  ratio_band->SetFillStyle(3010);

  h_kin_var->SetLineColor(1);
  h_kin_var->SetMarkerColor(1);

  h_dress_nom->SetLineColor(2);
  h_dress_nom->SetMarkerColor(2);
  h_dress_nom->SetMarkerSize(0.001);
  h_dress_nom->SetFillColor(kRed);
  h_dress_nom->SetFillStyle(3010);
  h_dress_stat->SetLineColor(2);
  ratio_nom->SetLineColor(1);
  ratio_nom->SetMarkerColor(1);
  //ratio_nom->SetFillColor(kRed);
  //ratio_nom->SetFillStyle(3010);

  char c_pred[200];
  char c_pred_int[200];
  char c_obs[200];
  sprintf(c_obs,"M_{J,obs}  = %3.1f", obs);
  if(blindPlot) sprintf(c_obs,"data blinded for M_{J} > 0.6");
  sprintf(c_pred,"M_{J,pred} = %3.1f #pm %3.1f #pm %3.1f", pred, pred_error, pred_error2);

  TCanvas *c_1 = new TCanvas("c_1","c_1");
  c_1->Divide(1, 2);
  c_1->GetPad(1)->SetPad(0,0.30,1,1.);
  c_1->GetPad(2)->SetPad(0,0,1,0.30);
  c_1->GetPad(2)->SetGridy();
  c_1->GetPad(1)->SetLogy();
  c_1->GetPad(2)->SetTopMargin(0.035);
  c_1->GetPad(1)->SetBottomMargin(0.025);
  c_1->GetPad(2)->SetBottomMargin(0.31);
  
  c_1->cd(1);
  h_kin_var->GetXaxis()->SetRangeUser(0,2);
  h_kin_var->GetYaxis()->SetRangeUser(max(1.,0.9*h_kin_var->GetMinimum()),3*h_kin_var->GetMaximum());
  h_kin_var->GetYaxis()->SetTitle("Events / Bin width [TeV^{-1}]");
  h_kin_var->GetYaxis()->SetTitleSize(18);
  h_kin_var->GetYaxis()->SetTitleFont(43);
  h_kin_var->GetYaxis()->SetTitleOffset(1.);
  h_kin_var->GetYaxis()->SetLabelSize(0.06);
  h_kin_var->GetXaxis()->SetLabelOffset(999);
  h_kin_var->GetXaxis()->SetLabelSize(0);
  h_kin_var->Draw("E");
  h_dress_nom->Draw("E2same");
  h_dress_stat->Draw("histsame");
  h_kin_var->Draw("Esame");

  c_1->cd(2);
  ratio_nom->GetXaxis()->SetRangeUser(0,2.0);
  ratio_nom->GetYaxis()->SetRangeUser(0,2.0);
  ratio_nom->GetYaxis()->SetTitle("Data/Pred");
  if(isMC) ratio_nom->GetYaxis()->SetTitle("Kin/Pred");
  ratio_nom->GetYaxis()->SetTitleSize(18);
  ratio_nom->GetYaxis()->SetTitleFont(43);
  ratio_nom->GetYaxis()->SetTitleOffset(1.);
  ratio_nom->GetYaxis()->SetLabelFont(43);
  ratio_nom->GetYaxis()->SetLabelSize(18);
  ratio_nom->GetYaxis()->SetNdivisions(5);
  ratio_nom->GetXaxis()->SetTitleSize(18);
  ratio_nom->GetXaxis()->SetTitleFont(43);
  ratio_nom->GetXaxis()->SetTitleOffset(4.0);
  ratio_nom->GetXaxis()->SetLabelFont(43);
  ratio_nom->GetXaxis()->SetLabelSize(18);
  ratio_nom->SetTitle("");
  ratio_nom->Draw("E1");
  //ratio_nom->Draw("E2same");
  ratio_band->Draw("E2same");
  ratio_nom->Draw("E1same");
  
  c_1->cd(1);

  double x_loc = 0.60; double y_loc = 0.60;
  ATLASLabel(x_loc-0.28,y_loc+0.25,"Internal",0.06,0.10);
  TLegend leg_1(x_loc-0.05,y_loc-0.05,x_loc+0.15,y_loc+0.15);

  char* kin_name = (char*)"Data";
  if(isMC) kin_name = (char*)"Kinematic";

  leg_1.AddEntry(h_kin,kin_name,"LP");
  leg_1.AddEntry(h_dress_nom,"Prediction","LF");
  leg_1.SetLineColor(0);
  leg_1.SetTextSize(0.06);
  leg_1.SetShadowColor(0);
  leg_1.SetFillStyle(0);
  leg_1.SetFillColor(0);
  leg_1.Draw();
  
  TLatex cap;
  cap.SetNDC();
  cap.SetTextColor(1);
  cap.SetTextSize(0.06);
  if(njets >= 4 && !blindPlot){
  cap.DrawLatex(x_loc-0.05,y_loc+0.18,c_obs);
  cap.DrawLatex(x_loc-0.05,y_loc+0.25,c_pred);}

  char jet_multi[100];
  sprintf(jet_multi, "%3.1f fb^{-1}  data",lumi);
  if(isMC) sprintf(jet_multi, "%3.1f fb^{-1}  pythia",lumi);
  cap.DrawLatex(x_loc-0.25,y_loc+0.18,jet_multi);
  
  if(!incl){sprintf(jet_multi, "N_{largeR jet} = %d",njets);}
  if(incl){sprintf(jet_multi, "N_{largeR jet} #geq %d",njets);}
  cap.DrawLatex(.15,0.25,jet_multi);

  if(reg == "CR") sprintf(jet_multi, "|#Delta#eta| > 1.4");
  if(reg == "VR") sprintf(jet_multi, "|#Delta#eta| > %3.1f",dEta_cut);
  if(reg == "SR") sprintf(jet_multi, "|#Delta#eta| < %3.1f",dEta_cut);
  cap.DrawLatex(0.15,0.1,jet_multi);

  if(btag == 0){sprintf(jet_multi,"b-veto");}
  if(btag == 1){sprintf(jet_multi,"b-tag");}
  if(btag == 9){sprintf(jet_multi,"inclusive");}
  cap.DrawLatex(0.15,0.17,jet_multi);

  region = Form("%s%ij%s_b%i",excStr.c_str(),njets,reg.c_str(),btag);

  char save_name[300]; 
  sprintf(save_name,"%s/%s/plot_MJ_%s_%s_%s_NS.pdf",plot_locations,region,SR_cut_str,region,source.c_str());
  c_1->SaveAs(save_name);
  sprintf(save_name,"%s/%s/plot_MJ_%s_%s_%s_NS.png",plot_locations,region,SR_cut_str,region,source.c_str());
  c_1->SaveAs(save_name);

 if(!incl) texfile << Form("%i &",njets);
 if(incl) texfile << Form("$ \\geq %i$ &",njets);
 texfile << jet_multi;
 
if(reg == "CR") texfile <<"& $ > 1.4$ &";
if(reg == "VR") texfile <<Form("& $ > %3.1f$ &",dEta_cut);
if(reg == "SR") texfile <<Form("& $ < %3.1f$ &",dEta_cut);

 if(blindPlot) {
  texfile << Form("\\textbf{\\textit{blinded}} &");
  texfile << Form(" $%3.1f \\pm %3.1f \\pm %3.1f$ \\\\[0.2em] \n", pred, pred_error, pred_error2);
}
 else {
  if(isMC) texfile << Form("$%3.1f \\pm %3.1f$ &", obs, obs_error);
  else texfile << Form("$%i$ &", (int)obs);
  texfile << Form(" $%3.1f \\pm %3.1f \\pm %3.1f$", pred, pred_error, pred_error2);
  texfile << Form("& $%3.3f$ \\\\[0.2em] \n", dev);
}
  return;
}

void compare_MJs_wSig(string reg,int njets, int btag,bool incl,string source){
  //compares MJ in dressed and kinematic samples "source", added two signal points.

  SetAtlasStyle();
  char hist_locations[200];
  char f_kinematic[300];
  char f_dressed[300];
  char nom[100];
  char dress[100];
  char dress_up[100];
  char dress_down[100];
  int SRbin = (int)(100*SR_cut+1);
  const int N_MJ_bins = 14;
  //double MJ_bins[N_MJ_bins+1] = {0,0.1,0.2,0.3,0.6,0.8,1,2.0};
  double MJ_bins[N_MJ_bins+1] = {0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.40,.45,0.5,0.6,0.8,1.0,2.0};

  bool blindPlot = (blinded && reg == "SR" && njets >= 4);

  double MJ_width[N_MJ_bins];
  for(int i = 0; i < N_MJ_bins; i++) MJ_width[i] = MJ_bins[i+1]-MJ_bins[i];

  string excStr = "n";
  if(incl) excStr = "m";

  char* region = Form("%s%ij%s_b%i",excStr.c_str(),njets,reg.c_str(),btag);
  cout << region << " Plot with Signal for " << source << endl;
  gROOT->ProcessLine(Form(".! mkdir %s/%s",plot_locations,region));

  sprintf(hist_locations,"/project/projectdirs/atlas/ndshal/RPV_SUSY/bkgEstimation");
  sprintf(f_kinematic,"%s/kinematic_hists/%s/main_kinematic_%s.root",hist_locations, source.c_str(), source.c_str());

  sprintf(nom,"MJ_%s",region);
  sprintf(dress,"MJ_%s",region);
  sprintf(dress_up,"MJ_%s_shiftUp",region);
  sprintf(dress_down,"MJ_%s_shiftDown",region);

  TFile *f_kin = TFile::Open(f_kinematic);
  TH1F* h_kin = (TH1F*)f_kin->Get(nom);
  if(isMC)h_kin->Scale(lumi);
  if(blindPlot){
    for(int i = 61; i < 201; i++){
      h_kin->SetBinContent(i,0);
      h_kin->SetBinError(i,0);}
  }

  TFile *f_sig[2];
  TH1F* h_kin_sig[2];

  sprintf(f_kinematic,"%s/kinematic_hists/mg1600_mx650/main_kinematic_mg1600_mx650.root",hist_locations);
  f_sig[0] = TFile::Open(f_kinematic);
  h_kin_sig[0] = (TH1F*)f_sig[0]->Get(nom);
  h_kin_sig[0]->Scale(lumi); 

  //sprintf(f_kinematic,"%s/kinematic_hists/mg1200_mx50/main_kinematic_mg1200_mx50.root",hist_locations);
  sprintf(f_kinematic,"%s/kinematic_hists/mg1200/main_kinematic_mg1200.root",hist_locations);
  f_sig[1] = TFile::Open(f_kinematic);
  h_kin_sig[1] = (TH1F*)f_sig[1]->Get(nom);
  h_kin_sig[1]->Scale(lumi);

  //Truth values
  double obs_error, obs;
  double obs_NR_error, obs_NR;
  obs = h_kin->IntegralAndError(SRbin,200,obs_error);
  obs_NR = h_kin->IntegralAndError(21,60,obs_NR_error);

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
    if(!isFile(f_dressed)) continue;
    TFile *f_dress = TFile::Open(f_dressed);

    temp_MJs[PE] = (TH1F*)f_dress->Get(dress);
    temp_MJs_up[PE] = (TH1F*)f_dress->Get(dress_up);
    temp_MJs_down[PE] = (TH1F*)f_dress->Get(dress_down);

  

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

  double pred = med_SR*norm;
  double pred_up = med_SR_up*obs_NR/med_NR_up;
  double pred_down = med_SR_down*obs_NR/med_NR_down;
  double pred_error = pred*sqrt(pow(rms_SR/med_SR,2)+pow(rms_NR/med_NR,2));
  double pred_error2 = (fabs(pred_up-pred) + fabs(pred_down-pred))/2;
  double pred_error3 = pred*getMCnonClosure(reg,btag,SR_cut);
  double dev = (pred-obs)/obs;

  //Switch to variable binning, then get errors bin by bin. 
  
  TH1F* h_kin_var = (TH1F*)h_kin->Rebin(N_MJ_bins,"h_kin_var",MJ_bins);
  TH1F* h_kin_sig_var[2];
  h_kin_sig_var[0] = (TH1F*)h_kin_sig[0]->Rebin(N_MJ_bins,"h_kin_sig_var_0",MJ_bins);
  h_kin_sig_var[1] = (TH1F*)h_kin_sig[1]->Rebin(N_MJ_bins,"h_kin_sig_var_1",MJ_bins);
  TH1F* temp_MJs_var[PE_number+1];
  TH1F* temp_MJs_up_var[PE_number+1];
  TH1F* temp_MJs_down_var[PE_number+1];

  for(int PE = 1; PE <=PE_number; PE++){
    if(!isFile(f_dressed)) continue;
    temp_MJs_var[PE] = (TH1F*)temp_MJs[PE]->Rebin(N_MJ_bins,Form("%s_PE%i",temp_MJs[PE]->GetName(),PE),MJ_bins);
    temp_MJs_up_var[PE] = (TH1F*)temp_MJs_up[PE]->Rebin(N_MJ_bins,Form("%s_PE%i",temp_MJs_up[PE]->GetName(),PE),MJ_bins);
    temp_MJs_down_var[PE] = (TH1F*)temp_MJs_down[PE]->Rebin(N_MJ_bins,Form("%s_PE%i",temp_MJs_down[PE]->GetName(),PE),MJ_bins);
  }

  //now calculate errors and bands
  TH1F* h_dress_nom = new TH1F("h_dress_nom","h_dress_nom",N_MJ_bins,MJ_bins);
  TH1F* h_dress_stat = new TH1F("h_dress_stat","h_dress_stat",N_MJ_bins,MJ_bins);
  TH1F* h_dress_syst_up = new TH1F("h_dress_syst_up","h_dress_syst_up",N_MJ_bins,MJ_bins);
  TH1F* h_dress_syst_down = new TH1F("h_dress_syst_down","h_dress_syst_down",N_MJ_bins,MJ_bins);
  for(int bin = 1; bin <= N_MJ_bins; bin++){
    TH1F* temp_bin = new TH1F("temp_bin","temp_bin",1e7,0,1e7);
    TH1F* temp_bin_up = new TH1F("temp_bin_up","temp_bin_up",1e7,0,1e7);
    TH1F* temp_bin_down = new TH1F("temp_bin_down","temp_bin_down",1e7,0,1e7);
    for(int PE = 1; PE <= PE_number; PE++){
      if(!isFile(f_dressed)) continue;
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

    h_dress_nom->SetBinContent(bin, med);
    h_dress_stat->SetBinContent(bin, med);
    h_dress_syst_up->SetBinContent(bin, med_up);
    h_dress_syst_down->SetBinContent(bin, med_down);

    //h_dress_nom->SetBinError(bin, total_uncert);
    h_dress_nom->SetBinError(bin, rms);
    h_dress_stat->SetBinError(bin, 0);
    h_dress_syst_up->SetBinError(bin, syst_uncert_up);
    h_dress_syst_down->SetBinError(bin, syst_uncert_down);

    delete temp_bin; delete temp_bin_up; delete temp_bin_down;
  }
  h_dress_nom->Scale(norm);
  h_dress_stat->Scale(norm);


  //Add mc non closure
  TH1F* mcHist;
  if(!isMC){
  mcHist = getMCnonClosureHist(reg,btag);
  for(int bin =1; bin <= N_MJ_bins; bin++){
    double mc_err = mcHist->GetBinContent(bin)*h_dress_nom->GetBinContent(bin);
    double data_err = h_dress_nom->GetBinError(bin);
    double err = pow(pow(mc_err,2)+pow(data_err,2),0.5);

    h_dress_nom->SetBinError(bin,err);
  }
}

  //Scale bin contents by bin width
  for(int bin = 1; bin <= N_MJ_bins; bin ++){
    h_kin_var->SetBinContent(bin, h_kin_var->GetBinContent(bin)/MJ_width[bin-1]);
    h_kin_var->SetBinError(bin, h_kin_var->GetBinError(bin)/MJ_width[bin-1]);

    h_kin_sig_var[0]->SetBinContent(bin, h_kin_sig_var[0]->GetBinContent(bin)/MJ_width[bin-1]);
    h_kin_sig_var[0]->SetBinError(bin, h_kin_sig_var[0]->GetBinError(bin)/MJ_width[bin-1]);
    h_kin_sig_var[1]->SetBinContent(bin, h_kin_sig_var[1]->GetBinContent(bin)/MJ_width[bin-1]);
    h_kin_sig_var[1]->SetBinError(bin, h_kin_sig_var[1]->GetBinError(bin)/MJ_width[bin-1]);

    h_dress_stat->SetBinContent(bin, h_dress_stat->GetBinContent(bin)/MJ_width[bin-1]);
    h_dress_nom->SetBinContent(bin, h_dress_nom->GetBinContent(bin)/MJ_width[bin-1]);
    h_dress_nom->SetBinError(bin, h_dress_nom->GetBinError(bin)/MJ_width[bin-1]);
  }

  TH1F *ratio_nom = (TH1F*)h_kin_var->Clone("ratio_nom");
  ratio_nom->Divide(h_dress_stat);
  ratio_nom->GetXaxis()->SetTitle("M_{J}^{#Sigma} [TeV]");

  TH1F *ratio_band = (TH1F*)h_dress_nom->Clone("ratio_band");
  ratio_band->Divide(h_dress_stat);
  ratio_band->SetLineColor(0);
  ratio_band->SetMarkerSize(0);
  ratio_band->SetFillColor(kRed);
  ratio_band->SetFillStyle(3010);

  h_kin_var->SetLineColor(1);
  h_kin_var->SetMarkerColor(1);
  
  h_kin_sig_var[0]->SetLineColor(8);
  h_kin_sig_var[0]->SetLineStyle(2);
  h_kin_sig_var[0]->SetLineWidth(2);

  h_kin_sig_var[1]->SetLineColor(9);
  h_kin_sig_var[1]->SetLineStyle(3);
  h_kin_sig_var[1]->SetLineWidth(2);

  h_dress_nom->SetLineColor(2);
  h_dress_nom->SetMarkerColor(2);
  h_dress_nom->SetMarkerSize(0.001);
  h_dress_nom->SetFillColor(kRed);
  h_dress_nom->SetFillStyle(3010);
  h_dress_stat->SetLineColor(2);
  ratio_nom->SetLineColor(1);
  ratio_nom->SetMarkerColor(1);
  //ratio_nom->SetFillColor(kRed);
  //ratio_nom->SetFillStyle(3010);

  char c_pred[200];
  char c_pred_int[200];
  char c_obs[200];
  sprintf(c_obs,"M_{J,obs}  = %3.1f", obs);
  if(blindPlot) sprintf(c_obs,"data blinded for M_{J} > 0.6");
  sprintf(c_pred,"M_{J,pred} = %3.1f #pm %3.1f #pm %3.1f", pred, pred_error, pred_error2);

  TCanvas *c_1 = new TCanvas("c_1","c_1");
  c_1->Divide(1, 2);
  c_1->GetPad(1)->SetPad(0,0.30,1,1.);
  c_1->GetPad(2)->SetPad(0,0,1,0.30);
  c_1->GetPad(2)->SetGridy();
  c_1->GetPad(1)->SetLogy();
  c_1->GetPad(2)->SetTopMargin(0.035);
  c_1->GetPad(1)->SetBottomMargin(0.025);
  c_1->GetPad(2)->SetBottomMargin(0.31);
  
  c_1->cd(1);
  h_kin_var->GetXaxis()->SetRangeUser(0,2);
  h_kin_var->GetYaxis()->SetRangeUser(max(1.,0.9*h_kin_var->GetMinimum()),3*h_kin_var->GetMaximum());
  h_kin_var->GetYaxis()->SetTitle("Events / Bin width [TeV^{-1}]");
  h_kin_var->GetYaxis()->SetTitleSize(18);
  h_kin_var->GetYaxis()->SetTitleFont(43);
  h_kin_var->GetYaxis()->SetTitleOffset(1.);
  h_kin_var->GetYaxis()->SetLabelSize(0.06);
  h_kin_var->GetXaxis()->SetLabelOffset(999);
  h_kin_var->GetXaxis()->SetLabelSize(0);
  h_kin_var->Draw("E");
  h_dress_nom->Draw("E2same");
  h_dress_stat->Draw("histsame");
  h_kin_sig_var[0]->Draw("histsame");
  h_kin_sig_var[1]->Draw("histsame");
  h_kin_var->Draw("Esame");

  c_1->cd(2);
  ratio_nom->GetXaxis()->SetRangeUser(0,2.0);
  ratio_nom->GetYaxis()->SetRangeUser(0,2.0);
  ratio_nom->GetYaxis()->SetTitle("Data/Pred"); 
  if(isMC) ratio_nom->GetYaxis()->SetTitle("Kin/Pred");
  ratio_nom->GetYaxis()->SetTitleSize(18);
  ratio_nom->GetYaxis()->SetTitleFont(43);
  ratio_nom->GetYaxis()->SetTitleOffset(1.);
  ratio_nom->GetYaxis()->SetLabelFont(43);
  ratio_nom->GetYaxis()->SetLabelSize(18);
  ratio_nom->GetYaxis()->SetNdivisions(5);
  ratio_nom->GetXaxis()->SetTitleSize(18);
  ratio_nom->GetXaxis()->SetTitleFont(43);
  ratio_nom->GetXaxis()->SetTitleOffset(4.0);
  ratio_nom->GetXaxis()->SetLabelFont(43);
  ratio_nom->GetXaxis()->SetLabelSize(18);
  ratio_nom->SetTitle("");
  ratio_nom->Draw("E1");
  //ratio_nom->Draw("E2same");
  ratio_band->Draw("E2same");
  ratio_nom->Draw("E1same");
  
  c_1->cd(1);

  double x_loc = 0.60; double y_loc = 0.60;
  ATLASLabel(x_loc-0.28,y_loc+0.25,"Internal",0.06,0.10);
  TLegend *leg_1 = new TLegend(x_loc-0.05,y_loc-0.25,x_loc+0.15,y_loc+0.15);
  //TLegend leg_1(x_loc-0.05,y_loc-0.25,x_loc+0.15,y_loc+0.15);

  char* kin_name = (char*)"Data";
  if(isMC) kin_name = (char*)"Kinematic";

  leg_1->AddEntry(h_kin,kin_name,"LP");
  leg_1->AddEntry(h_dress_nom,"Prediction","LF");
  leg_1->AddEntry(h_kin_sig_var[0],"m_{#tilde{g}} = 1600 GeV","L");
  leg_1->AddEntry((TObject*)0, "m_{#tilde{#chi_{1}^{0}}} = 650 GeV", "");
  leg_1->AddEntry((TObject*)0, "", "");
  leg_1->AddEntry(h_kin_sig_var[1],"m_{#tilde{g}} = 1200 GeV","L");
  //leg_1->AddEntry((TObject*)0, "m_{#tilde{#chi_{1}^{0}}} = 50 GeV", "");
  leg_1->SetLineColor(0);
  leg_1->SetTextSize(0.06);
  leg_1->SetShadowColor(0);
  leg_1->SetFillStyle(0);
  leg_1->SetFillColor(0);
  leg_1->Draw();
  
  TLatex cap;
  cap.SetNDC();
  cap.SetTextColor(1);
  cap.SetTextSize(0.06);
  //if(njets >= 4 && !blindPlot){
  //cap.DrawLatex(x_loc-0.05,y_loc+0.18,c_obs);
  //cap.DrawLatex(x_loc-0.05,y_loc+0.25,c_pred);}

  char jet_multi[100];
  sprintf(jet_multi, "%3.1f fb^{-1}  data",lumi);
  if(isMC) sprintf(jet_multi, "%3.1f fb^{-1}  pythia",lumi);
  cap.DrawLatex(x_loc-0.25,y_loc+0.18,jet_multi);
  
  if(!incl){sprintf(jet_multi, "N_{largeR jet} = %d",njets);}
  if(incl){sprintf(jet_multi, "N_{largeR jet} #geq %d",njets);}
  cap.DrawLatex(.15,0.25,jet_multi);

  if(reg == "CR") sprintf(jet_multi, "|#Delta#eta| > 1.4");
  if(reg == "VR") sprintf(jet_multi, "|#Delta#eta| > %3.1f",dEta_cut);
  if(reg == "SR") sprintf(jet_multi, "|#Delta#eta| < %3.1f",dEta_cut);
  cap.DrawLatex(0.15,0.1,jet_multi);

  if(btag == 0){sprintf(jet_multi,"b-veto");}
  if(btag == 1){sprintf(jet_multi,"b-tag");}
  if(btag == 9){sprintf(jet_multi,"inclusive");}
  cap.DrawLatex(0.15,0.17,jet_multi);

  region = Form("%s%ij%s_b%i",excStr.c_str(),njets,reg.c_str(),btag);
  char save_name[300]; 
  sprintf(save_name,"%s/%s/plot_MJ_%s_%s_%s_wSig_NS.pdf",plot_locations,region,SR_cut_str,region,source.c_str());
  c_1->SaveAs(save_name);
  sprintf(save_name,"%s/%s/plot_MJ_%s_%s_%s_wSig_NS.png",plot_locations,region,SR_cut_str,region,source.c_str());
  c_1->SaveAs(save_name);
  
/*
  char save_name[300]; 
  region = Form("%ij%s_b%i",njets,reg.c_str(),btag);
  sprintf(save_name,"%s/plot_MJ_%s_%s_%s_wSig_NS.pdf",plot_locations,SR_cut_str,region,source.c_str());
  c_1->SaveAs(save_name);
  sprintf(save_name,"%s/plot_MJ_%s_%s_%s_wSig_NS.C",plot_locations,SR_cut_str,region,source.c_str());
  c_1->SaveAs(save_name);
  */

  return;
}

void compare_MJ_and_Signal(string reg,int njets, int btag,bool incl,string source){
  //Version of compare_MJs() used for signal injection test

  SetAtlasStyle();
  char hist_locations[200];
  char f_kinematic[300];
  char f_dressed[300];
  char nom[100];
  char dress[100];
  char dress_up[100];
  char dress_down[100];
  int SRbin = (int)(100*SR_cut+1);
  const int N_MJ_bins = 14;
  double MJ_bins[N_MJ_bins+1] = {0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.40,.45,0.5,0.6,0.8,1.0,2.0};

  double MJ_width[N_MJ_bins];
  for(int i = 0; i < N_MJ_bins; i++) MJ_width[i] = MJ_bins[i+1]-MJ_bins[i];

  string excStr = "n";
  if(incl) excStr = "m";

  char* region = Form("%s%ij%s_b%i",excStr.c_str(),njets,reg.c_str(),btag);
  cout << region << endl;
  gROOT->ProcessLine(Form(".! mkdir %s/%s",plot_locations,region));

  sprintf(hist_locations,"/project/projectdirs/atlas/ndshal/RPV_SUSY/bkgEstimation");
  sprintf(f_kinematic,"%s/kinematic_hists/%s/main_kinematic_%s.root",hist_locations, source.c_str(), source.c_str());

  sprintf(nom,"MJ_%s",region);
  sprintf(dress,"MJ_%s",region);
  sprintf(dress_up,"MJ_%s_shiftUp",region);
  sprintf(dress_down,"MJ_%s_shiftDown",region);

  TFile *f_kin = TFile::Open(f_kinematic);
  TH1F* h_kin = (TH1F*)f_kin->Get(nom);
  h_kin->Scale(lumi);

  sprintf(f_kinematic,"%s/kinematic_hists/pythia/main_kinematic_pythia.root",hist_locations);
  TFile *f_kin_bkg = TFile::Open(f_kinematic);
  TH1F* h_kin_bkg = (TH1F*)f_kin_bkg->Get(nom);
  h_kin_bkg->Scale(lumi);

  sprintf(f_kinematic,"%s/kinematic_hists/ttbar/main_kinematic_ttbar.root",hist_locations);
  TFile *f_kin_sig = TFile::Open(f_kinematic);
  TH1F* h_kin_sig = (TH1F*)f_kin_sig->Get(nom);
  if(source == "pythia_ttbarx3") h_kin_sig->Scale(3);
  h_kin_sig->Scale(lumi);

  //Truth values
  double obs_error, obs;
  double obs_NR_error, obs_NR;
  obs = h_kin->IntegralAndError(SRbin,200,obs_error);
  obs_NR = h_kin->IntegralAndError(21,60,obs_NR_error);

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

    temp_MJs[PE] = (TH1F*)f_dress->Get(dress);
    temp_MJs_up[PE] = (TH1F*)f_dress->Get(dress_up);
    temp_MJs_down[PE] = (TH1F*)f_dress->Get(dress_down);

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

  double pred = med_SR*norm;
  double pred_up = med_SR_up*obs_NR/med_NR_up;
  double pred_down = med_SR_down*obs_NR/med_NR_down;
  double pred_error = pred*sqrt(pow(rms_SR/med_SR,2)+pow(rms_NR/med_NR,2));
  double pred_error2 = (fabs(pred_up-pred) + fabs(pred_down-pred))/2;
  double pred_error3 = pred*getMCnonClosure(reg,btag,SR_cut);

  //Switch to variable binning, then get errors bin by bin. 
  TH1F* h_kin_var = (TH1F*)h_kin->Rebin(N_MJ_bins,"h_kin_var",MJ_bins);
  TH1F* h_kin_bkg_var = (TH1F*)h_kin_bkg->Rebin(N_MJ_bins,"h_kin_bkg_var",MJ_bins);
  TH1F* h_kin_sig_var = (TH1F*)h_kin_sig->Rebin(N_MJ_bins,"h_kin_sig_var",MJ_bins);
  TH1F* temp_MJs_var[PE_number+1];
  TH1F* temp_MJs_up_var[PE_number+1];
  TH1F* temp_MJs_down_var[PE_number+1];

  for(int PE = 1; PE <=PE_number; PE++){
    temp_MJs_var[PE] = (TH1F*)temp_MJs[PE]->Rebin(N_MJ_bins,Form("%s_PE%i",temp_MJs[PE]->GetName(),PE),MJ_bins);
    temp_MJs_up_var[PE] = (TH1F*)temp_MJs_up[PE]->Rebin(N_MJ_bins,Form("%s_PE%i",temp_MJs_up[PE]->GetName(),PE),MJ_bins);
    temp_MJs_down_var[PE] = (TH1F*)temp_MJs_down[PE]->Rebin(N_MJ_bins,Form("%s_PE%i",temp_MJs_down[PE]->GetName(),PE),MJ_bins);
  }

  //now calculate errors and bands
  TH1F* h_dress_nom = new TH1F("h_dress_nom","h_dress_nom",N_MJ_bins,MJ_bins);
  TH1F* h_dress_stat = new TH1F("h_dress_stat","h_dress_stat",N_MJ_bins,MJ_bins);
  TH1F* h_dress_syst_up = new TH1F("h_dress_syst_up","h_dress_syst_up",N_MJ_bins,MJ_bins);
  TH1F* h_dress_syst_down = new TH1F("h_dress_syst_down","h_dress_syst_down",N_MJ_bins,MJ_bins);
  for(int bin = 1; bin <= N_MJ_bins; bin++){
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

    h_dress_nom->SetBinContent(bin, med);
    h_dress_stat->SetBinContent(bin, med);
    h_dress_syst_up->SetBinContent(bin, med_up);
    h_dress_syst_down->SetBinContent(bin, med_down);

    h_dress_nom->SetBinError(bin, total_uncert);
    h_dress_stat->SetBinError(bin, 0);
    h_dress_syst_up->SetBinError(bin, syst_uncert_up);
    h_dress_syst_down->SetBinError(bin, syst_uncert_down);

    delete temp_bin; delete temp_bin_up; delete temp_bin_down;
  }
  h_dress_nom->Scale(norm);
  h_dress_stat->Scale(norm);

  //Scale bin contents by bin width
  for(int bin = 1; bin <= N_MJ_bins; bin ++){
    h_kin_var->SetBinContent(bin, h_kin_var->GetBinContent(bin)/MJ_width[bin-1]);
    h_kin_var->SetBinError(bin, h_kin_var->GetBinError(bin)/MJ_width[bin-1]);

    h_kin_bkg_var->SetBinContent(bin, h_kin_bkg_var->GetBinContent(bin)/MJ_width[bin-1]);
    h_kin_bkg_var->SetBinError(bin, h_kin_bkg_var->GetBinError(bin)/MJ_width[bin-1]);

    h_kin_sig_var->SetBinContent(bin, h_kin_sig_var->GetBinContent(bin)/MJ_width[bin-1]);
    h_kin_sig_var->SetBinError(bin, h_kin_sig_var->GetBinError(bin)/MJ_width[bin-1]);

    h_dress_stat->SetBinContent(bin, h_dress_stat->GetBinContent(bin)/MJ_width[bin-1]);
    h_dress_nom->SetBinContent(bin, h_dress_nom->GetBinContent(bin)/MJ_width[bin-1]);
    h_dress_nom->SetBinError(bin, h_dress_nom->GetBinError(bin)/MJ_width[bin-1]);
  }


  TH1F *ratio_nom = (TH1F*)h_kin_var->Clone("ratio_nom");
  ratio_nom->Divide(h_dress_nom);
  ratio_nom->GetXaxis()->SetTitle("M_{J}^{#Sigma} [TeV]");

  h_kin_var->SetLineColor(1);
  h_kin_var->SetMarkerColor(1);

  h_kin_bkg_var->SetLineColor(1);
  h_kin_bkg_var->SetLineStyle(4);
  h_kin_sig_var->SetLineColor(8);
  h_kin_sig_var->SetLineStyle(3);
  h_kin_sig_var->SetLineWidth(2);
  
  h_dress_nom->SetLineColor(2);
  h_dress_nom->SetMarkerColor(2);
  h_dress_nom->SetMarkerSize(0.001);
  h_dress_nom->SetFillColor(kRed);
  h_dress_nom->SetFillStyle(3010);
  h_dress_stat->SetLineColor(2);
  ratio_nom->SetLineColor(0);
  ratio_nom->SetMarkerColor(1);
  ratio_nom->SetFillColor(kRed);
  ratio_nom->SetFillStyle(3010);

  char c_pred[200];
  char c_pred_int[200];
  char c_obs[200];
  sprintf(c_obs,"M_{J,obs}  = %3.1f #pm %3.1f", obs, obs_error);
  if(blinded) sprintf(c_obs,"data blinded for M_{J} > 0.6");
  sprintf(c_pred,"M_{J,pred} = %3.1f #pm %3.1f #pm %3.1f", pred, pred_error, pred_error2);

  TCanvas *c_1 = new TCanvas("c_1","c_1");
  c_1->Divide(1, 2);
  c_1->GetPad(1)->SetPad(0,0.30,1,1.);
  c_1->GetPad(2)->SetPad(0,0,1,0.30);
  c_1->GetPad(2)->SetGridy();
  c_1->GetPad(1)->SetLogy();
  c_1->GetPad(2)->SetTopMargin(0.035);
  c_1->GetPad(1)->SetBottomMargin(0.025);
  c_1->GetPad(2)->SetBottomMargin(0.31);
  
  c_1->cd(1);
  h_kin_var->GetXaxis()->SetRangeUser(0,2);
  h_kin_var->GetYaxis()->SetRangeUser(max(1.,0.9*h_kin_var->GetMinimum()),3*h_kin_var->GetMaximum());
  h_kin_var->GetYaxis()->SetTitle("Events / Bin width [TeV^{-1}]");
  h_kin_var->GetYaxis()->SetTitleSize(18);
  h_kin_var->GetYaxis()->SetTitleFont(43);
  h_kin_var->GetYaxis()->SetTitleOffset(1.);
  h_kin_var->GetYaxis()->SetLabelSize(0.06);
  h_kin_var->GetXaxis()->SetLabelOffset(999);
  h_kin_var->GetXaxis()->SetLabelSize(0);
  h_kin_var->Draw("E");
  h_dress_nom->Draw("E2same");
  h_dress_stat->Draw("histsame");
  h_kin_var->Draw("Esame");
  h_kin_bkg_var->Draw("histsame");
  h_kin_sig_var->Draw("histsame");

  c_1->cd(2);
  ratio_nom->GetXaxis()->SetRangeUser(0,2.0);
  ratio_nom->GetYaxis()->SetRangeUser(0,2.0);
  ratio_nom->GetYaxis()->SetTitle("Kin/Pred");
  ratio_nom->GetYaxis()->SetTitleSize(18);
  ratio_nom->GetYaxis()->SetTitleFont(43);
  ratio_nom->GetYaxis()->SetTitleOffset(1.);
  ratio_nom->GetYaxis()->SetLabelFont(43);
  ratio_nom->GetYaxis()->SetLabelSize(18);
  ratio_nom->GetYaxis()->SetNdivisions(5);
  ratio_nom->GetXaxis()->SetTitleSize(18);
  ratio_nom->GetXaxis()->SetTitleFont(43);
  ratio_nom->GetXaxis()->SetTitleOffset(4.0);
  ratio_nom->GetXaxis()->SetLabelFont(43);
  ratio_nom->GetXaxis()->SetLabelSize(18);
  ratio_nom->SetTitle("");
  ratio_nom->Draw("E1");
  ratio_nom->Draw("E2same");
  
  c_1->cd(1);

  double x_loc = 0.60; double y_loc = 0.60;
  ATLASLabel(x_loc-0.28,y_loc+0.25,"Internal",0.06,0.10);
  TLegend* leg_1 = new TLegend(x_loc-0.15,y_loc-0.1,x_loc+0.15,y_loc+0.1);

  char* kin_name = (char*)"Kinematic (B+ttbar)";
  if(source == "pythia_ttbarx3") kin_name = (char*)"Kinematic (B+ttbarx3)";


  leg_1->AddEntry(h_kin,kin_name,"LP");
  leg_1->AddEntry(h_dress_nom,"Prediction","LF");
  leg_1->AddEntry(h_kin_bkg_var,"Pythia Bkg","L");
  //leg_1->AddEntry(h_kin_sig_var,"m_{g} = 1200 GeV, m_{#Chi} = 450 GeV","L");
  if(source == "pythia_ttbar") leg_1->AddEntry(h_kin_sig_var,"ttbar","L");
  if(source == "pythia_ttbarx3") leg_1->AddEntry(h_kin_sig_var,"ttbarx3","L");
  leg_1->SetLineColor(0);
  leg_1->SetTextSize(0.06);
  leg_1->SetShadowColor(0);
  leg_1->SetFillStyle(0);
  leg_1->SetFillColor(0);
  leg_1->Draw();
  
  TLatex cap;
  cap.SetNDC();
  cap.SetTextColor(1);
  cap.SetTextSize(0.06);
  cap.DrawLatex(x_loc-0.05,y_loc+0.18,c_obs);
  cap.DrawLatex(x_loc-0.05,y_loc+0.25,c_pred);

  char jet_multi[100];
  sprintf(jet_multi, "%3.1f fb^{-1}  pythia",lumi);
  cap.DrawLatex(x_loc-0.25,y_loc+0.18,jet_multi);
  
  if(!incl){sprintf(jet_multi, "N_{largeR jet} = %d",njets);}
  if(incl){sprintf(jet_multi, "N_{largeR jet} #geq %d",njets);}
  cap.DrawLatex(.15,0.25,jet_multi);

  if(reg == "CR") sprintf(jet_multi, "|#Delta#eta| > 1.4");
  if(reg == "VR") sprintf(jet_multi, "|#Delta#eta| > %3.1f",dEta_cut);
  if(reg == "SR") sprintf(jet_multi, "|#Delta#eta| < %3.1f",dEta_cut);
  cap.DrawLatex(0.15,0.1,jet_multi);

  if(btag == 0){sprintf(jet_multi,"b-veto");}
  if(btag == 1){sprintf(jet_multi,"b-tag");}
  if(btag == 9){sprintf(jet_multi,"inclusive");}
  cap.DrawLatex(0.15,0.17,jet_multi);

  region = Form("%s%ij%s_b%i",excStr.c_str(),njets,reg.c_str(),btag);

  char save_name[300]; 
  sprintf(save_name,"%s/%s/plot_MJ_%s_%s_%s_NS.pdf",plot_locations,region,SR_cut_str,region, source.c_str());
  c_1->SaveAs(save_name);
  sprintf(save_name,"%s/%s/plot_MJ_%s_%s_%s_NS.png",plot_locations,region,SR_cut_str,region, source.c_str());
  c_1->SaveAs(save_name);

 if(!incl) texfile << Form("%i &",njets);
 if(incl) texfile << Form("$ \\geq %i$ &",njets);
 texfile << jet_multi;
 
if(reg == "CR") texfile << "& $ > 1.4$ &";
if(reg == "VR") texfile << Form("& $ > %3.1f$ &",dEta_cut);
if(reg == "SR") texfile << Form("& $ < %3.1f$ &",dEta_cut);

 if(blinded) texfile << Form("\\textbf{\\textit{blinded}} &");
 else texfile << Form("$%3.1f \\pm %3.1f$ &", obs, obs_error);
 texfile << Form(" $%3.1f \\pm %3.1f \\pm %3.1f$ \\\\[0.2em] \n", pred, pred_error, pred_error2);
 //texfile << Form("& $%3.3f$ \\\\[0.2em] \n", dev);
  return;
}

void compare_Mass(string reg,int jet, int njets,int btag,bool incl,string source,int etaCut = 0){
  //compare individual jet mass in dressed and kinematic samples "source"

  SetAtlasStyle();
  char hist_locations[200];
  char f_kinematic[300];
  char f_dressed[300];
  char nom[100];
  char dress[100];
  char dress_up[100];
  char dress_down[100];
  int SRbin = (int)(100*SR_cut+1);
  const int N_Mass_bins = 6;

  double Mass_bins[N_Mass_bins+1] = {0,0.1,0.2,0.3,0.6,1,2.0};
  double Mass_width[N_Mass_bins];
  for(int i = 0; i < N_Mass_bins; i++) Mass_width[i] = Mass_bins[i+1]-Mass_bins[i];

  string excStr = "n";
  if(incl) excStr = "m"; 

  char* region = Form("%s%ij%s_b%i",excStr.c_str(),njets,reg.c_str(),btag);
  cout << region << " Jet " << jet << endl;
  gROOT->ProcessLine(Form(".! mkdir %s/%s",plot_locations,region));

  sprintf(hist_locations,"/project/projectdirs/atlas/ndshal/RPV_SUSY/bkgEstimation");
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

  }
  //Switch to variable binning, then get errors bin by bin. 
  TH1F* h_kin_var = (TH1F*)h_kin->Rebin(N_Mass_bins,"h_kin_var",Mass_bins);
  TH1F* temp_MJs_var[PE_number+1];
  TH1F* temp_MJs_up_var[PE_number+1];
  TH1F* temp_MJs_down_var[PE_number+1];

  for(int PE = 1; PE <=PE_number; PE++){
    temp_MJs_var[PE] = (TH1F*)temp_MJs[PE]->Rebin(N_Mass_bins,Form("%s_PE%i",temp_MJs[PE]->GetName(),PE),Mass_bins);
    temp_MJs_up_var[PE] = (TH1F*)temp_MJs_up[PE]->Rebin(N_Mass_bins,Form("%s_PE%i",temp_MJs_up[PE]->GetName(),PE),Mass_bins);
    temp_MJs_down_var[PE] = (TH1F*)temp_MJs_down[PE]->Rebin(N_Mass_bins,Form("%s_PE%i",temp_MJs_down[PE]->GetName(),PE),Mass_bins);
  }

  //now calculate errors and bands
  TH1F* h_dress_nom = new TH1F("h_dress_nom","h_dress_nom",N_Mass_bins,Mass_bins);
  TH1F* h_dress_stat = new TH1F("h_dress_stat","h_dress_stat",N_Mass_bins,Mass_bins);
  TH1F* h_dress_syst_up = new TH1F("h_dress_syst_up","h_dress_syst_up",N_Mass_bins,Mass_bins);
  TH1F* h_dress_syst_down = new TH1F("h_dress_syst_down","h_dress_syst_down",N_Mass_bins,Mass_bins);
  
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

    h_dress_nom->SetBinContent(bin, med);
    h_dress_stat->SetBinContent(bin, med);
    h_dress_syst_up->SetBinContent(bin, med_up);
    h_dress_syst_down->SetBinContent(bin, med_down);

    h_dress_nom->SetBinError(bin, total_uncert);
    h_dress_stat->SetBinError(bin, 0);
    h_dress_syst_up->SetBinError(bin, syst_uncert_up);
    h_dress_syst_down->SetBinError(bin, syst_uncert_down);

    delete temp_bin; delete temp_bin_up; delete temp_bin_down;
  }  

  //Scale bin contents by bin width
  for(int bin = 1; bin <= N_Mass_bins; bin ++){
    h_kin_var->SetBinContent(bin, h_kin_var->GetBinContent(bin)/Mass_width[bin-1]);
    h_kin_var->SetBinError(bin, h_kin_var->GetBinError(bin)/Mass_width[bin-1]);

    h_dress_stat->SetBinContent(bin, h_dress_stat->GetBinContent(bin)/Mass_width[bin-1]);
    h_dress_nom->SetBinContent(bin, h_dress_nom->GetBinContent(bin)/Mass_width[bin-1]);
    h_dress_nom->SetBinError(bin, h_dress_nom->GetBinError(bin)/Mass_width[bin-1]);
  }

  double kin_area = h_kin_var->Integral();
  h_dress_nom->Scale(kin_area/h_dress_nom->Integral());
  h_dress_stat->Scale(kin_area/h_dress_stat->Integral());

  //Add mc non closure
  TH1F* mcHist;
  if(!isMC){
  mcHist = getMCnonClosureHistMass(reg,jet,btag);
  for(int bin =1; bin <= N_Mass_bins; bin++){
    double mc_err = mcHist->GetBinContent(bin)*h_dress_nom->GetBinContent(bin);
    double data_err = h_dress_nom->GetBinError(bin);
    double err = pow(pow(mc_err,2)+pow(data_err,2),0.5);

    h_dress_nom->SetBinError(bin,err);
  }
}

  //print out scale factors
  /*
    for(int bin = 1; bin <= N_Mass_bins; bin++){
    cout << "Mass range: [" << Mass_bins[bin-1] << " TeV, " << Mass_bins[bin] << " TeV]    ";
    cout << "data/pred: " << h_kin_var->GetBinContent(bin)/h_dress_nom->GetBinContent(bin) << endl;
  }
  */

  TH1F *ratio_nom = (TH1F*)h_kin_var->Clone("ratio_nom");
  ratio_nom->Divide(h_dress_stat);
  char* vartitle;
  if(jet == 1) vartitle = (char*)"Mass of Leading Jet [TeV]";
  if(jet == 2) vartitle = (char*)"Mass of Second Jet [TeV]";
  if(jet == 3) vartitle = (char*)"Mass of Third Jet [TeV]";
  if(jet == 4) vartitle = (char*)"Mass of Fourth Jet [TeV]";

  ratio_nom->GetXaxis()->SetTitle(vartitle);

  TH1F *ratio_band = (TH1F*)h_dress_nom->Clone("ratio_band");
  ratio_band->Divide(h_dress_stat);
  ratio_band->SetLineColor(0);
  ratio_band->SetMarkerSize(0);
  ratio_band->SetFillColor(kRed);
  ratio_band->SetFillStyle(3010);

  h_kin_var->SetLineColor(1);
  h_kin_var->SetMarkerColor(1);

  h_dress_nom->SetLineColor(2);
  h_dress_nom->SetMarkerColor(2);
  h_dress_nom->SetMarkerSize(0.001);
  h_dress_nom->SetFillColor(kRed);
  h_dress_nom->SetFillStyle(3010);
  h_dress_stat->SetLineColor(2);
  ratio_nom->SetLineColor(1);
  ratio_nom->SetMarkerColor(1);


  TCanvas *c_1 = new TCanvas("c_1","c_1");
  c_1->Divide(1, 2);
  c_1->GetPad(1)->SetPad(0,0.30,1,1.);
  c_1->GetPad(2)->SetPad(0,0,1,0.30);
  c_1->GetPad(2)->SetGridy();
  c_1->GetPad(1)->SetLogy();
  c_1->GetPad(2)->SetTopMargin(0.035);
  c_1->GetPad(1)->SetBottomMargin(0.025);
  c_1->GetPad(2)->SetBottomMargin(0.31);
  
  c_1->cd(1);
  h_kin_var->GetXaxis()->SetRangeUser(0,1);
  h_kin_var->GetYaxis()->SetRangeUser(1.E1,3*h_kin_var->GetMaximum());
  h_kin_var->GetYaxis()->SetTitle("Events / Bin width [TeV^{-1}]");
  h_kin_var->GetYaxis()->SetTitleSize(18);
  h_kin_var->GetYaxis()->SetTitleFont(43);
  h_kin_var->GetYaxis()->SetTitleOffset(1.);
  h_kin_var->GetYaxis()->SetLabelSize(0.06);
  h_kin_var->GetXaxis()->SetLabelOffset(999);
  h_kin_var->GetXaxis()->SetLabelSize(0);
  h_kin_var->Draw("E");
  h_dress_nom->Draw("E2same");
  h_dress_stat->Draw("histsame");
  h_kin_var->Draw("Esame");

  c_1->cd(2);
  ratio_nom->GetXaxis()->SetRangeUser(0,1.0);
  ratio_nom->GetYaxis()->SetRangeUser(0,2.0);
  ratio_nom->GetYaxis()->SetTitle("Data/Pred");
  if(isMC) ratio_nom->GetYaxis()->SetTitle("Kin/Pred");
  ratio_nom->GetYaxis()->SetTitleSize(18);
  ratio_nom->GetYaxis()->SetTitleFont(43);
  ratio_nom->GetYaxis()->SetTitleOffset(1.);
  ratio_nom->GetYaxis()->SetLabelFont(43);
  ratio_nom->GetYaxis()->SetLabelSize(18);
  ratio_nom->GetYaxis()->SetNdivisions(5);
  ratio_nom->GetXaxis()->SetTitleSize(18);
  ratio_nom->GetXaxis()->SetTitleFont(43);
  ratio_nom->GetXaxis()->SetTitleOffset(3.5);
  ratio_nom->GetXaxis()->SetLabelFont(43);
  ratio_nom->GetXaxis()->SetLabelSize(18);
  ratio_nom->SetTitle("");
  ratio_nom->Draw("E1");
  ratio_band->Draw("E2same");
  ratio_nom->Draw("E1same");
  
  c_1->cd(1);
  double x_loc = 0.60; double y_loc = 0.60;
  ATLASLabel(x_loc-0.28,y_loc+0.25,"Internal",0.06,0.10);
  TLegend leg_1(x_loc+0.05,y_loc+0.05,x_loc+0.25,y_loc+0.25);

  char* kin_name = (char*)"Data";
  if(isMC) kin_name = (char*)"Kinematic";

  leg_1.AddEntry(h_kin,kin_name,"LP");
  leg_1.AddEntry(h_dress_nom,"Prediction","LF");
  leg_1.SetLineColor(0);
  leg_1.SetTextSize(0.06);
  leg_1.SetShadowColor(0);
  leg_1.SetFillStyle(0);
  leg_1.SetFillColor(0);
  leg_1.Draw();
  
  TLatex cap;
  cap.SetNDC();
  cap.SetTextColor(1);
  cap.SetTextSize(0.06);

  char jet_multi[100];
  sprintf(jet_multi, "%3.1f fb^{-1}  data",lumi);
  if(isMC) sprintf(jet_multi, "%3.1f fb^{-1}  pythia",lumi);
  cap.DrawLatex(x_loc-0.25,y_loc+0.18,jet_multi);
  
  if(!incl){sprintf(jet_multi, "N_{largeR jet} = %d",njets);}
  if(incl){sprintf(jet_multi, "N_{largeR jet} #geq %d",njets);}
  cap.DrawLatex(.15+0.52,0.34+0.2,jet_multi);

  if(etaCut == 1) sprintf(jet_multi, "|#eta| < 1.0");
  if(etaCut == 2) sprintf(jet_multi, "|#eta| #geq 1.0");
  if(etaCut) cap.DrawLatex(.15+0.52,0.17+0.2,jet_multi);


  if(reg == "CR") sprintf(jet_multi, "|#Delta#eta| > 1.4");
  if(reg == "VR") sprintf(jet_multi, "|#Delta#eta| > %3.1f",dEta_cut);
  if(reg == "SR") sprintf(jet_multi, "|#Delta#eta| < %3.1f",dEta_cut);
  cap.DrawLatex(0.15+0.52,0.1+0.2,jet_multi);

  if(btag == 0){sprintf(jet_multi,"b-veto");}
  if(btag == 1){sprintf(jet_multi,"b-tag");}
  if(btag == 9){sprintf(jet_multi,"inclusive");}
  cap.DrawLatex(0.15+0.52,0.25+.2,jet_multi);

  region = Form("%s%ij%s_b%i",excStr.c_str(),njets,reg.c_str(),btag);
  /*
  char save_name[300]; 
  sprintf(save_name,"%s/%s/plot_m%i_%s%s_%s_NS.pdf",plot_locations,region,jet,etaStr,region, source.c_str());
  c_1->SaveAs(save_name);
  sprintf(save_name,"%s/%s/plot_m%i_%s%s_%s_NS.png",plot_locations,region,jet,etaStr,region, source.c_str());
  c_1->SaveAs(save_name);
  */
  
  char save_name[300]; 
  region = Form("%ij%s_b%i",njets,reg.c_str(),btag);
  sprintf(save_name,"%s/plot_m%i_%s%s_%s_NS.pdf",plot_locations,jet,etaStr,region, source.c_str());
  c_1->SaveAs(save_name);
  sprintf(save_name,"%s/plot_m%i_%s%s_%s_NS.C",plot_locations,jet,etaStr,region, source.c_str());
  c_1->SaveAs(save_name);
  return;
}


void compare_JetMass(string reg, int njets,int btag,bool incl,string source){
  //compare jet mass (all jets) in dressed and kinematic samples "source"

  SetAtlasStyle();
  char hist_locations[200];
  char f_kinematic[300];
  char f_dressed[300];
  char nom[100];
  char dress[100];
  char dress_up[100];
  char dress_down[100];
  int SRbin = (int)(100*SR_cut+1);
  const int N_Mass_bins = 6;

  double Mass_bins[N_Mass_bins+1] = {0,0.1,0.2,0.3,0.6,1,2.0};
  double Mass_width[N_Mass_bins];
  for(int i = 0; i < N_Mass_bins; i++) Mass_width[i] = Mass_bins[i+1]-Mass_bins[i];

  string excStr = "n";
  if(incl) excStr = "m"; 

  char* region = Form("%s%ij%s_b%i",excStr.c_str(),njets,reg.c_str(),btag);
  cout << region << " Jet Mass" << endl;
  gROOT->ProcessLine(Form(".! mkdir %s/%s",plot_locations,region));

  sprintf(hist_locations,"/project/projectdirs/atlas/ndshal/RPV_SUSY/bkgEstimation");
  sprintf(f_kinematic,"%s/kinematic_hists/%s/main_kinematic_%s.root",hist_locations, source.c_str(), source.c_str());

  TFile *f_kin = TFile::Open(f_kinematic);
  TH1F* h_kin = new TH1F("h_kin","h_kin",200,0,2);

  for(int jet = 1; jet < 5; jet++){
    region = Form("%s%ij%s_b%i",excStr.c_str(),njets,reg.c_str(),btag);
    sprintf(nom,"M%i_%s",jet,region);
    TH1F* h_temp = (TH1F*)f_kin->Get(nom);
    h_kin->Add(h_temp);
    delete h_temp;
  }
  
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

    for(int jet = 1; jet < 5; jet++){
      sprintf(dress,"M%i_%s",jet,region);
      sprintf(dress_up,"M%i_%s_shiftUp",jet,region);
      sprintf(dress_down,"M%i_%s_shiftDown",jet,region);

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


  }
  

  if(isMC)h_kin->Scale(lumi);

  //Switch to variable binning, then get errors bin by bin. 
  TH1F* h_kin_var = (TH1F*)h_kin->Rebin(N_Mass_bins,"h_kin_var",Mass_bins);
  TH1F* temp_MJs_var[PE_number+1];
  TH1F* temp_MJs_up_var[PE_number+1];
  TH1F* temp_MJs_down_var[PE_number+1];

  for(int PE = 1; PE <=PE_number; PE++){
    temp_MJs_var[PE] = (TH1F*)temp_MJs[PE]->Rebin(N_Mass_bins,Form("%s_PE%i",temp_MJs[PE]->GetName(),PE),Mass_bins);
    temp_MJs_up_var[PE] = (TH1F*)temp_MJs_up[PE]->Rebin(N_Mass_bins,Form("%s_PE%i",temp_MJs_up[PE]->GetName(),PE),Mass_bins);
    temp_MJs_down_var[PE] = (TH1F*)temp_MJs_down[PE]->Rebin(N_Mass_bins,Form("%s_PE%i",temp_MJs_down[PE]->GetName(),PE),Mass_bins);
  }

  //now calculate errors and bands
  TH1F* h_dress_nom = new TH1F("h_dress_nom","h_dress_nom",N_Mass_bins,Mass_bins);
  TH1F* h_dress_stat = new TH1F("h_dress_stat","h_dress_stat",N_Mass_bins,Mass_bins);
  TH1F* h_dress_syst_up = new TH1F("h_dress_syst_up","h_dress_syst_up",N_Mass_bins,Mass_bins);
  TH1F* h_dress_syst_down = new TH1F("h_dress_syst_down","h_dress_syst_down",N_Mass_bins,Mass_bins);
  
  for(int bin = 1; bin <= 7; bin++){
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

    h_dress_nom->SetBinContent(bin, med);
    h_dress_stat->SetBinContent(bin, med);
    h_dress_syst_up->SetBinContent(bin, med_up);
    h_dress_syst_down->SetBinContent(bin, med_down);

    h_dress_nom->SetBinError(bin, total_uncert);
    h_dress_stat->SetBinError(bin, 0);
    h_dress_syst_up->SetBinError(bin, syst_uncert_up);
    h_dress_syst_down->SetBinError(bin, syst_uncert_down);

    delete temp_bin; delete temp_bin_up; delete temp_bin_down;
  }

  //Scale bin contents by bin width
  for(int bin = 1; bin <= N_Mass_bins; bin ++){
    h_kin_var->SetBinContent(bin, h_kin_var->GetBinContent(bin)/Mass_width[bin-1]);
    h_kin_var->SetBinError(bin, h_kin_var->GetBinError(bin)/Mass_width[bin-1]);

    h_dress_stat->SetBinContent(bin, h_dress_stat->GetBinContent(bin)/Mass_width[bin-1]);
    h_dress_nom->SetBinContent(bin, h_dress_nom->GetBinContent(bin)/Mass_width[bin-1]);
    h_dress_nom->SetBinError(bin, h_dress_nom->GetBinError(bin)/Mass_width[bin-1]);
  }

  double kin_area = h_kin_var->Integral();
  h_dress_nom->Scale(kin_area/h_dress_nom->Integral());
  h_dress_stat->Scale(kin_area/h_dress_stat->Integral());


  TH1F *ratio_nom = (TH1F*)h_kin_var->Clone("ratio_nom");
  ratio_nom->Divide(h_dress_nom);
  char* vartitle = (char*) "Jet Mass [TeV]";

  ratio_nom->GetXaxis()->SetTitle(vartitle);

  TH1F *ratio_band = (TH1F*)h_dress_nom->Clone("ratio_band");
  ratio_band->Divide(h_dress_stat);
  ratio_band->SetLineColor(0);
  ratio_band->SetMarkerSize(0);
  ratio_band->SetFillColor(kRed);
  ratio_band->SetFillStyle(3010);

  h_kin_var->SetLineColor(1);
  h_kin_var->SetMarkerColor(1);

  h_dress_nom->SetLineColor(2);
  h_dress_nom->SetMarkerColor(2);
  h_dress_nom->SetMarkerSize(0.001);
  h_dress_nom->SetFillColor(kRed);
  h_dress_nom->SetFillStyle(3010);
  h_dress_stat->SetLineColor(2);
  ratio_nom->SetLineColor(1);
  ratio_nom->SetMarkerColor(1);


  TCanvas *c_1 = new TCanvas("c_1","c_1");
  c_1->Divide(1, 2);
  c_1->GetPad(1)->SetPad(0,0.30,1,1.);
  c_1->GetPad(2)->SetPad(0,0,1,0.30);
  c_1->GetPad(2)->SetGridy();
  c_1->GetPad(1)->SetLogy();
  c_1->GetPad(2)->SetTopMargin(0.035);
  c_1->GetPad(1)->SetBottomMargin(0.025);
  c_1->GetPad(2)->SetBottomMargin(0.31);
  
  c_1->cd(1);
  h_kin_var->GetXaxis()->SetRangeUser(0,1);
  h_kin_var->GetYaxis()->SetRangeUser(1.E1,3*h_kin_var->GetMaximum());
  h_kin_var->GetYaxis()->SetTitle("Events / Bin width [TeV^{-1}]");
  h_kin_var->GetYaxis()->SetTitleSize(18);
  h_kin_var->GetYaxis()->SetTitleFont(43);
  h_kin_var->GetYaxis()->SetTitleOffset(1.);
  h_kin_var->GetYaxis()->SetLabelSize(0.06);
  h_kin_var->GetXaxis()->SetLabelOffset(999);
  h_kin_var->GetXaxis()->SetLabelSize(0);
  h_kin_var->Draw("E");
  h_dress_nom->Draw("E2same");
  h_dress_stat->Draw("histsame");
  h_kin_var->Draw("Esame");

  c_1->cd(2);
  ratio_nom->GetXaxis()->SetRangeUser(0,1.0);
  ratio_nom->GetYaxis()->SetRangeUser(0,2.0);
  ratio_nom->GetYaxis()->SetTitle("Data/Pred");
  if(isMC) ratio_nom->GetYaxis()->SetTitle("Kin/Pred");
  ratio_nom->GetYaxis()->SetTitleSize(18);
  ratio_nom->GetYaxis()->SetTitleFont(43);
  ratio_nom->GetYaxis()->SetTitleOffset(1.);
  ratio_nom->GetYaxis()->SetLabelFont(43);
  ratio_nom->GetYaxis()->SetLabelSize(18);
  ratio_nom->GetYaxis()->SetNdivisions(5);
  ratio_nom->GetXaxis()->SetTitleSize(18);
  ratio_nom->GetXaxis()->SetTitleFont(43);
  ratio_nom->GetXaxis()->SetTitleOffset(3.5);
  ratio_nom->GetXaxis()->SetLabelFont(43);
  ratio_nom->GetXaxis()->SetLabelSize(18);
  ratio_nom->SetTitle("");
  ratio_nom->Draw("E1");
  ratio_band->Draw("E2same");
  ratio_nom->Draw("E1same");
  
  c_1->cd(1);
  double x_loc = 0.60; double y_loc = 0.60;
  ATLASLabel(x_loc-0.28,y_loc+0.25,"Internal",0.06,0.10);
  TLegend leg_1(x_loc+0.05,y_loc+0.05,x_loc+0.25,y_loc+0.25);

  char* kin_name = (char*)"Data";
  if(isMC) kin_name = (char*)"Kinematic";

  leg_1.AddEntry(h_kin,kin_name,"LP");
  leg_1.AddEntry(h_dress_nom,"Prediction","LF");
  leg_1.SetLineColor(0);
  leg_1.SetTextSize(0.06);
  leg_1.SetShadowColor(0);
  leg_1.SetFillStyle(0);
  leg_1.SetFillColor(0);
  leg_1.Draw();
  
  TLatex cap;
  cap.SetNDC();
  cap.SetTextColor(1);
  cap.SetTextSize(0.06);

  char jet_multi[100];
  sprintf(jet_multi, "%3.1f fb^{-1}  data",lumi);
  if(isMC) sprintf(jet_multi, "%3.1f fb^{-1}  pythia",lumi);
  cap.DrawLatex(x_loc-0.25,y_loc+0.18,jet_multi);
  
  if(!incl){sprintf(jet_multi, "N_{largeR jet} = %d",njets);}
  if(incl){sprintf(jet_multi, "N_{largeR jet} #geq %d",njets);}
  cap.DrawLatex(.15+0.52,0.34+0.2,jet_multi);

  if(reg == "CR") sprintf(jet_multi, "|#Delta#eta| > 1.4");
  if(reg == "VR") sprintf(jet_multi, "|#Delta#eta| > %3.1f",dEta_cut);
  if(reg == "SR") sprintf(jet_multi, "|#Delta#eta| < %3.1f",dEta_cut);
  cap.DrawLatex(0.15+0.52,0.1+0.2,jet_multi);

  if(btag == 0){sprintf(jet_multi,"b-veto");}
  if(btag == 1){sprintf(jet_multi,"b-tag");}
  if(btag == 9){sprintf(jet_multi,"inclusive");}
  cap.DrawLatex(0.15+0.52,0.25+.2,jet_multi);

  region = Form("%s%ij%s_b%i",excStr.c_str(),njets,reg.c_str(),btag);

  char save_name[300]; 
  sprintf(save_name,"%s/%s/plot_jet_m_%s_%s_NS.pdf",plot_locations,region,region, source.c_str());
  c_1->SaveAs(save_name);
  sprintf(save_name,"%s/%s/plot_jet_m_%s_%s_NS.png",plot_locations,region,region, source.c_str());
  c_1->SaveAs(save_name);
  return;
}

/*************************************************************************************************/
/**************************** TEMPLATE STATISTICS PLOTS ******************************************/
/*************************************************************************************************/


void templateStatsLog(int btag, string source,bool temp3j=false,int split=0){
  //template statistics plot, with log grid overlaid. somewhat hacked together.

  sprintf(plot_locations,"/project/projectdirs/atlas/www/multijet/RPV/ndshal/bkgEstimation/%s_%s",dateStr,source.c_str());
  gROOT->ProcessLine(Form(".! mkdir %s",plot_locations));
  SetAtlasStyle();
  char hist_locations[200];
  char f_kinematic[300];
  char templ[100];
  //double etaBins[9] = {0,0.25,0.5,0.75,1.0,1.25,1.5,1.75,2};
  double etaBins[5] = {0,0.5,1.0,1.5,2};

  char* region = Form("CR_b%i",btag);
  cout << region << " Templates" << endl;
  sprintf(hist_locations,"/project/projectdirs/atlas/ndshal/RPV_SUSY/bkgEstimation");
  sprintf(f_kinematic,"%s/kinematic_hists/%s/main_kinematic_%s.root",hist_locations, source.c_str(), source.c_str());

  TFile *f_kin = TFile::Open(f_kinematic);
  sprintf(templ,"templGrid_b%i",btag);
  if(temp3j) sprintf(templ,"templGrid_3j_b%i",btag);
  if(split == 1) sprintf(templ,"templGrid_12_b%i",btag);
  if(split == 2) sprintf(templ,"templGrid_3_b%i",btag);
  TH2F* h_grid = (TH2F*)f_kin->Get(templ);
  sprintf(templ,"templGrid_log_b%i",btag);
  if(temp3j) sprintf(templ,"templGrid_3j_log_b%i",btag);
  TH2F* h_grid_log = (TH2F*)f_kin->Get(templ);
  
  const int nPtBins = (const int)h_grid->GetNbinsX();
  double lowEdge;
  for(int j =1; j <= nPtBins;j++){
    lowEdge = h_grid->GetXaxis()->GetBinLowEdge(j)*1e3;
    h_grid_log->GetXaxis()->SetBinLabel(j, Form("> %i",(int)lowEdge));
  }


  for(int i = 1; i < 5; i++) h_grid_log->GetYaxis()->SetBinLabel(i,Form("%3.1f-%3.1f",etaBins[i-1],etaBins[i]));
  h_grid_log->GetYaxis()->SetTitle("|#eta|");
  h_grid_log->GetXaxis()->SetTitle("Jet p_{T} [GeV]");

  
  TCanvas *c_1 = new TCanvas("c_1","c_1");
  TPad *pad1 = new TPad("pad1","",0,0,1,1);
  TPad *pad2 = new TPad("pad2","",0,0,1,1);
  pad2->SetFillColor(0);
  pad2->SetFillStyle(4000);
  pad2->SetFrameFillStyle(0);

   pad1->Draw();
   pad1->cd();
   pad1->SetLogx();
  h_grid->GetXaxis()->SetNdivisions(0);
  h_grid->GetYaxis()->SetNdivisions(0);
  
  gStyle->SetPaintTextFormat("3.0f");
  h_grid->SetMarkerSize(1.5);
  
  h_grid->Draw("text60");

  pad2->Draw();
  pad2->cd();
  pad2->SetGrid();
  h_grid_log->SetMarkerColor(0);
  h_grid_log->SetMarkerSize(0);
  h_grid_log->Draw("text");
  
  h_grid_log->GetXaxis()->LabelsOption("d");
  h_grid_log->GetXaxis()->SetTitleSize(0.04);
  //h_grid_log->GetXaxis()->SetLabelSize(0.03);
  h_grid_log->GetXaxis()->SetTitleOffset(1.3);
  h_grid_log->GetYaxis()->SetTitleSize(0.04);
  h_grid_log->GetYaxis()->SetTitleOffset(0.8);

  ATLASLabel(0.15,0.90,"Internal",0.04,0.10);

  TLatex cap;
  cap.SetNDC();
  cap.SetTextColor(1);
  cap.SetTextSize(0.04);

  char label[100];
  sprintf(label,"#sqrt{s} = 13TeV  %3.1f fb^{-1}",lumi);
  cap.DrawLatex(0.35,0.9,label);
  sprintf(label,"b-veto");
  if(btag ==1 ) sprintf(label,"b-tag");
  if(btag ==9 ) sprintf(label,"inclusive");
  cap.DrawLatex(0.79,0.9,label);

  char* split_str = (char*)"";
  if(temp3j) {
    split_str = (char*)"3j_"; 
    sprintf(label,"3j + 0 soft jets");
    cap.DrawLatex(0.6,0.9,label);}


  if(split){
    if(split == 1) {sprintf(label,"2 lead jets"); split_str = (char*)"2lead_";}
    if(split == 2) {sprintf(label,"3rd jet"); split_str = (char*)"3rd_";}
    cap.DrawLatex(0.6,0.9,label);

  }
  c_1->SaveAs(Form("%s/templateStats_%sb%i.png",plot_locations,split_str,btag));
  c_1->SaveAs(Form("%s/templateStats_%sb%i.pdf",plot_locations,split_str,btag));

return;
}

void templateStats(int btag, string source,int softJet = 0){
  //template statistics plot, allows for multiple grids in one kinematic samples
  //still somewhat hacked together.

  sprintf(plot_locations,"/project/projectdirs/atlas/www/multijet/RPV/ndshal/bkgEstimation/%s_%s",dateStr,source.c_str());
  gROOT->ProcessLine(Form(".! mkdir %s",plot_locations));
  SetAtlasStyle();
  char hist_locations[200];
  char f_kinematic[300];
  char templ[100];
  double etaBins[5] = {0,0.5,1.0,1.5,2};
  //double ptBins[9] = {0.2,0.244,0.293,0.364,0.445,0.52,0.6,0.733,0.896};

  char* region = Form("CR_b%i",btag);
  cout << region << " Templates" << endl;
  sprintf(hist_locations,"/project/projectdirs/atlas/ndshal/RPV_SUSY/bkgEstimation");
  sprintf(f_kinematic,"%s/kinematic_hists/%s/main_kinematic_%s.root",hist_locations, source.c_str(), source.c_str());

  TFile *f_kin = TFile::Open(f_kinematic);
  sprintf(templ,"templGrid_3j_b%i",btag);
  if(softJet == 1) sprintf(templ,"templGrid_4j_b%i",btag);
  if(softJet == 2) sprintf(templ,"templGrid_5j_b%i",btag);
  TH1F* h_grid = (TH1F*)f_kin->Get(templ);
  
  const int nPtBins = (const int)h_grid->GetNbinsX();
  double lowEdge;
  for(int j =1; j <= nPtBins;j++){
    lowEdge = h_grid->GetXaxis()->GetBinLowEdge(j)*1e3;
    h_grid->GetXaxis()->SetBinLabel(j, Form("> %i",(int)lowEdge));
  }


  for(int i = 1; i < 5; i++) h_grid->GetYaxis()->SetBinLabel(i,Form("%3.1f - %3.1f",etaBins[i-1],etaBins[i]));
  h_grid->GetYaxis()->SetTitle("|#eta|");
  h_grid->GetXaxis()->SetTitle("Jet p_{T} [GeV]");

  
  TCanvas *c_1 = new TCanvas("c_1","c_1");
  c_1->SetGridy();
  
  gStyle->SetPaintTextFormat("3.0f");
  h_grid->SetMarkerSize(1.5);
  
  h_grid->Draw("text60");

  TLine l;
  l.SetLineStyle(2);
  for(int i = 2; i < nPtBins+1; i++){
    lowEdge = h_grid->GetXaxis()->GetBinLowEdge(i);
    l.DrawLine(lowEdge,0,lowEdge,2);
  }
  
  h_grid->GetXaxis()->LabelsOption("d");
  h_grid->GetXaxis()->SetTitleSize(0.04);
  //h_grid->GetXaxis()->SetLabelSize(0.03);
  h_grid->GetXaxis()->SetTitleOffset(1.3);
  h_grid->GetYaxis()->SetTitleSize(0.04);
  h_grid->GetYaxis()->SetTitleOffset(0.8);

  ATLASLabel(0.15,0.90,"Internal",0.04,0.10);

  TLatex cap;
  cap.SetNDC();
  cap.SetTextColor(1);
  cap.SetTextSize(0.04);

  char label[100];
  sprintf(label,"#sqrt{s} = 13TeV  %3.1f fb^{-1}",lumi);
  cap.DrawLatex(0.35,0.9,label);
  sprintf(label,"b-veto");
  if(btag ==1 ) sprintf(label,"b-tag");
  if(btag ==9 ) sprintf(label,"inclusive");
  cap.DrawLatex(0.77,0.9,label);

  char* split_str = (char*)"3j_";

  if(softJet){
    if(softJet == 1) {sprintf(label,"3j+1 soft jet"); split_str = (char*)"4j_";}
    if(softJet == 2) {sprintf(label,"3j+2 soft jets"); split_str = (char*)"5j_";}
    cap.DrawLatex(0.6,0.9,label);

  }
  c_1->SaveAs(Form("%s/templateStats_%sb%i.png",plot_locations,split_str,btag));
  c_1->SaveAs(Form("%s/templateStats_%sb%i.pdf",plot_locations,split_str,btag));

return;
}

void drawTemplates(int ptBin, int btag,string source){
  //compares jet mass pdfs in a given pt bin and b-tag region
  //jet mass pdfs in different eta bins are overlaid.

  SetAtlasStyle();
  char hist_locations[200];
  char f_kinematic[300];
  char templname[100];

  gROOT->ProcessLine(Form(".! mkdir %s/templates",plot_locations));

  sprintf(hist_locations,"/project/projectdirs/atlas/ndshal/RPV_SUSY/bkgEstimation");
  sprintf(f_kinematic,"%s/kinematic_hists/%s/main_kinematic_%s.root",hist_locations, source.c_str(), source.c_str());
  TFile *f_kin = TFile::Open(f_kinematic);

  TH2F *h_grid = (TH2F*)f_kin->Get(Form("templGrid_b%i",btag));
  TH1F* h_temp[4];

  for(int i = 0; i < 4; i ++){
    sprintf(templname,"templ_b%i_etaBin%i_ptBin%i",btag,i+1,ptBin);
    h_temp[i] = (TH1F*)f_kin->Get(templname);
    h_temp[i]->Scale(1/h_temp[i]->Integral());
    h_temp[i]->SetLineColor(i+1);
  }

  TH1F* h_ratio[3];
  for(int i = 0; i < 3; i++){
    h_ratio[i] = (TH1F*)h_temp[i+1]->Clone(Form("ratio_%i",i+2));
    h_ratio[i]->Divide(h_temp[0]);
    h_ratio[i]->SetLineColor(i+2);
  }

  TCanvas *c_1 = new TCanvas("c_1","c_1");
  c_1->Divide(1, 2);
  c_1->GetPad(1)->SetPad(0,0.30,1,1.);
  c_1->GetPad(2)->SetPad(0,0,1,0.30);
  c_1->GetPad(2)->SetGridy();
  //c_1->GetPad(1)->SetLogy();
  c_1->GetPad(2)->SetTopMargin(0.035);
  c_1->GetPad(1)->SetBottomMargin(0.025);
  c_1->GetPad(2)->SetBottomMargin(0.31);
  
  c_1->cd(1);
  h_temp[0]->GetXaxis()->SetRangeUser(-7,0);
  h_temp[0]->GetYaxis()->SetRangeUser(0,1.1*h_temp[0]->GetMaximum());
  h_temp[0]->GetYaxis()->SetTitle("Events (Normalized)");
  h_temp[0]->GetYaxis()->SetTitleSize(18);
  h_temp[0]->GetYaxis()->SetTitleFont(43);
  h_temp[0]->GetYaxis()->SetTitleOffset(1.2);
  h_temp[0]->GetYaxis()->SetLabelSize(0.06);
  h_temp[0]->GetXaxis()->SetLabelOffset(999);
  h_temp[0]->GetXaxis()->SetLabelSize(0);
  h_temp[0]->Draw("hist");
  h_temp[1]->Draw("histsame");
  h_temp[2]->Draw("histsame");
  h_temp[3]->Draw("histsame");

  c_1->cd(2);
  h_ratio[0]->GetXaxis()->SetRangeUser(-7,0);
  h_ratio[0]->GetYaxis()->SetRangeUser(0,2.0);
  h_ratio[0]->GetXaxis()->SetTitle("log(m/pt)");
  h_ratio[0]->GetYaxis()->SetTitle(". / etaBin 1");
  h_ratio[0]->GetYaxis()->SetTitleSize(18);
  h_ratio[0]->GetYaxis()->SetTitleFont(43);
  h_ratio[0]->GetYaxis()->SetTitleOffset(1.);
  h_ratio[0]->GetYaxis()->SetLabelFont(43);
  h_ratio[0]->GetYaxis()->SetLabelSize(18);
  h_ratio[0]->GetYaxis()->SetNdivisions(5);
  h_ratio[0]->GetXaxis()->SetTitleSize(18);
  h_ratio[0]->GetXaxis()->SetTitleFont(43);
  h_ratio[0]->GetXaxis()->SetTitleOffset(3.6);
  h_ratio[0]->GetXaxis()->SetLabelFont(43);
  h_ratio[0]->GetXaxis()->SetLabelSize(18);
  h_ratio[0]->SetTitle("");
  h_ratio[0]->Draw("hist");
  h_ratio[1]->Draw("histsame");
  h_ratio[2]->Draw("histsame");
  
  c_1->cd(1);


  ATLASLabel(0.6,0.9,"Internal",0.06,0.10);
  TLegend *leg_1 = new TLegend(0.15,0.7,0.35,0.9);

  for(int i = 0; i < 4; i++){
    leg_1->AddEntry(h_temp[i],Form("%3.1f < |#eta| < %3.1f",i*0.5,(i+1)*0.5),"F");  
  }
  
  leg_1->SetLineColor(0);
  leg_1->SetTextSize(0.06);
  leg_1->SetShadowColor(0);
  leg_1->SetFillStyle(0);
  leg_1->SetFillColor(0);
  leg_1->Draw();
  
  TLatex cap;
  cap.SetNDC();
  cap.SetTextColor(1);
  cap.SetTextSize(0.06);

 char jet_multi[100];
 double ptLow = h_grid->GetXaxis()->GetBinLowEdge(ptBin)*1e3;
 double ptHigh = h_grid->GetXaxis()->GetBinLowEdge(ptBin+1)*1e3;

  sprintf(jet_multi, "%i GeV < p_{T} < %i GeV",(int)ptLow,(int)ptHigh);
    if(ptBin ==  h_grid->GetNbinsX()){
      sprintf(jet_multi,"p_{T} > %i GeV",(int)ptLow);
    }
  cap.DrawLatex(0.12,0.6,jet_multi);

  if(btag == 0){sprintf(jet_multi,"b-veto");}
  if(btag == 1){sprintf(jet_multi,"b-tag");}
  if(btag == 9){sprintf(jet_multi,"inclusive");}
  cap.DrawLatex(0.25,0.5,jet_multi);


  char save_name[300]; 
  sprintf(save_name,"%s/templates/templ_%s_ptBin%i_b%i_NS.pdf",plot_locations,source.c_str(),ptBin,btag);
  c_1->SaveAs(save_name);
  sprintf(save_name,"%s/templates/templ_%s_ptBin%i_b%i_NS.png",plot_locations,source.c_str(),ptBin,btag);
  c_1->SaveAs(save_name);
  
  return;
}

void drawTemplatesBin(int etaBin, int ptBin,string source){
  //compares jet mass pdfs in a given pt,eta bin, b-veto and b-tag templates are overlaid.

  SetAtlasStyle();
  char hist_locations[200];
  char f_kinematic[300];
  char templname[100];

  gROOT->ProcessLine(Form(".! mkdir %s/templates",plot_locations));

  sprintf(hist_locations,"/project/projectdirs/atlas/ndshal/RPV_SUSY/bkgEstimation");
  sprintf(f_kinematic,"%s/kinematic_hists/%s/main_kinematic_%s.root",hist_locations, source.c_str(), source.c_str());
  TFile *f_kin = TFile::Open(f_kinematic);

  TH2F *h_grid = (TH2F*)f_kin->Get("templGrid_b0");
  TH1F* h_temp[2];

  sprintf(templname,"templ_b0_etaBin%i_ptBin%i",etaBin,ptBin);
  h_temp[0] = (TH1F*)f_kin->Get(templname);
  h_temp[0]->Scale(1/h_temp[0]->Integral());
  h_temp[0]->SetLineColor(1);
  h_temp[0]->SetMarkerSize(0.001);

  sprintf(templname,"templ_b1_etaBin%i_ptBin%i",etaBin,ptBin);
  h_temp[1] = (TH1F*)f_kin->Get(templname);
  h_temp[1]->Scale(1/h_temp[1]->Integral());
  h_temp[1]->SetLineColor(2);
  h_temp[1]->SetMarkerSize(0.001);


  TH1F* h_ratio = (TH1F*)h_temp[1]->Clone(Form("ratio"));
  h_ratio->Divide(h_temp[0]);
 // h_ratio->SetLineColor(1);

  TCanvas *c_1 = new TCanvas("c_1","c_1");
  c_1->Divide(1, 2);
  c_1->GetPad(1)->SetPad(0,0.30,1,1.);
  c_1->GetPad(2)->SetPad(0,0,1,0.30);
  c_1->GetPad(2)->SetGridy();
  c_1->GetPad(1)->SetLogy();
  c_1->GetPad(2)->SetTopMargin(0.035);
  c_1->GetPad(1)->SetBottomMargin(0.025);
  c_1->GetPad(2)->SetBottomMargin(0.31);
  
  c_1->cd(1);
  h_temp[0]->GetXaxis()->SetRangeUser(-7,0);
  h_temp[0]->GetYaxis()->SetRangeUser(1e-3,1.5*h_temp[0]->GetMaximum());
  h_temp[0]->GetYaxis()->SetTitle("Events (Normalized)");
  h_temp[0]->GetYaxis()->SetTitleSize(18);
  h_temp[0]->GetYaxis()->SetTitleFont(43);
  h_temp[0]->GetYaxis()->SetTitleOffset(1.2);
  h_temp[0]->GetYaxis()->SetLabelSize(0.06);
  h_temp[0]->GetXaxis()->SetLabelOffset(999);
  h_temp[0]->GetXaxis()->SetLabelSize(0);
  h_temp[0]->Draw("Ehist");
  h_temp[1]->Draw("Ehistsame");

  c_1->cd(2);
  h_ratio->GetXaxis()->SetRangeUser(-7,0);
  h_ratio->GetYaxis()->SetRangeUser(0,2.0);
  h_ratio->GetXaxis()->SetTitle("log(m/pt)");
  h_ratio->GetYaxis()->SetTitle("b-tag / b-veto");
  h_ratio->GetYaxis()->SetTitleSize(18);
  h_ratio->GetYaxis()->SetTitleFont(43);
  h_ratio->GetYaxis()->SetTitleOffset(1.);
  h_ratio->GetYaxis()->SetLabelFont(43);
  h_ratio->GetYaxis()->SetLabelSize(18);
  h_ratio->GetYaxis()->SetNdivisions(5);
  h_ratio->GetXaxis()->SetTitleSize(18);
  h_ratio->GetXaxis()->SetTitleFont(43);
  h_ratio->GetXaxis()->SetTitleOffset(3.6);
  h_ratio->GetXaxis()->SetLabelFont(43);
  h_ratio->GetXaxis()->SetLabelSize(18);
  h_ratio->SetTitle("");
  h_ratio->Draw("E1");
  
  c_1->cd(1);


  ATLASLabel(0.12,0.1,"Internal",0.06,0.10);
  TLegend *leg_1 = new TLegend(0.15,0.7,0.35,0.9);
  leg_1->AddEntry(h_temp[0],"b-veto","F");
  leg_1->AddEntry(h_temp[1],"b-tag","F");
  
  leg_1->SetLineColor(0);
  leg_1->SetTextSize(0.06);
  leg_1->SetShadowColor(0);
  leg_1->SetFillStyle(0);
  leg_1->SetFillColor(0);
  leg_1->Draw();
  
  TLatex cap;
  cap.SetNDC();
  cap.SetTextColor(1);
  cap.SetTextSize(0.06);

 char jet_multi[100];
 double ptLow = h_grid->GetXaxis()->GetBinLowEdge(ptBin)*1e3;
 double ptHigh = h_grid->GetXaxis()->GetBinLowEdge(ptBin+1)*1e3;

  sprintf(jet_multi, "%i GeV < p_{T} < %i GeV",(int)ptLow,(int)ptHigh);
    if(ptBin ==  h_grid->GetNbinsX()){
      sprintf(jet_multi,"p_{T} > %i GeV",(int)ptLow);
    }
  cap.DrawLatex(0.12,0.6,jet_multi);

  sprintf(jet_multi,"%3.1f < |#eta| < %3.1f",(etaBin-1)*0.5,etaBin*0.5);
  cap.DrawLatex(0.18,0.5,jet_multi);


  char save_name[300]; 
  sprintf(save_name,"%s/templates/templ_%s_etaBin%i_ptBin%i_NS.pdf",plot_locations,source.c_str(),etaBin,ptBin);
  c_1->SaveAs(save_name);
  sprintf(save_name,"%s/templates/templ_%s_etaBin%i_ptBin%i_NS.png",plot_locations,source.c_str(),etaBin,ptBin);
  c_1->SaveAs(save_name);
  

  return;
}

/*************************************************************************************************/
/**************************** HELPER FUNCTIONS FOR TABLES ****************************************/
/*************************************************************************************************/


void predTableLine(string reg,int njets, int btag,bool incl,string source){
  //used by predTable(), makes predictions with variable MJ cut.

  //SetAtlasStyle();
  char hist_locations[200];
  char f_kinematic[300];
  char f_dressed[300];
  char nom[100];
  char dress[100];
  char dress_up[100];
  char dress_down[100];
  int SRbin = (int)(100*SR_cut+1);

  string excStr = "n";
  if(incl) excStr = "m";

  char* region = Form("%s%ij%s_b%i",excStr.c_str(),njets,reg.c_str(),btag);

  cout << region << " Predictions at with MJCut " << SR_cut << endl;
  //gROOT->ProcessLine(Form(".! mkdir %s/%s",plot_locations,region));

  sprintf(hist_locations,"/project/projectdirs/atlas/ndshal/RPV_SUSY/bkgEstimation");
  sprintf(f_kinematic,"%s/kinematic_hists/%s/main_kinematic_%s.root",hist_locations, source.c_str(), source.c_str());

  sprintf(nom,"MJ_%s",region);
  sprintf(dress,"MJ_%s",region);
  sprintf(dress_up,"MJ_%s_shiftUp",region);
  sprintf(dress_down,"MJ_%s_shiftDown",region);

  TFile *f_kin = TFile::Open(f_kinematic);
  TH1F* h_kin = (TH1F*)f_kin->Get(nom);
  if(isMC)h_kin->Scale(lumi);

  //Truth values
  double obs_error, obs;
  double obs_NR_error, obs_NR;
  obs = h_kin->IntegralAndError(SRbin,200,obs_error);
  obs_NR = h_kin->IntegralAndError(21,60,obs_NR_error);

   //Open files and calculate yield distributions
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

    temp_MJs[PE] = (TH1F*)f_dress->Get(dress);
    temp_MJs_up[PE] = (TH1F*)f_dress->Get(dress_up);
    temp_MJs_down[PE] = (TH1F*)f_dress->Get(dress_down);

  

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

  double pred = med_SR*norm;
  double pred_up = med_SR_up*obs_NR/med_NR_up;
  double pred_down = med_SR_down*obs_NR/med_NR_down;
  double pred_error = pred*sqrt(pow(rms_SR/med_SR,2)+pow(rms_NR/med_NR,2));
  double pred_error2 = (fabs(pred_up-pred) + fabs(pred_down-pred))/2;
  double pred_error3 = pred*getMCnonClosure(reg,btag,SR_cut);

  char jet_multi[100];
  if(btag == 0){sprintf(jet_multi,"b-veto");}
  if(btag == 1){sprintf(jet_multi,"b-tag");}
  if(btag == 9){sprintf(jet_multi,"inclusive");}

  char* dEta_str = Form("& $ < %3.1f$",dEta_cut);
  if(reg == "VR") dEta_str = Form("& $ >%3.1f$",dEta_cut);

  double dev = pred/obs - 1;
  double pred_err_tot = sqrt(pow(pred_error,2) + pow(pred_error2,2));
  double dev_err = sqrt(pow(pred_err_tot/obs,2) + pow(pred/obs,2)*pow(0./obs,2));


  // MJCut & njet & btag & |dEta| cut & truth & pred & pred/truth -1
  texfile_pred << Form("$ > %3.2f$ & ",SR_cut);
  if(!incl) texfile_pred << Form("%i & ",njets);
  if(incl) texfile_pred << Form("$ \\geq %i$ &",njets);
  texfile_pred << jet_multi;
  texfile_pred << dEta_str;
  texfile_pred << Form("& $%3.1f \\pm %3.1f$", obs, obs_error);
  texfile_pred << Form("& $%3.1f \\pm %3.1f \\pm %3.1f$",pred,pred_error,pred_error2); 
  texfile_pred << Form("& $%3.3f \\pm %3.4f$ \\\\ \n",dev,dev_err);

  return;

}

double sigPredTableLine(string reg,int njets, int btag,bool incl,string source){
  //used by sigPredTable(), for signal injection test

  //SetAtlasStyle();
  char hist_locations[200];
  char f_kinematic[300];
  char f_dressed[300];
  char nom[100];
  char dress[100];
  char dress_up[100];
  char dress_down[100];
  int SRbin = (int)(100*SR_cut+1);

  string excStr = "n";
  if(incl) excStr = "m";

  char* region = Form("%s%ij%s_b%i",excStr.c_str(),njets,reg.c_str(),btag);

  cout << "Predictions in " << region << " from " << source << endl;
  //gROOT->ProcessLine(Form(".! mkdir %s/%s",plot_locations,region));

  sprintf(hist_locations,"/project/projectdirs/atlas/ndshal/RPV_SUSY/bkgEstimation");
  sprintf(f_kinematic,"%s/kinematic_hists/%s/main_kinematic_%s.root",hist_locations, source.c_str(), source.c_str());

  sprintf(nom,"MJ_%s",region);
  sprintf(dress,"MJ_%s",region);
  sprintf(dress_up,"MJ_%s_shiftUp",region);
  sprintf(dress_down,"MJ_%s_shiftDown",region);

  TFile *f_kin = TFile::Open(f_kinematic);
  TH1F* h_kin = (TH1F*)f_kin->Get(nom);
  if(isMC)h_kin->Scale(lumi);

  //Truth values
  double obs_error, obs;
  double obs_NR_error, obs_NR;
  obs = h_kin->IntegralAndError(SRbin,200,obs_error);
  obs_NR = h_kin->IntegralAndError(21,60,obs_NR_error);

   //Open files and calculate yield distributions
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

    temp_MJs[PE] = (TH1F*)f_dress->Get(dress);
    temp_MJs_up[PE] = (TH1F*)f_dress->Get(dress_up);
    temp_MJs_down[PE] = (TH1F*)f_dress->Get(dress_down);

  

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

  double pred = med_SR*norm;
  double pred_up = med_SR_up*obs_NR/med_NR_up;
  double pred_down = med_SR_down*obs_NR/med_NR_down;
  double pred_error = pred*sqrt(pow(rms_SR/med_SR,2)+pow(rms_NR/med_NR,2));
  double pred_error2 = (fabs(pred_up-pred) + fabs(pred_down-pred))/2;
  double pred_error3 = pred*getMCnonClosure(reg,btag,SR_cut);

  double dev = pred/obs - 1;

  //texfile_predSig << Form("$%3.1f \\pm %3.1f \\pm %3.1f$",pred,pred_error,pred_error2); 
  texfile_predSig << Form("$%3.1f$",pred); /*" \\pm %3.1f \\pm %3.1f$",pred,pred_error,pred_error2); */
  //texfile_pred << Form("& $%3.3f$ \\\\ \n",dev);
  return pred;

}


void bkgTableLine(string reg,int njets, int btag,bool incl,string source){
  //used by bkgTable(), makes prediction in NR.

  char hist_locations[200];
  char f_kinematic[300];
  char f_dressed[300];
  char nom[100];
  char dress[100];
  char dress_up[100];
  char dress_down[100];
  int SRbin = (int)(100*SR_cut+1);

  string excStr = "n";
  if(incl) excStr = "m";

  char* region = Form("%s%ij%s_b%i",excStr.c_str(),njets,reg.c_str(),btag);
  cout << region << " background prediction" << endl;
  //gROOT->ProcessLine(Form(".! mkdir %s/%s",plot_locations,region));

  sprintf(hist_locations,"/project/projectdirs/atlas/ndshal/RPV_SUSY/bkgEstimation");
  sprintf(f_kinematic,"%s/kinematic_hists/%s/main_kinematic_%s.root",hist_locations, source.c_str(), source.c_str());

  sprintf(nom,"MJ_%s",region);
  sprintf(dress,"MJ_%s",region);
  sprintf(dress_up,"MJ_%s_shiftUp",region);
  sprintf(dress_down,"MJ_%s_shiftDown",region);

  TFile *f_kin = TFile::Open(f_kinematic);
  TH1F* h_kin = (TH1F*)f_kin->Get(nom);
  if(isMC)h_kin->Scale(lumi);

  //Truth values
  double obs_NR_error, obs_NR;
  obs_NR = h_kin->IntegralAndError(21,60,obs_NR_error);

  //first open all files and calculate ratio
  TH1F* temp_MJs[PE_number+1];
  TH1F* temp_MJs_up[PE_number+1];
  TH1F* temp_MJs_down[PE_number+1];
  TH1F* NR_yield = new TH1F("NR_yield","NR_yield",1e7,0,1e7);
  TH1F* NR_yield_up = new TH1F("NR_yield_up","NR_yield_up",1e7,0,1e7);
  TH1F* NR_yield_down = new TH1F("NR_yiel_down","NR_yield_down",1e7,0,1e7);
  for(int PE = 1; PE <= PE_number; PE++){
    sprintf(f_dressed,"%s/dressed_hists/%s/main_dressed_%s_%d.root",hist_locations, source.c_str(), source.c_str(), PE);
    TFile *f_dress = TFile::Open(f_dressed);

    temp_MJs[PE] = (TH1F*)f_dress->Get(dress);
    temp_MJs_up[PE] = (TH1F*)f_dress->Get(dress_up);
    temp_MJs_down[PE] = (TH1F*)f_dress->Get(dress_down);

  

    double NR_pred = temp_MJs[PE]->Integral(21,60);
    double NR_pred_up = temp_MJs_up[PE]->Integral(21,60);
    double NR_pred_down = temp_MJs_down[PE]->Integral(21,60);

    NR_yield->Fill( NR_pred );
    NR_yield_up->Fill( NR_pred_up );
    NR_yield_down->Fill( NR_pred_down );
  }
  double pred = GetMedian(NR_yield); 
  double pred_up = GetMedian(NR_yield_up); 
  double pred_down = GetMedian(NR_yield_down);
  double pred_rms = NR_yield->GetRMS();

  double pred_error_up = pred_up-pred;
  double pred_error_down = pred - pred_down;

  double pred_error2=(fabs(pred_error_up)+fabs(pred_error_down))/2;
  double pred_error3 = pred*getMCnonClosure(reg,btag,SR_cut);

  double err_tot = pow(pow(pred_rms,2) + pow(pred_error2,2),0.5);
  double dev = (obs_NR-pred)/err_tot;

  char jet_multi[100];
  if(btag == 0){sprintf(jet_multi,"b-veto");}
  if(btag == 1){sprintf(jet_multi,"b-tag");}
  //if(btag == 9){sprintf(jet_multi,"inclusive");}

    char* dEta_str = Form("& $ < %3.1f$",dEta_cut);
  if(reg == "VR") dEta_str = Form("& $ >%3.1f$",dEta_cut);


  // njet & btag & |dEta| cut & pred
  if(!incl) texfile_bkg << Form("%i & ",njets);
  if(incl) texfile_bkg << Form("$ \\geq %i$ &",njets);
  texfile_bkg << jet_multi;
  texfile_bkg << dEta_str;
  texfile_bkg << Form("& $%i$",(int)obs_NR);
  texfile_bkg << Form("& $%3.1f \\pm %3.1f \\pm %3.1f$",pred,pred_rms,pred_error2); 
  texfile_bkg << Form("& $%3.1f$ \\\\ \n",dev);
}


void bkgTableTextLine(string reg,int njets, int btag,bool incl,string source){
  //used by bktTableText(), makes prediction in some low MJ region, outputs in text format
  //somewhat hacked together.

  //SetAtlasStyle();
  char hist_locations[200];
  char f_kinematic[300];
  char f_dressed[300];
  char nom[100];
  char dress[100];
  char dress_up[100];
  char dress_down[100];

  int SRbin = (int)(100*SR_cut+1);

  string excStr = "n";
  if(incl) excStr = "m";

  char* region = Form("%s%ij%s_b%i",excStr.c_str(),njets,reg.c_str(),btag);
  cout << region << " text background prediction"  << endl;
  //gROOT->ProcessLine(Form(".! mkdir %s/%s",plot_locations,region));

  sprintf(hist_locations,"/project/projectdirs/atlas/ndshal/RPV_SUSY/bkgEstimation");
  sprintf(f_kinematic,"%s/kinematic_hists/%s/main_kinematic_%s.root",hist_locations, source.c_str(), source.c_str());

  sprintf(nom,"MJ_%s",region);
  sprintf(dress,"MJ_%s",region);
  sprintf(dress_up,"MJ_%s_shiftUp",region);
  sprintf(dress_down,"MJ_%s_shiftDown",region);


  TFile *f_kin = TFile::Open(f_kinematic);
  TH1F* h_kin = (TH1F*)f_kin->Get(nom);
  if(isMC)h_kin->Scale(lumi);

  //Truth values
  double obs_NR_error, obs_NR,obs_lowMJ;
  obs_NR = h_kin->IntegralAndError(21,60,obs_NR_error);
  obs_lowMJ = h_kin->Integral(0,20);

  //Open all files and calculate yield distributions
  TH1F* temp_MJs[PE_number+1];
  TH1F* temp_MJs_up[PE_number+1];
  TH1F* temp_MJs_down[PE_number+1];
  TH1F* yield_lowMJ = new TH1F("yield_SR","yield_SR",1e6,0,1e6);
  TH1F* yield_lowMJ_up = new TH1F("yield_SR_up","yield_SR_up",1e6,0,1e6);
  TH1F* yield_lowMJ_down = new TH1F("yield_SR_down","yield_SR_down",1e6,0,1e6);
  TH1F* yield_NR = new TH1F("yield_NR","yield_NR",1e6,0,1e6);
  TH1F* yield_NR_up = new TH1F("yield_NR_up","yield_NR_up",1e6,0,1e6);
  TH1F* yield_NR_down = new TH1F("yield_NR_down","yield_NR_down",1e6,0,1e6);
  for(int PE = 1; PE <= PE_number; PE++){
    sprintf(f_dressed,"%s/dressed_hists/%s/main_dressed_%s_%d.root",hist_locations, source.c_str(), source.c_str(), PE);
    TFile *f_dress = TFile::Open(f_dressed);

    temp_MJs[PE] = (TH1F*)f_dress->Get(dress);
    temp_MJs_up[PE] = (TH1F*)f_dress->Get(dress_up);
    temp_MJs_down[PE] = (TH1F*)f_dress->Get(dress_down);

  

    yield_lowMJ->Fill(temp_MJs[PE]->Integral(0,20));
    yield_lowMJ_up->Fill(temp_MJs_up[PE]->Integral(0,20));
    yield_lowMJ_down->Fill(temp_MJs_down[PE]->Integral(0,20));
    yield_NR->Fill(temp_MJs[PE]->Integral(21,60));
    yield_NR_up->Fill(temp_MJs_up[PE]->Integral(21,60));
    yield_NR_down->Fill(temp_MJs_down[PE]->Integral(21,60));

  }

  double med_lowMJ = GetMedian(yield_lowMJ);
  double med_lowMJ_up = GetMedian(yield_lowMJ_up);
  double med_lowMJ_down = GetMedian(yield_lowMJ_down);
  double rms_lowMJ = yield_lowMJ->GetRMS();
  double med_NR = GetMedian(yield_NR);
  double med_NR_up = GetMedian(yield_NR_up);
  double med_NR_down = GetMedian(yield_NR_down);
  double rms_NR = yield_NR->GetRMS();
  double norm = obs_NR/med_NR;

  double pred = med_lowMJ*norm;
  double pred_up = med_lowMJ_up*obs_NR/med_NR_up;
  double pred_down = med_lowMJ_down*obs_NR/med_NR_down;
  double pred_error = pred*sqrt(pow(rms_lowMJ/med_lowMJ,2)+pow(rms_NR/med_NR,2));
  double pred_error2 = (fabs(pred_up-pred) + fabs(pred_down-pred))/2;
  double pred_err_tot = sqrt(pow(pred_error,2)+pow(pred_error2,2));


  char* dEta_strNum = (char*)"07";
  if(dEta_cut == 1.0) dEta_strNum = (char*)"10";
  if(dEta_cut == 1.4) dEta_strNum = (char*)"14";

  char* dEta_str = Form("deta%sa",dEta_strNum);
  if(reg == "VR") dEta_str = Form("deta%sb",dEta_strNum);
  char* jetStr;
  if(!incl) jetStr = Form("n%i",njets);
  if(incl) jetStr = Form("m%i",njets);

  region = Form("bkg_MJ_000_200_b%i_%s_%s",btag,jetStr,dEta_str);
  textfile_bkg << Form("%s %i %3.1f %3.2f \n",region,(int)obs_lowMJ, pred,pred_err_tot);

 // region = Form("bkg_MJ_600_13000_b%i_%s_%s",btag,jetStr,dEta_str);
 // textfile_bkg << Form("%s %3.1f \n",region, pred);
}

void summaryTableLine(string reg,int njets, int btag,bool incl,string source){
  //used by summaryTable, like predTableLine, but adds MC non closure uncertainty.
  
  char hist_locations[200];
  char f_kinematic[300];
  char f_dressed[300];
  char nom[100];
  char dress[100];
  char dress_up[100];
  char dress_down[100];
  int SRbin = (int)(100*SR_cut+1);

  string excStr = "n";
  if(incl) excStr = "m";

  char* region = Form("%s%ij%s_b%i",excStr.c_str(),njets,reg.c_str(),btag);

  cout << region << " Background prediction for summary table" << endl;

  sprintf(hist_locations,"/project/projectdirs/atlas/ndshal/RPV_SUSY/bkgEstimation");
  sprintf(f_kinematic,"%s/kinematic_hists/%s/main_kinematic_%s.root",hist_locations, source.c_str(), source.c_str());

  sprintf(nom,"MJ_%s",region);
  sprintf(dress,"MJ_%s",region);
  sprintf(dress_up,"MJ_%s_shiftUp",region);
  sprintf(dress_down,"MJ_%s_shiftDown",region);

  TFile *f_kin = TFile::Open(f_kinematic);
  TH1F* h_kin = (TH1F*)f_kin->Get(nom);
  if(isMC)h_kin->Scale(lumi);

  //Truth values
  double obs_error, obs;
  double obs_NR_error, obs_NR;
  obs = h_kin->IntegralAndError(SRbin,200,obs_error);
  obs_NR = h_kin->IntegralAndError(21,60,obs_NR_error);

  //Try just calculating yield in SR
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

    temp_MJs[PE] = (TH1F*)f_dress->Get(dress);
    temp_MJs_up[PE] = (TH1F*)f_dress->Get(dress_up);
    temp_MJs_down[PE] = (TH1F*)f_dress->Get(dress_down);

  

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

  double pred = med_SR*norm;
  double pred_up = med_SR_up*obs_NR/med_NR_up;
  double pred_down = med_SR_down*obs_NR/med_NR_down;
  //double pred_error = rms_SR*obs_NR/med_NR;
  double pred_error = pred*sqrt(pow(rms_SR/med_SR,2)+pow(rms_NR/med_NR,2));
  double pred_error2 = (fabs(pred_up-pred) + fabs(pred_down-pred))/2;

  double pred_error3 = pred*getMCnonClosure(reg,btag,SR_cut);

  region = Form("%ij%s",njets,reg.c_str());
  if(btag == 1) region = Form("%ij%sb1",njets,reg.c_str());

  //region & MJcut& obs & pred
  texfile_summary << region;
  texfile_summary << Form("& $> %i$ GeV",(int)(SR_cut*1e3));
  if(blinded && reg == "SR") texfile_summary << "& {\\bf NA} &" << endl;
  else texfile_summary << Form("& %i &",(int)obs); 
  texfile_summary << Form(" %3.1f $\\pm$ %3.1f $\\pm$ %3.1f $\\pm$ %3.1f \\\\ \n",pred,pred_error,pred_error2,pred_error3); 
}

/*************************************************************************************************/
/************************************ TABLES OF RESULTS  *****************************************/
/*************************************************************************************************/

void sigPredTable(){
  //makes a tex table of signal injection results

  string source;
  double predB, predBS, predB10S;
  vector<string> source_vec = {"pythia","pythia_ttbar","pythia_ttbarx3"};
  vector<string> btagging = {"b-veto","b-tag","inclusive"};

  sprintf(plot_locations,"/project/projectdirs/atlas/www/multijet/RPV/ndshal/bkgEstimation/%s_signalInjection",dateStr);
  gROOT->ProcessLine(Form(".! mkdir %s",plot_locations));

  texfile_predSig.open(Form("%s/Predictions_signalInjection_wRatios.tex",plot_locations));
  texfile_predSig << "\\documentclass[11pt]{article} \n";
  texfile_predSig << "\\usepackage{amsmath} \n \n";
  texfile_predSig << "\\begin{document} \n";
  //texfile_predSig << "\\centering\n";

  texfile_predSig << "\\hspace*{-9em}\n";  
  texfile_predSig << "\\begin{tabular}{c c c c c c c c c}\n";
  texfile_predSig << "$n_{\\text{jet}}$ & b-tagging & $|\\Delta \\eta|$ cut & pred(B) & pred(B+ttbar) & (B+ttbar)-(B)/(B) &pred(B+3ttbar) &(B+3ttbar)-(B)/(B)\\\\[0.2em] \\hline \n";

  for(int jets = 3; jets < 5; jets++){

    for(int k = 0; k<3; k++){
      //region
      texfile_predSig << Form("%i &",jets);
      texfile_predSig << btagging[k];
      texfile_predSig << "& $ < 1.4$ &";

      //loop through line/source_vec
      predB = sigPredTableLine("SR",jets,btag[k],0,source_vec[0]);
      texfile_predSig << "&";
      predBS = sigPredTableLine("SR",jets,btag[k],0,source_vec[1]);
      texfile_predSig << Form("& %3.3f &",predBS/predB - 1);
      predB10S = sigPredTableLine("SR",jets,btag[k],0,source_vec[2]);
      texfile_predSig << Form("& %3.3f \\\\\n",predB10S/predB - 1);

      texfile_predSig << Form("%i &",jets);
      texfile_predSig << btagging[k];
      texfile_predSig << "& $ > 1.4$ &";
      predB = sigPredTableLine("VR",jets,btag[k],0,source_vec[0]);
      texfile_predSig << "&";
      predBS = sigPredTableLine("VR",jets,btag[k],0,source_vec[1]);
      texfile_predSig << Form("& %3.3f &",predBS/predB - 1);
      predB10S = sigPredTableLine("VR",jets,btag[k],0,source_vec[2]);
      texfile_predSig << Form("& %3.3f \\\\\n",predB10S/predB - 1);
    }
  }
  for(int jets = 4; jets < 6; jets++){
    for(int k = 0; k<3; k++){
      //region
      texfile_predSig << Form("$ \\geq %i$ &",jets);
      texfile_predSig << btagging[k];
      texfile_predSig << "& $ < 1.4$ &";

      //loop through line/source_vec
      predB = sigPredTableLine("SR",jets,btag[k],1,source_vec[0]);
      texfile_predSig << "&";
      predBS = sigPredTableLine("SR",jets,btag[k],1,source_vec[1]);
      texfile_predSig << Form("& %3.3f &",predBS/predB - 1);
      predB10S = sigPredTableLine("SR",jets,btag[k],1,source_vec[2]);
      texfile_predSig << Form("& %3.3f \\\\\n",predB10S/predB - 1);

      texfile_predSig << Form("$ \\geq %i$ &",jets);
      texfile_predSig << btagging[k];
      texfile_predSig << "& $ > 1.4$ &";
      predB = sigPredTableLine("VR",jets,btag[k],1,source_vec[0]);
      texfile_predSig << "&";
      predBS = sigPredTableLine("VR",jets,btag[k],1,source_vec[1]);
      texfile_predSig << Form("& %3.3f &",predBS/predB - 1);
      predB10S = sigPredTableLine("VR",jets,btag[k],1,source_vec[2]);
      texfile_predSig << Form("& %3.3f \\\\\n",predB10S/predB - 1);
    }
  }
  texfile_predSig << "\\hline \n \\end{tabular} \n";
  texfile_predSig << "\\end{document}";
  texfile_predSig.close();

  return;
}

void predTable(string source){
  //makes a tex table of predicted and observed values with different MJ cuts.
  //needed to get MC non-closure uncertainty.

  sprintf(plot_locations,"/project/projectdirs/atlas/www/multijet/RPV/ndshal/bkgEstimation/%s_%s",dateStr,source.c_str());
  gROOT->ProcessLine(Form(".! mkdir %s",plot_locations));

  texfile_pred.open(Form("%s/Predictions_%s.tex",plot_locations,source.c_str()));
  texfile_pred << "\\documentclass[11pt]{article} \n";
  texfile_pred << "\\usepackage{amsmath} \n \n";
  texfile_pred << "\\begin{document} \n";
  //texfile_pred << "\\centering\n";

  vector<double> Mjcut_vec = {0.6,0.65,0.7,0.75,0.8};

  for(int jets = 3; jets < 5; jets++){
  texfile_pred << "\\hspace*{-7em}\n";  
  texfile_pred << "\\begin{tabular}{c c c c c c c}\n";
  texfile_pred << "$M_{J}$ Cut & $n_{\\text{jet}}$ & b-tagging & $|\\Delta \\eta|$ cut & Truth & Pred &(Pred-Truth)/Truth\\\\[0.2em] \\hline \n";
    for(int k = 0; k<3; k++){
      for(int i=0; i < 5; i++){
        SR_cut = Mjcut_vec[i];
        predTableLine("SR",jets,btag[k],0,source);
        predTableLine("VR",jets,btag[k],0,source);
      }
    }
    texfile_pred << "\\hline \n \\end{tabular} \n";
    if(jets < 5) texfile_pred << " \\newpage \n";

  }
  for(int jets = 4; jets < 6; jets++){
     texfile_pred << "\\hspace*{-7em}\n";  
    texfile_pred << "\\begin{tabular}{c c c c c c c}\n";
  texfile_pred << "$M_{J}$ Cut & $n_{\\text{jet}}$ & b-tagging & $|\\Delta \\eta|$ cut & Truth & Pred &(Pred-Truth)/Truth\\\\[0.2em] \\hline \n";
    for(int k = 0; k<3; k++){
      for(int i=0; i < 5; i++){
        SR_cut = Mjcut_vec[i];
        predTableLine("SR",jets,btag[k],1,source);
        predTableLine("VR",jets,btag[k],1,source);
      }
    }
    texfile_pred << "\\hline \n \\end{tabular} \n";
    if(jets < 5) texfile_pred << " \\newpage \n";
  }
  texfile_pred << "\\end{document}";
  texfile_pred.close();

  return;

}

void bkgTable(string source){ 
  //makes tex table of predicted and observed values in NR.

  sprintf(plot_locations,"/project/projectdirs/atlas/www/multijet/RPV/ndshal/bkgEstimation/%s_%s",dateStr,source.c_str());
  gROOT->ProcessLine(Form(".! mkdir %s",plot_locations));
  texfile_bkg.open(Form("%s/Predictions_%s_NR.tex",plot_locations,source.c_str()));
  texfile_bkg << "\\documentclass[11pt]{article} \n";
  texfile_bkg << "\\usepackage{booktabs} \n";
  texfile_bkg << "\\usepackage{amsmath} \n \n";
  texfile_bkg << "\\begin{document} \n";
  texfile_bkg << "\\centering\n";

  texfile_bkg << "\\begin{tabular}{c c c c c c}\n";
  texfile_bkg << "$n_{jet}$ & b-tagging & $|\\Delta\\eta|$ cut & observed & predicted & deviation\\\\ \n";
  texfile_bkg << "\\toprule \n";

  for(int jets = 3; jets < 5; jets++){
    for(int k = 0; k < 2; k++){ //btags
      bkgTableLine("SR",jets,btag[k],0,source);
      bkgTableLine("VR",jets,btag[k],0,source);
    }
  }

  for(int jets = 4; jets < 6; jets++){
    for(int k = 0; k < 2; k++){ 
      bkgTableLine("SR",jets,btag[k],1,source);
      bkgTableLine("VR",jets,btag[k],1,source);
    }
  }
  texfile_bkg << "\\midrule \n";
  texfile_bkg << "\\\\ \\end{tabular} \n";
  texfile_bkg << "\\end{document}";
  texfile_bkg.close();

  if(strstr(plot_locations,"www")) gROOT->ProcessLine(Form(".! chmod -R 777 %s",plot_locations));
}

void bkgTableText(string source){ 
  //makes a text file of predicted (and observed) values in some MJ region.
  //somewhat hacked together.

  sprintf(plot_locations,"/project/projectdirs/atlas/www/multijet/RPV/ndshal/bkgEstimation/%s_%s",dateStr,source.c_str());
  gROOT->ProcessLine(Form(".! mkdir %s",plot_locations));
  textfile_bkg.open(Form("%s/Predictions_%s.txt",plot_locations,source.c_str()));

  textfile_bkg << "Region obs pred pred_err \n";

  for(int jets = 3; jets < 5; jets++){
    for(int k = 0; k < 3; k++){ //btags
      bkgTableTextLine("SR",jets,btag[k],0,source);
      bkgTableTextLine("VR",jets,btag[k],0,source);
    }
  }

  for(int jets = 4; jets < 6; jets++){
    for(int k = 0; k < 3; k++){
      bkgTableTextLine("SR",jets,btag[k],1,source);
      bkgTableTextLine("VR",jets,btag[k],1,source);
    }
  }
  textfile_bkg.close();
  if(strstr(plot_locations,"www")) gROOT->ProcessLine(Form(".! chmod -R 777 %s",plot_locations));
}

void summaryTable(string reg, string source){
  //makes *nice* tex table of observed and predicted values in signal or validation regions.
  //adds MC non closure uncertainty.

  sprintf(plot_locations,"/project/projectdirs/atlas/www/multijet/RPV/ndshal/bkgEstimation/%s_%s",dateStr,source.c_str());
  gROOT->ProcessLine(Form(".! mkdir %s",plot_locations));
  texfile_summary.open(Form("%s/SummaryTable_%s_%s.tex",plot_locations,reg.c_str(),source.c_str()));
  texfile_summary << "\\documentclass[11pt]{article} \n";
  texfile_summary << "\\usepackage{amsmath} \n";
  texfile_summary << "\\usepackage{graphicx} \n \n";
  texfile_summary << "\\begin{document} \n";

  texfile_summary << "\\begin{table} \n";
  texfile_summary << "\\begin{center} \n";
  texfile_summary << "\\vspace{0.3cm} \n";
  texfile_summary << "\\scalebox{1.0}{ \n";
  texfile_summary << "\\hspace*{-0.1cm} \\begin{tabular}{lclclcl} \n";
  texfile_summary << "\\hline \n";
  texfile_summary << "Region & $M_{J}^{\\Sigma}$ Cut & observed & expected \\\\ \n";
  texfile_summary << "\\hline \n";

  SR_cut = 0.6; summaryTableLine(reg,4,1,1,source);
  SR_cut = 0.8; summaryTableLine(reg,4,1,1,source);
  SR_cut = 0.8; summaryTableLine(reg,4,9,1,source);
  SR_cut = 0.6; summaryTableLine(reg,5,1,1,source);
  SR_cut = 0.6; summaryTableLine(reg,5,9,1,source);

  texfile_summary << "\\hline \n";
  texfile_summary << "\\end{tabular} \n";
  texfile_summary << "} \n";
  texfile_summary << "\\caption{ \n";
  if(reg == "SR") texfile_summary << "Observed and expected background yields in four signal regions.";
  else texfile_summary << "Observed and expected background yields in four validation regions.";
  texfile_summary << "The three uncertainty components of the background prediction are the ";
  texfile_summary << "template and jet mass randomization statistical uncertainty,";
  texfile_summary << "residual \\pt-dependence uncertainty and the Monte Carlo-based non-closure uncertainty. \n}";
  texfile_summary << "\\label{tab:bkgsummary} \n";
  texfile_summary << "\\end{center} \n";
  texfile_summary << "\\end{table} \n";

  texfile_summary << "\\end{document}";

  texfile_summary.close();
  if(strstr(plot_locations,"www")) gROOT->ProcessLine(Form(".! chmod -R 777 %s",plot_locations));
}

void htmlScript(string source,string message){
  sprintf(plot_locations,"/project/projectdirs/atlas/www/multijet/RPV/ndshal/bkgEstimation/%s_%s",dateStr,source.c_str());
  gROOT->ProcessLine(Form(".! mkdir %s",plot_locations));
  htmlfile.open(Form("%s/viewPlots.html",plot_locations));
  htmlfile << "<HTML> \n";
  htmlfile << "<HEAD> </HEAD>\n";
  htmlfile << "<BODY> \n";
  htmlfile << "<CENTER><H2 style=\"color:red\">Jet mass template method </H2> \n";
  htmlfile << "<H3>" << message.c_str() << "</H3> \n";
  htmlfile << "<TABLE border=3> \n";
  htmlfile << "<TR><TH colspan=4 align=center><H3>Estimation</H3></TH></TR> \n"; 
  //5j 4j n4j 3j
  //  b0 b1 b9
  //   VR SR
  vector<char*> jet_vec = {(char*)"5j",(char*)"4j",(char*)"n4j",(char*)"3j"};
  vector<char*> reg_vec = {(char*)"VR",(char*)"SR"};
  char region1[50];
  char region2[50];
  char plotname[100];

  for(int j = 0; j < 4; j++){
    for(int k = 0; k < 3; k++){
      htmlfile << "<TR> \n";
      for(int r = 0; r < 2; r++){
        sprintf(region1,"%s%s_b%i",jet_vec[j],reg_vec[r],btag[k]);
        sprintf(region2,"%s%sb%i",jet_vec[j],reg_vec[r],btag[k]);
        sprintf(plotname,"plot_MJ_%s_%s_%s_NS",SR_cut_str,region1,source.c_str());

        htmlfile << "<TD><CENTER><img src=\"";
        htmlfile << region1 << "/" << plotname << ".png\" height=\"600\" width=\"800\"><BR>";
        htmlfile << "(<a href=\"" <<region1 << "/" << plotname << ".pdf\"> " << region2 << "</a>) </CENTER></TD> \n";
      }
      htmlfile << "</TR> \n";
    }
  }

  htmlfile << "</TABLE></CENTER> \n";
  htmlfile << "</BODY> \n";
  htmlfile << "</HTML> \n";
  htmlfile.close();
  if(strstr(plot_locations,"www")) gROOT->ProcessLine(Form(".! chmod -R 777 %s",plot_locations));
}

/*************************************************************************************************/
/****************************************** LOOPING MACROS ***************************************/
/*************************************************************************************************/

void plotMJ_and_SRpredictions(string source, bool drawSig=false){
  //loop macro to plot compare_MJs() output in all regions

  sprintf(plot_locations,"/project/projectdirs/atlas/www/multijet/RPV/ndshal/bkgEstimation/%s_%s",dateStr,source.c_str());
  gROOT->ProcessLine(Form(".! mkdir %s",plot_locations));

  texfile.open(Form("%s/Predictions_%s_%s.tex",plot_locations,source.c_str(),SR_cut_str));
  texfile << "\\documentclass[11pt]{article} \n";
  texfile << "\\usepackage{amsmath} \n \n";
  texfile << "\\begin{document} \n";
  texfile << "\\centering\n \\begin{tabular}{c c c c c c}\n";
  texfile << "$n_{\\text{jet}}$ & b-tagging & $|\\Delta \\eta|$ cut & observed & predicted & (pred-obs)/obs\\\\[0.2em] \\hline \n";

  for(int jets = 3; jets < 5; jets++){
    for(int k = 0; k <3; k++){
      compare_MJs("SR",jets,btag[k],0,source);
      compare_MJs("VR",jets,btag[k],0,source);
    }
  }
  
  for(int jets = 4; jets <= 5; jets++){
    for(int k = 0; k <3; k++){
      compare_MJs("SR",jets,btag[k],1,source);
      compare_MJs("VR",jets,btag[k],1,source);
    }
  }

  texfile << "\\hline \n \\end{tabular} \n \\end{document}";
  texfile.close();

  if(drawSig){
    for(int jets = 3; jets < 5; jets++){
      for(int k = 0; k <3; k++){
        compare_MJs_wSig("SR",jets,btag[k],0,source);
        compare_MJs_wSig("VR",jets,btag[k],0,source);
      }
    }
  
    for(int jets = 4; jets <= 5; jets++){
      for(int k = 0; k <3; k++){
        compare_MJs_wSig("SR",jets,btag[k],1,source);
        compare_MJs_wSig("VR",jets,btag[k],1,source);
      }
    }
  }

  if(strstr(plot_locations,"www")) gROOT->ProcessLine(Form(".! chmod -R 777 %s",plot_locations));
  return;
}

void plotMass(string source){
  //loop macro to plot compare_Mass() output in all regions.

  sprintf(plot_locations,"/project/projectdirs/atlas/www/multijet/RPV/ndshal/bkgEstimation/%s_%s",dateStr,source.c_str());
  gROOT->ProcessLine(Form(".! mkdir %s",plot_locations));
/*
  for(int i = 3; i < 5 ; i++){
    for(int jet = 1; jet < 5; jet++){
      for(int k = 0; k < 3; k++){
        if(i ==3 && jet == 4) continue;
        compare_Mass("VR",jet,i,btag[k],0,source);
        compare_Mass("SR",jet,i,btag[k],0,source);
      }
    }
  }
  */


  for(int i = 4; i < 6 ; i++){
    for(int jet = 1; jet < 5; jet++){
      for(int k = 0; k < 3; k++){
        compare_Mass("VR",jet,i,btag[k],1,source);
        compare_Mass("SR",jet,i,btag[k],1,source);
      }
    }
  }
  
  
  if(strstr(plot_locations,"www")) gROOT->ProcessLine(Form(".! chmod -R 777 %s",plot_locations));
  return;
}


int bkgPredTools(){
  SR_cut = 0.6;
  sprintf(SR_cut_str,"SR_cut_%igev",(int)(SR_cut*1e3));
  string source;
  
  sprintf(dateStr,"07_19");
  //lumi = 5.808; //DS1
  lumi = 8.944; //D2
  //lumi = 11.778;  //D7

  //isMC = true;
  PE_number = 1000;
  source = "dataD2";
  sprintf(plot_locations,"/project/projectdirs/atlas/www/multijet/RPV/ndshal/bkgEstimation/%s_%s",dateStr,source.c_str());
  gROOT->ProcessLine(Form(".! mkdir %s",plot_locations));
  //plotMJ_and_SRpredictions(source);
/*
  for(int j = 4; j < 6; j++){
    for(int k = 1; k < 3; k++){
      for(int jet = 1; jet < 5; jet ++){
        compare_Mass("VR",jet,j,btag[k],1,source);
        compare_Mass("SR",jet,j,btag[k],1,source);
      }
    }
  }
  */


  compare_MJs_wSig("VR",4,1,1,source);
  compare_MJs_wSig("VR",4,9,1,source);
  compare_MJs_wSig("SR",4,1,1,source);
  compare_MJs_wSig("SR",4,9,1,source);
  compare_MJs_wSig("VR",5,1,1,source);
  compare_MJs_wSig("VR",5,9,1,source);
  compare_MJs_wSig("SR",5,1,1,source);
  compare_MJs_wSig("SR",5,9,1,source);
  
  //plotMass(source);
  /*
  templateStatsLog(0,source,true);
  templateStatsLog(1,source,true);
  templateStats(0,source,1);
  templateStats(1,source,1);
  templateStats(9,source,2);
  summaryTable("VR",source);
  summaryTable("SR",source);
  htmlScript(source,"Baseline prediction with 11.3fb-1");
  */
  /*
  for(int i=1; i < 5; i++){
  for(int j = 1; j <16; j++){
      drawTemplatesBin(i,j,source);
      drawTemplatesBin(i,j,source);
  }
  }
  */
  

  return 0;
}