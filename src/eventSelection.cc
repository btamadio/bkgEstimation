#include "eventSelection.hh"

int main (int argc, char **argv)
{
		 if (argc < 2)
		 {
		    printf("\nUsage: %s *.root\n\n",argv[0]);
		    exit(0);
		 }
		 
		 int const ind = argc - 1; 
 		 TString *path = new TString[ind];
		 TChain *chain = new TChain("outTree/nominal");
		 //TChain *chain = new TChain("outTree");

		 for ( int j = 0; j < argc-1; j++)
		 {
			path[j] = (argv[j+1]);
			ifstream file_to_read((TString)"./files/"+path[j]);
			if(file_to_read.is_open() != 1 )
			  {
			    cout << "The input file " << "./files/"<< path[j]  << " does not exist." << endl;
			    exit(1);
			  }

		 while(!file_to_read.eof())
		   {
		     TString path_to_the_file ;
		     file_to_read >> path_to_the_file ;
		     chain->Add(path_to_the_file);
		   }
		 }

		if(path[0].Contains("data2015")) m_GRLFileName = "/project/projectdirs/atlas/ndshal/RPV_SUSY/bkgEstimation/GRL/data15_13TeV.periodAllYear_DetStatus-v79-repro20-02_DQDefects-00-02-02_PHYS_StandardGRL_All_Good_25ns.xml";
		if(path[0].Contains("data2016")) m_GRLFileName = "/project/projectdirs/atlas/ndshal/RPV_SUSY/bkgEstimation/GRL/data16_13TeV.periodAllYear_DetStatus-v81-pro20-09_DQDefects-00-02-02_PHYS_StandardGRL_All_Good_25ns.xml";

		p = new nominalMC(chain);

        p -> fChain ->SetBranchStatus("*",0);
        p -> fChain ->SetBranchStatus("runNumber",1);
        p -> fChain ->SetBranchStatus("mcChannelNumber",1);
        p -> fChain ->SetBranchStatus("eventNumber",1);
        p -> fChain ->SetBranchStatus("averageInteractionsPerCrossing",1);
		p -> fChain ->SetBranchStatus("lumiBlock",1);
		p -> fChain ->SetBranchStatus("TileError",1);
		p -> fChain ->SetBranchStatus("SCTError",1);
		p -> fChain ->SetBranchStatus("LArError",1);
        p -> fChain ->SetBranchStatus("jet_pt",1);
        p -> fChain ->SetBranchStatus("jet_eta",1);
        p -> fChain ->SetBranchStatus("jet_phi",1);
        p -> fChain ->SetBranchStatus("jet_E",1);
        p -> fChain ->SetBranchStatus("fatjet_pt",1);
        p -> fChain ->SetBranchStatus("fatjet_eta",1);
        p -> fChain ->SetBranchStatus("fatjet_phi",1);
        p -> fChain ->SetBranchStatus("fatjet_m",1);
        p -> fChain ->SetBranchStatus("weight",1);
        p -> fChain ->SetBranchStatus("passedTriggers",1);
        p -> fChain ->SetBranchStatus("jet_clean_passLooseBad",1);

        /*
        p -> fChain ->SetBranchStatus("jet_MV2c20_isFix70",1);
        p -> fChain ->SetBranchStatus("jet_MV2c20_SFFlt70",1);
        p -> fChain ->SetBranchStatus("weight_jet__MV2c20_SFFlt70",1);
        p -> fChain ->SetBranchStatus("weight_jet__MV2c20_SFFix70",1);
        */
        p -> fChain -> SetBranchStatus("weight_jet_SFFix60",1);
        p -> fChain -> SetBranchStatus("weight_jet_SFFix70",1);
        p -> fChain -> SetBranchStatus("weight_jet_SFFix77",1);
        p -> fChain -> SetBranchStatus("weight_jet_SFFix85",1);
        p -> fChain ->SetBranchStatus("jet_isFix60",1);
        p -> fChain ->SetBranchStatus("jet_isFix70",1);
        p -> fChain ->SetBranchStatus("jet_isFix77",1);
        p -> fChain ->SetBranchStatus("jet_isFix85",1);
        p -> fChain ->SetBranchStatus("jet_isFlt60",1);
        p -> fChain ->SetBranchStatus("jet_isFlt70",1);
        p -> fChain ->SetBranchStatus("jet_isFlt77",1);
        p -> fChain ->SetBranchStatus("jet_isFlt85",1);



        //Cutflow
        TH1F *h_cutflow = new TH1F("h_cutflow","Cutflow",6,0,6);
        h_cutflow->GetXaxis()->SetBinLabel(1,"GRL");
        h_cutflow->GetXaxis()->SetBinLabel(2,"SCT/Tile/LAr");
        h_cutflow->GetXaxis()->SetBinLabel(3,"Trigger");
    	h_cutflow->GetXaxis()->SetBinLabel(4,"lead LargeRJet pT");
    	h_cutflow->GetXaxis()->SetBinLabel(5,"njet == 3");
    	h_cutflow->GetXaxis()->SetBinLabel(6,"njet == 3 && btag (fix70)");

        //miniTree
        TFile f("miniTree.root","recreate");
        TTree *mini = new TTree("miniTree","miniTree");

	int mini_runNumber,mini_mcChannelNumber=-1,mini_lumiBlock,mini_njet,mini_nbjet_fix60,mini_nbjet_fix70,mini_nbjet_fix77,mini_nbjet_fix85,mini_nbjet_flt60,mini_nbjet_flt70,mini_nbjet_flt77,mini_nbjet_flt85,mini_njet_ak4, mini_njet_soft;
		Long64_t mini_eventNumber;
		bool mini_isMC;
		double mini_HT, mini_MJ,mini_wt, mini_dEta,mini_mu;
		vector<double> *mini_jet_pt = 0;
		vector<double> *mini_jet_eta= 0;
		vector<double> *mini_jet_phi = 0;
		vector<double> *mini_jet_m =0;
		vector<int> *mini_jet_bmatched_fix60 =0;
		vector<int> *mini_jet_bmatched_fix70 =0;
		vector<int> *mini_jet_bmatched_fix77 =0;
		vector<int> *mini_jet_bmatched_fix85 =0;
		vector<int> *mini_jet_bmatched_flt60 =0;
		vector<int> *mini_jet_bmatched_flt70 =0;
		vector<int> *mini_jet_bmatched_flt77 =0;
		vector<int> *mini_jet_bmatched_flt85 =0;

		mini->Branch("runNumber",&mini_runNumber);
		mini->Branch("mcChannelNumber",&mini_mcChannelNumber);
		mini->Branch("eventNumber",&mini_eventNumber);
		mini->Branch("lumiBlock",&mini_lumiBlock);
		mini->Branch("mu",&mini_mu);
		mini->Branch("isMC",&mini_isMC);
		mini->Branch("weight",&mini_wt);
		mini->Branch("njet",&mini_njet);
		mini->Branch("njet_soft",&mini_njet_soft);
		mini->Branch("njet_ak4",&mini_njet_ak4);
		mini->Branch("nbjet_Fix60",&mini_nbjet_fix60);
		mini->Branch("nbjet_Fix70",&mini_nbjet_fix70);
		mini->Branch("nbjet_Fix77",&mini_nbjet_fix77);
		mini->Branch("nbjet_Fix85",&mini_nbjet_fix85);
		mini->Branch("nbjet_Flt60",&mini_nbjet_flt60);
		mini->Branch("nbjet_Flt70",&mini_nbjet_flt70);
		mini->Branch("nbjet_Flt77",&mini_nbjet_flt77);
		mini->Branch("nbjet_Flt85",&mini_nbjet_flt85);
		mini->Branch("jet_pt",&mini_jet_pt);
		mini->Branch("jet_eta",&mini_jet_eta);
		mini->Branch("jet_phi",&mini_jet_phi);
		mini->Branch("jet_m",&mini_jet_m);
		mini->Branch("jet_bmatched_Fix60",&mini_jet_bmatched_fix60);
		mini->Branch("jet_bmatched_Fix70",&mini_jet_bmatched_fix70);
		mini->Branch("jet_bmatched_Fix77",&mini_jet_bmatched_fix77);
		mini->Branch("jet_bmatched_Fix85",&mini_jet_bmatched_fix85);

		mini->Branch("jet_bmatched_Flt60",&mini_jet_bmatched_flt60);
		mini->Branch("jet_bmatched_Flt70",&mini_jet_bmatched_flt70);
		mini->Branch("jet_bmatched_Flt77",&mini_jet_bmatched_flt77);
		mini->Branch("jet_bmatched_Flt85",&mini_jet_bmatched_flt85);

		mini->Branch("dEta",&mini_dEta);
		mini->Branch("HT_ak4",&mini_HT);
		mini->Branch("MJ",&mini_MJ);

		 int numev = p->fChain->GetEntries();
		 cout << "Total events to process: " << numev << endl;
		 int pass_evt_counter = 0;
		 bool isMC = false;
		 if(p->fChain->GetBranchStatus("mcChannelNumber")) isMC = true;
		 if(!isMC) readGRL();

		 for( int i = 0 ; i < numev; i ++ )
		   {
		     get_event(i) ;
             if( i%10000 == 0 ) cout << i << " events processed " << endl;
             
             int check_run = p->runNumber;
             int check_lumi = p->lumiBlock;
             long int check_event = p->eventNumber;

             double wt=1;
		     if(isMC) wt = 1e6*(p->weight)*((*p->weight_jet_SFFix70)[0])/Nevent(p->mcChannelNumber);

		     //check good run list
    		if(!isMC && !passGRL(check_run,check_lumi)){
      			cout << "run Number: " << check_run << ", event: " << check_event << ", LumiBlock: " << check_lumi << " was a bad GRL event"<<endl;
     		    continue;}

     		h_cutflow->Fill(0);

      		//check SCT/Tile/LAr
     		if(!isMC && (p->LArError || p->TileError || p->SCTError)) continue;
      		h_cutflow->Fill(1);
    
    		//check trigger
    		bool trigger = false;
    		for( unsigned int k = 0 ; k <  (*p->passedTriggers).size()  ;  k ++ ){
      			TString pass_trigger = (*p->passedTriggers)[k];
      			if(pass_trigger.Contains("HLT_j360_a10") || pass_trigger.Contains("HLT_j380_a10") || pass_trigger.Contains("HLT_j400_a10")  || pass_trigger.Contains("HLT_j420_a10") ) {trigger = true;}
    		}
    		if(trigger == false) continue;

		    h_cutflow->Fill(2);

		     //Loop over small R jets
		     int njet = (int)(*p->jet_pt).size(),ak4_counter=0;

		     //TODO: Fix this code using TLorentzVectors
		     int bjet_counter_fix60=0,bjet_counter_fix70 = 0,bjet_counter_fix77=0,bjet_counter_fix85=0;
		     int bjet_counter_flt60=0,bjet_counter_flt70 = 0,bjet_counter_flt77=0,bjet_counter_flt85=0;

		     vector<double> bjet_flt60_eta;
		     vector<double> bjet_flt70_eta;
		     vector<double> bjet_flt77_eta;
		     vector<double> bjet_flt85_eta;

		     vector<double> bjet_fix60_eta;
		     vector<double> bjet_fix70_eta;
		     vector<double> bjet_fix77_eta;
		     vector<double> bjet_fix85_eta;

		     vector<double> bjet_flt60_phi;
		     vector<double> bjet_flt70_phi;
		     vector<double> bjet_flt77_phi;
		     vector<double> bjet_flt85_phi;

		     vector<double> bjet_fix60_phi;
		     vector<double> bjet_fix70_phi;
		     vector<double> bjet_fix77_phi;
		     vector<double> bjet_fix85_phi;

		    for( int k = 0 ; k < njet ; k ++ ){
		       if( fabs((*p->jet_eta)[k]) > 2.8 ) continue ;
		       if( (*p->jet_pt)[k] < 50 ) continue ;
		       if((*p->jet_clean_passLooseBad)[k] == 0) continue;    
		       ak4_counter++;
		       if((*p->jet_isFix60)[k]) {
				bjet_counter_fix60++;
				//save kinematics to match to largeR jet
				bjet_fix60_eta.push_back((*p->jet_eta)[k]);
				bjet_fix60_phi.push_back((*p->jet_phi)[k]);
		       }
		       if((*p->jet_isFix70)[k]) {
				bjet_counter_fix70++;
				//save kinematics to match to largeR jet
				bjet_fix70_eta.push_back((*p->jet_eta)[k]);
				bjet_fix70_phi.push_back((*p->jet_phi)[k]);
		       }
		       if((*p->jet_isFix77)[k]) {
				bjet_counter_fix77++;
				//save kinematics to match to largeR jet
				bjet_fix77_eta.push_back((*p->jet_eta)[k]);
				bjet_fix77_phi.push_back((*p->jet_phi)[k]);
		       }
		       if((*p->jet_isFix85)[k]) {
				bjet_counter_fix85++;
				//save kinematics to match to largeR jet
				bjet_fix85_eta.push_back((*p->jet_eta)[k]);
				bjet_fix85_phi.push_back((*p->jet_phi)[k]);
		       }

		       if((*p->jet_isFlt60)[k]) {
			 bjet_counter_flt60++;
			 //save kinematics to match to largeR jet
			 bjet_flt60_eta.push_back((*p->jet_eta)[k]);
			 bjet_flt60_phi.push_back((*p->jet_phi)[k]);
		       }
		       if((*p->jet_isFlt70)[k]) {
			 bjet_counter_flt70++;
			 //save kinematics to match to largeR jet
			 bjet_flt70_eta.push_back((*p->jet_eta)[k]);
			 bjet_flt70_phi.push_back((*p->jet_phi)[k]);
		       }
		       if((*p->jet_isFlt77)[k]) {
			 bjet_counter_flt77++;
			 //save kinematics to match to largeR jet
			 bjet_flt77_eta.push_back((*p->jet_eta)[k]);
			 bjet_flt77_phi.push_back((*p->jet_phi)[k]);
		       }
		       if((*p->jet_isFlt85)[k]) {
			 bjet_counter_flt85++;
			 //save kinematics to match to largeR jet
			 bjet_flt85_eta.push_back((*p->jet_eta)[k]);
			 bjet_flt85_phi.push_back((*p->jet_phi)[k]);
		       }
		    }

			//Loop over fat jets
		    int nfatjet = (int)(*p->fatjet_pt).size(), max_ind ;
		    vector<double> jet_pt;
		    vector<double> jet_eta;
		    vector<double> jet_phi;
		    vector<double> jet_m;
		    mini_HT = 0; mini_MJ = 0;
		    int NsoftJet = 0;
		    double dummy_m;
		    for(int l = 0; l < nfatjet; l++){
		      if((*p->fatjet_pt)[l] > 100 && (*p->fatjet_pt)[l] < 200 && fabs((*p->fatjet_eta)[l]) < 2.0 ) NsoftJet++;
		      if( fabs((*p->fatjet_eta)[l]) > 2.0 ) continue ;
			  if( (*p->fatjet_pt)[l] < 200 ) continue ;
			  TLorentzVector thisfatjet ;
			  thisfatjet.SetPtEtaPhiM(  (*p->fatjet_pt)[l] ,  (*p->fatjet_eta)[l],  (*p->fatjet_phi)[l],  (*p->fatjet_m)[l] );
			  if( thisfatjet.M()/thisfatjet.Pt() > 1.0) continue;

			  jet_pt.push_back(thisfatjet.Pt()/1000);
			  jet_eta.push_back(thisfatjet.Eta());
			  jet_phi.push_back(thisfatjet.Phi());
			  dummy_m = thisfatjet.M()/1000;
			  if(dummy_m < 0) dummy_m = 0;
			  jet_m.push_back(dummy_m);
			  mini_HT += thisfatjet.Pt()/1000;

		    }
		    mini_njet = jet_pt.size();
		    mini_njet_soft = NsoftJet;

		    //Sort by pt
		    sort(jet_pt.begin(),jet_pt.end(),greater<double>());
		    vector<int> idx = sort_indices(jet_pt);
		    reorder(jet_eta,idx);
		    reorder(jet_phi,idx);
		    reorder(jet_m,idx);

		    //match to ak4 bjets
		    double dEta, dPhi, dR;
		    vector<int> jet_bmatched_fix60;
		    vector<int> jet_bmatched_fix70;
		    vector<int> jet_bmatched_fix77;
		    vector<int> jet_bmatched_fix85;

		    vector<int> jet_bmatched_flt60;
		    vector<int> jet_bmatched_flt70;
		    vector<int> jet_bmatched_flt77;
		    vector<int> jet_bmatched_flt85;

		    bool matched = false;
		    for(int k = 0; k < mini_njet; k++){
		    	//fix60
		    	matched = false;
		    	for(int l = 0; l < bjet_counter_fix60; l++){
		    		dPhi = fabs(jet_phi[k] - bjet_fix60_phi[l]);
		    		if(dPhi > 3.141) dPhi = 2*3.141 - dPhi;
		    		dEta = fabs(jet_eta[k] - bjet_fix60_eta[l]);
		    		dR = sqrt(dEta*dEta + dPhi*dPhi);
		    		if(dR < 1.0) matched = true;
		    	}
		    	jet_bmatched_fix60.push_back((int)matched);

		    	//fix70
		    	matched = false;
		    	for(int l = 0; l < bjet_counter_fix70; l++){
		    		dPhi = fabs(jet_phi[k] - bjet_fix70_phi[l]);
		    		if(dPhi > 3.141) dPhi = 2*3.141 - dPhi;
		    		dEta = fabs(jet_eta[k] - bjet_fix70_eta[l]);
		    		dR = sqrt(dEta*dEta + dPhi*dPhi);
		    		if(dR < 1.0) matched = true;
		    	}
		    	jet_bmatched_fix70.push_back((int)matched);

		    	//fix77
		    	matched = false;
		    	for(int l = 0; l < bjet_counter_fix77; l++){
		    		dPhi = fabs(jet_phi[k] - bjet_fix77_phi[l]);
		    		if(dPhi > 3.141) dPhi = 2*3.141 - dPhi;
		    		dEta = fabs(jet_eta[k] - bjet_fix77_eta[l]);
		    		dR = sqrt(dEta*dEta + dPhi*dPhi);
		    		if(dR < 1.0) matched = true;
		    	}
		    	jet_bmatched_fix77.push_back((int)matched);

		    	//fix85
		    	matched = false;
		    	for(int l = 0; l < bjet_counter_fix85; l++){
		    		dPhi = fabs(jet_phi[k] - bjet_fix85_phi[l]);
		    		if(dPhi > 3.141) dPhi = 2*3.141 - dPhi;
		    		dEta = fabs(jet_eta[k] - bjet_fix85_eta[l]);
		    		dR = sqrt(dEta*dEta + dPhi*dPhi);
		    		if(dR < 1.0) matched = true;
		    	}
		    	jet_bmatched_fix85.push_back((int)matched);

		    	//flt60
		    	matched = false;
		    	for(int l = 0; l < bjet_counter_flt60; l++){
		    		dPhi = fabs(jet_phi[k] - bjet_flt60_phi[l]);
		    		if(dPhi > 3.141) dPhi = 2*3.141 - dPhi;
		    		dEta = fabs(jet_eta[k] - bjet_flt60_eta[l]);
		    		dR = sqrt(dEta*dEta + dPhi*dPhi);
		    		if(dR < 1.0) matched = true;
		    	}
		    	jet_bmatched_flt60.push_back((int)matched);
		    	//flt70
		    	matched = false;
		    	for(int l = 0; l < bjet_counter_flt70; l++){
		    		dPhi = fabs(jet_phi[k] - bjet_flt70_phi[l]);
		    		if(dPhi > 3.141) dPhi = 2*3.141 - dPhi;
		    		dEta = fabs(jet_eta[k] - bjet_flt70_eta[l]);
		    		dR = sqrt(dEta*dEta + dPhi*dPhi);
		    		if(dR < 1.0) matched = true;
		    	}
		    	jet_bmatched_flt70.push_back((int)matched);
		    	//flt77
		    	matched = false;
		    	for(int l = 0; l < bjet_counter_flt77; l++){
		    		dPhi = fabs(jet_phi[k] - bjet_flt77_phi[l]);
		    		if(dPhi > 3.141) dPhi = 2*3.141 - dPhi;
		    		dEta = fabs(jet_eta[k] - bjet_flt77_eta[l]);
		    		dR = sqrt(dEta*dEta + dPhi*dPhi);
		    		if(dR < 1.0) matched = true;
		    	}
		    	jet_bmatched_flt77.push_back((int)matched);
		    	//flt85
		    	matched = false;
		    	for(int l = 0; l < bjet_counter_flt85; l++){
		    		dPhi = fabs(jet_phi[k] - bjet_flt85_phi[l]);
		    		if(dPhi > 3.141) dPhi = 2*3.141 - dPhi;
		    		dEta = fabs(jet_eta[k] - bjet_flt85_eta[l]);
		    		dR = sqrt(dEta*dEta + dPhi*dPhi);
		    		if(dR < 1.0) matched = true;
		    	}
		    	jet_bmatched_flt85.push_back((int)matched);


		    }

		    if(mini_njet == 0 || jet_pt[0] < 0.44) continue;
		    h_cutflow->Fill(3);

		    //Calculate MJ
		    max_ind = 4;
		    if(mini_njet < 4) max_ind = mini_njet;
		    for(int ii =0; ii < max_ind; ii++) mini_MJ += jet_m[ii]; 

		    //Calculate dEta
		    mini_dEta = fabs(jet_eta[0]-jet_eta[1]);

			//cutflows
			if(mini_njet == 3) h_cutflow->Fill(4);
			if(mini_njet == 3 && bjet_counter_fix70 > 0) h_cutflow->Fill(5);
			if(mini_njet < 3) continue; //all regions have njet >= 3

		     //Save in new Tree
		     mini_runNumber = p->runNumber;
		     mini_eventNumber = p->eventNumber;
		     mini_lumiBlock = p->lumiBlock;
		     mini_mu = p->averageInteractionsPerCrossing;
		     if(isMC) mini_mcChannelNumber = p->mcChannelNumber;
		     mini_isMC = isMC;
		     mini_wt = wt;
		     mini_njet_ak4 = ak4_counter;
		     mini_nbjet_flt60 = bjet_counter_flt60;
		     mini_nbjet_flt70 = bjet_counter_flt70;
		     mini_nbjet_flt77 = bjet_counter_flt77;
		     mini_nbjet_flt85 = bjet_counter_flt85;
		     mini_nbjet_fix60 = bjet_counter_fix60;
		     mini_nbjet_fix70 = bjet_counter_fix70;
		     mini_nbjet_fix77 = bjet_counter_fix77;
		     mini_nbjet_fix85 = bjet_counter_fix85;
		     mini_jet_pt = &jet_pt;
		     mini_jet_eta = &jet_eta;
		     mini_jet_phi = &jet_phi;
		     mini_jet_m = &jet_m;
		     mini_jet_bmatched_flt60 = &jet_bmatched_flt60;
		     mini_jet_bmatched_flt70 = &jet_bmatched_flt70;
		     mini_jet_bmatched_flt77 = &jet_bmatched_flt77;
		     mini_jet_bmatched_flt85 = &jet_bmatched_flt85;

		     mini_jet_bmatched_fix60 = &jet_bmatched_fix60;
		     mini_jet_bmatched_fix70 = &jet_bmatched_fix70;
		     mini_jet_bmatched_fix77 = &jet_bmatched_fix77;
		     mini_jet_bmatched_fix85 = &jet_bmatched_fix85;

		     mini->GetEntry(pass_evt_counter);
		     mini->Fill();

		     pass_evt_counter++;

		   }

		mini->Write();
		h_cutflow->Write();
		f.Close();

	return 0 ; 
}







