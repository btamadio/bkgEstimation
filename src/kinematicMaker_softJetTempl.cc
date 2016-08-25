#include "kinematicMaker_softJetTempl.hh"
int main (int argc, char **argv){

  if (argc < 2)
    {
      printf("\nUsage: %s *.root\n\n",argv[0]);
      exit(0);
    }
  
  int const ind = argc - 2;
  TString *path = new TString[ind];
  char* configFile = (char*)argv[1];
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
  
  TFile *f = new TFile("RPV_kin.root", "recreate");
  f->cd();
  nameHistos();
  
  const int nEtaBins = (const int)(config.etaBins.size()-1);
  double *etaBins = &(config.etaBins[0]);
  
  const int nPtBins3 = (const int)(config.ptBins3.size()-1);
  double *ptBins3 = &(config.ptBins3[0]);
  const int nPtBins4 = (const int)(config.ptBins4.size()-1);
  double *ptBins4 = &(config.ptBins4[0]);
  const int nPtBins5 = (const int)(config.ptBins5.size()-1);
  double *ptBins5 = &(config.ptBins5[0]);

  double logPtLow = log(ptBins3[0]/0.05);
  double logPtHigh = log(ptBins3[nPtBins3]/0.05);

  TH1F* h_mpt[3][3][nEtaBins][nPtBins3];
  TH2F* h_templGrid[3][3];
  TH2F* h_templGrid_log[3];

  for(int k = 0; k < 3; k++){
    hname = Form("templGrid_3j_b%i",b_tag[k]);
    h_templGrid[0][k] = new TH2F(hname,hname,nPtBins3,ptBins3,nEtaBins,etaBins);
    hname = Form("templGrid_4j_b%i",b_tag[k]);
    h_templGrid[1][k] = new TH2F(hname,hname,nPtBins4,ptBins4,nEtaBins,etaBins);
    hname = Form("templGrid_5j_b%i",b_tag[k]);
    h_templGrid[2][k] = new TH2F(hname,hname,nPtBins5,ptBins5,nEtaBins,etaBins);
    hname = Form("templGrid_3j_log_b%i",b_tag[k]);
    h_templGrid_log[k] = new TH2F(hname,hname,nPtBins3,logPtLow,logPtHigh,nEtaBins,etaBins[0],etaBins[nEtaBins]);

    for(int i = 0; i <nEtaBins; i++){
      for(int j = 0; j < nPtBins3; j++){
        hname = Form("templ_3j_b%i_etaBin%i_ptBin%i",b_tag[k],i+1,j+1);
        h_mpt[0][k][i][j] = new TH1F(hname,hname,50,-7,0);
        h_mpt[0][k][i][j]->Sumw2();
      }

      for(int j = 0; j < nPtBins4; j++){
        hname = Form("templ_4j_b%i_etaBin%i_ptBin%i",b_tag[k],i+1,j+1);
        h_mpt[1][k][i][j] = new TH1F(hname,hname,50,-7,0);
        h_mpt[1][k][i][j]->Sumw2();
      }
      for(int j = 0; j < nPtBins5; j++){
        hname = Form("templ_5j_b%i_etaBin%i_ptBin%i",b_tag[k],i+1,j+1);
        h_mpt[2][k][i][j] = new TH1F(hname,hname,50,-7,0);
        h_mpt[2][k][i][j]->Sumw2();
      }
    }
  } 

  //initialize loop
  int numev = p->fChain->GetEntries();
  cout << " Total number of events " << numev << endl;

  //event loop
  for( int nEvent = 0 ; nEvent < numev ; nEvent ++ ){  
    get_event(nEvent);
    if( nEvent%10000 == 0 ) cout << nEvent << " events processed " << endl;

    double wt = p->weight; 
    int njet = p->njet; 
    int btags = p->nbjet_Fix70;
    //int btags = p->nbjet_Flt70;
    int b_tag = (int)((bool)btags);
    double dEta_cut = 1.4;

    bool isVR = false;
    isVR = (p->dEta > dEta_cut);
    isVR = (int)isVR;
    bool isCR = (njet == 3 && ((isVR && b_tag == 1) || b_tag==0));

    for(int n = 0; n < min(njet,4); n++){
      h_pt[b_tag][n]->Fill((*p->jet_pt)[n],wt);
    }

    double dummyMJ = 0,dummyMass;
    dummyMJ = min(p->MJ,1.99);

    //Fill MJ distributions
      if(njet < 5){
        MJ[njet-3][isVR][2]->Fill(dummyMJ,wt); //b-inclusive
        MJ[njet-3][isVR][b_tag]->Fill(dummyMJ,wt); 
      }
      for(int jet = 4; jet <6; jet++){
        if(njet >= jet){
          MJ[jet-2][isVR][2]->Fill(dummyMJ,wt); 
          MJ[jet-2][isVR][b_tag]->Fill(dummyMJ,wt); 
        }
      }


    ///Fill Mass distributions
    for(int k = 0; k <min(njet,5); k++){
      dummyMass = min((*p->jet_m)[k],1.99);
      bool cent = (fabs((*p->jet_eta)[k]) < 1.0);
      //if(fabs((*p->jet_eta)[k]) > 1.0) continue;
        if(njet < 5){
          mass[k][njet-3][isVR][2]->Fill(dummyMass,wt);
          mass[k][njet-3][isVR][b_tag]->Fill(dummyMass,wt);
          if(cent) {
            mass_central[k][njet-3][isVR][2]->Fill(dummyMass,wt);
            mass_central[k][njet-3][isVR][b_tag]->Fill(dummyMass,wt);
          }
          else{
            mass_forward[k][njet-3][isVR][2]->Fill(dummyMass,wt);
            mass_forward[k][njet-3][isVR][b_tag]->Fill(dummyMass,wt);
          }
        }
        for(int jet = 4; jet < 6; jet++){
          if(njet >= jet){
            mass[k][jet-2][isVR][2]->Fill(dummyMass,wt);
            mass[k][jet-2][isVR][b_tag]->Fill(dummyMass,wt);
          if(cent) {
            mass_central[k][jet-2][isVR][2]->Fill(dummyMass,wt);
            mass_central[k][jet-2][isVR][b_tag]->Fill(dummyMass,wt);
          }
          else{
            mass_forward[k][jet-2][isVR][2]->Fill(dummyMass,wt);
            mass_forward[k][jet-2][isVR][b_tag]->Fill(dummyMass,wt);
          }
          }
        }
    }

    //Fill templates
    double pt,eta,mass,dummyPt,dummyEta;
    int bin,i,j,l;
    if(isCR){
      for(int k = 0; k < njet; k++){
        //b_tag = (*p->jet_bmatched_Fix70)[k];
        pt = (*p->jet_pt)[k];
        eta = (*p->jet_eta)[k];
        dummyEta = min(fabs(eta),config.etaBins[nEtaBins]-0.01);
        mass = (*p->jet_m)[k];
        
        if(p->njet_soft == 0){
          dummyPt = min(pt,config.ptBins3[nPtBins3]-0.001);

          h_templGrid_log[b_tag]->Fill(log(dummyPt/0.05),dummyEta,wt);
          bin = h_templGrid[0][b_tag]->Fill(dummyPt,dummyEta,wt);
          h_templGrid[0][b_tag]->GetBinXYZ(bin,j,i,l);
          h_mpt[0][b_tag][i-1][j-1]->Fill(log(mass/pt),wt);
          h_mpt[0][2][i-1][j-1]->Fill(log(mass/pt),wt);
        }

        if(p->njet_soft >= 1){
          dummyPt = min(pt,config.ptBins4[nPtBins4]-0.001);

          bin = h_templGrid[1][b_tag]->Fill(dummyPt,dummyEta,wt);
          h_templGrid[1][b_tag]->GetBinXYZ(bin,j,i,l);
          h_mpt[1][b_tag][i-1][j-1]->Fill(log(mass/pt),wt);
          h_mpt[1][2][i-1][j-1]->Fill(log(mass/pt),wt);
        }
          
        if(p->njet_soft >= 2){
          dummyPt = min(pt,config.ptBins5[nPtBins5]-0.001);

          bin = h_templGrid[2][b_tag]->Fill(dummyPt,dummyEta,wt);
          bin = h_templGrid[2][2]->Fill(dummyPt,dummyEta,wt);

          h_templGrid[2][b_tag]->GetBinXYZ(bin,j,i,l);
          h_mpt[2][b_tag][i-1][j-1]->Fill(log(mass/pt),wt);
          h_mpt[2][2][i-1][j-1]->Fill(log(mass/pt),wt);
        }

      }
    }
    //in the case of signal, dont want empty statistics bins. Fill with small values.
    for(int b = 0; b < 3; b++){
    for(int k = 0; k < 3; k++){
      for(int i =1; i <= h_templGrid[b][k]->GetNbinsX(); i++){
        for(int j =1; j <= h_templGrid[b][k]->GetNbinsY(); j++){
          if(h_templGrid[b][k]->GetBinContent(i,j) == 0) h_templGrid[b][k]->SetBinContent(i,j,1e-4);
        }
      }
    }
  }

}

f->Write();
f->Close();
return 0;

}

