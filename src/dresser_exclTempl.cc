#include "dresser_exclTempl.hh"
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
  TH2F* templGrid[2];
  templGrid[0] = (TH2F*)file->Get("templGrid_12_b0");
  templGrid[1] = (TH2F*)file->Get("templGrid_3_b0");
  //assume same grid for b0 and b1

  const int nEtaBins = (const int)templGrid[0]->GetNbinsY();
  const int nPtBins12 = (const int)templGrid[0]->GetNbinsX();
  const int nPtBins3 = (const int)templGrid[1]->GetNbinsX();

  TH1F* h_mpt_12[3][nEtaBins][nPtBins12];
  TH1F* h_mpt_3[3][nEtaBins][nPtBins3];

  for(int k =0; k< 3; k++){
    for(int i = 0; i < nEtaBins; i++){
      for(int j = 0; j < nPtBins12; j++){
        hname = Form("templ_12_b%i_etaBin%i_ptBin%i",b_tag[k],i+1,j+1);
        h_mpt_12[k][i][j]= (TH1F*)file->Get(hname);
        if(isMC) fluctuateMCTemp(PE_loop,h_mpt_12[k][i][j]);
        if(!isMC) fluctuateDataTemp(PE_loop,h_mpt_12[k][i][j]);
      }      
      for(int j = 0; j < nPtBins3; j++){
        hname = Form("templ_3_b%i_etaBin%i_ptBin%i",b_tag[k],i+1,j+1);
        h_mpt_3[k][i][j]= (TH1F*)file->Get(hname);
        if(isMC) fluctuateMCTemp(PE_loop,h_mpt_3[k][i][j]);
        if(!isMC) fluctuateDataTemp(PE_loop,h_mpt_3[k][i][j]);
      }
    }
  }

  //set seed
  if(gRandom) delete gRandom;
  gRandom = new TRandom3(PE_loop);

  //Later: check if every template has enough statistics, merge neighbors.

  //event loop
  int numev = p->fChain->GetEntries();
  cout << " Total number of events " << numev << endl;
  for( int nEvent = 0 ; nEvent < numev ; nEvent ++ ){  
    get_event(nEvent);
    if( nEvent%10000 == 0 ) cout << nEvent << " events processed " << endl;


    double wt = p->weight; 
    int njet = p->njet;

    if(njet < 3) continue;
    int btags = p->nbjet_Fix70;
    int b_tag = (int)((bool)btags);
    double dEta_cut = 1.4;

    bool isVR = (p->dEta > dEta_cut);

    int etaBin,ptBin,bin,zBin,ptShiftBin;
    double dummyMJ, dummyMJ_up, dummyMJ_down, dummyMass, dummyMass_up, dummyMass_down;
    double dummyPt, dummyEta;

    //loop through jets, dress
    double MJ = 0, MJ_shiftUp = 0, MJ_shiftDown = 0, pt, eta,dressed_mass;
    double mass[4], mass_shiftUp[4], mass_shiftDown[4];

    int tmp_ind;

    for(int k = 0; k < 2; k++){
      //dress two leading jets.

      if(config.tmpType12 != -9){ // grid specific
          if(config.tmpType12 == -1) tmp_ind = b_tag; //default
          if(config.tmpType12 > -1) tmp_ind = config.tmpType12; //b_tag_vec[ind]
          if(config.tmpType12 == 3) tmp_ind = (*p->jet_bmatched_Fix70)[k]; //b-matched templates
      }
      else{ //same for both grids
          if(config.tmpType == -1) tmp_ind = b_tag; //default
          if(config.tmpType > -1) tmp_ind = config.tmpType; //b_tag_vec[ind]
          if(config.tmpType == 3) tmp_ind = (*p->jet_bmatched_Fix70)[k]; //b-matched templates
      } 

      pt = (*p->jet_pt)[k];
      eta = (*p->jet_eta)[k];
      
      dummyPt = min(pt,templGrid[0]->GetXaxis()->GetXmax()-0.001);
      dummyEta = min(fabs(eta),1.99);
      bin = templGrid[0]->Fill(dummyPt,dummyEta,0);
      templGrid[0]->GetBinXYZ(bin,ptBin,etaBin,zBin);

      //Nominal
      calc_mass(dressed_mass,h_mpt_12[tmp_ind][etaBin-1][ptBin-1],pt);
      MJ += dressed_mass;
      mass[k] = dressed_mass;

      //1Up
      ptShiftBin = ptBin+1;
      if(ptShiftBin > nPtBins12) ptShiftBin = nPtBins12;
      calc_mass(dressed_mass,h_mpt_12[tmp_ind][etaBin-1][ptShiftBin-1],pt);
      MJ_shiftUp += dressed_mass;
      mass_shiftUp[k] = dressed_mass;

      //1Down
      ptShiftBin = ptBin-1;
      if(ptShiftBin < 1) ptShiftBin = 1;
      calc_mass(dressed_mass,h_mpt_12[tmp_ind][etaBin-1][ptShiftBin-1],pt);
      MJ_shiftDown += dressed_mass;
      mass_shiftDown[k] = dressed_mass;
    }

    for(int k = 2; k < min(njet,4); k++){
      //dress third and fourth jets
  
      if(config.tmpType3 != -9){ // grid specific
          if(config.tmpType3 == -1) tmp_ind = b_tag; //default
          if(config.tmpType3 > -1) tmp_ind = config.tmpType3; //b_tag_vec[ind]
          if(config.tmpType3 == 3) tmp_ind = (*p->jet_bmatched_Fix70)[k]; //b-matched templates
      }
      else{ //same for both grids
          if(config.tmpType == -1) tmp_ind = b_tag; //default
          if(config.tmpType > -1) tmp_ind = config.tmpType; //b_tag_vec[ind]
          if(config.tmpType == 3) tmp_ind = (*p->jet_bmatched_Fix70)[k]; //b-matched templates
      }

      pt = (*p->jet_pt)[k];
      eta = (*p->jet_eta)[k];

      dummyPt = min(pt,templGrid[1]->GetXaxis()->GetXmax()-0.001);
      dummyEta = min(fabs(eta),1.99);
      bin = templGrid[1]->Fill(dummyPt,dummyEta,0);
      templGrid[1]->GetBinXYZ(bin,ptBin,etaBin,zBin);

      //Nominal
      calc_mass(dressed_mass,h_mpt_3[tmp_ind][etaBin-1][ptBin-1],pt);
      MJ += dressed_mass;
      mass[k] = dressed_mass;

      //1Up
      ptShiftBin = ptBin+1;
      if(ptShiftBin > nPtBins3) ptShiftBin = nPtBins3;
      calc_mass(dressed_mass,h_mpt_3[tmp_ind][etaBin-1][ptShiftBin-1],pt);
      MJ_shiftUp += dressed_mass;
      mass_shiftUp[k] = dressed_mass;

      //1Down
      ptShiftBin = ptBin-1;
      if(ptShiftBin < 1) ptShiftBin = 1;
      calc_mass(dressed_mass,h_mpt_3[tmp_ind][etaBin-1][ptShiftBin-1],pt);
      MJ_shiftDown += dressed_mass;
      mass_shiftDown[k] = dressed_mass;
    }

    //Fill MJ distributions
    dummyMJ = min(MJ,1.99);
    dummyMJ_up = min(MJ_shiftUp,1.99);
    dummyMJ_down = min(MJ_shiftDown,1.99);
      if(njet < 5){
        h_MJ[njet-3][isVR][2]->Fill(dummyMJ,wt);
        h_MJ_shiftUp[njet-3][isVR][2]->Fill(dummyMJ_up,wt);
        h_MJ_shiftDown[njet-3][isVR][2]->Fill(dummyMJ_down,wt);

        h_MJ[njet-3][isVR][b_tag]->Fill(dummyMJ,wt);
        h_MJ_shiftUp[njet-3][isVR][b_tag]->Fill(dummyMJ_up,wt);
        h_MJ_shiftDown[njet-3][isVR][b_tag]->Fill(dummyMJ_down,wt);
      }
      for(int jets = 4; jets < 6; jets++){
        if(njet >= jets){
          h_MJ[jets-2][isVR][2]->Fill(dummyMJ,wt);
          h_MJ_shiftUp[jets-2][isVR][2]->Fill(dummyMJ_up,wt);
          h_MJ_shiftDown[jets-2][isVR][2]->Fill(dummyMJ_down,wt);

          h_MJ[jets-2][isVR][b_tag]->Fill(dummyMJ,wt);
          h_MJ_shiftUp[jets-2][isVR][b_tag]->Fill(dummyMJ_up,wt);
          h_MJ_shiftDown[jets-2][isVR][b_tag]->Fill(dummyMJ_down,wt);
        }      
      }
      
    
    //Fill Mass Distributions
    for(int k = 0; k < min(4,njet); k++){
      dummyMass = min(mass[k],1.99);
      dummyMass_up = min(mass_shiftUp[k],1.99);
      dummyMass_down = min(mass_shiftDown[k],1.99);
      if(njet < 5){
        h_mass[k][njet-3][isVR][2]->Fill(dummyMass,wt);
        h_mass_shiftUp[k][njet-3][isVR][2]->Fill(dummyMass_up,wt);
        h_mass_shiftDown[k][njet-3][isVR][2]->Fill(dummyMass_down,wt);

        h_mass[k][njet-3][isVR][b_tag]->Fill(dummyMass,wt);
        h_mass_shiftUp[k][njet-3][isVR][b_tag]->Fill(dummyMass_up,wt);
        h_mass_shiftDown[k][njet-3][isVR][b_tag]->Fill(dummyMass_down,wt);

        if(fabs((*p->jet_eta)[k]) < 1.0){
          h_mass_central[k][njet-3][isVR][2]->Fill(dummyMass,wt);
          h_mass_central_shiftUp[k][njet-3][isVR][2]->Fill(dummyMass_up,wt);
          h_mass_central_shiftDown[k][njet-3][isVR][2]->Fill(dummyMass_down,wt);

          h_mass_central[k][njet-3][isVR][b_tag]->Fill(dummyMass,wt);
          h_mass_central_shiftUp[k][njet-3][isVR][b_tag]->Fill(dummyMass_up,wt);
          h_mass_central_shiftDown[k][njet-3][isVR][b_tag]->Fill(dummyMass_down,wt);
        }
        else{
          h_mass_forward[k][njet-3][isVR][2]->Fill(dummyMass,wt);
          h_mass_forward_shiftUp[k][njet-3][isVR][2]->Fill(dummyMass_up,wt);
          h_mass_forward_shiftDown[k][njet-3][isVR][2]->Fill(dummyMass_down,wt);

          h_mass_forward[k][njet-3][isVR][b_tag]->Fill(dummyMass,wt);
          h_mass_forward_shiftUp[k][njet-3][isVR][b_tag]->Fill(dummyMass_up,wt);
          h_mass_forward_shiftDown[k][njet-3][isVR][b_tag]->Fill(dummyMass_down,wt);
        }
      }
      for(int jets = 4; jets < 6; jets++){
        if(njet >= jets){
          h_mass[k][jets-2][isVR][2]->Fill(dummyMass,wt);
          h_mass_shiftUp[k][jets-2][isVR][2]->Fill(dummyMass_up,wt);
          h_mass_shiftDown[k][jets-2][isVR][2]->Fill(dummyMass_down,wt);

          h_mass[k][jets-2][isVR][b_tag]->Fill(dummyMass,wt);
          h_mass_shiftUp[k][jets-2][isVR][b_tag]->Fill(dummyMass_up,wt);
          h_mass_shiftDown[k][jets-2][isVR][b_tag]->Fill(dummyMass_down,wt);

          if(fabs((*p->jet_eta)[k]) < 1.0){
          h_mass_central[k][jets-2][isVR][2]->Fill(dummyMass,wt);
          h_mass_central_shiftUp[k][jets-2][isVR][2]->Fill(dummyMass_up,wt);
          h_mass_central_shiftDown[k][jets-2][isVR][2]->Fill(dummyMass_down,wt);

          h_mass_central[k][jets-2][isVR][b_tag]->Fill(dummyMass,wt);
          h_mass_central_shiftUp[k][jets-2][isVR][b_tag]->Fill(dummyMass_up,wt);
          h_mass_central_shiftDown[k][jets-2][isVR][b_tag]->Fill(dummyMass_down,wt);
        }
        else{
          h_mass_forward[k][jets-2][isVR][2]->Fill(dummyMass,wt);
          h_mass_forward_shiftUp[k][jets-2][isVR][2]->Fill(dummyMass_up,wt);
          h_mass_forward_shiftDown[k][jets-2][isVR][2]->Fill(dummyMass_down,wt);

          h_mass_forward[k][jets-2][isVR][b_tag]->Fill(dummyMass,wt);
          h_mass_forward_shiftUp[k][jets-2][isVR][b_tag]->Fill(dummyMass_up,wt);
          h_mass_forward_shiftDown[k][jets-2][isVR][b_tag]->Fill(dummyMass_down,wt);
        }
        }      
      }
    
  }
}

f->Write();
f->Close();

return 0;

}



