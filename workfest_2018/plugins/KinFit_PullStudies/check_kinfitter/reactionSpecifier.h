//
//  reactionspecifier.hpp
//  
//
//  Created by Daniel Lersch on 12.04.18.
//

#ifndef reactionSpecifier_h
#define reactionSpecifier_h

#include <stdio.h>
#include <map>
#include <set>
#include <deque>
#include <iostream>

#include "particleType.h"

#include "ANALYSIS/DReaction.h"
#include "ANALYSIS/DReactionStep.h"

using namespace std;

class reactionSpecifier{
    
public:
    reactionSpecifier();
    ~reactionSpecifier();
    
    //Define a reaction:
    //*******************************************************************
    void setupReaction(map<string, string> map, string particle, string status);
    
    //==============================================
    
    DReaction* defineReaction(string reactionName);
    
    //==============================================
    
    void loadReactionStepToReaction(DReaction* reaction, DReactionStep* step, deque<DReactionStep*> pool);
    //*******************************************************************
    
    
    //Adding particles to a reaction:
    //*******************************************************************
    void setupReactionStep(DReactionStep *step, map<string,string> reaction);
    
    //==============================================
    
    void addParticleToReaction(DReactionStep *step, string particleName, string condition);
    //*******************************************************************
    
    
    //Stuff to do other stuff
    //*******************************************************************
     Particle_t setParticle(string particleName);
    //*******************************************************************
    
    
};

#endif /* reactionspecifier_hpp */
