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

 LineShape_Library::LineShape_Library(const char *name, const char *title, RooRealVar* totalNorm, RooArgList& _values, std::vector<RooArgList*> _res, Particle_t Dau1, Particle_t Dau2) :
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
  dau1=Dau1;
  dau2=Dau2;
  mDau1=ParticleMass(Dau1);
  mDau2=ParticleMass(Dau2);
  TotalNorm=totalNorm;
  delete coefIter;
  delete coef;
 } 
LineShape_Library::LineShape_Library(const char *name, const char *title, RooRealVar* totalNorm, RooArgList& _values, Particle_t Dau1, Particle_t Dau2) :
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
  dau1=Dau1;
  dau2=Dau2;
  mDau1=ParticleMass(Dau1);
  mDau2=ParticleMass(Dau2);
  TotalNorm=totalNorm;
 }

 LineShape_Library::LineShape_Library(const LineShape_Library& other, const char* name) :  
   RooAbsPdf(other,name),
   fitted_values("fitted_values",this,other.fitted_values)
   
 { 
fitted_values=other.fitted_values;   
res=other.res;
res_map=other.res_map;
dau1=other.dau1;
dau2=other.dau2;
mDau1=other.mDau1;
mDau2=other.mDau2;  
TotalNorm=other.TotalNorm;
 } 


int LineShape_Library::findIndex(int i,string key) const
{
  //std::cout<<i<<"th resonance "<<key<<endl;
  //std::cout<<res_map.size()<<endl;

  //std::cout<<res_map[i]<<endl;
  auto it=(res_map[i]).find(key);

  if( it != res_map[i].end() )
    return it->second;
  else
    return -1;
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
    //std::cout<<"INDEX OF Normalization is: "<<findIndex(i,"Norm")<<std::endl;
    double norm=((RooAbsReal&) ((*res[i])[findIndex(i,"Norm")])).getVal();
    
    if(((RooAbsReal&) ((*res[i])[findIndex(i,"Shape")])).getVal()==POLYNOMIAL)
    {
      int start_index=findIndex(i,"c0");
      for(int j=start_index;j<res[i]->getSize();j++)
      {
        incoherent_sum+=((RooAbsReal&) ((*res[i])[j])).getVal()*pow(m,j-start_index);
        //sumsq+=((RooAbsReal&) ((*res[i])[PARAM_START])).getVal()*m+((RooAbsReal&) ((*res[i])[PARAM_START+1])).getVal();
      }
    }
    else if(((RooAbsReal&) ((*res[i])[findIndex(i,"Shape")])).getVal()==BREITWIGNER)
    {
      
      double res_size= ((RooAbsReal&) ((*res[i])[findIndex(i,"Size")])).getVal();
      double res_L= ((RooAbsReal&) ((*res[i])[findIndex(i,"L")])).getVal();
      double fitted_mass= ((RooAbsReal&) ((*res[i])[findIndex(i,"Mass")])).getVal();
      double fitted_width= ((RooAbsReal&) ((*res[i])[findIndex(i,"Width")])).getVal();
      double rel_phase=((RooAbsReal&) ((*res[i])[findIndex(i,"RelativePhase")])).getVal();


      //double q=sqrt(std::max(m*m-4*m0_pi*m0_pi,.0001))/2;
      //double q0=sqrt(fitted_mass*fitted_mass-4*m0_pi*m0_pi)/2;
      double q  = 0.5*sqrt( std::max((m+mDau1+mDau2)*(m+mDau1-mDau2)*(m-mDau1+mDau2)*(m-mDau1-mDau2),.0001))/m;
      double q0= 0.5*sqrt( std::max((fitted_mass+mDau1+mDau2)*(fitted_mass+mDau1-mDau2)*(fitted_mass-mDau1+mDau2)*(fitted_mass-mDau1-mDau2),.0001))/fitted_mass;

      if(norm<0)
        norm=0;

      TComplex exp_phase(cos(rel_phase),sin(rel_phase));
      sumsq+=norm*R(m, fitted_mass, fitted_width, q, q0, res_L, res_size)*exp_phase;
      //sumsq=BW(m, m0_rho, gamma0, q, q0, 1, size);
    }
    else if(((RooAbsReal&) ((*res[i])[findIndex(i,"Shape")])).getVal()==JBREITWIGNER)
    {
      
      double res_size= ((RooAbsReal&) ((*res[i])[findIndex(i,"Size")])).getVal();
      double res_L= ((RooAbsReal&) ((*res[i])[findIndex(i,"L")])).getVal();
      double fitted_mass= ((RooAbsReal&) ((*res[i])[findIndex(i,"Mass")])).getVal();
      double fitted_width= ((RooAbsReal&) ((*res[i])[findIndex(i,"Width")])).getVal();
      double rel_phase=((RooAbsReal&) ((*res[i])[findIndex(i,"RelativePhase")])).getVal();


      //double q=sqrt(std::max(m*m-4*m0_pi*m0_pi,.0001))/2;
      //double q0=sqrt(fitted_mass*fitted_mass-4*m0_pi*m0_pi)/2;
      double q  = 0.5*sqrt( std::max((m+mDau1+mDau2)*(m+mDau1-mDau2)*(m-mDau1+mDau2)*(m-mDau1-mDau2),.0001))/m;
      double q0= 0.5*sqrt( std::max((fitted_mass+mDau1+mDau2)*(fitted_mass+mDau1-mDau2)*(fitted_mass-mDau1+mDau2)*(fitted_mass-mDau1-mDau2),.0001))/fitted_mass;

      if(norm<0)
        norm=0;

      TComplex exp_phase(cos(rel_phase),sin(rel_phase));
      sumsq+=norm*RJackson(m, fitted_mass, fitted_width, q, q0, res_L, res_size)*exp_phase;
      //sumsq=BW(m, m0_rho, gamma0, q, q0, 1, size);
    }
     else if(((RooAbsReal&) ((*res[i])[findIndex(i,"Shape")])).getVal()==NONRESONANTG)
    {

      double turnOn= ((RooAbsReal&) ((*res[i])[findIndex(i,"TurnOn")])).getVal();
      double c1= ((RooAbsReal&) ((*res[i])[findIndex(i,"c1")])).getVal();
      double c2= ((RooAbsReal&) ((*res[i])[findIndex(i,"c2")])).getVal();
      
     incoherent_sum+=norm*NRG_R(m, turnOn, c1, c2).Re();
    }
    else if(((RooAbsReal&) ((*res[i])[findIndex(i,"Shape")])).getVal()==NONRESONANT)
    {
      //std::cout<<"NONRESONANT"<<endl;
      double res_size= ((RooAbsReal&) ((*res[i])[findIndex(i,"Size")])).getVal();
      double res_L= ((RooAbsReal&) ((*res[i])[findIndex(i,"L")])).getVal();
      double alpha= ((RooAbsReal&) ((*res[i])[findIndex(i,"Alpha")])).getVal();
      
      double fitted_mass= ((RooAbsReal&) ((*res[i])[findIndex(i,"Center")])).getVal();;
      
      //double rel_phase=((RooAbsReal&) ((*res[i])[RESRELPHASE])).getVal();


      //double q=sqrt(std::max(m*m-4*m0_pi*m0_pi,.0001))/2;
      //double q0=sqrt(fitted_mass*fitted_mass-4*m0_pi*m0_pi)/2;
      double q  = 0.5*sqrt( std::max((m+mDau1+mDau2)*(m+mDau1-mDau2)*(m-mDau1+mDau2)*(m-mDau1-mDau2),.0001))/m;
      double q0= 0.5*sqrt( std::max((fitted_mass+mDau1+mDau2)*(fitted_mass+mDau1-mDau2)*(fitted_mass-mDau1+mDau2)*(fitted_mass-mDau1-mDau2),.0001))/fitted_mass;

      if(norm<0)
        norm=0;

      //TComplex exp_phase(-1*cos(rel_phase),sin(rel_phase));
     incoherent_sum+=norm*NR_R(m, fitted_mass, q, q0,res_L, res_size,alpha).Re();//*exp_phase;
      //sumsq=BW(m, m0_rho, gamma0, q, q0, 1, size);
    }
    else if(((RooAbsReal&) ((*res[i])[findIndex(i,"Shape")])).getVal()==VOIGTIAN)
    {
      
      double m0= ((RooAbsReal&) ((*res[i])[findIndex(i,"Mass")])).getVal();
      double width= ((RooAbsReal&) ((*res[i])[findIndex(i,"Width")])).getVal();
      double sigma= ((RooAbsReal&) ((*res[i])[findIndex(i,"Sigma")])).getVal();
      
      
     //incoherent_sum+=pow(sqrt(norm)*VOIGT(m,m0,width, sigma),2);
     incoherent_sum+=norm*VOIGT(m,m0,width, sigma);
      
    }
  }

   return sumsq.Rho2()+incoherent_sum;
 }
 double LineShape_Library::VOIGT(const double& m,const double& m0,const double& width, const double& sigma) const
{

  Double_t s = (sigma>0) ? sigma : -sigma ;
   Double_t w = (width>0) ? width : -width ;
 
   Double_t coef= -0.5/(s*s);
   Double_t arg = m - m0;
 
   // return constant for zero width and sigma
   if (s==0. && w==0.) return 1.;
 
   // Breit-Wigner for zero sigma
   if (s==0.) return (1./(arg*arg+0.25*w*w));
 
   // Gauss for zero width
   if (w==0.) return exp(coef*arg*arg);
 
   // actual Voigtian for non-trivial width and sigma
   Double_t c = 1./(sqrt(2.)*s);
   Double_t a = 0.5*c*w;
   Double_t u = c*arg;
   std::complex<Double_t> z(u,a) ;
   std::complex<Double_t> v(0.) ;
 

   v = RooMath::faddeeva(z);

  double _invRootPi= 1./sqrt(atan2(0.,-1.));

   return c*_invRootPi*v.real();

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
TComplex LineShape_Library::NR_R(const double& m, const double& fittedm, const double& q, const double& q0, const double& res_L, const double& d, const double& alpha) const
{
  double BPrime = Bprime(res_L,q,q0,d);
  //std::cout<<BPrime1<<"  "<<BPrime2<<std::endl;                                                                                                                                                             
  //std::cout<<Lchain<<"  "<<q<<"  "<<q0<<"  "<<d<<"  "<<BPrime2<<std::endl;                                                                                                                  
  //std::cout<<mmChain<<"  "<<mChain<<"  "<<alpha<<std::endl;
  //std::cout<<pow(mmChain,2)-pow(mChain,2)<<std::endl;
  //std::cout<<exp(-(pow(mmChain,2)-pow(mChain,2))*alpha )<<std::endl;
  //std::cout<<"---------------------------------------------------"<<std::endl;
     
  TComplex Rnum=pow((q/q0),2*res_L+1)*BPrime*exp(-1*fabs(alpha)*(pow(m,2)-pow(fittedm,2)));

  return Rnum;
}
TComplex LineShape_Library::NRG_R(const double& m, const double& turnOn, const double& c1, const double& c2) const
{
     
  TComplex Rnum=pow(m-(mDau1+mDau2),turnOn)*exp(1.+c1*m+c2*m*m);

  return Rnum;
}
TComplex LineShape_Library::RJackson(const double& m, const double& m0, const double& gamma0, const double& q, const double& q0, const int& LKs, const double& d) const
{
  //std::cout<<"q0: "<<q0<<" q: "<<q<<" LKS: "<<LKs<<std::endl;
  double q_term=1/(q*q);
  
  //TComplex BWTerm = BW(m,m0,gamma0,q,q0,LKs,d);
  //std::cout<<"Computing term: "<<BPrime1<<" | "<<pow1<<" | "<<BW(m,m0,gamma0,q,q0,LKs,d).Rho()<<std::endl;
  TComplex Rnum=q_term*BWJackson(m,m0,gamma0,q,q0,LKs,d);
  return Rnum;
}
TComplex LineShape_Library::BWJackson(const double& m, const double& m0, const double& gamma0, const double& q, const double& q0, const int& L, const double& d) const
{
  double gamma = Gamma(m,gamma0,q,q0,L,m0,d);
  TComplex num(m*gamma,0);
  TComplex denom((m0+m)*(m0-m),-m*gamma);
  return (num/denom);  
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
void LineShape_Library::CreateComponent(TString name,int shape,double size,double L=0, double mass=0, double width=0,bool isNorm=0)
{
  std::map<string,int> comp_map;

  //std::cout<<"Create Component "<<comp_map.size()<<endl;

  RooRealVar* A_Particle_Shape=new RooRealVar(name+"_Shape",name+"_Shape",shape);
  RooRealVar* A_Particle_Norm=new RooRealVar(name+"_Norm",name+"_Norm",0);
              A_Particle_Norm->setError(.1);
              A_Particle_Norm->setMin(0);
              A_Particle_Norm->setMax(1E4);
              A_Particle_Norm->setConstant(false);

  if(isNorm==1)
  {
    A_Particle_Norm->setVal(1);
  }

  RooArgList* A_Particle=new RooArgList(*A_Particle_Shape,*A_Particle_Norm,name);
  comp_map["Shape"]=comp_map.size()-1;
  //std::cout<<"Add Component "<<comp_map.size()<<endl;
  //exit(1);
  comp_map["Norm"]=comp_map.size()-1;

  TString string_name( A_Particle->GetName() );
  RooListProxy* tempProxy= new RooListProxy(string_name,string_name,this);

  
  switch(shape)
  {
    case POLYNOMIAL:{
      A_Particle_Norm->setVal(.01);
      A_Particle_Norm->setError(.001);
      for(int i=0;i<=size;i++)
                        {
                          TString intname;
                          intname.Form ("%i", i);
                           RooRealVar* A_Particle_coef=new RooRealVar(name+"_c"+intname,name+"_c"+intname,0);//from example fit?
                            A_Particle_coef->setError(1);
                             A_Particle_coef->setConstant(false);
                             A_Particle->add(*A_Particle_coef);
                             string coef_name="c"+string(intname);
                             cout<<coef_name<<endl;
                             comp_map[coef_name]=comp_map.size()-1;

                             if(i==0)
                             {
                              A_Particle_coef->setVal(1);
                              A_Particle_coef->setError(0);
                              A_Particle_coef->setConstant(true);
                             }
                        } }break;

    case BREITWIGNER: case JBREITWIGNER:
        {
          

          RooRealVar* A_Particle_Size=new RooRealVar(name+"_Size",name+"Size",size);

          RooRealVar* A_Particle_L=new RooRealVar(name+"_L",name+"L",L);
 
          RooRealVar* A_Particle_mass=new RooRealVar(name+"_Mass", name+"_Mass",mass);//from example fit?
          A_Particle_mass->setError(A_Particle_mass->getVal()/10.);
          A_Particle_mass->setMin(mDau1+mDau2);
          A_Particle_mass->setMax(100);
          A_Particle_mass->setConstant(false);

          RooRealVar* A_Particle_width=new RooRealVar(name+"_Width", name+"_Width", width);//from example fit?
          A_Particle_width->setError(A_Particle_width->getVal()/10.);
          A_Particle_width->setMin(0);
          A_Particle_width->setMax(100);
          A_Particle_width->setConstant(false);

          RooRealVar* phase=new RooRealVar(name+"_RelativePhase", name+"_RelativePhase", 0,0,2*TMath::Pi());//from example fit?
          phase->setError(1);
          phase->setConstant(false);
          if(isNorm)
          { 
            phase->setVal(0);
            phase->setConstant(true);
          }

          A_Particle->add(*A_Particle_Size);
          comp_map["Size"]=comp_map.size()-1;
          A_Particle->add(*A_Particle_L);
          comp_map["L"]=comp_map.size()-1;
          A_Particle->add(*A_Particle_mass);
          comp_map["Mass"]=comp_map.size()-1;
          A_Particle->add(*A_Particle_width);
          comp_map["Width"]=comp_map.size()-1;
          A_Particle->add(*phase);
          comp_map["RelativePhase"]=comp_map.size()-1;
          
         // res.push_back(A_Particle);
        }break;

          case NONRESONANT: 
          {

            A_Particle_Norm->setVal(.01);
            A_Particle_Norm->setError(.001);
          RooRealVar* A_Particle_Size=new RooRealVar(name+"_Size",name+"Size",size);
          RooRealVar* A_Particle_L=new RooRealVar(name+"_L",name+"L",L); 

          RooRealVar* A_Particle_alpha=new RooRealVar(name+"_Alpha", name+"_Alpha", mass);//from example fit?
          A_Particle_alpha->setError(A_Particle_alpha->getVal()/10.);
          A_Particle_alpha->setMin(-100);
          A_Particle_alpha->setMax(100);
          A_Particle_alpha->setConstant(false);

          double rangeMin=( (RooRealVar*) &fitted_values[0])->getMin();
          double rangeMax=( (RooRealVar*) &fitted_values[0])->getMax();

          RooRealVar* A_Particle_center=new RooRealVar(name+"_Center", name+"_Center", ((rangeMin+rangeMax)/2.));//from example fit?
          A_Particle_center->setError(A_Particle_center->getVal()/10.);
          A_Particle_center->setMin(mDau1+mDau2);
          A_Particle_center->setMax(100);
          A_Particle_center->setConstant(false);

          A_Particle->add(*A_Particle_Size);
          comp_map["Size"]=comp_map.size()-1;
          A_Particle->add(*A_Particle_L);
          comp_map["L"]=comp_map.size()-1;
          A_Particle->add(*A_Particle_alpha);
          comp_map["Alpha"]=comp_map.size()-1;
          A_Particle->add(*A_Particle_center);
          comp_map["Center"]=comp_map.size()-1;

            
          }break;
          case NONRESONANTG: 
          {

            A_Particle_Norm->setVal(.01);
            A_Particle_Norm->setError(.001);
          RooRealVar* A_Particle_turnon_exp=new RooRealVar(name+"_TurnOn",name+"_TurnOn",size);
          A_Particle_turnon_exp->setError(A_Particle_turnon_exp->getVal()/10.);
          A_Particle_turnon_exp->setMin(-10);
          A_Particle_turnon_exp->setMax(10);
          A_Particle_turnon_exp->setConstant(false);


          RooRealVar* A_Particle_c1=new RooRealVar(name+"_c1",name+"c1",L); 
          A_Particle_c1->setError(A_Particle_c1->getVal()/10.);
          A_Particle_c1->setConstant(false);

          RooRealVar* A_Particle_c2=new RooRealVar(name+"_c2",name+"c2",mass); 
          A_Particle_c2->setError(A_Particle_c2->getVal()/10.);
          A_Particle_c2->setConstant(false);

          

          A_Particle->add(*A_Particle_turnon_exp);
          comp_map["TurnOn"]=comp_map.size()-1;
          A_Particle->add(*A_Particle_c1);
          comp_map["c1"]=comp_map.size()-1;
          A_Particle->add(*A_Particle_c2);
          comp_map["c2"]=comp_map.size()-1;
            
          }break;

          case VOIGTIAN:
          {
              RooRealVar* A_Particle_mass=new RooRealVar(name+"_Mass", name+"_Mass",mass);//from example fit?
              A_Particle_mass->setError(A_Particle_mass->getVal()/10.);
              A_Particle_mass->setMin(mDau1+mDau2);
              A_Particle_mass->setMax(100);
              A_Particle_mass->setConstant(false);

              RooRealVar* A_Particle_width=new RooRealVar(name+"_Width", name+"_Width", width);//from example fit?
              A_Particle_width->setError(A_Particle_width->getVal()/10.);
              A_Particle_width->setMin(0);
              A_Particle_width->setMax(100);
              A_Particle_width->setConstant(false);

              RooRealVar* A_Particle_sigma=new RooRealVar(name+"_Sigma", name+"_Sigma",.001);//from example fit?
              A_Particle_sigma->setError(.0001);
              A_Particle_sigma->setMin(0);
              A_Particle_sigma->setMax(100);
              A_Particle_sigma->setConstant(false);


              A_Particle->add(*A_Particle_mass);
              comp_map["Mass"]=comp_map.size()-1;
              A_Particle->add(*A_Particle_width);
              comp_map["Width"]=comp_map.size()-1;
              A_Particle->add(*A_Particle_sigma);
              comp_map["Sigma"]=comp_map.size()-1;
          
          }break;
  }
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
    res_map.push_back(comp_map);

}
void LineShape_Library::AddComponent(RooListProxy* toAdd, std::map<string,int > toAdd_map)
{
  this->res.push_back(toAdd);
  this->res_map.push_back(toAdd_map);
}
RooListProxy* LineShape_Library::GetComponent(TString name)
{
  for(uint i=0; i<res.size();i++)
  {
    if(res[i]->GetName() == name)
    {
      return res[i];
    }
    
  }
    return 0;
}
std::map<string,int >  LineShape_Library::GetComponentResMap(TString name)
{
  std::map<string,int> nullmap;
  nullmap["NULL"]=-1;
  for(uint i=0; i<res.size();i++)
  {
    if(res[i]->GetName() == name)
    {
      return res_map[i];
    }
    
  }
    return nullmap;
}
RooRealVar* LineShape_Library::GetParameter(RooListProxy* resonance, TString name)
{
    return ( (RooRealVar*) resonance->find(name));
}
 RooRealVar* LineShape_Library::GetParameterFromComponent(TString compname,TString paramname)
 {
  for(uint i=0; i<res.size();i++)
  {
    if(res[i]->GetName() == compname)
    {
      return ( (RooRealVar*) res[i]->find(compname+"_"+paramname)); 
    } 
  }
    return 0;
 }

 void LineShape_Library::ReplaceResList(std::vector<RooListProxy*> newres)
 {
   //cout<<"replacing"<<endl;
   //cout<<"start: "<<res.size()<<" and "<<newres.size()<<std::endl;
   this->res.clear();
   this->res=newres;
   //cout<<"end: "<<this->res.size()<<std::endl;
   return;
 }

double LineShape_Library::GetIntegralOf(TString resname,double int_min=0, double int_max=0)
  {
    //std::cout<<"NAME: "<<resname<<endl;
    double denom=DoNumericIntegral(0/*int_min*/,0/*int_max*/);
 
    double total_normies=0;
    double this_normy=0;
    std::vector<double> saved_normies;
    for(uint i=0;i<res.size();i++)
    {
      RooRealVar* normy=GetParameterFromComponent(res[i]->GetName(),"Norm");
      total_normies+=normy->getVal();
      saved_normies.push_back(normy->getVal());

      if(res[i]->GetName() == resname)
      {
        this_normy=normy->getVal();
        //normy->setVal(1);
        
         continue;
      }
       

      if(normy)
      {
        normy->setVal(0);
      }
    }

    double numerator=DoNumericIntegral(int_min,int_max);

      for(uint i=0;i<res.size();i++)
    {
      RooRealVar* normy=GetParameterFromComponent(res[i]->GetName(),+"Norm");
      normy->setVal(saved_normies[i]);
    }
      //std::cout<<"part PDF int: "<<numerator<<endl;
     //std::cout<<"whole PDF int: "<<denom<<endl;
      return (numerator/denom);

  }

double LineShape_Library::GetTotalNorm()
{
  return this->TotalNorm->getVal();
}

RooAddPdf* LineShape_Library::GetComponentModel(TString name,double intval)
{
  if(!GetSingleComponent_PDF(name))
  {
    return NULL;
  }
  RooRealVar* yield = new RooRealVar(name+"_yield",name+"_yield",intval);


  LineShape_Library* ls= new LineShape_Library(*GetSingleComponent_PDF(name));
  //std::cout<<"Lineshape of comp: "<< ls << endl;
  RooAddPdf* model = new RooAddPdf(name+"_model", "", RooArgList(*ls), RooArgList(*yield));
  std::cout<<model<<endl;
  return model;
}

LineShape_Library* LineShape_Library::GetSingleComponent_PDF(TString name)
 {
  LineShape_Library* tempComp = new LineShape_Library("tempsig", "tempsig", this->TotalNorm,this->fitted_values,this->dau1,this->dau2);

  if(!this->GetComponent(name))
  {
    return NULL;
  }

  RooListProxy* tempProxy = new RooListProxy(*this->GetComponent(name));
  tempProxy->setName(name);

  tempComp->AddComponent(tempProxy,this->GetComponentResMap(name));
  //cout<<"Returning component: "<<tempComp<<endl;
  return (tempComp);
  
 }
 double LineShape_Library::DoNumericIntegral(double min=0,double max=0)///VERY SIMPLE FOR NOW
 {
   
   double sum=0;

   double intMin=min;
   double intMax=max;

  if(intMax==intMin)
    {
      //std::cout<<"FULL RANGE INT"<<std::endl;
      intMin=( (RooRealVar*) &fitted_values[0])->getMin();
      intMax=( (RooRealVar*) &fitted_values[0])->getMax();
    }

    //cout<<"INTEGRATE: "<<intMin<<" to "<<intMax<<endl;
   
    int bins=50000;
    double delta=(intMax-intMin)/bins;
    //cout<<"DELTA IS: "<<delta<<endl;
    for(int i=0;i<bins;i++)
    {
      double leftEdge=intMin+i*delta;
      double rightEdge=(intMin+delta)+i*delta;

      double val=(rightEdge+leftEdge)/2.;
     
      sum+= delta*GetSumOfShapesSquared(val);

    }
   
  return sum;

 }
 void LineShape_Library::PlotComponent(TString name, RooPlot* frame,EColor color,double yield)
 {
   //std::cout<<"PLOTTING: "<<name<<endl;
   RooAddPdf* component_to_plot=GetComponentModel(name,yield);
   //std::cout<<component_to_plot<<" on "<<frame<<endl;
   if(component_to_plot && frame)
   {

     component_to_plot->plotOn(frame,RooFit::LineColor(color),RooFit::Normalization(yield,RooAbsReal::NumEvent));
   }
   
 }
 double LineShape_Library::GetYieldold(TString resname,double int_min=0, double int_max=0)
 {
   return this->TotalNorm->getVal()*GetIntegralOf(resname,int_min,int_max);
 }
 double LineShape_Library::GetYield(TString resname,double min=0, double max=0)
 {
  double totalNorm=this->TotalNorm->getVal();

  double Denom=DoNumericIntegral(min,max);

  std::vector<double> Norms;

  bool found=false;
   for(uint i=0; i<res.size();i++)
  {
      string name=res[i]->GetName();
      Norms.push_back(GetParameterFromComponent(name,"Norm")->getVal());
      if(res[i]->GetName() != resname)
      {
       GetParameterFromComponent(name,"Norm")->setVal(0);
      }
      else
      {
        found=true;
      }
  }

  double numerator=0;
  
  if(found)
    numerator=DoNumericIntegral(min,max);

  for(uint i=0; i<res.size();i++)
  {
       string name=res[i]->GetName();
       GetParameterFromComponent(name,"Norm")->setVal(Norms[i]);
  }
  double toRet=totalNorm*numerator/Denom;

  //cout<<"Numerator="<<numerator<<" / "<<Denom<<endl;
  //cout<<totalNorm<<endl;
  cout<<resname<<" yield: "<<toRet<<endl;

  return toRet;
 }