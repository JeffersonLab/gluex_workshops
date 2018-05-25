#include "RooArgList.h"
#include "RooBreitWigner.h"
#include "LineShape_Library.h"
#include <math.h> 
#include "TMath.h"

double getbincontent(TH1F* AccH, int bin);
double getbinerror(TH1F* AccH, int bin);
TGraphErrors* getAcceptance(int numBins,double minVal,double maxVal,LineShape_Library sig,RooAddPdf model,RooDataSet* MC, TFile* thrownTreeF);


void FitData(TString FitTag)
{
    int numBins=22;
    double minVal=7.;
    double maxVal=11.4;

  const double Constant_term= /*binw**/1.29*pow(10,24);
  
  //TH1F*  AccH= (TH1F*) acceptance_file->Get("Accepted");

  double effArray[22]={
0.151112,
0.152873,
0.161183,
0.15471,
0.164844,
0.159575,
0.157601,
0.162276,
0.157587,
0.155876,
0.172324,
0.160799,
0.146415,
0.154183,
0.150602,
0.154085,
0.15037,
0.13963,
0.149505,
0.142958,
0.136865,
0.138488
 
};

double effArray_err[22]=
{
0.00200769,
0.00197421,
0.00211835,
0.0012137,
0.000992322,
0.000923025,
0.000801966,
0.000739633,
0.000633757,
0.00128405,
0.00156704,
0.00161985,
0.00147881,
0.00115728,
0.00110346,
0.0013743,
0.00137596,
0.00158119,
0.00158298,
0.00150899,
0.00135446,
0.00093724
};

     TCanvas * can = new TCanvas( "canvas1" );
    
     TGraphErrors hyield;
     TGraphErrors hflux;
     TGraphErrors xsec;

    TFile *f = new TFile("/w/halld-scifs17exp/halld2/home/tbritton/ANA/PipPim/Study_Fitting/RooDataSet.root","READ");
    //TFile f("FullSet.root") ;
    //f.cd();
     RooDataSet* loaded_RooDataSet=(RooDataSet*) gDirectory->Get("DATA");
     loaded_RooDataSet->Print();

     TFile *fmc = new TFile("/w/halld-scifs17exp/halld2/home/tbritton/ANA/PipPim/Study_Fitting/MCRooDataSet.root","READ");
    //TFile f("FullSet.root") ;
    //f.cd();
     RooDataSet* MC_RooDataSet=(RooDataSet*) gDirectory->Get("DATA");


     const RooArgSet Allvars= *loaded_RooDataSet->get(0);
    RooDataSet * Promptsubset=new RooDataSet("binned","binned",loaded_RooDataSet,Allvars,"weight>0 && t<-0.1 && t>-.5");

    RooDataSet * OOTsubset=new RooDataSet("binned","binned",loaded_RooDataSet,Allvars,"weight<0 && t<-0.1 && t>-.5");

    Promptsubset->Print();
    OOTsubset->Print();


    RooRealVar *m=new RooRealVar("mPipPim","",.3,3,"GeV");
    
    RooRealVar nevents("nevt", "N_{evt}", 2e3,0,1e8);    
    nevents.setError(10.0);
     nevents.setConstant(false);


    RooArgList BValues;
     BValues.add(*m);

    LineShape_Library sig("sig", "sig", &nevents, BValues,PiPlus,PiMinus); 
     sig.CreateComponent("rho",JBREITWIGNER,5.3,1, .757, .15,true);
    // sig.CreateComponent("omega",JBREITWIGNER,5.3,1, .783, .008,false); 
    sig.CreateComponent("sigma",NONRESONANT,5.3,1, 1);

    //sig.GetParameterFromComponent("rho","Mass")->setConstant(true);
    //sig.GetParameterFromComponent("rho","Width")->setConstant(true);

    /*sig.GetParameterFromComponent("sigma","Alpha")->setVal(-.2);
    sig.GetParameterFromComponent("sigma","Alpha")->setError(.02);
    sig.GetParameterFromComponent("sigma","Norm")->setVal(.8);
    sig.GetParameterFromComponent("sigma","Norm")->setError(.1);

    sig.GetParameterFromComponent("sigma","Alpha")->setConstant(true);
    sig.GetParameterFromComponent("sigma","Norm")->setConstant(true);*/
    


  //sig.CreateComponent("bkgd",POLYNOMIAL,2);
    
     RooAddPdf model("model", "G+poly", RooArgList(sig), RooArgList(nevents));
    const RooArgSet vars= *loaded_RooDataSet->get(0);

  TFile* flux_file = TFile::Open("flux_30730.root");
  TH1F*  FluxH= (TH1F*) flux_file->Get("PS_TaggedFlux");

  TFile* thrownTreeF=TFile::Open("tree_thrown_gen_2pi_amp_030730.root");
  TGraphErrors* AccH=getAcceptance(numBins,minVal,maxVal,sig,model,MC_RooDataSet, thrownTreeF);
  AccH->Draw();
  //TFile* acceptance_file = TFile::Open("workFestAccH.root");//("Edep_Acceptance_11366_v4.root");

//RUNNING OVER BINS
TFile* outFile=new TFile("Output_"+FitTag+".root","recreate");
TDirectory* BinFits=outFile->mkdir("Bin_Fits");
BinFits->cd();
    for(int i=0;i<numBins;i++)
    {
      double eff=1;
      double eff_err=0;
      AccH->GetPoint(i,eff,eff_err);
      double flux=1;
      double flux_err=0;
      flux=getbincontent(FluxH,i+1);
      flux_err=getbinerror(FluxH,i+1);

      


    double delta=(maxVal-minVal)/numBins;
    double Emin=minVal+i*delta;
    double Emax=Emin+delta;
     char ecut[80];

    cout<<"BEGINNING FIT: "<<Emin<<" to "<<Emax<<endl;

    sprintf(ecut,"E>%f && E<%f",Emin,Emax);
    RooDataSet *Promptsubsubset=new RooDataSet("pbinned","pbinned",Promptsubset,vars,ecut);
    RooDataSet *OOTsubsubset=new RooDataSet("sbbinned","sbbinned",OOTsubset,vars,ecut);
  
   //PROMPT FIT================================================================

    RooPlot * frame = m->frame(.5,1,200);
    Promptsubsubset->plotOn(frame);
    frame->Draw();
  
    model.fitTo(*Promptsubsubset,RooFit::Extended(),RooFit::SumW2Error(kFALSE),RooFit::NumCPU(4));

   model.plotOn(frame);
   //model.paramOn(frame);
    frame->Draw();

    double rho_yield=sig.GetYield("rho",.5,1.);// nevents.getVal()*sig.GetIntegral("rho",.5,1);
    double omega_yield=0;//sig.GetYield("omega",.5,1);
    double sigma_yield=sig.GetYield("sigma",.5,1.);

    //sig.GetSingleComponent_PDF("omega").plotOn(frame,RooFit::LineColor(kGreen),RooFit::Normalization(omega_yield,RooAbsReal::NumEvent));
    sig.GetSingleComponent_PDF("rho").plotOn(frame,RooFit::LineColor(kRed),RooFit::Normalization(rho_yield,RooAbsReal::NumEvent));
    sig.GetSingleComponent_PDF("sigma").plotOn(frame,RooFit::LineColor(kMagenta),RooFit::Normalization(sigma_yield,RooAbsReal::NumEvent));
    model.paramOn(frame);
    frame->Draw();


    TString Cutl=Form("%f",Emin);
     TString Cuth=Form("%f",Emax);
    TString File="Bin_"+Cutl.ReplaceAll(".","x")+"_"+Cuth.ReplaceAll(".","x")+".C";
  
    can->Write(File);
    //can->SaveAs("./Fit_out/"+File);

     //SB FIT================================================================
    RooPlot * sbframe = m->frame(.5,1.,200);
    OOTsubsubset->plotOn(sbframe);
    sbframe->Draw();
  
    double OOTweight=((RooRealVar* )OOTsubset->get(0)->find("weight"))->getVal();
    std::cout<<"OOT WEIGHT: "<<OOTweight<<endl;
    model.fitTo(*OOTsubsubset,RooFit::Extended(),RooFit::SumW2Error(kFALSE),RooFit::NumCPU(4));

    model.plotOn(sbframe);
   //model.paramOn(frame);
    sbframe->Draw();

    double sbrho_yield=sig.GetYield("rho",.5,1.);
    double sbomega_yield=0;//sig.GetYield("omega",.5,1);
    double sbsigma_yield=sig.GetYield("sigma",.5,1.);

    //sig.GetSingleComponent_PDF("omega").plotOn(sbframe,RooFit::LineColor(kGreen),RooFit::Normalization(omega_yield,RooAbsReal::NumEvent));
    sig.GetSingleComponent_PDF("rho").plotOn(sbframe,RooFit::LineColor(kRed),RooFit::Normalization(rho_yield,RooAbsReal::NumEvent));
    sig.GetSingleComponent_PDF("sigma").plotOn(sbframe,RooFit::LineColor(kMagenta),RooFit::Normalization(sigma_yield,RooAbsReal::NumEvent));
    model.paramOn(sbframe);
    sbframe->Draw();

    TString sbFile="sbBin_"+Cutl.ReplaceAll(".","x")+"_"+Cuth.ReplaceAll(".","x")+".C";
    can->Write(sbFile);
    //can->SaveAs("./Fit_out/"+sbFile);

    //CALCULATIONS==========================================================
    double yield=(rho_yield+OOTweight*sbrho_yield);
    double yielderr=sqrt(omega_yield+sigma_yield+sbomega_yield+sbsigma_yield);

    std::cout<<"sbrho yield: "<<sbrho_yield<<endl;



     hyield.SetPoint(i,(Emin+Emax)/2,(yield));
     hyield.SetPointError(i,(Emax-Emin)/2,yielderr);

    hflux.SetPoint(i,(Emin+Emax)/2,flux);
    hflux.SetPointError(i,((Emax-Emin)/2),flux_err);

      double conversion=pow(10,28)*pow(10,2);//convert from area to barns 10^28
      double denom=flux*eff;
      double denom_err=(flux*eff*sqrt(pow(flux_err/flux,2)+pow(effArray_err[i]/effArray[i],2)));

      double xsecnum=((yield)/(eff*flux*Constant_term))*conversion;

      double xsec_err=xsecnum*sqrt((pow(yielderr/yield,2)+pow(denom_err/denom,2)));
     xsec.SetPoint(i,(Emin+Emax)/2,xsecnum);
     xsec.SetPointError(i,(Emax-Emin)/2,xsec_err);

    delete Promptsubsubset;
    delete OOTsubsubset;
    }
    outFile->cd();
    hyield.SetName("Yields");
    hyield.Draw("AP");
    //hyield.SaveAs("Yield.C");
    hyield.Write();
    hflux.SetName("Flux");
    hflux.Draw("AP");
    hflux.Write();
    xsec.SetName("CrossSection");
    xsec.Draw("AP");
    //xsec.SaveAs("XSec.C");
    xsec.Write();
    Acch->SetName("Acceptance");
    AccH->Write();
    outFile->Write();
    outFile->Close();
    //gROOT->ProcessLine(".! Replace");

}

double getbincontent(TH1F* AccH, int bin)
{
  return AccH->GetBinContent(bin);
}
double getbinerror(TH1F* AccH, int bin)
{
  return AccH->GetBinError(bin);
}
TGraphErrors* getAcceptance(int numBins,double minVal,double maxVal,LineShape_Library sig, RooAddPdf model,RooDataSet* MC, TFile* thrownTreeF)
{
  
  TH1F* denom=new TH1F("denom","denom",numBins,minVal,maxVal);
  thrownTreeF->cd();
  TTree* thrownT=(TTree*) gDirectory->Get("Thrown_Tree");

  thrownT->Draw("ThrownBeam__P4->E()>>denom");

    const RooArgSet Allvars= *MC->get(0);
    RooDataSet * Promptsubset=new RooDataSet("binned","binned",MC,Allvars,"weight>0 && t<-0.1 && t>-.5");

    RooDataSet * OOTsubset=new RooDataSet("binned","binned",MC,Allvars,"weight<0 && t<-0.1 && t>-.5");

  TGraphErrors* eff = new TGraphErrors("Acceptance");
const RooArgSet vars= *MC->get(0);
   for(int i=0;i<numBins;i++)
    {
      
    double delta=(maxVal-minVal)/numBins;
    double Emin=minVal+i*delta;
    double Emax=Emin+delta;
     char ecut[80];

    cout<<"BEGINNING FIT: "<<Emin<<" to "<<Emax<<endl;

    sprintf(ecut,"E>%f && E<%f",Emin,Emax);
    RooDataSet *Promptsubsubset=new RooDataSet("pbinned","pbinned",Promptsubset,vars,ecut);
    RooDataSet *OOTsubsubset=new RooDataSet("sbbinned","sbbinned",OOTsubset,vars,ecut);
  
   //PROMPT FIT================================================================

  
    model.fitTo(*Promptsubsubset,RooFit::Extended(),RooFit::SumW2Error(kFALSE),RooFit::NumCPU(4));

    double rho_yield=sig.GetYield("rho",.5,1.);// nevents.getVal()*sig.GetIntegral("rho",.5,1);
    double omega_yield=0;//sig.GetYield("omega",.5,1);
    double sigma_yield=sig.GetYield("sigma",.5,1.);

    //sig.GetSingleComponent_PDF("omega").plotOn(frame,RooFit::LineColor(kGreen),RooFit::Normalization(omega_yield,RooAbsReal::NumEvent));


     //SB FIT================================================================
  
    double OOTweight=((RooRealVar* )OOTsubset->get(0)->find("weight"))->getVal();
    model.fitTo(*OOTsubsubset,RooFit::Extended(),RooFit::SumW2Error(kFALSE),RooFit::NumCPU(4));

    double sbrho_yield=sig.GetYield("rho",.5,1.);
    double sbomega_yield=0;//sig.GetYield("omega",.5,1);
    double sbsigma_yield=sig.GetYield("sigma",.5,1.);



    //CALCULATIONS==========================================================
    double yield=(rho_yield+OOTweight*sbrho_yield);
    double yielderr=sqrt(omega_yield+sigma_yield+sbomega_yield+sbsigma_yield);

    eff->SetPoint(i,(Emin+Emax)/2,yield/denom->GetBinContent(i+1));
    eff->SetPointError(i,(Emax-Emin)/2,0);


    delete Promptsubsubset;
    delete OOTsubsubset;
    }
    return eff;
}
