// Split one tree into 5 trees based on the 4 polarization orientations
// 	+ 1 for the amorphous. 

void split_flat_pols(string fname){
	// Load the tree you want to split
	TFile* f=new TFile(fname.c_str());
	TTree* t;
	f->GetObject("kin",t);

	// Load the pol_angle branch so we can filter the tree
	int pol_angle;
	t->SetBranchAddress("pol_angle",&pol_angle);
	Long64_t nentries=t->GetEntries();

	// Create some TFiles and TTrees for storage
	map<int,int> polMap={ {0,0}, {45,1}, {90,2}, {135,3}, {-1, 4} };
	vector<TFile*> fs(5);
	vector<TTree*> ts(5);
	fs[0] = new TFile(("pol000_"+fname).c_str(),"RECREATE");
	ts[0] = t->CloneTree(0);
	fs[1] = new TFile(("pol045_"+fname).c_str(),"RECREATE");
	ts[1] = t->CloneTree(0);
	fs[2] = new TFile(("pol090_"+fname).c_str(),"RECREATE");
	ts[2] = t->CloneTree(0);
	fs[3] = new TFile(("pol135_"+fname).c_str(),"RECREATE");
	ts[3] = t->CloneTree(0);
	fs[4] = new TFile(("polAMO_"+fname).c_str(),"RECREATE");
	ts[4] = t->CloneTree(0);
	
	// Loop over original tree and fill new trees
	for (Long64_t ientry=0; ientry<nentries; ++ientry){
		t->GetEntry(ientry);
		ts[polMap[pol_angle]]->Fill();
	}	

	// Save the trees
	for (auto _t :ts){
		_t->AutoSave();
	}
}
