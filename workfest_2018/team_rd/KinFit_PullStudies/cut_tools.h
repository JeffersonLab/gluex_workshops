#include "TMath.h"


Bool_t isInsideDM(Double_t dmass, Double_t mean, Double_t sigma, Double_t nSigma){
    Bool_t out = false;	
	
	if(dmass >= mean -nSigma*sigma && dmass <= mean + nSigma*sigma){
		out = true;
	}else out = false;
	
    return out;
}
