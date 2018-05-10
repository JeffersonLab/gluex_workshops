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
  
  RooRealVar x("x","",rangeMin,rangeMax,"GeV");
  RooRealVar y("y","",0,1000000);
  RooDataSet DATA("test_data","test_data",RooArgSet(x)); 

 //tt->SetBranchAddress("PipPim_FM_combos", &KpKm_Mass_combos, &b_KpKm_Mass_combos);


    //RooRealVar KpKm("KpKm_MM_combos","KK_mass",KpKm_Mass_combos[combo]);
    //RooDataSet* data_KKMass = new RooDataSet("KK_mass","KK_mass",RooArgSet(*KKMass,*cut_binning,*Weight,*KinFit_CL));   
  int count=0;

  RooRealVar* A_Particle_Norm=new RooRealVar("_Norm","_Norm",1);
  RooRealVar* A_Particle_Shape=new RooRealVar("_Shape","_Shape",BREITWIGNER);
  RooRealVar* A_Particle_Size=new RooRealVar("_Size","Size",5.3);
  RooRealVar* A_Particle_L=new RooRealVar("_L","L",1);
 
  RooRealVar mass("m", "m", .77,rangeMin,rangeMax);//from example fit?
  mass.setError(.1);
  //mass.setConstant(true);

  RooRealVar width("width", "width", .15,0,10);//from example fit?
  width.setError(1);
  //width.setConstant(true);

  RooRealVar phase1("phase1", "phase1",0,-TMath::Pi()/2,TMath::Pi());//from example fit?
  phase1.setError(1);
  phase1.setConstant(true);



RooArgList* A_Particle=new RooArgList(*A_Particle_Shape,*A_Particle_Norm,*A_Particle_Size,*A_Particle_L);
  A_Particle->add(mass);
  A_Particle->add(width);
  A_Particle->add(phase1);

//---------------------------------------------------------------------------------------------------------
  RooRealVar* A2_Particle_Norm=new RooRealVar("_Norm","_Norm",1);
  RooRealVar* A2_Particle_Shape=new RooRealVar("_Shape","_Shape",BREITWIGNER);
  RooRealVar* A2_Particle_Size=new RooRealVar("_Size","Size",5.3);
  RooRealVar* A2_Particle_L=new RooRealVar("_L","L",1);
 
  RooRealVar mass2("m", "m", 1.1,rangeMin,rangeMax);//from example fit?
  mass.setError(.1);
  //mass.setConstant(true);

  RooRealVar width2("width", "width", .2,0,10);//from example fit?
  width.setError(1);

  RooRealVar phase2("phase2", "phase2", TMath::Pi(),-TMath::Pi(),TMath::Pi());//from example fit?
  width.setError(1);

  RooArgList* A2_Particle=new RooArgList(*A2_Particle_Shape,*A2_Particle_Norm,*A2_Particle_Size,*A2_Particle_L);
  A2_Particle->add(mass2);
  A2_Particle->add(width2);
  A2_Particle->add(phase2);
  //A2_Particle->add(para1);
//----------------------------------------------------------------------------------------------------------

  RooArgList Values;
  Values.add(x);

  
  std::vector<RooArgList*> Res;
  Res.push_back(A_Particle);
  Res.push_back(A2_Particle);

  LineShape_Library testsig("tsig", "tsig", Values,Res,PiPlus,PiMinus);

  //for(int i=1;i<=num;i++)
  while(count<num)
    {
      val=randnum.Uniform(rangeMin,rangeMax);
      //y.setVal(g+A*val+B*val*val);
      x=val;
      //std::cout<<"Rolled: "<<val<<std::endl;
      double fitted_mass= mass.getVal();
      double fitted_width= width.getVal();

      double preq=std::max(val*val-4*m0_pi*m0_pi,.0001);
      //std::cout<<"preq: "<<preq<<std::endl;
      double q=sqrt(preq)/2;
     double q0=sqrt(fitted_mass*fitted_mass-4*m0_pi*m0_pi)/2;// m0_rho are supposed to be the fitted
  

      //cout<<"Value of BW: "<<testsig.R(val, m0_rho, gamma0, q, q0, 1, size).Rho()<<std::endl;
      //y.setVal(0);
      //y.setVal(testsig.R(val, fitted_mass, fitted_width, q, q0, A_Particle_L->getVal(), A_Particle_Size->getVal()).Rho2());
      y.setVal(testsig.GetSumOfShapesSquared(val));

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

  RooArgList BValues;
  BValues.add(x);

LineShape_Library sig("sig", "sig", BValues,PiPlus,PiMinus);
  
  sig.CreateComponent("rho",BREITWIGNER,5.3,1, .77, .15,true);
  sig.CreateComponent("omega",BREITWIGNER,5.3,1, 1.1, .2,false);
  

  RooRealVar nevents("nevt", "N_{evt}", 1000,0,100000);    
  nevents.setError(10.0);
  nevents.setConstant(false);

  RooAddPdf model_new("model_new", "G+poly", RooArgList(sig), RooArgList(nevents));


  RooPlot * frame_KKMass = x.frame(rangeMin,rangeMax,100);
  DATA.plotOn(frame_KKMass); 




  model_new.fitTo(DATA,RooFit::Extended(),RooFit::SumW2Error(kFALSE));


  
  model_new.plotOn(frame_KKMass,RooFit::Components(sig),RooFit::LineColor(kRed));//,RooFit::Weight(y.getVal());
  model_new.plotOn(frame_KKMass);
  frame_KKMass->Draw();
 // hist->Draw("AP");
}
