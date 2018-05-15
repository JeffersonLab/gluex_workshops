#include "DReaction_factory_check_kinfitter.h"

//Copied from Pauls b1pi-example


using namespace std;
//------------------
// init
//------------------
jerror_t DReaction_factory_check_kinfitter::init(void)
{
    rs = new reactionSpecifier();
    
    //Define a bunch of reactions:
    //******************************************************************************************************************
    
    //gp->p K+ K-
    //========================================================================
    DReaction* kpkm_reaction = rs->defineReaction("kpkm_reaction");
    DReactionStep* kpkm_reaction_step = new DReactionStep();
    
    rs->addParticleToReaction(kpkm_reaction_step,"g","beam");
    rs->addParticleToReaction(kpkm_reaction_step,"p","target");
    rs->addParticleToReaction(kpkm_reaction_step,"K+","final");
    rs->addParticleToReaction(kpkm_reaction_step,"K-","final");
    rs->addParticleToReaction(kpkm_reaction_step,"p","final");
    rs->loadReactionStepToReaction(kpkm_reaction,kpkm_reaction_step,dReactionStepPool);
    //========================================================================
   
    //gp->p pi+ pi-
    //========================================================================
    DReaction* pippim_reaction = rs->defineReaction("pippim_reaction");
    DReactionStep* pippim_reaction_step = new DReactionStep();
    
    rs->addParticleToReaction(pippim_reaction_step,"g","beam");
    rs->addParticleToReaction(pippim_reaction_step,"p","target");
    rs->addParticleToReaction(pippim_reaction_step,"pi+","final");
    rs->addParticleToReaction(pippim_reaction_step,"pi-","final");
    rs->addParticleToReaction(pippim_reaction_step,"p","final");
    rs->loadReactionStepToReaction(pippim_reaction,pippim_reaction_step,dReactionStepPool);
    //========================================================================
    
    //gp->p phi [phi-> K+ K-]
    //========================================================================
    DReaction* phikpkm_reaction = rs->defineReaction("phikpkm_reaction");
    DReactionStep* phikpkm_reaction_step = new DReactionStep();
    
    //gp->phi p
    rs->addParticleToReaction(phikpkm_reaction_step,"g","beam");
    rs->addParticleToReaction(phikpkm_reaction_step,"p","target");
    rs->addParticleToReaction(phikpkm_reaction_step,"phi","final");
    rs->addParticleToReaction(phikpkm_reaction_step,"p","final");
    rs->loadReactionStepToReaction(phikpkm_reaction,phikpkm_reaction_step,dReactionStepPool);
    
    //phi->K+ K-
    phikpkm_reaction_step = new DReactionStep();
    rs->addParticleToReaction(phikpkm_reaction_step,"phi","decaying");
    rs->addParticleToReaction(phikpkm_reaction_step,"K+","final");
    rs->addParticleToReaction(phikpkm_reaction_step,"K-","final");
    rs->loadReactionStepToReaction(phikpkm_reaction,phikpkm_reaction_step,dReactionStepPool);
    //========================================================================

    
    //gp->p (pi+) pi- p
    //========================================================================
    DReaction* rhopippim_reaction = rs->defineReaction("pippimMpip_reaction");
    DReactionStep* rhopippim_reaction_step = new DReactionStep();
    
    rs->addParticleToReaction(rhopippim_reaction_step,"g","beam");
    rs->addParticleToReaction(rhopippim_reaction_step,"p","target");
    rs->addParticleToReaction(rhopippim_reaction_step,"pi+","missing");
    rs->addParticleToReaction(rhopippim_reaction_step,"pi-","final");
    rs->addParticleToReaction(rhopippim_reaction_step,"p","final");
    rs->loadReactionStepToReaction(rhopippim_reaction,rhopippim_reaction_step,dReactionStepPool);
    //========================================================================
    
    //gp->p pi+ pi- p P4 Fit only
    //========================================================================
    DReaction* pippimP4_reaction = rs->defineReaction("pippimP4_reaction");
    DReactionStep* pippimP4_reaction_step = new DReactionStep();
    
    rs->addParticleToReaction(pippimP4_reaction_step,"g","beam");
    rs->addParticleToReaction(pippimP4_reaction_step,"p","target");
    rs->addParticleToReaction(pippimP4_reaction_step,"pi+","final");
    rs->addParticleToReaction(pippimP4_reaction_step,"pi-","final");
    rs->addParticleToReaction(pippimP4_reaction_step,"p","final");
    rs->loadReactionStepToReaction(pippimP4_reaction,pippimP4_reaction_step,dReactionStepPool);
    //========================================================================
    
    
    //gp->p pi+ pi- p Vertex Fit only
    //========================================================================
    DReaction* pippimVtx_reaction = rs->defineReaction("pippimVtx_reaction");
    DReactionStep* pippimVtx_reaction_step = new DReactionStep();
    
    rs->addParticleToReaction(pippimVtx_reaction_step,"g","beam");
    rs->addParticleToReaction(pippimVtx_reaction_step,"p","target");
    rs->addParticleToReaction(pippimVtx_reaction_step,"pi+","final");
    rs->addParticleToReaction(pippimVtx_reaction_step,"pi-","final");
    rs->addParticleToReaction(pippimVtx_reaction_step,"p","final");
    rs->loadReactionStepToReaction(pippimVtx_reaction,pippimVtx_reaction_step,dReactionStepPool);
    //========================================================================
    
    //******************************************************************************************************************
    
    //Do your analysis here:
    //******************************************************************************************************************
    //gp-> p K+ K-
    
    kpkm_reaction->Set_KinFitType(d_P4AndVertexFit);
    kpkm_reaction->Set_NumPlusMinusRFBunches(0);
    kpkm_reaction->Enable_TTreeOutput("kpkm_check_kinfitter.root");
    kpkm_reaction->Set_MaxExtraGoodTracks(4);
    
    pippim_reaction->Set_KinFitType(d_P4AndVertexFit);
    pippim_reaction->Set_NumPlusMinusRFBunches(0);
    pippim_reaction->Enable_TTreeOutput("pippim_check_kinfitter.root");
    pippim_reaction->Set_MaxExtraGoodTracks(4);
    
    
    //gp-> p phi [phi-> K+ K-]
    phikpkm_reaction->Set_KinFitType(d_P4AndVertexFit);
    phikpkm_reaction->Set_NumPlusMinusRFBunches(0);
    phikpkm_reaction->Enable_TTreeOutput("phikpkm_check_kinfitter.root");
    phikpkm_reaction->Set_MaxExtraGoodTracks(4);
    
    //gp-> (pi+) pi- p
    rhopippim_reaction->Set_KinFitType(d_P4AndVertexFit);
    rhopippim_reaction->Set_NumPlusMinusRFBunches(0);
    rhopippim_reaction->Enable_TTreeOutput("pippimMpip_check_kinfitter.root");
    rhopippim_reaction->Set_MaxExtraGoodTracks(4);
    
    //gp-> pi+ pi- p P4 fit only
    pippimP4_reaction->Set_KinFitType(d_P4Fit);
    pippimP4_reaction->Set_NumPlusMinusRFBunches(0);
    pippimP4_reaction->Enable_TTreeOutput("pippimP4_check_kinfitter.root");
    pippimP4_reaction->Set_MaxExtraGoodTracks(4);
    
    //gp-> pi+ pi- p Vertex fit only
    pippimVtx_reaction->Set_KinFitType(d_VertexFit);
    pippimVtx_reaction->Set_NumPlusMinusRFBunches(0);
    pippimVtx_reaction->Enable_TTreeOutput("pippimVtx_check_kinfitter.root");
    pippimVtx_reaction->Set_MaxExtraGoodTracks(4);
    
    //******************************************************************************************************************
    
    
    //Do some analysis here:
    //******************************************************************************************************************
    pippim_reaction->Add_AnalysisAction(new DCutAction_CombinedPIDFOM(pippim_reaction, 0.01));
    kpkm_reaction->Add_AnalysisAction(new DCutAction_CombinedPIDFOM(kpkm_reaction, 0.01));
    phikpkm_reaction->Add_AnalysisAction(new DCutAction_CombinedPIDFOM(phikpkm_reaction, 0.01));
    rhopippim_reaction->Add_AnalysisAction(new DCutAction_CombinedPIDFOM(rhopippim_reaction, 0.01));
    pippimP4_reaction->Add_AnalysisAction(new DCutAction_CombinedPIDFOM(pippimP4_reaction, 0.01));
    pippimVtx_reaction->Add_AnalysisAction(new DCutAction_CombinedPIDFOM(pippimVtx_reaction, 0.01));
    //******************************************************************************************************************
    
    
    _data.push_back(kpkm_reaction); //Register the DReaction
    _data.push_back(pippim_reaction);
    _data.push_back(phikpkm_reaction);
    _data.push_back(rhopippim_reaction);
    _data.push_back(pippimP4_reaction);
    _data.push_back(pippimVtx_reaction);
    
	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t DReaction_factory_check_kinfitter::brun(jana::JEventLoop *eventLoop, int32_t runnumber)
{
	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t DReaction_factory_check_kinfitter::evnt(JEventLoop *loop, uint64_t eventnumber)
{
	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t DReaction_factory_check_kinfitter::erun(void)
{
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t DReaction_factory_check_kinfitter::fini(void)
{
	for(size_t loc_i = 0; loc_i < dReactionStepPool.size(); ++loc_i)
		delete dReactionStepPool[loc_i];
	return NOERROR;
}

