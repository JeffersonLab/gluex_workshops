#ifndef DSelector_OmegaPi0_h
#define DSelector_OmegaPi0_h

#include <iostream>

#include "DSelector/DSelector.h"
#include "DSelector/DHistogramActions.h"
#include "DSelector/DCutActions.h"

#include "TH1I.h"
#include "TH2I.h"

class DSelector_OmegaPi0 : public DSelector
{
	public:

		DSelector_OmegaPi0(TTree* locTree = NULL) : DSelector(locTree){}
		virtual ~DSelector_OmegaPi0(){}

		void Init(TTree *tree);
		Bool_t Process(Long64_t entry);

	private:

		void Get_ComboWrappers(void);
		void Finalize(void);

		// BEAM POLARIZATION INFORMATION
		UInt_t dPreviousRunNumber;
		bool dIsPolarizedFlag; //else is AMO
		bool dIsPARAFlag; //else is PERP or AMO

		bool dIsMC;

		// ANALYZE CUT ACTIONS
		// // Automatically makes mass histograms where one cut is missing
		DHistogramAction_AnalyzeCutActions* dAnalyzeCutActions;

		//CREATE REACTION-SPECIFIC PARTICLE ARRAYS

		//Step 0
		DParticleComboStep* dStep0Wrapper;
		DBeamParticle* dComboBeamWrapper;
		DChargedTrackHypothesis* dPiPlusWrapper;
		DChargedTrackHypothesis* dPiMinusWrapper;
		DChargedTrackHypothesis* dProtonWrapper;

		//Step 1
		DParticleComboStep* dStep1Wrapper;
		DKinematicData* dDecayingPi01Wrapper;
		DNeutralParticleHypothesis* dPhoton1Wrapper;
		DNeutralParticleHypothesis* dPhoton2Wrapper;

		//Step 2
		DParticleComboStep* dStep2Wrapper;
		DKinematicData* dDecayingPi02Wrapper;
		DNeutralParticleHypothesis* dPhoton3Wrapper;
		DNeutralParticleHypothesis* dPhoton4Wrapper;

		// DEFINE YOUR HISTOGRAMS HERE
		// EXAMPLES:
		// TH1I* dHist_MissingMassSquared;
		// TH1I* dHist_BeamEnergy;
		// TH1I* dHist_BeamEnergy_BestChiSq;

                /// Added Histogram
                TH1D* dHist_MissingMassSquared;
                TH1D* dHist_MissingEnergy;
                TH1D* dHist_BeamEnergy;
                TH1D* dHist_BeamTiming;

                TH1D* dHist_ChiSq;
                TH1D* dHist_ChiSq_z;
                TH1D* dHist_ConfidenceLevel_logX;
                TH2D* dHist_ChiSqVsConfidenceLevel_logX;
    
                TH2D* dHist_DeltaTVsP_Proton_BCAL;
                TH2D* dHist_DeltaTVsP_Proton_TOF;
                TH2D* dHist_dEdxVsP_Proton_CDC;
                TH1D* dHist_VertexProtonZ;
                TH2D* dHist_VertexProtonXY;

                TH2D* dHist_DeltaTVsP_PiP_BCAL;
                TH2D* dHist_DeltaTVsP_PiP_TOF;
                TH2D* dHist_dEdxVsP_PiP_CDC;
  
                TH2D* dHist_DeltaTVsP_PiM_BCAL;
                TH2D* dHist_DeltaTVsP_PiM_TOF;
                TH2D* dHist_dEdxVsP_PiM_CDC;
  
                TH2D* dHist_DeltaTVsP_Photon_FCAL;
                TH2D* dHist_DeltaTVsP_Photon_BCAL;
                TH1D* dHist_ShowerQuality1;
                TH1D* dHist_ShowerQuality2;
                TH1D* dHist_ShowerQuality3;
                TH1D* dHist_ShowerQuality4;

                TH1D* dHist_UnusedShowerEnergy;
  
                TH1D* dHist_Mass_g1_g2_M;
                TH1D* dHist_Mass_g1_g3_M;
                TH1D* dHist_Mass_g1_g4_M;
                TH1D* dHist_Mass_g2_g3_M;
                TH1D* dHist_Mass_g2_g4_M;
                TH1D* dHist_Mass_g3_g4_M;

                TH1D* dHist_Mass_g1_g2;
                TH1D* dHist_Mass_g1_g3;
                TH1D* dHist_Mass_g1_g4;
                TH1D* dHist_Mass_g2_g3;
                TH1D* dHist_Mass_g2_g4;
                TH1D* dHist_Mass_g3_g4;

                TH1D* dHist_Mass_g1_g2_g3;
                TH1D* dHist_Mass_g1_g2_g4;
                TH1D* dHist_Mass_g1_g3_g4;
                TH1D* dHist_Mass_g2_g3_g4;
  
                TH1D* dHist_Mass_g1_g2_g3_g4;
  
                TH2D* dHist_g1g2vsg3g4_M;
                TH2D* dHist_g1g3vsg2g4_M;
                TH2D* dHist_g1g4vsg2g3_M;
  
                TH2D* dHist_g1g2vsg3g4;
                TH2D* dHist_g1g3vsg2g4;
                TH2D* dHist_g1g4vsg2g3;

                TH1D* dHist_Mass3pi1;
                TH1D* dHist_Mass3pi2;
                TH2D* dHist_Mass3pi2vs3pi1;
  
                TH1D* dHist_MassPipP;
                TH1D* dHist_MassPimP;
                TH1D* dHist_MassPi01P;
                TH1D* dHist_MassPi02P;
                TH2D* dHist_3piMassCorr;
                TH2D* dHist_3piMassCorr2DWeight;
  
                TH1D* dHist_MassPipPim;
                TH1D* dHist_MassPipPi01;
                TH1D* dHist_MassPipPi02;
                TH1D* dHist_MassPimPi01;
                TH1D* dHist_MassPimPi02;

                TH2D* dHist_pipPvspimPi01;
                TH2D* dHist_pipPvspimPi02;
                TH2D* dHist_pipPvspi0Pi02;

                TH2D* dHist_pimPvspipPi01;
                TH2D* dHist_pimPvspipPi02;
                TH2D* dHist_pimPvspi0Pi02;

                TH2D* dHist_pi01PvspipPim;
                TH2D* dHist_pi01PvspipPi02;
                TH2D* dHist_pi01PvspimPi02;

                TH2D* dHist_pi02PvspipPim;
                TH2D* dHist_pi02PvspipPi01;
                TH2D* dHist_pi02PvspimPi01;

                TH2D* dHist_pipPimvspi01Pi02;
                TH2D* dHist_pipPi01vspimPi02;
                TH2D* dHist_pipPi02vspimPi01;

                TH1D* dHist_Mass4p;
                TH1D* dHist_4piMassSum;
  
                TH1D* dHist_t;
                TH2D* dHist_tVs4piMass;
                TH1D* dHist_tprime;
                TH2D* dHist_tprimeVs4piMass;
  
	ClassDef(DSelector_OmegaPi0, 0);
};

void DSelector_OmegaPi0::Get_ComboWrappers(void)
{
	//Step 0
	dStep0Wrapper = dComboWrapper->Get_ParticleComboStep(0);
	dComboBeamWrapper = static_cast<DBeamParticle*>(dStep0Wrapper->Get_InitialParticle());
	dPiPlusWrapper = static_cast<DChargedTrackHypothesis*>(dStep0Wrapper->Get_FinalParticle(2));
	dPiMinusWrapper = static_cast<DChargedTrackHypothesis*>(dStep0Wrapper->Get_FinalParticle(3));
	dProtonWrapper = static_cast<DChargedTrackHypothesis*>(dStep0Wrapper->Get_FinalParticle(4));

	//Step 1
	dStep1Wrapper = dComboWrapper->Get_ParticleComboStep(1);
	dDecayingPi01Wrapper = dStep1Wrapper->Get_InitialParticle();
	dPhoton1Wrapper = static_cast<DNeutralParticleHypothesis*>(dStep1Wrapper->Get_FinalParticle(0));
	dPhoton2Wrapper = static_cast<DNeutralParticleHypothesis*>(dStep1Wrapper->Get_FinalParticle(1));

	//Step 2
	dStep2Wrapper = dComboWrapper->Get_ParticleComboStep(2);
	dDecayingPi02Wrapper = dStep2Wrapper->Get_InitialParticle();
	dPhoton3Wrapper = static_cast<DNeutralParticleHypothesis*>(dStep2Wrapper->Get_FinalParticle(0));
	dPhoton4Wrapper = static_cast<DNeutralParticleHypothesis*>(dStep2Wrapper->Get_FinalParticle(1));
}

#endif // DSelector_OmegaPi0_h
