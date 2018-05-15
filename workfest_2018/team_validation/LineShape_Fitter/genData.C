//TEST DRIVING PROGRAM FOR THE FITTER
#include "RooArgList.h"
#include "RooBreitWigner.h"
#include "LineShape_Library.h"
#include <math.h> 
#include "TMath.h"

const double m0_pi=.13957061;
const double m0_rho=.77526;

void genData(){

  //RooMsgService::instance().setGlobalKillBelow(RooFit::ERROR);
  TRandom3 randnum(0);
  int num=10000;
  double rangeMax=1.7;
  double rangeMin=.3;

  double A=3;
  double B=5;
  double g=2;
  double val;
  
  RooRealVar x("x","mass",rangeMin,rangeMax,"GeV");
  RooRealVar y("y","",0,1000000);
  RooDataSet DATA("test_data","test_data",RooArgSet(x)); 


  RooArgList BValues;
  BValues.add(x);
 //tt->SetBranchAddress("PipPim_FM_combos", &KpKm_Mass_combos, &b_KpKm_Mass_combos);


    //RooRealVar KpKm("KpKm_MM_combos","KK_mass",KpKm_Mass_combos[combo]);
    //RooDataSet* data_KKMass = new RooDataSet("KK_mass","KK_mass",RooArgSet(*KKMass,*cut_binning,*Weight,*KinFit_CL));   
  int count=0;

  LineShape_Library testsig("testsig", "testsig", BValues,PiPlus,PiMinus);


  testsig.CreateComponent("rho",BREITWIGNER,5.3,1, .77, .15,true);
  //sig.CreateComponent("omega",POLYNOMIAL,2,false);
  testsig.CreateComponent("omega",JBREITWIGNER,5.3,1, 1.1, .2,false);
  
  testsig.GetParameterFromComponent("omega","omega_Mass")->setVal(1.3);
  testsig.GetParameterFromComponent("omega","omega_Mass")->setConstant(true);

  testsig.GetParameterFromComponent("omega","omega_Norm")->setVal(5);
  testsig.GetParameterFromComponent("omega","omega_Norm")->setConstant(true);

  testsig.GetParameterFromComponent("omega","omega_Width")->setVal(.5);
  testsig.GetParameterFromComponent("omega","omega_Width")->setConstant(true);

  testsig.GetParameterFromComponent("omega","omega_RelativePhase")->setVal(TMath::Pi());
  testsig.GetParameterFromComponent("omega","omega_RelativePhase")->setConstant(true);

  testsig.GetParameterFromComponent("rho","rho_Mass")->setVal(.77);
  testsig.GetParameterFromComponent("rho","rho_Mass")->setConstant(true);

  testsig.GetParameterFromComponent("rho","rho_Width")->setVal(.15);
  testsig.GetParameterFromComponent("rho","rho_Width")->setConstant(true);



  //for(int i=1;i<=num;i++)
  while(count<num)
    {
      val=randnum.Uniform(rangeMin,rangeMax);
      //y.setVal(g+A*val+B*val*val);
      x=val;
      //std::cout<<"Rolled: "<<val<<std::endl;

      //cout<<"Value of BW: "<<testsig.R(val, m0_rho, gamma0, q, q0, 1, size).Rho()<<std::endl;
      //y.setVal(0);
      //y.setVal(testsig.R(val, fitted_mass, fitted_width, q, q0, A_Particle_L->getVal(), A_Particle_Size->getVal()).Rho2());
      
      double interfere=testsig.GetSumOfShapesSquared(val)-testsig.GetSingleComponent_PDF("rho").GetSumOfShapesSquared(val)-testsig.GetSingleComponent_PDF("omega").GetSumOfShapesSquared(val);
      //cout<<interfere<<endl;

      //cout<<testsig.GetSumOfShapesSquared(val)<<endl;
      y.setVal(testsig.GetSumOfShapesSquared(val));
      //y.setVal(interfere);

      double acc=randnum.Uniform(0,10000);
      //std::cout<<y.getVal()<<std::endl;
        if(acc>y.getVal() )
          continue;

        //std::cout<<"YAY"<<std::endl;

        count++;

      //accept/reject

      //hist->Fill(val,y);
      //hist->SetPoint(i,val,y);
        DATA.add(RooArgSet(x));
    }
//================================================================================================================================================================
//std::vector<RooArgList*> BRes;
  //BRes.push_back(B_Particle);
  //BRes.push_back(B2_Particle);
  int bins=100;
  TH1F* hist = new TH1F("int","int",bins,rangeMin,rangeMax);

  double deltaBin=(rangeMax-rangeMin)/bins;
  for(int i=0;i<bins;i++)
  {
    double nextVal=rangeMin+i*deltaBin;
    double interfere=testsig.GetSumOfShapesSquared(nextVal)-testsig.GetSingleComponent_PDF("rho").GetSumOfShapesSquared(nextVal)-testsig.GetSingleComponent_PDF("omega").GetSumOfShapesSquared(nextVal);
    hist->Fill(nextVal,interfere);
  }

  hist->SetMarkerSize(5);
  hist->Draw("HISTP");
  
  hist->SaveAs("inputInterference.C");

LineShape_Library sig("sig", "sig", BValues,PiPlus,PiMinus);
  
  sig.CreateComponent("rho",BREITWIGNER,5.3,1, .77, .15,true);
  //sig.CreateComponent("omega",POLYNOMIAL,2,false);
  sig.CreateComponent("omega",JBREITWIGNER,5.3,1, 1.1, .2,false);
  
  sig.GetParameterFromComponent("omega","omega_Mass")->setVal(1.2);
  sig.GetParameterFromComponent("omega","omega_Mass")->setConstant(true);

  sig.GetParameterFromComponent("omega","omega_Norm")->setVal(5);
  //sig.GetParameterFromComponent("omega","omega_Norm")->setConstant(true);

  sig.GetParameterFromComponent("omega","omega_Width")->setVal(.3);
  sig.GetParameterFromComponent("omega","omega_Width")->setConstant(true);

  sig.GetParameterFromComponent("rho","rho_Mass")->setVal(.77);
  //sig.GetParameterFromComponent("rho","rho_Mass")->setConstant(true);

  sig.GetParameterFromComponent("rho","rho_Width")->setVal(.15);
  //sig.GetParameterFromComponent("rho","rho_Width")->setConstant(true);


  RooRealVar nevents("nevt", "N_{evt}", 1000,0,100000);    
  nevents.setError(10.0);
  nevents.setConstant(false);

  RooAddPdf model_new("model_new", "G+poly", RooArgList(sig), RooArgList(nevents));

  model_new.fitTo(DATA,RooFit::Extended(),RooFit::SumW2Error(kFALSE));
  double omega_yield=nevents.getVal()*sig.GetIntegral("omega");
  double rho_yield=nevents.getVal()*sig.GetIntegral("rho");
  std::cout<<"omega yield: "<<omega_yield<<endl;
  std::cout<<"rho yield: "<<rho_yield<<endl;
  std::cout<<"sum: "<<omega_yield+rho_yield<<endl;
  std::cout<<"ratio: "<<rho_yield/omega_yield<<endl;

  RooPlot * frame_KKMass = x.frame(rangeMin,rangeMax,100);
  DATA.plotOn(frame_KKMass); 
  (sig).plotOn(frame_KKMass,RooFit::LineColor(kBlue));


  /*sig.GetSingleComponent_PDF("omega").plotOn(frame_KKMass,RooFit::LineColor(kGreen),RooFit::Normalization(sig.GetIntegral("omega"),RooAbsReal::Relative));
  sig.GetSingleComponent_PDF("rho").plotOn(frame_KKMass,RooFit::LineColor(kRed),RooFit::Normalization(sig.GetIntegral("rho"),RooAbsReal::Relative));*/
  
  sig.GetSingleComponent_PDF("omega").plotOn(frame_KKMass,RooFit::LineColor(kGreen),RooFit::Normalization(omega_yield,RooAbsReal::NumEvent));
  sig.GetSingleComponent_PDF("rho").plotOn(frame_KKMass,RooFit::LineColor(kRed),RooFit::Normalization(rho_yield,RooAbsReal::NumEvent));
  
  frame_KKMass->Draw();
  frame_KKMass->SaveAs("MassFit.C");
 // hist->Draw("AP");
}
