const Int_t nfiles=8;
TCanvas *c[nfiles];
Int_t read_parms (Int_t j, TString filename, vector<double> &yparms, vector<double> &yparms_err, vector< vector< double > > & covMatrix ) {

  vector <TString> sdme;
  
    yparms.clear();
    yparms_err.clear();
    covMatrix.clear();
    
    TString infile = filename+".fit2";   // file with parameters
    
    cout << endl << "Opening parameters file: " << infile.Data() << endl;
  
    // now read and print fitted values
    ifstream parameters;
    parameters.open (infile.Data());
    if (!parameters) {
        cout << "ERROR: Failed to open data file= " << infile.Data() << endl;
        return 1;   // failed to open data file
    }

    TString CanvasName = filename;
    // cout << "j=" << j << " CanvasName=" << CanvasName << endl;
    c[j] = new TCanvas(CanvasName,CanvasName,200,10,1000,700);
    
    TString line;
    Int_t ndx=0;
    while (line.ReadLine(parameters)){
        
        TObjArray *tokens = line.Tokenize("\t");
        Int_t ntokens = tokens->GetEntries();
        
        // cout << " ntokens=" << ntokens << " line=" << line.Data() << endl;
	Int_t jmax=ntokens/3;
        for (Int_t j=0; j<jmax; j++){
	  sdme.push_back( (((TObjString*)tokens->At(3*j))->GetString()) );
          yparms.push_back( (((TObjString*)tokens->At(3*j+1))->GetString()).Atof() );
          yparms_err.push_back( (((TObjString*)tokens->At(3*j+2))->GetString()).Atof());
        }
	if (ntokens != 1) continue;       // get entries with covariance matrix
        
    TObjArray *tokens2 = line.Tokenize(" ");
    Int_t ntokens2 = tokens2->GetEntries();
	// cout << " ntokens2=" << ntokens2 << " line=" << line << endl;
    vector<double> temp;
    temp.clear();
	for (Int_t j=0; j<ntokens2; j++) {
        temp.push_back(  (((TObjString*)tokens2->At(j))->GetString()).Atof() );
	    }
        // cout << " vector=" << setw(20) << temp[0] << setw(20) << temp[1] << setw(20) << temp[2] << setw(20) << temp[3] << setw(20) << temp[4] << setw(20) << temp[5] << setw(20) << temp[6] << setw(20) << temp[7] << endl;
        covMatrix.push_back(temp);
        ndx++;
    }   // end loop over lines
    

    // Output parameters to canvas

        TString title = filename;
        TLatex *t1 = new TLatex(0.2,0.85,title);
        t1->SetNDC();
        t1->SetTextSize(0.04);
        t1->Draw();
        
        for (Int_t j=0; j<sdme.size()-2; j++) {     // -2 to eliminate Sigma and P
        //cout << sdme[j] << "=" << yparms[j] << " err=" << yparms_err[j] << endl;
        
            TString sdmename;
            sdmename = sdme[j];
            title.Form("%s = \t%.3f #pm %.3f\n",sdmename.Data(),yparms[j],yparms_err[j]);
	    // cout << "title=" << title << endl;
        	TLatex *t1 = new TLatex(0.2,0.75 - 0.05*j,title);
        	t1->SetNDC();
        	t1->SetTextSize(0.04);
        	t1->Draw();
        }

        

	// cout << " parms.size()=" << parms.size() << endl;
	/*for (jj=0; jj<parms.size()-2; jj++) {
	yparms[jj] = parms[jj];
	yparms_err[jj] = parms_err[jj];
	}*/

    
    parameters.close();

    return 0;
    
}


void fits_errors()
{
// File: fits_errors.C
  // Read text files with fitted parameters and covariance matrix and compute derived parameters.
//

  gStyle->SetPalette(1,0);
  gStyle->SetOptStat(111111);
  gStyle->SetOptFit(111111);
  gStyle->SetPadRightMargin(0.15);
  gStyle->SetPadLeftMargin(0.15);
  gStyle->SetPadBottomMargin(0.15);

  vector <double> yparms;
  vector <double> yparms_err;
  vector< vector< double > > covMatrix;
  vector <TString> filenames;
    
    
    Float_t g1V00_re[nfiles];
    Float_t g1V00_im[nfiles];
    Float_t g1V11_re[nfiles];
    Float_t g1V11_im[nfiles];
    Float_t g1V10_re[nfiles];
    Float_t g1V10_im[nfiles];
    Float_t g1V1m1_re[nfiles];
    Float_t g1V1m1_im[nfiles];
    Float_t g1V00_re_err[nfiles];
    Float_t g1V00_im_err[nfiles];
    Float_t g1V11_re_err[nfiles];
    Float_t g1V11_im_err[nfiles];
    Float_t g1V10_re_err[nfiles];
    Float_t g1V10_im_err[nfiles];
    Float_t g1V1m1_re_err[nfiles];
    Float_t g1V1m1_im_err[nfiles];
    
    Float_t xparms[nfiles]={1,2,3,4,5,6,7,8};
    Float_t xparms_err[nfiles]={0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01};
    Float_t g1V00_rho[nfiles];
    Float_t g1V00_phi[nfiles];
    Float_t g1V11_rho[nfiles];
    Float_t g1V11_phi[nfiles];
    Float_t g1V10_rho[nfiles];
    Float_t g1V10_phi[nfiles];
    Float_t g1V1m1_rho[nfiles];
    Float_t g1V1m1_phi[nfiles];
    
    Float_t g1V00_rho_err[nfiles];
    Float_t g1V00_phi_err[nfiles];
    Float_t g1V11_rho_err[nfiles];
    Float_t g1V11_phi_err[nfiles];
    Float_t g1V10_rho_err[nfiles];
    Float_t g1V10_phi_err[nfiles];
    Float_t g1V1m1_rho_err[nfiles];
    Float_t g1V1m1_phi_err[nfiles];
    
    
    const Int_t nparms=8;
    
    TGraphErrors *gr_g1V00_re;
    TGraphErrors *gr_g1V00_im;
    TGraphErrors *gr_g1V11_re;
    TGraphErrors *gr_g1V11_im;
    TGraphErrors *gr_g1V10_re;
    TGraphErrors *gr_g1V10_im;
    TGraphErrors *gr_g1V1m1_re;
    TGraphErrors *gr_g1V1m1_im;
    
    TGraphErrors *gr_g1V00_rho;
    TGraphErrors *gr_g1V00_phi;
    TGraphErrors *gr_g1V11_rho;
    TGraphErrors *gr_g1V11_phi;
    TGraphErrors *gr_g1V10_rho;
    TGraphErrors *gr_g1V10_phi;
    TGraphErrors *gr_g1V1m1_rho;
    TGraphErrors *gr_g1V1m1_phi;
    


  filenames.push_back("twopi_primakoff_DSelect_sw1pw1000_NOTAG_W_1000000");
    filenames.push_back("twopi_primakoff_DSelect_sw1pw1000_NOTAG_File_1000000");
    filenames.push_back("twopi_primakoff_DSelect_sw1pw1000_TAG_W_1000000");
    filenames.push_back("twopi_primakoff_DSelect_sw1pw1000_TAG_File_1000000");
    filenames.push_back("twopi_primakoff_DSelect_sw1pw1000_TAG10_W_1000000");
    filenames.push_back("twopi_primakoff_DSelect_sw1pw1000_TAG10_File_1000000");
    filenames.push_back("twopi_primakoff_DSelect_sw1pw1000_TAG40_W_1000000");
    filenames.push_back("twopi_primakoff_DSelect_sw1pw1000_TAG40_File_1000000");

    // filenames.push_back("fits_errors");

    for (j=0; j<filenames.size(); j++) {
      read_parms(j, filenames[j], yparms, yparms_err, covMatrix);
        cout << " yparms=" << yparms[0] << " " << yparms[1] << " "  << yparms[2] << " "  << yparms[3] << " "  << yparms[4] << " "  << yparms[5] << " "  << yparms[6] << " "  << yparms[7] << endl;
        cout << " yparms_err=" << yparms_err[0] << " " << yparms_err[1] << " "  << yparms_err[2] << " "  << yparms_err[3] << " "  << yparms_err[4] << " "  << yparms_err[5] << " "  << yparms_err[6] << " "  << yparms_err[7] << endl;
        g1V00_re[j] = yparms[0];
        g1V00_im[j] = yparms[1];
        g1V11_re[j] = yparms[2];
        g1V11_im[j] = yparms[3];
        g1V10_re[j] = yparms[4];
        g1V10_im[j] = yparms[5];
        g1V1m1_re[j] = yparms[6];
        g1V1m1_im[j] = yparms[7];
        g1V00_re_err[j] = yparms_err[0];
        g1V00_im_err[j] = yparms_err[1];
        g1V11_re_err[j] = yparms_err[2];
        g1V11_im_err[j] = yparms_err[3];
        g1V10_re_err[j] = yparms_err[4];
        g1V10_im_err[j] = yparms_err[5];
        g1V1m1_re_err[j] = yparms_err[6];
        g1V1m1_im_err[j] = yparms_err[7];
        
        
        g1V00_rho[j] = sqrt(yparms[0]*yparms[0]+yparms[1]*yparms[1]);
        g1V00_phi[j] = atan2(yparms[1],yparms[0])*180./3.14159;
        g1V11_rho[j] = sqrt(yparms[2]*yparms[2]+yparms[3]*yparms[3]);
        g1V11_phi[j] = atan2(yparms[3],yparms[2])*180./3.14159;
        g1V10_rho[j] = sqrt(yparms[4]*yparms[4]+yparms[5]*yparms[5]);
        g1V10_phi[j] = atan2(yparms[5],yparms[4])*180./3.14159;
        g1V1m1_rho[j] = sqrt(yparms[6]*yparms[6]+yparms[7]*yparms[7]);
        g1V1m1_phi[j] = atan2(yparms[7],yparms[6])*180./3.14159;
        
        /*cout << " covMatrix.size()=" << covMatrix.size() << endl;
        for (Int_t j=0; j<covMatrix.size(); j++) {
            for (Int_t jj=0; jj<covMatrix.size(); jj++) {
                // double corr = covMatrix[j][j]*covMatrix[jj][jj] != 0?  covMatrix[j][jj]/sqrt(covMatrix[j][j]*covMatrix[jj][jj]) : 0;
                cout << setw(20) << covMatrix[j][jj];
            }
            cout << endl;
        }*/
        
        // See https://en.wikipedia.org/wiki/Propagation_of_uncertainty#cite_note-15
        
        double A = g1V00_re[j];
        double B = g1V00_im[j];
        double rho = sqrt (A*A + B*B);
        double A_err = g1V00_re_err[j];
        double B_err = g1V00_im_err[j];
        double AB_corr = covMatrix[0][1];
        g1V00_rho_err[j] = sqrt(  (A*A)*(A_err*A_err) + (B*B)*(B_err*B_err) + 2*A*B*AB_corr )/rho;
        double R = B==0? 0 : A/B;
        double R_err = abs(R)*sqrt( (A_err/A)*(A_err/A) + (B_err/B)*(B_err/B) -2*AB_corr/(A*B));   // error on ratio
        double phi_err = R_err*(180./3.14159)/(1+R*R);     // error on ArcTan(R)
        double phi = g1V00_phi[j];
        g1V00_phi_err[j] = phi_err;
        cout << endl << " g1V00";
        cout << " rho=" << rho << " A=" << A << " B=" << B << " AB_corr=" << AB_corr << " R=" << R << " R_err=" << R_err << " phi=" << phi << " phi_err=" << phi_err << endl;
        cout << " A_err/A=" << A_err/A << " B_err/B=" << B_err/B << " rho_err/rho=" << g1V00_rho_err[j]/rho << " R_err/R=" << R_err/R << " phi_err/phi=" << phi_err/phi << endl;
        
        
        
        A = g1V11_re[j];
        B = g1V11_im[j];
        rho = sqrt (A*A + B*B);
        A_err = g1V11_re_err[j];
        B_err = g1V11_im_err[j];
        AB_corr = covMatrix[2][3];
        g1V11_rho_err[j] = sqrt(  (A*A)*(A_err*A_err) + (B*B)*(B_err*B_err) + 2*A*B*AB_corr )/rho;
        R = B==0? 0 : A/B;
        R_err = abs(R)*sqrt( (A_err/A)*(A_err/A) + (B_err/B)*(B_err/B) -2*AB_corr/(A*B));   // error on ratio
        phi_err = R_err*(180./3.14159)/(1+R*R);     // error on ArcTan(R)
        phi = g1V11_phi[j];
        g1V11_phi_err[j] = phi_err;
        cout << endl << " g1V11";
        cout << " rho=" << rho << " A=" << A << " B=" << B << " AB_corr=" << AB_corr << " R=" << R << " R_err=" << R_err << " phi=" << phi << " phi_err=" << phi_err << endl;
        cout << " A_err/A=" << A_err/A << " B_err/B=" << B_err/B << " rho_err/rho=" << g1V00_rho_err[j]/rho << " R_err/R=" << R_err/R << " phi_err/phi=" << phi_err/phi << endl;
        
        A = g1V10_re[j];
        B = g1V10_im[j];
        rho = sqrt (A*A + B*B);
        A_err = g1V10_re_err[j];
        B_err = g1V10_im_err[j];
        AB_corr = covMatrix[4][5];
        g1V10_rho_err[j] = sqrt(  (A*A)*(A_err*A_err) + (B*B)*(B_err*B_err) + 2*A*B*AB_corr )/rho;
        R = B==0? 0 : A/B;
        R_err = abs(R)*sqrt( (A_err/A)*(A_err/A) + (B_err/B)*(B_err/B) -2*AB_corr/(A*B));   // error on ratio
        phi_err = R_err*(180./3.14159)/(1+R*R);     // error on ArcTan(R)
        phi = g1V10_phi[j];
        g1V10_phi_err[j] = phi_err;
        cout << endl << " g1V10";
        cout << " rho=" << rho << " A=" << A << " B=" << B << " AB_corr=" << AB_corr << " R=" << R << " R_err=" << R_err << " phi=" << phi << " phi_err=" << phi_err << endl;
        cout << " A_err/A=" << A_err/A << " B_err/B=" << B_err/B << " rho_err/rho=" << g1V00_rho_err[j]/rho << " R_err/R=" << R_err/R << " phi_err/phi=" << phi_err/phi << endl;
        
        
        A = g1V1m1_re[j];
        B = g1V1m1_im[j];
        rho = sqrt (A*A + B*B);
        A_err = g1V1m1_re_err[j];
        B_err = g1V1m1_im_err[j];
        AB_corr = covMatrix[6][7];
        g1V1m1_rho_err[j] = sqrt(  (A*A)*(A_err*A_err) + (B*B)*(B_err*B_err) + 2*A*B*AB_corr )/rho;
        R = B==0? 0 : A/B;
        R_err = abs(R)*sqrt( (A_err/A)*(A_err/A) + (B_err/B)*(B_err/B) -2*AB_corr/(A*B));   // error on ratio
        phi_err = R_err*(180./3.14159)/(1+R*R);     // error on ArcTan(R)
        phi = g1V1m1_phi[j];
        g1V1m1_phi_err[j] = phi_err;
        cout << endl << " g1V1m1";
        cout << " rho=" << rho << " A=" << A << " B=" << B << " AB_corr=" << AB_corr << " R=" << R << " R_err=" << R_err << " phi=" << phi << " phi_err=" << phi_err << endl;
        cout << " A_err/A=" << A_err/A << " B_err/B=" << B_err/B << " rho_err/rho=" << g1V00_rho_err[j]/rho << " R_err/R=" << R_err/R << " phi_err/phi=" << phi_err/phi << endl;
    }
    
        gr_g1V00_re = new TGraphErrors(nfiles,xparms,g1V00_re,xparms_err,g1V00_re_err);
        gr_g1V00_im = new TGraphErrors(nfiles,xparms,g1V00_im,xparms_err,g1V00_im_err);
        gr_g1V11_re = new TGraphErrors(nfiles,xparms,g1V11_re,xparms_err,g1V11_re_err);
        gr_g1V11_im = new TGraphErrors(nfiles,xparms,g1V11_im,xparms_err,g1V11_im_err);
        gr_g1V10_re = new TGraphErrors(nfiles,xparms,g1V10_re,xparms_err,g1V10_re_err);
        gr_g1V10_im = new TGraphErrors(nfiles,xparms,g1V10_im,xparms_err,g1V10_im_err);
        gr_g1V1m1_re = new TGraphErrors(nfiles,xparms,g1V1m1_re,xparms_err,g1V1m1_re_err);
        gr_g1V1m1_im = new TGraphErrors(nfiles,xparms,g1V1m1_im,xparms_err,g1V1m1_im_err);
        
        gr_g1V00_rho = new TGraphErrors(nfiles,xparms,g1V00_rho,xparms_err,g1V00_rho_err);
        gr_g1V00_phi = new TGraphErrors(nfiles,xparms,g1V00_phi,xparms_err,g1V00_phi_err);
        gr_g1V11_rho = new TGraphErrors(nfiles,xparms,g1V11_rho,xparms_err,g1V11_rho_err);
        gr_g1V11_phi = new TGraphErrors(nfiles,xparms,g1V11_phi,xparms_err,g1V11_phi_err);
        gr_g1V10_rho = new TGraphErrors(nfiles,xparms,g1V10_rho,xparms_err,g1V10_rho_err);
        gr_g1V10_phi = new TGraphErrors(nfiles,xparms,g1V10_phi,xparms_err,g1V10_phi_err);
        gr_g1V1m1_rho = new TGraphErrors(nfiles,xparms,g1V1m1_rho,xparms_err,g1V1m1_rho_err);
        gr_g1V1m1_phi = new TGraphErrors(nfiles,xparms,g1V1m1_phi,xparms_err,g1V1m1_phi_err);
        
        
        
        
        c0 = new TCanvas("c0","c0",200,10,1000,700);
        c0->Divide(3,3);
        c0->cd(1);
        gr_g1V00_re->SetTitle("g1V00_re");
        gr_g1V00_re->GetXaxis()->SetTitleSize(0.05);
        gr_g1V00_re->GetYaxis()->SetTitleSize(0.05);
        gr_g1V00_re->GetXaxis()->SetTitle("Measurement");
        gr_g1V00_re->SetMarkerStyle(20);
        gr_g1V00_re->SetMarkerColor(4);
        gr_g1V00_re->Draw("p");
        gr_g1V00_re->Draw();
        
        c0->cd(2);
        gr_g1V00_im->SetTitle("g1V00_im");
        gr_g1V00_im->GetXaxis()->SetTitleSize(0.05);
        gr_g1V00_im->GetYaxis()->SetTitleSize(0.05);
        gr_g1V00_im->GetXaxis()->SetTitle("Measurement");
        gr_g1V00_im->SetMarkerStyle(20);
        gr_g1V00_im->SetMarkerColor(4);
        gr_g1V00_im->Draw("p");
        gr_g1V00_im->Draw();
        
        c0->cd(3);
        gr_g1V11_re->SetTitle("g1V11_re");
        gr_g1V11_re->GetXaxis()->SetTitleSize(0.05);
        gr_g1V11_re->GetYaxis()->SetTitleSize(0.05);
        gr_g1V11_re->GetXaxis()->SetTitle("Measurement");
        gr_g1V11_re->SetMarkerStyle(20);
        gr_g1V11_re->SetMarkerColor(4);
        gr_g1V11_re->Draw("p");
        gr_g1V11_re->Draw();
        
        c0->cd(4);
        gr_g1V11_im->SetTitle("g1V11_im");
        gr_g1V11_im->GetXaxis()->SetTitleSize(0.05);
        gr_g1V11_im->GetYaxis()->SetTitleSize(0.05);
        gr_g1V11_im->GetXaxis()->SetTitle("Measurement");
        gr_g1V11_im->SetMarkerStyle(20);
        gr_g1V11_im->SetMarkerColor(4);
        gr_g1V11_im->Draw("p");
        gr_g1V11_im->Draw();
        
        
        c0->cd(5);
        gr_g1V10_re->SetTitle("g1V10_re");
        gr_g1V10_re->GetXaxis()->SetTitleSize(0.05);
        gr_g1V10_re->GetYaxis()->SetTitleSize(0.05);
        gr_g1V10_re->GetXaxis()->SetTitle("Measurement");
        gr_g1V10_re->SetMarkerStyle(20);
        gr_g1V10_re->SetMarkerColor(4);
        gr_g1V10_re->Draw("p");
        gr_g1V10_re->Draw();
        
        c0->cd(6);
        gr_g1V10_im->SetTitle("g1V10_im");
        gr_g1V10_im->GetXaxis()->SetTitleSize(0.05);
        gr_g1V10_im->GetYaxis()->SetTitleSize(0.05);
        gr_g1V10_im->GetXaxis()->SetTitle("Measurement");
        gr_g1V10_im->SetMarkerStyle(20);
        gr_g1V10_im->SetMarkerColor(4);
        gr_g1V10_im->Draw("p");
        gr_g1V10_im->Draw();
        
        c0->cd(7);
        gr_g1V1m1_re->SetTitle("g1V1m1_re");
        gr_g1V1m1_re->GetXaxis()->SetTitleSize(0.05);
        gr_g1V1m1_re->GetYaxis()->SetTitleSize(0.05);
        gr_g1V1m1_re->GetXaxis()->SetTitle("Measurement");
        gr_g1V1m1_re->SetMarkerStyle(20);
        gr_g1V1m1_re->SetMarkerColor(4);
        gr_g1V1m1_re->Draw("p");
        gr_g1V1m1_re->Draw();
        
        c0->cd(8);
        gr_g1V1m1_im->SetTitle("g1V1m1_im");
        gr_g1V1m1_im->GetXaxis()->SetTitleSize(0.05);
        gr_g1V1m1_im->GetYaxis()->SetTitleSize(0.05);
        gr_g1V1m1_im->GetXaxis()->SetTitle("Measurement");
        gr_g1V1m1_im->SetMarkerStyle(20);
        gr_g1V1m1_im->SetMarkerColor(4);
        gr_g1V1m1_im->Draw("p");
        gr_g1V1m1_im->Draw();
        
        c1 = new TCanvas("c1","c1",200,10,1000,700);
        c1->Divide(3,3);
        c1->cd(1);
        gr_g1V00_rho->SetTitle("g1V00_rho");
        gr_g1V00_rho->GetXaxis()->SetTitleSize(0.05);
        gr_g1V00_rho->GetYaxis()->SetTitleSize(0.05);
        gr_g1V00_rho->GetXaxis()->SetTitle("Measurement");
        gr_g1V00_rho->SetMarkerStyle(20);
        gr_g1V00_rho->SetMarkerColor(4);
        gr_g1V00_rho->Draw("p");
        gr_g1V00_rho->Draw();
        
        c1->cd(2);
        gr_g1V00_phi->SetTitle("g1V00_phi");
        gr_g1V00_phi->GetXaxis()->SetTitleSize(0.05);
        gr_g1V00_phi->GetYaxis()->SetTitleSize(0.05);
        gr_g1V00_phi->GetXaxis()->SetTitle("Measurement");
        gr_g1V00_phi->SetMarkerStyle(20);
        gr_g1V00_phi->SetMarkerColor(4);
        gr_g1V00_phi->Draw("p");
        gr_g1V00_phi->Draw();
        
        c1->cd(3);
        gr_g1V11_rho->SetTitle("g1V11_rho");
        gr_g1V11_rho->GetXaxis()->SetTitleSize(0.05);
        gr_g1V11_rho->GetYaxis()->SetTitleSize(0.05);
        gr_g1V11_rho->GetXaxis()->SetTitle("Measurement");
        gr_g1V11_rho->SetMarkerStyle(20);
        gr_g1V11_rho->SetMarkerColor(4);
        gr_g1V11_rho->Draw("p");
        gr_g1V11_rho->Draw();
        
        c1->cd(4);
        gr_g1V11_phi->SetTitle("g1V11_phi");
        gr_g1V11_phi->GetXaxis()->SetTitleSize(0.05);
        gr_g1V11_phi->GetYaxis()->SetTitleSize(0.05);
        gr_g1V11_phi->GetXaxis()->SetTitle("Measurement");
        gr_g1V11_phi->SetMarkerStyle(20);
        gr_g1V11_phi->SetMarkerColor(4);
        gr_g1V11_phi->Draw("p");
        gr_g1V11_phi->Draw();
        
        c1->cd(5);
        gr_g1V10_rho->SetTitle("g1V10_rho");
        gr_g1V10_rho->GetXaxis()->SetTitleSize(0.05);
        gr_g1V10_rho->GetYaxis()->SetTitleSize(0.05);
        gr_g1V10_rho->GetXaxis()->SetTitle("Measurement");
        gr_g1V10_rho->SetMarkerStyle(20);
        gr_g1V10_rho->SetMarkerColor(4);
        gr_g1V10_rho->Draw("p");
        gr_g1V10_rho->Draw();
        
        c1->cd(6);
        gr_g1V10_phi->SetTitle("g1V10_phi");
        gr_g1V10_phi->GetXaxis()->SetTitleSize(0.05);
        gr_g1V10_phi->GetYaxis()->SetTitleSize(0.05);
        gr_g1V10_phi->GetXaxis()->SetTitle("Measurement");
        gr_g1V10_phi->SetMarkerStyle(20);
        gr_g1V10_phi->SetMarkerColor(4);
        gr_g1V10_phi->Draw("p");
        gr_g1V10_phi->Draw();
        
        c1->cd(7);
        gr_g1V1m1_rho->SetTitle("g1V1m1_rho");
        gr_g1V1m1_rho->GetXaxis()->SetTitleSize(0.05);
        gr_g1V1m1_rho->GetYaxis()->SetTitleSize(0.05);
        gr_g1V1m1_rho->GetXaxis()->SetTitle("Measurement");
        gr_g1V1m1_rho->SetMarkerStyle(20);
        gr_g1V1m1_rho->SetMarkerColor(4);
        gr_g1V1m1_rho->Draw("p");
        gr_g1V1m1_rho->Draw();
        
        c1->cd(8);
        gr_g1V1m1_phi->SetTitle("g1V1m1_phi");
        gr_g1V1m1_phi->GetXaxis()->SetTitleSize(0.05);
        gr_g1V1m1_phi->GetYaxis()->SetTitleSize(0.05);
        gr_g1V1m1_phi->GetXaxis()->SetTitle("Measurement");
        gr_g1V1m1_phi->SetMarkerStyle(20);
        gr_g1V1m1_phi->SetMarkerColor(4);
        gr_g1V1m1_phi->Draw("p");
        gr_g1V1m1_phi->Draw();
        
    
    
    c[0]->SaveAs("fits_errors.pdf(");
    for (j=1; j<nfiles; j++) {
      c[j]->SaveAs("fits_errors.pdf");
    }
    c0->SaveAs("fits_errors.pdf");
    c1->SaveAs("fits_errors.pdf)");
}
