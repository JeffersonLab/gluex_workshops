#ifndef DSelector_Thrown_h
#define DSelector_Thrown_h

#include <iostream>

#include "DSelector/DSelector.h"

#include "TH1I.h"
#include "TH2I.h"

class DSelector_Thrown : public DSelector
{
	public:

		DSelector_Thrown(TTree* locTree = NULL) : DSelector(locTree){}
		virtual ~DSelector_Thrown(){}

		void Init(TTree *tree);
		Bool_t Process(Long64_t entry);

	private:

		void Finalize(void);

		// BEAM POLARIZATION INFORMATION
		UInt_t dPreviousRunNumber;
		bool dIsPolarizedFlag; //else is AMO
		bool dIsPARAFlag; //else is PERP or AMO

	ClassDef(DSelector_Thrown, 0);
};

#endif // DSelector_Thrown_h
