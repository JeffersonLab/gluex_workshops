void gluex_style() {
	///////// 
	// Make the plots prettier with bigger fonts
	// Original code is https://halldweb.jlab.org/wiki-private/index.php/Guidance_for_Presentations 
	// 	slightly modified
	/////////
	auto gluex_style = new TStyle("GlueX","Default GlueX Style");

        gluex_style->SetCanvasBorderMode(0);
        gluex_style->SetPadBorderMode(0);
        gluex_style->SetPadColor(0);
        gluex_style->SetCanvasColor(0);
        gluex_style->SetStatColor(0);

	// some default window sizes
	gluex_style->SetCanvasDefW(800);
	gluex_style->SetCanvasDefH(600);

	// let's change the default margins
	gluex_style->SetPadBottomMargin(0.15);
	gluex_style->SetPadLeftMargin(0.2);
	gluex_style->SetPadTopMargin(0.08);
	gluex_style->SetPadRightMargin(0.15);

	// axis labels and settings
        gluex_style->SetStripDecimals(0);
 	gluex_style->SetLabelSize(0.045,"xyz"); // size of axis value font
 	gluex_style->SetTitleSize(0.06,"xyz"); // size of axis title font
 	gluex_style->SetTitleFont(42,"xyz"); // font option
 	gluex_style->SetLabelFont(42,"xyz"); 
 	gluex_style->SetTitleOffset(1.5,"y"); 
 	gluex_style->SetLabelOffset(0.01,"xyz");   // stop collisions of "0"s at the origin
 	
	// histogram settings
	gluex_style->SetOptStat(0);     // no stats box by default
	gluex_style->SetOptTitle(0);    // no title by default
	gluex_style->SetHistLineWidth(2); 
	gluex_style->SetNdivisions(508,"xyz"); // some ticks were very bunched, lets reduce the number of divisions to label 

	gluex_style->SetHistFillColor(920);   // grey
	//gluex_style->SetPalette(kViridis); // kViridis is perceptually uniform and colorblind friendly
	gluex_style->cd();
}

void drawCuts(){
	/////////////////////////
	// Load the histograms from the histogram root file and draw 
	// 	some shaded boxes / curves to denote the selection.
	// 	For all other variables, simply loop over them and
	// 	draw them all
	/////////////////////////
	gSystem->Exec("mkdir -p plots");	

	gluex_style();
	gROOT->ForceStyle();	

	string drawOptions="HIST";

	TFile* f=new TFile("output_hists.root");
	TH1F* h1;
	TH2F* h2;
	TBox* box=new TBox();
	box->SetFillColorAlpha(kGreen+2,0.3);
	TCanvas* c=new TCanvas("","",800,600);
	string hname;

	hname="photonThetaPi0";
	f->GetObject(hname.c_str(),h1);
	h1->Draw(drawOptions.c_str());
	h1->SetMinimum(0);
	box->DrawBox(2.5,0,10.3,h1->GetMaximum()*1.05);
	box->DrawBox(11.9,0,h1->GetXaxis()->GetXmax(),h1->GetMaximum()*1.05);
	c->SaveAs(("plots/"+hname+".pdf").c_str());

	hname="photonThetaEta";
	f->GetObject(hname.c_str(),h1);
	h1->Draw(drawOptions.c_str());
	h1->SetMinimum(0);
	box->DrawBox(2.5,0,10.3,h1->GetMaximum()*1.05);
	box->DrawBox(11.9,0,h1->GetXaxis()->GetXmax(),h1->GetMaximum()*1.05);
	c->SaveAs(("plots/"+hname+".pdf").c_str());

	hname="proton_z";
	f->GetObject(hname.c_str(),h1);
	h1->Draw(drawOptions.c_str());
	box->DrawBox(52,0,78,h1->GetMaximum()*1.05);
	c->SaveAs(("plots/"+hname+".pdf").c_str());

	hname="dEdx_momentum";
	f->GetObject(hname.c_str(),h2);
	h2->Draw("COLZ");
	TF1 *f_dedx = new TF1("fa1","10^(-6)*(0.9+exp(3.0-3.5*(x+0.05)/0.93827))",h2->GetXaxis()->GetXmin(),h2->GetXaxis()->GetXmax());
	f_dedx->SetLineColor(kRed);
	f_dedx->SetLineWidth(3);
	f_dedx->Draw("SAME");
	c->SaveAs(("plots/"+hname+".pdf").c_str());

	hname="mmsq";
	f->GetObject(hname.c_str(),h1);
	h1->Draw(drawOptions.c_str());
	h1->SetMinimum(1); // mmsq distribution is very narrow so its good to look at it log-scale. Cannot set minimum to 0 since log(0)->inf
	gPad->SetLogy();
	box->DrawBox(-0.05,0,0.05,h1->GetMaximum()*1.05);
	c->SaveAs(("plots/"+hname+".pdf").c_str());
	gPad->SetLogy(0);

	// This is another plot that should be looked at in log-scale since most events should have pretty good chiSq
	hname="chiSq"; 
	f->GetObject(hname.c_str(),h1);
	h1->Draw(drawOptions.c_str());
	h1->SetMinimum(1);
	gPad->SetLogy();
	box->DrawBox(0,0,13.277,h1->GetMaximum()*1.05);
	c->SaveAs(("plots/"+hname+".pdf").c_str());
	gPad->SetLogy(0);

	// Useful to look in log since the t-slopes are interesting. Different t-slopes can indicate different production
	//   t distributed as exp(-at), plotting in log-scale exposes the argument, a. 
	hname="mandelstam_t";
	f->GetObject(hname.c_str(),h1);
	h1->Draw(drawOptions.c_str());
	h1->SetMinimum(1);
	gPad->SetLogy();
	box->DrawBox(0.1,0,0.3,h1->GetMaximum()*1.05);	
	c->SaveAs(("plots/"+hname+".pdf").c_str());
	gPad->SetLogy(0);

	hname="BeamEnergy";
	f->GetObject(hname.c_str(),h1);
	h1->Draw(drawOptions.c_str());
	h1->SetMinimum(0);
	box->DrawBox(8.2,0,8.8,h1->GetMaximum()*1.05);	
	c->SaveAs(("plots/"+hname+".pdf").c_str());

	hname="Mpi0p";
	f->GetObject(hname.c_str(),h1);
	h1->Draw(drawOptions.c_str());
	h1->SetMinimum(0);
	box->DrawBox(1.4,0,h1->GetXaxis()->GetXmax(),h1->GetMaximum()*1.05);	
	c->SaveAs(("plots/"+hname+".pdf").c_str());

	hname="Metapi_sig";
	f->GetObject(hname.c_str(),h1);
	h1->Draw(drawOptions.c_str());
	h1->SetMinimum(0);
	box->DrawBox(1.04,0,1.56,h1->GetMaximum()*1.05);	
	c->SaveAs(("plots/"+hname+".pdf").c_str());

	float pi0Mean=0.135881;
	float pi0Std=0.0076;
	hname="Mpi0";
	f->GetObject(hname.c_str(),h1);
	h1->Draw(drawOptions.c_str());
	h1->SetMinimum(0);
	box->SetFillColorAlpha(kRed,0.3);
	box->DrawBox(pi0Mean-5.5*pi0Std,0,pi0Mean-4*pi0Std,h1->GetMaximum()*1.05);	
	box->DrawBox(pi0Mean+4*pi0Std,0,pi0Mean+5.5*pi0Std,h1->GetMaximum()*1.05);	
	box->SetFillColorAlpha(kGreen+2,0.3);
	box->DrawBox(pi0Mean-3*pi0Std,0,pi0Mean+3*pi0Std,h1->GetMaximum()*1.05);	
	c->SaveAs(("plots/"+hname+".pdf").c_str());

	float etaMean=0.548625;
	float etaStd=0.0191;
	hname="Meta";
	f->GetObject(hname.c_str(),h1);
	h1->Draw(drawOptions.c_str());
	h1->SetMinimum(0);
	box->SetFillColorAlpha(kRed,0.3);
	box->DrawBox(etaMean-6*etaStd,0,etaMean-4*etaStd,h1->GetMaximum()*1.05);	
	box->DrawBox(etaMean+4*etaStd,0,etaMean+6*etaStd,h1->GetMaximum()*1.05);	
	box->SetFillColorAlpha(kGreen+2,0.3);
	box->DrawBox(etaMean-3*etaStd,0,etaMean+3*etaStd,h1->GetMaximum()*1.05);	
	c->SaveAs(("plots/"+hname+".pdf").c_str());

	hname="rftime";
	f->GetObject(hname.c_str(),h1);
	h1->Draw(drawOptions.c_str());
	h1->SetMinimum(0);
	box->SetFillColorAlpha(kRed,0.3);
	box->DrawBox(-20,0,-6,h1->GetMaximum()*1.05);	
	box->DrawBox(6,0,20,h1->GetMaximum()*1.05);	
	box->SetFillColorAlpha(kGreen+2,0.3);
	box->DrawBox(-2,0,2,h1->GetMaximum()*1.05);	
	c->SaveAs(("plots/"+hname+".pdf").c_str());

	/// PLOT ALL EXTRA HISTOGRAMS THAT ARE 1-DIMENSIONAL
	vector<string> hnames={"Metapi_sig_zoom","Metap","combosRemaining"};
	for (auto name: hnames){
		f->GetObject(name.c_str(),h1);
		h1->Draw(drawOptions.c_str());
		h1->SetMinimum(0);
		c->SaveAs(("plots/"+name+".pdf").c_str());
	}

	/// PLOT ALL EXTRA HISTOGRAMS THAT ARE 2-DIMENSIONAL
	hnames={"cosThetaVsMpi0eta"};
	for (auto name: hnames){
		f->GetObject(name.c_str(),h2);
		h2->Draw("COLZ");
		c->SaveAs(("plots/"+name+".pdf").c_str());
	}
}





















