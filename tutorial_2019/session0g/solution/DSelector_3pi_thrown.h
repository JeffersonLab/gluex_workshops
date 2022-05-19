#ifndef DSelector_3pi_thrown_h
#define DSelector_3pi_thrown_h

#include <iostream>

#include "DSelector/DSelector.h"

#include "TH1I.h"
#include "TH2I.h"

class DSelector_3pi_thrown : public DSelector
{
	public:

		DSelector_3pi_thrown(TTree* locTree = NULL) : DSelector(locTree){}
		virtual ~DSelector_3pi_thrown(){}

		void Init(TTree *tree);
		Bool_t Process(Long64_t entry);

	private:

		void Finalize(void);

		// BEAM POLARIZATION INFORMATION
		UInt_t dPreviousRunNumber;
		bool dIsPolarizedFlag; //else is AMO
		bool dIsPARAFlag; //else is PERP or AMO

		TH1F* hEgamma;
		TH2F* ht_Egamma;

	ClassDef(DSelector_3pi_thrown, 0);
};

#endif // DSelector_3pi_thrown_h
