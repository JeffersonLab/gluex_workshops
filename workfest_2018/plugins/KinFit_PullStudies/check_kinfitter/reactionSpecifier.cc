//
//  reactionspecifier.cpp
//  
//
//  Created by Daniel Lersch on 12.04.18.
//

#include "reactionSpecifier.h"

using namespace std;

//BLA BLA
//*******************************************************************
reactionSpecifier::reactionSpecifier(){

}

//==============================================

reactionSpecifier::~reactionSpecifier(){
    
}
//*******************************************************************


//Define a simple reaction:
//*******************************************************************
void reactionSpecifier::setupReaction(map<string, string> mymap, string particle, string status){
    cout <<"Adding particle: "<<particle<<" with condition: "<<status<< endl;
    mymap[particle] = status;
}

//==============================================

DReaction* reactionSpecifier::defineReaction(string reactionName){
    DReaction *out = new DReaction(reactionName);
    return out;
}

//==============================================

void reactionSpecifier::loadReactionStepToReaction(DReaction* reaction, DReactionStep* step, deque<DReactionStep*> pool){
    reaction->Add_ReactionStep(step);
    pool.push_back(step);
}
//*******************************************************************



//Add particles to a reaction:
//*******************************************************************
void reactionSpecifier::setupReactionStep(DReactionStep *step, map<string,string> reaction){
    step = new DReactionStep();
    
    if(reaction.empty())cout <<"bekackt!"<< endl;
    
    //-------------------------------------------------
    while(!reaction.empty()){
        string fname = reaction.begin()->first;
        string sname = reaction.begin()->second;
        
        cout <<fname<<"  "<<sname<< endl;
        
        addParticleToReaction(step,fname,sname);
        reaction.erase(reaction.begin());
    }
    //-------------------------------------------------
}

//==============================================

void reactionSpecifier::addParticleToReaction(DReactionStep *step, string particleName, string condition){
    Particle_t particle = setParticle(particleName);
    
    if(condition == "beam" || condition == "decaying"){
        step->Set_InitialParticleID(particle);
    }else if(condition == "target"){
        step->Set_TargetParticleID(particle);
    }else if(condition == "missing"){
        step->Add_FinalParticleID(particle, true);
    }else if(condition == "final"){
        step->Add_FinalParticleID(particle);
    }else cout <<"Sorry. Wrong input."<< endl;
}
//*******************************************************************


//Stuff to do other stuff:
//*******************************************************************
Particle_t reactionSpecifier::setParticle(string particleName){
    Particle_t out = Proton;
    
    if(particleName == "p"){
        out = Proton;
    }else if(particleName == "ap"){
        out = AntiProton;
    }else if(particleName == "e-"){
        out = Electron;
    }else if(particleName == "e+"){
        out = Positron;
    }else if(particleName == "pi+"){
        out = PiPlus;
    }else if(particleName == "pi-"){
        out = PiMinus;
    }else if(particleName == "K+"){
        out = KPlus;
    }else if(particleName == "K-"){
        out = KMinus;
    }else if(particleName == "g"){
        out = Gamma;
    }else if(particleName == "pi0"){
        out = Pi0;
    }else if(particleName == "omega"){
        out = omega;
    }else if(particleName == "rho0"){
        out = Rho0;
    }else if(particleName == "rho+"){
        out = RhoPlus;
    }else if(particleName == "rho-"){
        out = RhoMinus;
    }else if(particleName == "phi"){
        out = phiMeson;
    }else cout <<"Sorry. You did not eneter a known particle."<< endl;
    
    return out;
}
//*******************************************************************







