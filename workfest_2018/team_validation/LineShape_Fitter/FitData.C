#include "RooArgList.h"
#include "RooBreitWigner.h"
#include "LineShape_Library.h"
#include <math.h> 
#include "TMath.h"

double getbincontent(TH1F* AccH, int bin);

void FitData()
{
  const double Constant_term= /*binw**/1.29*pow(10,24);
  
  TFile* flux_file = TFile::Open("flux_30730.root");
  TH1F*  FluxH= (TH1F*) flux_file->Get("PS_TaggedFlux");

  TFile* acceptance_file = TFile::Open("workFestAccH.root");//("Edep_Acceptance_11366_v4.root");
  //TH1F*  AccH= (TH1F*) acceptance_file->Get("Accepted");

     TCanvas * can = new TCanvas( "canvas1" );

    int numBins=22;
    double minVal=7.;
    double maxVal=11.4;
    
     TGraphErrors hyield;
     TGraphErrors xsec;

    TFile *f = new TFile("FullSet.root","READ");
    //TFile f("FullSet.root") ;
    //f.cd();
     RooDataSet* loaded_RooDataSet=(RooDataSet*) gDirectory->Get("MassData");
     loaded_RooDataSet->Print();


     const RooArgSet Allvars= *loaded_RooDataSet->get(0);
    RooDataSet * Promptsubset=new RooDataSet("binned","binned",loaded_RooDataSet,Allvars,"weight>0");

    RooDataSet * OOTsubset=new RooDataSet("binned","binned",loaded_RooDataSet,Allvars,"weight<0");

    Promptsubset->Print();
    OOTsubset->Print();


    RooRealVar *m=new RooRealVar("m","",.3,3.,"GeV");
    
    RooRealVar nevents("nevt", "N_{evt}", 1e3,0,1e8);    
    nevents.setError(10.0);
     nevents.setConstant(false);


    RooArgList BValues;
     BValues.add(*m);

    LineShape_Library sig("sig", "sig", &nevents, BValues,PiPlus,PiMinus); 
     sig.CreateComponent("rho",JBREITWIGNER,5.3,1, .757, .15,true);
     sig.CreateComponent("omega",JBREITWIGNER,5.3,1, .783, .008,false); 
    sig.CreateComponent("sigma",NONRESONANT,5.3,1, 1);


  //sig.CreateComponent("bkgd",POLYNOMIAL,2);
    
     RooAddPdf model("model", "G+poly", RooArgList(sig), RooArgList(nevents));
    const RooArgSet vars= *loaded_RooDataSet->get(0);

//RUNNING OVER BINS
    for(int i=0;i<numBins;i++)
    {
      double eff=1;
      //eff=getbincontent(AccH,i+1)
      double flux=1;
      flux=getbincontent(FluxH,i+1);


    double delta=(maxVal-minVal)/numBins;
    double Emin=minVal+i*delta;
    double Emax=Emin+delta;
     char ecut[80];

    cout<<"BEGINNING FIT: "<<Emin<<" to "<<Emax<<endl;

    sprintf(ecut,"E>%f && E<%f",Emin,Emax);
    RooDataSet *Promptsubsubset=new RooDataSet("pbinned","pbinned",Promptsubset,vars,ecut);
    RooDataSet *OOTsubsubset=new RooDataSet("sbbinned","sbbinned",OOTsubset,vars,ecut);
  
   //PROMPT FIT================================================================

    RooPlot * frame = m->frame(.3,3.,200);
    Promptsubsubset->plotOn(frame);
    frame->Draw();
  
    model.fitTo(*Promptsubsubset,RooFit::Extended(),RooFit::SumW2Error(kFALSE),RooFit::NumCPU(4));

   model.plotOn(frame);
   //model.paramOn(frame);
    frame->Draw();

    double rho_yield=sig.GetYield("rho",.5,1);// nevents.getVal()*sig.GetIntegral("rho",.5,1);
    double omega_yield=0;//nevents.getVal()*sig.GetIntegral("omega",.5,1.1);
    double sigma_yield=sig.GetYield("sigma",.5,1);

   // sig.GetSingleComponent_PDF("omega").plotOn(frame,RooFit::LineColor(kGreen),RooFit::Normalization(omega_yield,RooAbsReal::NumEvent));
    sig.GetSingleComponent_PDF("rho").plotOn(frame,RooFit::LineColor(kRed),RooFit::Normalization(rho_yield,RooAbsReal::NumEvent));
    sig.GetSingleComponent_PDF("sigma").plotOn(frame,RooFit::LineColor(kMagenta),RooFit::Normalization(sigma_yield,RooAbsReal::NumEvent));
    model.paramOn(frame);
    frame->Draw();


    TString Cutl=Form("%f",Emin);
     TString Cuth=Form("%f",Emax);
    TString File="Bin_"+Cutl.ReplaceAll(".","x")+"_"+Cuth.ReplaceAll(".","x")+".C";
  
    can->SaveAs("./Fit_out/"+File);

     //SB FIT================================================================
    RooPlot * sbframe = m->frame(.3,3.,200);
    OOTsubsubset->plotOn(sbframe);
    sbframe->Draw();
  
    double OOTweight=((RooRealVar* )OOTsubset->get(0)->find("weight"))->getVal();
    std::cout<<"OOT WEIGHT: "<<OOTweight<<endl;
    model.fitTo(*OOTsubsubset,RooFit::Extended(),RooFit::SumW2Error(kFALSE),RooFit::NumCPU(4));

    model.plotOn(sbframe);
   //model.paramOn(frame);
    sbframe->Draw();

    double sbrho_yield=sig.GetYield("rho",.5,1);
    double sbomega_yield=0;//nevents.getVal()*sig.GetIntegral("omega");
    double sbsigma_yield=sig.GetYield("sigma",.5,1);

   // sig.GetSingleComponent_PDF("omega").plotOn(frame,RooFit::LineColor(kGreen),RooFit::Normalization(omega_yield,RooAbsReal::NumEvent));
    sig.GetSingleComponent_PDF("rho").plotOn(sbframe,RooFit::LineColor(kRed),RooFit::Normalization(rho_yield,RooAbsReal::NumEvent));
    sig.GetSingleComponent_PDF("sigma").plotOn(sbframe,RooFit::LineColor(kMagenta),RooFit::Normalization(sigma_yield,RooAbsReal::NumEvent));
    model.paramOn(sbframe);
    sbframe->Draw();

    TString sbFile="sbBin_"+Cutl.ReplaceAll(".","x")+"_"+Cuth.ReplaceAll(".","x")+".C";
  
    can->SaveAs("./Fit_out/"+sbFile);

    //CALCULATIONS==========================================================
    double yielderr=sqrt(omega_yield+sigma_yield+sbomega_yield+sbsigma_yield);

    std::cout<<"sbrho yield: "<<sbrho_yield<<endl;

     hyield.SetPoint(i,(Emin+Emax)/2,(rho_yield+OOTweight*sbrho_yield));
     hyield.SetPointError(i,(Emax-Emin)/2,yielderr);

      double conversion=pow(10,28)*pow(10,6);//convert from area to barns 10^28
     xsec.SetPoint(i,(Emin+Emax)/2,((rho_yield+OOTweight*sbrho_yield)/(eff*flux*Constant_term))*conversion);
     xsec.SetPointError(i,(Emax-Emin)/2,yielderr);

    delete Promptsubsubset;
    delete OOTsubsubset;
    }

    hyield.Draw("AP");
    hyield.SaveAs("Yield.C");
    xsec.Draw("AP");
    xsec.SaveAs("XSec.C");
    gROOT->ProcessLine(".! Replace");

}

double getbincontent(TH1F* AccH, int bin)
{
  return AccH->GetBinContent(bin);
}
