/***************************************************************************** 
 * Project: RooFit                                                           * 
 *                                                                           * 
 * Thomas Britton                                                            * 
 *****************************************************************************/ 

// Framework to support multiple models for easily fitting RooDataSets 

#include "Riostream.h" 
#include "RooBreitWigner.h"
#include "LineShape_Library.h" 
#include "RooAbsReal.h" 
#include "RooAbsCategory.h" 
#include <math.h> 
#include "TMath.h" 

ClassImp(LineShape_Library) 

 LineShape_Library::LineShape_Library(const char *name, const char *title, RooArgList& _values, std::vector<RooArgList*> _res, Particle_t Dau1, Particle_t Dau2) :
   RooAbsPdf(name,title),
   fitted_values("fitted_values","fitted_values",this)
 { 

TIterator* coefIter = _values.createIterator();
  RooAbsArg* coef;
  while((coef = (RooAbsArg*)coefIter->Next())){
    if(!dynamic_cast<RooAbsReal*>(coef)){
      assert(0);
    }
    fitted_values.add(*coef);    
  }  
    //std::cout<<"start SIZE: "<<fitted_values.getSize()<<endl;
   int num=_res.size();
  int counter(0);
  //   std::cout<<counter<<"  "<<num<<std::endl;
  while(counter < num){
    
    TString string_name( _res[counter]->GetName() );
    
    RooListProxy* tempProxy= new RooListProxy(string_name,string_name,this);
    
    TIterator* coefIter = _res[counter]->createIterator() ;
    RooAbsArg* coef ;
    //std::cout<<counter<<std::endl;
    while((coef = (RooAbsArg*)coefIter->Next())) {
      if (!dynamic_cast<RooAbsReal*>(coef)) {
        assert(0) ;
      }
      (*tempProxy).add(*coef);
    }
    //parameter tempPara= new parameter;
    //tempPara.x=*tempProxy;
    //tempPara.name=string_name;
    res.push_back(tempProxy);
    
    counter++;
    
  }

  mDau1=ParticleMass(Dau1);
  mDau2=ParticleMass(Dau2);
  delete coefIter;
  delete coef;
 } 
LineShape_Library::LineShape_Library(const char *name, const char *title, RooArgList& _values, Particle_t Dau1, Particle_t Dau2) :
   RooAbsPdf(name,title),
   fitted_values("fitted_values","fitted_values",this)
 { 

TIterator* coefIter = _values.createIterator();
  RooAbsArg* coef;
  while((coef = (RooAbsArg*)coefIter->Next())){
    if(!dynamic_cast<RooAbsReal*>(coef)){
      assert(0);
    }
    fitted_values.add(*coef);    
  }
  mDau1=ParticleMass(Dau1);
  mDau2=ParticleMass(Dau2);
 }

 LineShape_Library::LineShape_Library(const LineShape_Library& other, const char* name) :  
   RooAbsPdf(other,name),
   fitted_values("fitted_values",this,other.fitted_values)
   
 { 
fitted_values=other.fitted_values;   
res=other.res; 
mDau1=other.mDau1;
mDau2=other.mDau2;  
 } 



 Double_t LineShape_Library::evaluate() const 
 { 
   // ENTER EXPRESSION IN TERMS OF VARIABLE ARGUMENTS HERE 
   //cout<<"A= "<<A<<"  m="<<m<<"  g= "<<g<<std::endl;
   //std::cout<<res[0]->getSize()<<std::endl;
   //std::cout<<"NEW SIZE: "<<fitted_values.getSize()<<endl;
   double m=(((RooAbsReal&) fitted_values[0]).getVal());
   double sumsq=GetSumOfShapesSquared(m);

  //std::cout<<sumsq<<std::endl;
   return sumsq; 
 } 
 double LineShape_Library::GetSumOfShapesSquared(double m) const
 {
   TComplex sumsq(0,0);
   double incoherent_sum=0;
   for(uint i=0; i<res.size();i++)
  {

    double norm=((RooAbsReal&) ((*res[i])[RESNORM])).getVal();
    if(((RooAbsReal&) ((*res[i])[RESSHAPE])).getVal()==POLYNOMIAL)
    {
      for(int j=0;j<res[i]->getSize()-PARAM_START;j++)
      {
        incoherent_sum+=sqrt(norm)*((RooAbsReal&) ((*res[i])[j+PARAM_START])).getVal()*pow(m,j);
        //sumsq+=((RooAbsReal&) ((*res[i])[PARAM_START])).getVal()*m+((RooAbsReal&) ((*res[i])[PARAM_START+1])).getVal();
      }
    }
    else if(((RooAbsReal&) ((*res[i])[RESSHAPE])).getVal()==BREITWIGNER)
    {
      
      double res_size= ((RooAbsReal&) ((*res[i])[RESSIZE])).getVal();
      double res_L= ((RooAbsReal&) ((*res[i])[RESL])).getVal();
      double fitted_mass= ((RooAbsReal&) ((*res[i])[RESMASS])).getVal();
      double fitted_width= ((RooAbsReal&) ((*res[i])[RESWIDTH])).getVal();
      double rel_phase=((RooAbsReal&) ((*res[i])[RESRELPHASE])).getVal();


      //double q=sqrt(std::max(m*m-4*m0_pi*m0_pi,.0001))/2;
      //double q0=sqrt(fitted_mass*fitted_mass-4*m0_pi*m0_pi)/2;
      double q  = 0.5*sqrt( std::max((m+mDau1+mDau2)*(m+mDau1-mDau2)*(m-mDau1+mDau2)*(m-mDau1-mDau2),.0001))/m;
      double q0= 0.5*sqrt( std::max((fitted_mass+mDau1+mDau2)*(fitted_mass+mDau1-mDau2)*(fitted_mass-mDau1+mDau2)*(fitted_mass-mDau1-mDau2),.0001))/fitted_mass;

      if(norm<0)
        norm=0;

      TComplex exp_phase(cos(rel_phase),sin(rel_phase));
      sumsq+=sqrt(norm)*R(m, fitted_mass, fitted_width, q, q0, res_L, res_size)*exp_phase;
      //sumsq=BW(m, m0_rho, gamma0, q, q0, 1, size);
    }
  }

   return sumsq.Rho2()+incoherent_sum;
 }

 double LineShape_Library::Bprime(const int& L,const double& q, const double& q0, const double& d) const
{
  double z= pow(q*d,2);
  double z0 =pow(q0*d,2);
  if(L == 0) return 1.0;
  if(L == 1) return sqrt((1.0+z0)/(1.0+z)); 
  if(L == 2) return sqrt((9.+(3.+z0)*z0)/(9.+(3.+z)*z));
  if(L == 3) return sqrt((z0*(z0*(z0+6.)+45.)+225.)/(z*(z*(z+6.)+45.)+225.));
  if(L == 4) return sqrt((z0*(z0*(z0*(z0+10.)+135.)+1575.)+11025.)/(z*(z*(z*(z+10.)+135.)+1575.)+11025.));
  if(L == 5) return sqrt((z0*(z0*(z0*(z0*(z0+15.)+315.)+6300.)+99225.)+893025.)/(z*(z*(z*(z*(z+15.)+315.)+6300.)+99225.)+893025.));
  return 1.0; 
}

TComplex LineShape_Library::R(const double& m, const double& m0, const double& gamma0, const double& q, const double& q0, const int& LKs, const double& d) const
{
  //std::cout<<"q0: "<<q0<<" q: "<<q<<" LKS: "<<LKs<<std::endl;
  double BPrime1 = Bprime(LKs,q,q0,d);
  double pow1 = pow((q/q0),2*LKs+1);
  
  
  //TComplex BWTerm = BW(m,m0,gamma0,q,q0,LKs,d);
  //std::cout<<"Computing term: "<<BPrime1<<" | "<<pow1<<" | "<<BW(m,m0,gamma0,q,q0,LKs,d).Rho()<<std::endl;
  TComplex Rnum=BPrime1*pow1*BW(m,m0,gamma0,q,q0,LKs,d);
  return Rnum;
}

TComplex LineShape_Library::BW(const double& m, const double& m0, const double& gamma0, const double& q, const double& q0, const int& L, const double& d) const
{
  
  TComplex num(1,0);
  TComplex denom(m0*m0-m*m,-m0*Gamma(m,gamma0,q,q0,L,m0,d));
  
  return (num/denom);  
}
double LineShape_Library::Gamma(const double& m, const double& gamma0, const double& q, const double& q0, const int& L, const double& m0, const double& d) const
{
  double gammaM = gamma0*pow(q/q0,2*L+1)*(m0/m)*pow(Bprime(L,q,q0,d),2);
  
  return gammaM;
}
void LineShape_Library::CreateComponent(TString name,int shape,double size,double L, double mass, double width,bool isNorm)
{
  switch(shape)
  {
    case 0:cout<<"Building POLY"<<std::endl;break;

    case 1:
          
          RooRealVar* A_Particle_Norm=new RooRealVar(name+"_Norm",name+"_Norm",1);
          A_Particle_Norm->setError(.1);
          A_Particle_Norm->setMin(0);
          A_Particle_Norm->setMax(1E7);
          A_Particle_Norm->setConstant(false);
          if(isNorm)
          {
            A_Particle_Norm->setConstant(true);
          }

           RooRealVar* A_Particle_Shape=new RooRealVar(name+"_Shape",name+"_Shape",shape);

          RooRealVar* A_Particle_Size=new RooRealVar(name+"_Size",name+"Size",size);

          RooRealVar* A_Particle_L=new RooRealVar(name+"_L",name+"L",L);
 
          RooRealVar* A_Particle_mass=new RooRealVar(name+"_Mass", name+"_Mass",mass);//from example fit?
          A_Particle_mass->setError(A_Particle_mass->getVal()/10.);
          A_Particle_mass->setMin(0);
          A_Particle_mass->setMax(100);
          A_Particle_mass->setConstant(false);

          RooRealVar* A_Particle_width=new RooRealVar(name+"_Width", name+"_Width", width);//from example fit?
          A_Particle_width->setError(A_Particle_width->getVal()/10.);
          A_Particle_width->setMin(0);
          A_Particle_width->setMax(100);
          A_Particle_width->setConstant(false);

          RooRealVar* phase=new RooRealVar(name+"_RelativePhase", name+"_RelativePhase", TMath::Pi(),0,2*TMath::Pi());//from example fit?
          phase->setError(1);
          phase->setConstant(false);
          if(isNorm)
          {
            phase->setConstant(true);
          }

           RooArgList* A_Particle=new RooArgList(*A_Particle_Shape,*A_Particle_Norm,*A_Particle_Size,name);
          A_Particle->add(*A_Particle_L);
          A_Particle->add(*A_Particle_mass);
          A_Particle->add(*A_Particle_width);
          A_Particle->add(*phase);
          
          TString string_name( A_Particle->GetName() );
    
          RooListProxy* tempProxy= new RooListProxy(string_name,string_name,this);
             
             TIterator* coefIter = A_Particle->createIterator() ;
            RooAbsArg* coef;

    //std::cout<<counter<<std::endl;
    while((coef = (RooAbsArg*)coefIter->Next())) {
      if (!dynamic_cast<RooAbsReal*>(coef)) {
        assert(0) ;
      }

      (*tempProxy).add(*coef);
    }
    //parameter tempPara= new parameter;
    //tempPara.x=*tempProxy;
    //tempPara.name=string_name;
    res.push_back(tempProxy);
          
         // res.push_back(A_Particle);
          break;
  }

}
/*
 Double_t LineShape_Library::analyticalIntegral(Int_t code, const char* rangeName) const
{
  assert(code==1);
  double tmin = m.min(rangeName);
  double tmax = m.max(rangeName);
  std::cout<<tmin<<"---->"<<tmax<<endl;
  std::cout<<((A/2)*tmax*tmax+g*tmax)-((A/2)*tmin*tmin+g*tmin)<<endl;

  return ((A/2)*tmax*tmax+g*tmax)-((A/2)*tmin*tmin+g*tmin);

}

Int_t LineShape_Library::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* rangeName) const
{
  if( matchArgs(allVars,analVars,m) )
    return 1 ;
  
  return 0;
}*/

