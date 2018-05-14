// $Id$
//
//    File: DEventWriterROOT_someTest.h
// Created: Sa 14. Apr 18:30:24 EDT 2018
// Creator: dlersch (on Linux ifarm1402.jlab.org 3.10.0-327.el7.x86_64 x86_64)
//

#ifndef _DEventWriterROOT_someTest_
#define _DEventWriterROOT_someTest_

#include <map>
#include <string>

#include <ANALYSIS/DEventWriterROOT.h>

using namespace std;
using namespace jana;

class DEventWriterROOT_someTest : public DEventWriterROOT
{
	public:
		virtual ~DEventWriterROOT_someTest(void){};

	protected:

		//CUSTOM FUNCTIONS: //Inherit from this class and write custom code in these functions

		virtual void Create_CustomBranches_ThrownTree(DTreeBranchRegister& locBranchRegister, JEventLoop* locEventLoop) const;
		virtual void Fill_CustomBranches_ThrownTree(DTreeFillData* locTreeFillData, JEventLoop* locEventLoop, const DMCReaction* locMCReaction, const vector<const DMCThrown*>& locMCThrowns) const;

		virtual void Create_CustomBranches_DataTree(DTreeBranchRegister& locBranchRegister, JEventLoop* locEventLoop, const DReaction* locReaction, bool locIsMCDataFlag) const;
		virtual void Fill_CustomBranches_DataTree(DTreeFillData* locTreeFillData, JEventLoop* locEventLoop, const DReaction* locReaction, const DMCReaction* locMCReaction, const vector<const DMCThrown*>& locMCThrowns,
				const DMCThrownMatching* locMCThrownMatching, const DDetectorMatches* locDetectorMatches,
				const vector<const DBeamPhoton*>& locBeamPhotons, const vector<const DChargedTrackHypothesis*>& locChargedHypos,
				const vector<const DNeutralParticleHypothesis*>& locNeutralHypos, const deque<const DParticleCombo*>& locParticleCombos) const;
    
    string getParticleName(Particle_t particlePid){
        if(particlePid == 11){
            return "KPlus";
        }else if(particlePid == 12){
            return "KMinus";
        }
    }
    
    
private:
    int dInitNumComboArraySize = 100;
    
    //Lets try to get some pulls:
    //****************************************************************************************************************
    mutable map< const JObject*, map<DKinFitPullType, double> > myBeamPullsMap;
    mutable map< const JObject*, map<DKinFitPullType, double> > myChargedPullsMap;
    mutable map< const JObject*, map<DKinFitPullType, double> > myNeutralPullsMap;
     mutable map< const JObject*, map<DKinFitPullType, double> > myPullsMap;
    
    
    void Get_PullsFromFit(const DParticleCombo* particleCombos)const;
    
    //===================================================================
    
    map<DKinFitPullType, double> getBeamPulls(const DBeamPhoton* beamPhoton) const;
    
    //===================================================================
    
    map<DKinFitPullType, double> getChargedPulls(const DChargedTrackHypothesis* chargedTrack) const;
    
    //===================================================================
    
    map<DKinFitPullType, double> getNeutralPulls(const DNeutralParticleHypothesis* neutralTrack) const;
    
    //===================================================================
    
    map<DKinFitPullType, double> getPulls(const DKinematicData* data) const;
    //****************************************************************************************************************
};

#endif //_DEventWriterROOT_someTest_
