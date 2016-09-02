#include "dresser_subjetTempl.hh"
int main (int argc, char **argv){

  if (argc < 3)
    {
      printf("\nUsage: %s configFile *.root\n\n",argv[0]);
      exit(0);
    }

  int const ind = argc - 2;
  char* configFile = (char*)argv[1];
  TString *path = new TString[ind];
  TChain *chain = new TChain("miniTree");

       for ( int j = 0; j < argc-2; j++)
       {
         path[j] = (argv[j+2]); 
         chain->Add(path[j]);
       }

  p = new miniTree(chain);
  Config config;

  if(!isFile(configFile)){
    cout << "config file " << configFile << " not found." << endl;
    exit(1);
  }

  loadConfig(config,configFile);

  TFile *f = new TFile("RPV_dress.root", "recreate");
  f->cd();
  nameHistos();

  get_event(0);
  bool isMC = p->isMC;
  //Assumes that kinematic and training samples are either both MC or both data. 

  int PE_loop = config.PE;

  //Open templates
  TFile *file = TFile::Open(config.templates);
  TH2F* templGrid = (TH2F*)file->Get("templGrid_b0");
  //assume same grid for b0 and b1

  const int nSubjetBins = (const int)templGrid->GetNbinsY();
  const int nPtBins = (const int)templGrid->GetNbinsX();
  TH1F* h_mpt[3][nSubjetBins][nPtBins];

  for(int k =0; k< 3; k++){
    for(int i = 0; i < nSubjetBins; i++){
      for(int j = 0; j < nPtBins; j++){
        hname = Form("templ_b%i_subjetBin%i_ptBin%i",b_tag[k],i+1,j+1);
        h_mpt[k][i][j]= (TH1F*)file->Get(hname);
        if(isMC) fluctuateMCTemp(PE_loop,h_mpt[k][i][j]);
        if(!isMC) fluctuateDataTemp(PE_loop,h_mpt[k][i][j]);
      }
    }
  }

  //set seed
  if(gRandom) delete gRandom;
  gRandom = new TRandom3(PE_loop);

  //Later: check if every template has enough statistics, merge neighbors.

  bool useSF = (config.jetMassSF.size() != 0);
  double *jetMassSF = &(config.jetMassSF[0]);

  //event loop
  int numev = p->fChain->GetEntries();
  cout << " Total number of events " << numev << endl;
  for( int nEvent = 0 ; nEvent < numev ; nEvent ++ ){  
    get_event(nEvent);
    if( nEvent%10000 == 0 ) cout << nEvent << " events processed " << endl;


    double wt = p->weight; 
    int njet = p->njet;

    if(njet < 3) continue;
    int btags=0;
    if(config.tmpType==4){ btags = p->nbjet_Fix60; }
    if(config.tmpType==5){ btags = p->nbjet_Fix70; }
    if(config.tmpType==6){ btags = p->nbjet_Fix77; }
    if(config.tmpType==7){ btags = p->nbjet_Fix85; }
    if(config.tmpType==8){ btags = p->nbjet_Flt60; }
    if(config.tmpType==9){ btags = p->nbjet_Flt70; }
    if(config.tmpType==10){ btags = p->nbjet_Flt77; }
    if(config.tmpType==11){ btags = p->nbjet_Flt85; }

    int b_tag = (int)((bool)btags);
   double dEta_cut = 1.4;

    bool isVR = (p->dEta > dEta_cut);

    int subjetBin,ptBin,bin,zBin,ptShiftBin;
    double dummyMJ, dummyMJ_up, dummyMJ_down, dummyMass, dummyMass_up, dummyMass_down;
    double dummyPt, dummyNTrimSubjets;

    //loop through jets, dress
    double MJ = 0, MJ_shiftUp = 0, MJ_shiftDown = 0, pt, NTrimSubjets,dressed_mass;
    double mass[4], mass_shiftUp[4], mass_shiftDown[4];
    double nom_wt = 1, up_wt = 1, down_wt = 1;

    int tmp_ind;
    if(config.tmpType == -1) tmp_ind = b_tag; //default
    if(config.tmpType > -1) tmp_ind = config.tmpType; //b_tag_vec[ind]

    for(int k = 0; k < min(4,njet); k++){
      //dress jet k

      if(config.tmpType == 4) tmp_ind = (*p->jet_bmatched_Fix60)[k]; //b-matched templ
      if(config.tmpType == 5) tmp_ind = (*p->jet_bmatched_Fix70)[k]; //b-matched templ
      if(config.tmpType == 6) tmp_ind = (*p->jet_bmatched_Fix77)[k]; //b-matched templ
      if(config.tmpType == 7) tmp_ind = (*p->jet_bmatched_Fix85)[k]; //b-matched templ
      if(config.tmpType == 8) tmp_ind = (*p->jet_bmatched_Flt60)[k]; //b-matched templ
      if(config.tmpType == 9) tmp_ind = (*p->jet_bmatched_Flt70)[k]; //b-matched templ
      if(config.tmpType == 10) tmp_ind = (*p->jet_bmatched_Flt77)[k]; //b-matched templ
      if(config.tmpType == 11) tmp_ind = (*p->jet_bmatched_Flt85)[k]; //b-matched templ

      pt = (*p->jet_pt)[k];
      NTrimSubjets = (*p->jet_NTrimSubjets)[k];
      
      dummyPt = min(pt,templGrid->GetXaxis()->GetXmax()-0.001);
      dummyNTrimSubjets = min(NTrimSubjets,4.0);
      bin = templGrid->Fill(dummyPt,dummyNTrimSubjets,0);
      templGrid->GetBinXYZ(bin,ptBin,subjetBin,zBin);
      
      //Nominal
      calc_mass(dressed_mass,h_mpt[tmp_ind][subjetBin-1][ptBin-1],pt);
      if(useSF && k == 3 && b_tag && njet >= 4){
        if(dressed_mass < 0.1) nom_wt = jetMassSF[0];
        if((0.1 <= dressed_mass) && (dressed_mass < 0.2)) nom_wt = jetMassSF[1];
        if((0.2 <= dressed_mass) && (dressed_mass < 0.3)) nom_wt = jetMassSF[2];
      }
      MJ += dressed_mass;
      mass[k] = dressed_mass;

      //1Up
      ptShiftBin = ptBin+1;
      if(ptShiftBin > nPtBins) ptShiftBin = nPtBins;
      calc_mass(dressed_mass,h_mpt[tmp_ind][subjetBin-1][ptShiftBin-1],pt);
      if(useSF && k == 3 && b_tag && njet >= 4){
        if(dressed_mass < 0.1) up_wt = jetMassSF[0];
        if((0.1 <= dressed_mass) && (dressed_mass < 0.2)) up_wt = jetMassSF[1];
        if((0.2 <= dressed_mass) && (dressed_mass < 0.3)) up_wt = jetMassSF[2];
      }
      MJ_shiftUp += dressed_mass;
      mass_shiftUp[k] = dressed_mass;

      //1Down
      ptShiftBin = ptBin-1;
      if(ptShiftBin < 1) ptShiftBin = 1;
      calc_mass(dressed_mass,h_mpt[tmp_ind][subjetBin-1][ptShiftBin-1],pt);
      if(useSF && k == 3 && b_tag && njet >= 4){
        if(dressed_mass < 0.1) down_wt = jetMassSF[0];
        if((0.1 <= dressed_mass) && (dressed_mass < 0.2)) down_wt = jetMassSF[1];
        if((0.2 <= dressed_mass) && (dressed_mass < 0.3)) down_wt = jetMassSF[2];
      }
      MJ_shiftDown += dressed_mass;
      mass_shiftDown[k] = dressed_mass;
    }


    //Fill MJ distributions
    dummyMJ = min(MJ,1.99);
    dummyMJ_up = min(MJ_shiftUp,1.99);
    dummyMJ_down = min(MJ_shiftDown,1.99);
      if(njet < 5){
        h_MJ[njet-3][isVR][2]->Fill(dummyMJ,wt*nom_wt);
        h_MJ_shiftUp[njet-3][isVR][2]->Fill(dummyMJ_up,wt*up_wt);
        h_MJ_shiftDown[njet-3][isVR][2]->Fill(dummyMJ_down,wt*down_wt);

        h_MJ[njet-3][isVR][b_tag]->Fill(dummyMJ,wt*nom_wt);
        h_MJ_shiftUp[njet-3][isVR][b_tag]->Fill(dummyMJ_up,wt*up_wt);
        h_MJ_shiftDown[njet-3][isVR][b_tag]->Fill(dummyMJ_down,wt*down_wt);
      }
      for(int jets = 4; jets < 6; jets++){
        if(njet >= jets){
          h_MJ[jets-2][isVR][2]->Fill(dummyMJ,wt*nom_wt);
          h_MJ_shiftUp[jets-2][isVR][2]->Fill(dummyMJ_up,wt*up_wt);
          h_MJ_shiftDown[jets-2][isVR][2]->Fill(dummyMJ_down,wt*down_wt);

          h_MJ[jets-2][isVR][b_tag]->Fill(dummyMJ,wt*nom_wt);
          h_MJ_shiftUp[jets-2][isVR][b_tag]->Fill(dummyMJ_up,wt*up_wt);
          h_MJ_shiftDown[jets-2][isVR][b_tag]->Fill(dummyMJ_down,wt*down_wt);
        }      
      }
      
    
    //Fill Mass Distributions
    for(int k = 0; k < min(4,njet); k++){
      dummyMass = min(mass[k],1.99);
      dummyMass_up = min(mass_shiftUp[k],1.99);
      dummyMass_down = min(mass_shiftDown[k],1.99);
      if(njet < 5){
        h_mass[k][njet-3][isVR][2]->Fill(dummyMass,wt*nom_wt);
        h_mass_shiftUp[k][njet-3][isVR][2]->Fill(dummyMass_up,wt*up_wt);
        h_mass_shiftDown[k][njet-3][isVR][2]->Fill(dummyMass_down,wt*down_wt);

        h_mass[k][njet-3][isVR][b_tag]->Fill(dummyMass,wt*nom_wt);
        h_mass_shiftUp[k][njet-3][isVR][b_tag]->Fill(dummyMass_up,wt*up_wt);
        h_mass_shiftDown[k][njet-3][isVR][b_tag]->Fill(dummyMass_down,wt*down_wt);

        if(fabs((*p->jet_eta)[k]) < 1.0){
          h_mass_central[k][njet-3][isVR][2]->Fill(dummyMass,wt*nom_wt);
          h_mass_central_shiftUp[k][njet-3][isVR][2]->Fill(dummyMass_up,wt*up_wt);
          h_mass_central_shiftDown[k][njet-3][isVR][2]->Fill(dummyMass_down,wt*down_wt);

          h_mass_central[k][njet-3][isVR][b_tag]->Fill(dummyMass,wt*nom_wt);
          h_mass_central_shiftUp[k][njet-3][isVR][b_tag]->Fill(dummyMass_up,wt*up_wt);
          h_mass_central_shiftDown[k][njet-3][isVR][b_tag]->Fill(dummyMass_down,wt*down_wt);
        }
        else{
          h_mass_forward[k][njet-3][isVR][2]->Fill(dummyMass,wt*nom_wt);
          h_mass_forward_shiftUp[k][njet-3][isVR][2]->Fill(dummyMass_up,wt*up_wt);
          h_mass_forward_shiftDown[k][njet-3][isVR][2]->Fill(dummyMass_down,wt*down_wt);

          h_mass_forward[k][njet-3][isVR][b_tag]->Fill(dummyMass,wt*nom_wt);
          h_mass_forward_shiftUp[k][njet-3][isVR][b_tag]->Fill(dummyMass_up,wt*up_wt);
          h_mass_forward_shiftDown[k][njet-3][isVR][b_tag]->Fill(dummyMass_down,wt*down_wt);
        }
      }
      for(int jets = 4; jets < 6; jets++){
        if(njet >= jets){
          h_mass[k][jets-2][isVR][2]->Fill(dummyMass,wt*nom_wt);
          h_mass_shiftUp[k][jets-2][isVR][2]->Fill(dummyMass_up,wt*up_wt);
          h_mass_shiftDown[k][jets-2][isVR][2]->Fill(dummyMass_down,wt*down_wt);

          h_mass[k][jets-2][isVR][b_tag]->Fill(dummyMass,wt*nom_wt);
          h_mass_shiftUp[k][jets-2][isVR][b_tag]->Fill(dummyMass_up,wt*up_wt);
          h_mass_shiftDown[k][jets-2][isVR][b_tag]->Fill(dummyMass_down,wt*down_wt);

          if(fabs((*p->jet_eta)[k]) < 1.0){
          h_mass_central[k][jets-2][isVR][2]->Fill(dummyMass,wt*nom_wt);
          h_mass_central_shiftUp[k][jets-2][isVR][2]->Fill(dummyMass_up,wt*up_wt);
          h_mass_central_shiftDown[k][jets-2][isVR][2]->Fill(dummyMass_down,wt*down_wt);

          h_mass_central[k][jets-2][isVR][b_tag]->Fill(dummyMass,wt*nom_wt);
          h_mass_central_shiftUp[k][jets-2][isVR][b_tag]->Fill(dummyMass_up,wt*up_wt);
          h_mass_central_shiftDown[k][jets-2][isVR][b_tag]->Fill(dummyMass_down,wt*down_wt);
        }
        else{
          h_mass_forward[k][jets-2][isVR][2]->Fill(dummyMass,wt*nom_wt);
          h_mass_forward_shiftUp[k][jets-2][isVR][2]->Fill(dummyMass_up,wt*up_wt);
          h_mass_forward_shiftDown[k][jets-2][isVR][2]->Fill(dummyMass_down,wt*down_wt);

          h_mass_forward[k][jets-2][isVR][b_tag]->Fill(dummyMass,wt*nom_wt);
          h_mass_forward_shiftUp[k][jets-2][isVR][b_tag]->Fill(dummyMass_up,wt*up_wt);
          h_mass_forward_shiftDown[k][jets-2][isVR][b_tag]->Fill(dummyMass_down,wt*down_wt);
        }
        }      
      }
    
  }
}

f->Write();
f->Close();

return 0;

}



