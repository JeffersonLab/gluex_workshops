#include "RooArgList.h"
#include "RooBreitWigner.h"
#include "LineShape_Library.h"
#include <math.h> 
#include "TMath.h"

void FitData()
{

     TCanvas * can = new TCanvas( "canvas1" );

    int numBins=22;
    double minVal=7.;
    double maxVal=11.4;
    
     TGraphErrors hyield;

    TFile *f = new TFile("FullSet.root","READ");
    //TFile f("FullSet.root") ;
    //f.cd();
     RooDataSet* loaded_RooDataSet=(RooDataSet*) gDirectory->Get("MassData");

    RooRealVar *m=new RooRealVar("m","",.3,3.,"GeV");
    
   
    RooArgList BValues;
     BValues.add(*m);

    LineShape_Library sig("sig", "sig", BValues,PiPlus,PiMinus); 
     sig.CreateComponent("rho",JBREITWIGNER,5.3,1, .757, .15,true);
     sig.CreateComponent("omega",JBREITWIGNER,5.3,1, .783, .008,false); 
    sig.CreateComponent("sigma",NONRESONANT,5.3,0, 1);


  //sig.CreateComponent("bkgd",POLYNOMIAL,2);
    RooRealVar nevents("nevt", "N_{evt}", 1e3,0,1e8);    
    nevents.setError(10.0);
     nevents.setConstant(false);
     RooAddPdf model("model", "G+poly", RooArgList(sig), RooArgList(nevents));
    const RooArgSet vars= *loaded_RooDataSet->get(0);


    for(int i=0;i<numBins;i++)
    {
    double delta=(maxVal-minVal)/numBins;
    double Emin=minVal+i*delta;
    double Emax=Emin+delta;
     char ecut[80];

    cout<<"BEGINNING FIT: "<<Emin<<" to "<<Emax<<endl;

    sprintf(ecut,"E>%f && E<%f",Emin,Emax);
     RooDataSet *subset=new RooDataSet("binned","binned",loaded_RooDataSet,vars,ecut);
  
  //subset->Print();

    RooPlot * frame = m->frame(.3,3.,200);
    subset->plotOn(frame);
    frame->Draw();
  
    model.fitTo(*subset,RooFit::Extended(),RooFit::SumW2Error(kFALSE),RooFit::NumCPU(4));

   model.plotOn(frame);
   //model.paramOn(frame);
    frame->Draw();

    double rho_yield=nevents.getVal()*sig.GetIntegral("rho");
    double omega_yield=nevents.getVal()*sig.GetIntegral("omega");
    double sigma_yield=nevents.getVal()*sig.GetIntegral("sigma");

    sig.GetSingleComponent_PDF("omega").plotOn(frame,RooFit::LineColor(kGreen),RooFit::Normalization(omega_yield,RooAbsReal::NumEvent));
    sig.GetSingleComponent_PDF("rho").plotOn(frame,RooFit::LineColor(kRed),RooFit::Normalization(rho_yield,RooAbsReal::NumEvent));
    sig.GetSingleComponent_PDF("sigma").plotOn(frame,RooFit::LineColor(kMagenta),RooFit::Normalization(sigma_yield,RooAbsReal::NumEvent));
    model.paramOn(frame);
    frame->Draw();


    TString Cutl=Form("%f",Emin);
     TString Cuth=Form("%f",Emax);
    TString File="Bin_"+Cutl.ReplaceAll(".","x")+"_"+Cuth.ReplaceAll(".","x")+".C";
  
    can->SaveAs("./Fit_out/"+File);

    double yielderr=sqrt(omega_yield+sigma_yield);

    std::cout<<"rho yield: "<<rho_yield<<endl;

     hyield.SetPoint(i,(Emin+Emax)/2,(rho_yield));
     hyield.SetPointError(i,(Emax-Emin)/2,yielderr);

    delete subset;
    }

    hyield.Draw("AP");
    hyield.SaveAs("Yield.C");
    gROOT->ProcessLine(".! Replace");

}
