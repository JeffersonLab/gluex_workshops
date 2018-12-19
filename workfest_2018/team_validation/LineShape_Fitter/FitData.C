#include "RooArgList.h"
#include "RooBreitWigner.h"
#include "LineShape_Library.h"
#include <math.h> 
#include "TMath.h"

double getbincontent(TH1F* AccH, int bin);
double getbinerror(TH1F* AccH, int bin);
TGraphErrors* getAcceptance(TString FitTag,int numBins,double minVal,double maxVal,LineShape_Library sig,RooAddPdf model,RooDataSet* MC, TFile* thrownTreeF, TString addCuts);
std::vector<string> list_myfiles(const char *dirname, const char *ext=".root");
RooDataSet* CollectDataSets(const char* dataFilePath);

void FitData(TString FitTag, TString dataFilePath, TString mcDataFilePath, TString accDenomFilePath,TString fluxFilePath,TString addCuts)
{
    bool DoFit=true;
    int numBins=1;//22
    double minVal=3.5;
    double maxVal=3.6;//11.4

  const double Constant_term= /*binw**/1.29*pow(10,24);
  
  //TH1F*  AccH= (TH1F*) acceptance_file->Get("Accepted");     
    
     TGraphErrors hyield;
     TGraphErrors hflux;
     TGraphErrors xsec;
     TGraphErrors hfluxnormyields;
     TGraphErrors* AccH;
     TH1F* mass_hist= new TH1F("Mass_hist","Mass_hist",100,.5,1);
 

   // TFile *f = new TFile(dataFilePath,"READ");
    //TFile f("FullSet.root") ;
    //f.cd();
     RooDataSet* loaded_RooDataSet=CollectDataSets(dataFilePath);//(RooDataSet*) gDirectory->Get("DATA");
     loaded_RooDataSet->Print();

    bool loaded_ACC=false;

    TFile* AcceptanceF = new TFile(FitTag+"_Acceptance.root","READ");
    
    if(AcceptanceF)
    {
      AcceptanceF->cd();
      gDirectory->ls();
      AccH=(TGraphErrors*) gDirectory->Get("Acceptance");
      cout<<"LOADED AccH ADDRESS: "<<AccH<<endl;
      if(AccH)
      {
        loaded_ACC=true;
      }
    }

     const RooArgSet Allvars= *loaded_RooDataSet->get(0);
    RooDataSet * Promptsubset=new RooDataSet("binned","binned",loaded_RooDataSet,Allvars,"weight>0"+addCuts/* && t<-0.1 && t>-.5"*/);

    RooDataSet * OOTsubset=new RooDataSet("binned","binned",loaded_RooDataSet,Allvars,"weight<0"+addCuts/* && t<-0.1 && t>-.5"*/);

    Promptsubset->Print();
    OOTsubset->Print();


    RooRealVar *m=new RooRealVar("mPipPim","",.5,1,"GeV");
    
    RooRealVar nevents("nevt", "N_{evt}", 2e5,0,1e8);    
    nevents.setError(10.0);
     nevents.setConstant(false);


    RooArgList BValues;
     BValues.add(*m);

     LineShape_Library sig("sig", "sig", &nevents, BValues,PiPlus,PiMinus); 
     sig.CreateComponent("rho",JBREITWIGNER,5.3,1, .757, .15,true);
     sig.CreateComponent("omega",JBREITWIGNER,5.3,1, .783, .0085,false); 
     //sig.CreateComponent("sigma",NONRESONANTG,1,1,-1,false);
     //sig.CreateComponent("sigma",POLYNOMIAL,10,false);

    //sig.GetParameterFromComponent("rho","Norm")->setVal(.1);
    //sig.GetParameterFromComponent("omega","Norm")->setConstant(true);
    //sig.GetParameterFromComponent("omega","RelativePhase")->setVal(0);
    
    //sig.GetParameterFromComponent("omega","Norm")->setError(.00001);
    
    //sig.GetParameterFromComponent("omega","Mass")->setConstant(true);
    //sig.GetParameterFromComponent("omega","Width")->setConstant(true);
    sig.GetParameterFromComponent("rho","Mass")->setConstant(true);
    sig.GetParameterFromComponent("rho","Width")->setConstant(true);


    /*sig.GetParameterFromComponent("sigma","c1")->setVal(0);
    sig.GetParameterFromComponent("sigma","c1")->setError(.1);
    sig.GetParameterFromComponent("sigma","c2")->setVal(0);
    sig.GetParameterFromComponent("sigma","c2")->setError(.1);*/

    //sig.GetParameterFromComponent("sigma","Norm")->setVal(0);
    //sig.GetParameterFromComponent("sigma","Norm")->setError(.1);

    //sig.GetParameterFromComponent("sigma","Alpha")->setVal(0);
    //sig.GetParameterFromComponent("sigma","Alpha")->setError(.02);
    //sig.GetParameterFromComponent("sigma","Norm")->setVal(0);
    //sig.GetParameterFromComponent("sigma","Norm")->setError(.1);

    //sig.GetParameterFromComponent("sigma","Alpha")->setConstant(true);
    //sig.GetParameterFromComponent("sigma","Norm")->setConstant(true);
    //sig.GetParameterFromComponent("sigma","Center")->setConstant(true);
    


  //sig.CreateComponent("bkgd",POLYNOMIAL,2);
    
     RooAddPdf model("model", "G+poly", RooArgList(sig), RooArgList(nevents));
    const RooArgSet vars= *loaded_RooDataSet->get(0);

  TFile* flux_file = TFile::Open(fluxFilePath);
  TH1F*  FluxH= (TH1F*) flux_file->Get("tagged_flux");

std::cout<<"IS LOADED? "<<loaded_ACC<<endl;
if(!loaded_ACC)
{//"/w/halld-scifs17exp/halld2/home/tbritton/ANA/PipPim/Study_Fitting/MCRooDataSet.root"
  //TFile *fmc = new TFile(mcDataFilePath,"READ");
     RooDataSet* MC_RooDataSet=CollectDataSets(mcDataFilePath);//(RooDataSet*) gDirectory->Get("DATA");
     
    //  /"tree_thrown_gen_2pi_amp_030730.root"
       TFile* thrownTreeF=TFile::Open(accDenomFilePath);
       cout<<"Acceptance address: "<<AccH<<endl;
  AccH=getAcceptance(FitTag,numBins,minVal,maxVal,sig,model,MC_RooDataSet, thrownTreeF, addCuts);
  cout<<"Acceptance address: "<<AccH<<endl;
}
  //TFile* acceptance_file = TFile::Open("workFestAccH.root");//("Edep_Acceptance_11366_v4.root");

//RUNNING OVER BINS
TCanvas * can = new TCanvas( "canvas1" );
TFile* outFile=new TFile("Output_"+FitTag+".root","recreate");
TDirectory* BinFits=outFile->mkdir("Bin_Fits");
BinFits->cd();
    for(int i=0;i<numBins;i++)
    {
      double effnum=1;
      double binx=1;
      cout<<"AccH ADRESS: "<<AccH<<endl;
      AccH->GetPoint(i,binx,effnum);
      double eff_err=AccH->GetErrorY(i);
      double flux=1;
      double flux_err=0;
      cout<<"FLUX ADDRESS: "<<FluxH<<endl;
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
  

    for(int i=0;i<Promptsubsubset->sumEntries();i++)
    {
      const RooArgSet* iargs= Promptsubsubset->get(i);
      mass_hist->Fill(iargs->getRealValue("mPipPim"));
    }

    if(DoFit)
    {
      m->setVal(Promptsubsubset->sumEntries());
      model.fitTo(*Promptsubsubset,RooFit::Extended(),RooFit::SumW2Error(kFALSE),RooFit::NumCPU(4));
    }
    model.plotOn(frame,RooFit::LineColor(kBlue),RooFit::Normalization(nevents.getVal(),RooAbsReal::NumEvent));

    frame->Draw();

cout<<"TOTAL: "<<Promptsubsubset->sumEntries()<<endl;  

    cout<<"GETTING YIELDS"<<endl;
    double rho_yield=sig.GetYield("rho",.5,1.);//Promptsubsubset->sumEntries();// nevents.getVal()*sig.GetIntegral("rho",.5,1);
    double omega_yield=sig.GetYield("omega",.5,1);
    double sigma_yield=sig.GetYield("sigma",.5,1.);

    if(omega_yield>rho_yield)
    {
      double temp=rho_yield;
      rho_yield=omega_yield;
      omega_yield=temp;
      
    }


   // RooAddPdf rhomodel("rhomodel", "", RooArgList(*rho_pdf), RooArgList(rhovar));
    //sig.GetSingleComponent_PDF("omega").plotOn(frame,RooFit::LineColor(kGreen),RooFit::Normalization(omega_yield,RooAbsReal::NumEvent));
    sig.PlotComponent("rho",frame,kRed,rho_yield);
    sig.PlotComponent("omega",frame,kGreen,omega_yield);
    sig.PlotComponent("sigma",frame,kMagenta,sigma_yield);


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
    if(DoFit)
    {
    m->setVal(OOTsubsubset->sumEntries());
    model.fitTo(*OOTsubsubset,RooFit::Extended(),RooFit::SumW2Error(kFALSE),RooFit::NumCPU(4));
    }

    model.plotOn(sbframe,RooFit::LineColor(kBlue),RooFit::Normalization(nevents.getVal(),RooAbsReal::NumEvent));
   //model.paramOn(frame);
    sbframe->Draw();
    cout<<"TOTAL: "<<OOTsubsubset->sumEntries()<<endl;
    double sbrho_yield=sig.GetYield("rho",.5,1.);//OOTsubsubset->sumEntries();//
    double sbomega_yield=sig.GetYield("omega",.5,1);
    double sbsigma_yield=sig.GetYield("sigma",.5,1.);

    if(sbomega_yield>sbrho_yield)
    {
      double temp=sbrho_yield;
      sbrho_yield=sbomega_yield;
      sbomega_yield=temp;
      
    }

    sig.PlotComponent("rho",sbframe,kRed,sbrho_yield);
    sig.PlotComponent("omega",sbframe,kGreen,sbomega_yield);
    sig.PlotComponent("sigma",sbframe,kMagenta,sbsigma_yield);
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

    hfluxnormyields.SetPoint(i,(Emin+Emax)/2,yield/flux);
    hfluxnormyields.SetPointError(i,((Emax-Emin)/2),abs(yield/flux)*sqrt(pow(yielderr/yield,2)+pow(flux_err/flux,2)));

      double conversion=pow(10,28)*pow(10,2);//convert from area to barns 10^28
      double denom=flux*effnum;
      double denom_err=(denom*sqrt(pow(flux_err/flux,2)+pow(eff_err/effnum,2)));

      double xsecnum=((yield)/(denom*Constant_term))*conversion;

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
    hfluxnormyields.SetName("Flux normalized yields");
    hfluxnormyields.Draw("AP");
    hfluxnormyields.Write();
    xsec.SetName("CrossSection");
    xsec.Draw("AP");
    //xsec.SaveAs("XSec.C");
    xsec.Write();
    AccH->SetName("Acceptance");
    AccH->Write();
    mass_hist->SetName("Mass_Hist");
    mass_hist->Write();
    outFile->Write();
    outFile->Close();
    //gROOT->ProcessLine(".! Replace");

}

double getbincontent(TH1F* hist, int bin)
{
  return hist->GetBinContent(bin);
}
double getbinerror(TH1F* AccH, int bin)
{
  return AccH->GetBinError(bin);
}
TGraphErrors* getAcceptance(TString FitTag,int numBins,double minVal,double maxVal,LineShape_Library sig, RooAddPdf model,RooDataSet* MC, TFile* thrownTreeF, TString addCuts)
{
  sig.GetParameterFromComponent("omega","Norm")->setVal(0);
  sig.GetParameterFromComponent("omega","Norm")->setConstant(true);
  sig.GetParameterFromComponent("omega","Mass")->setVal(0);
  sig.GetParameterFromComponent("omega","Mass")->setConstant(true);
  sig.GetParameterFromComponent("omega","RelativePhase")->setVal(0);
  sig.GetParameterFromComponent("omega","RelativePhase")->setConstant(true);
  sig.GetParameterFromComponent("omega","Width")->setVal(0);
  sig.GetParameterFromComponent("omega","Width")->setConstant(true);

  TH1F* denom=new TH1F("denom","denom",numBins,minVal,maxVal);
  thrownTreeF->cd();
  TTree* thrownT=(TTree*) gDirectory->Get("Thrown_Tree");

  thrownT->Draw("ThrownBeam__P4->E()>>denom");

    const RooArgSet Allvars= *MC->get(0);
    RooDataSet * Promptsubset=new RooDataSet("binned","binned",MC,Allvars,"weight>0"+addCuts/* && t<-0.1 && t>-.5"*/);

    RooDataSet * OOTsubset=new RooDataSet("binned","binned",MC,Allvars,"weight<0"+addCuts/* && t<-0.1 && t>-.5"*/);

  TGraphErrors* eff = new TGraphErrors("Acceptance");
  const RooArgSet vars= *MC->get(0);
  RooRealVar *MCm=new RooRealVar("mPipPim","",.3,3,"GeV");
  TFile* newacc = new TFile(FitTag+"_Acceptance.root","recreate");
  TDirectory* MCBinFits=newacc->mkdir("MCBin_Fits");
  MCBinFits->cd();
  TCanvas * MCcan = new TCanvas( "MCcanvas1" );
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

  if(omega_yield>rho_yield)
    {
      double temp=rho_yield;
      rho_yield=omega_yield;
      omega_yield=temp;
      
    }

    TString MCCutl=Form("%f",Emin);
    TString MCCuth=Form("%f",Emax);
    TString MCFile="MCBin_"+MCCutl.ReplaceAll(".","x")+"_"+MCCuth.ReplaceAll(".","x")+".C";
  
    RooPlot * MCframe = MCm->frame(.5,1,200);
    Promptsubsubset->plotOn(MCframe);
    model.plotOn(MCframe,RooFit::LineColor(kBlue),RooFit::Normalization(sig.GetTotalNorm(),RooAbsReal::NumEvent));
    model.paramOn(MCframe);
    sig.PlotComponent("rho",MCframe,kRed,rho_yield);
    sig.PlotComponent("omega",MCframe,kGreen,omega_yield);
    sig.PlotComponent("sigma",MCframe,kMagenta,sigma_yield);
    //sig.GetSingleComponent_PDF("omega").plotOn(frame,RooFit::LineColor(kGreen),RooFit::Normalization(omega_yield,RooAbsReal::NumEvent));
    MCframe->Draw();

    MCcan->Write(MCFile);

     //SB FIT================================================================
  
    double OOTweight=((RooRealVar* )OOTsubset->get(0)->find("weight"))->getVal();
    model.fitTo(*OOTsubsubset,RooFit::Extended(),RooFit::SumW2Error(kFALSE),RooFit::NumCPU(4));

    double sbrho_yield=sig.GetYield("rho",.5,1.);
    double sbomega_yield=sig.GetYield("omega",.5,1);
    double sbsigma_yield=sig.GetYield("sigma",.5,1.);

  if(sbomega_yield>sbrho_yield)
    {
      double temp=sbrho_yield;
      sbrho_yield=sbomega_yield;
      sbomega_yield=temp;
      
    }

    TString MCsbCutl=Form("%f",Emin);
    TString MCsbCuth=Form("%f",Emax);
    TString MCsbFile="MCsbBin_"+MCsbCutl.ReplaceAll(".","x")+"_"+MCsbCuth.ReplaceAll(".","x")+".C";
  
    RooPlot * MCsbframe = MCm->frame(.5,1,200);
    OOTsubsubset->plotOn(MCsbframe);
    model.plotOn(MCsbframe,RooFit::LineColor(kBlue),RooFit::Normalization(sig.GetTotalNorm(),RooAbsReal::NumEvent));
    model.paramOn(MCsbframe);
    sig.PlotComponent("rho",MCsbframe,kRed,sbrho_yield);
    sig.PlotComponent("omega",MCsbframe,kGreen,sbomega_yield);
    sig.PlotComponent("sigma",MCsbframe,kMagenta,sbsigma_yield);
    MCsbframe->Draw();

    MCcan->Write(MCsbFile);


    //CALCULATIONS==========================================================
    double yield=(rho_yield+OOTweight*sbrho_yield);
    double yielderr=sqrt(omega_yield+sigma_yield+sbomega_yield+sbsigma_yield);
    double denomnum=denom->GetBinContent(i+1);
    eff->SetPoint(i,(Emin+Emax)/2,yield/denomnum);
    eff->SetPointError(i,(Emax-Emin)/2,(yield/denomnum)*sqrt(pow((yielderr/yield),2)+(1.0/(sqrt(denomnum)*denomnum))));

    delete Promptsubsubset;
    delete OOTsubsubset;
    }
    eff->SetName("Acceptance");
    newacc->cd();
    eff->Write();
    newacc->Close();
    std::cout<<"eff address: "<<eff<<endl;
    return eff;
}
std::vector<string> list_myfiles(const char *dirname, const char *ext=".root")
{
    std::vector<string> Files;
    TSystemDirectory dir(dirname, dirname);
     TList *files = dir.GetListOfFiles();
     if (files)
     {
         TSystemFile *file;
         TString fname;
          TIter next(files);
           while ((file=(TSystemFile*)next()))
           {
               fname = file->GetName();
               if (!file->IsDirectory() && fname.EndsWith(ext))
                {
                    //cout << fname.Data() << endl;
                    Files.push_back(fname.Data());
                }
            }
        }
        return Files;
}
RooDataSet* CollectDataSets(const char* dataPath){
  std::vector<string> ourFiles=list_myfiles(dataPath,".root");
    RooDataSet* FullSet = new RooDataSet();
    for(int i=0;i<ourFiles.size();i++)
    {
      TString Filename=dataPath+TString(ourFiles[i]);
      TFile f(Filename);
      f.cd();
      RooDataSet* fileSet = (RooDataSet*) gDirectory->Get("DATA");
      if(i==0)
      {
        FullSet=fileSet;
      }
      else
      {
        if(fileSet)
          {
            FullSet->append(*fileSet);
          }
      }
    }
    return FullSet;
}
