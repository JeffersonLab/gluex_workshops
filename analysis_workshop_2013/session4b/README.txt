Purpose:
 - The purpose of this exercise is to familiarize users with using analysis actions. 

Data File:
 - $GLUEX_DATA/session3_and_4/rest_60001.hddm (REST file of g, p -> pi+, pi+, pi-, n generated signal data)

Exercise:
 - Using a JANA plugin ("workshop_4b" in this folder) and hd_root:
   - Using the "" script pointed to in the "Helpful Links" section, create custom DAnalysisActions to histogram:
     - The invariant mass of pi+, pi-, pi+
     - A dalitz plot (invariant mass squared of pi+_1 & pi- vs. invariant mass squared of pi+_2 & pi-)
   - Add the custom analysis actions to the DReaction:
     - before the kinematic fit
     - after the kinematic fit confidence level cut: histogram both measured AND kinematic-fit data
     - REMEMBER: when creating two different actions of the same type, give at least one of them a unique (string) name!!!!
 - View these histograms in ROOT
   - Compare the invariant mass & dalitz plots to each other

Helpful Links:
 - Workshop Wiki: https://halldweb1.jlab.org/wiki/index.php/July_2013_Tutorial

 - MakeAnalysisAction.pl: $ANALYSIS_SCRIPTS/MakeAnalysisAction.pl (in svn, located at: trunk/scripts/analysis/)

 - DReaction (DOxygen): http://www.jlab.org/Hall-D/software/HDSoftware_Documentation/class_d_reaction.html
 - DParticleCombo (DOxygen): http://www.jlab.org/Hall-D/software/HDSoftware_Documentation/class_d_particle_combo.html
 - DKinematicData (DOxygen): http://www.jlab.org/Hall-D/software/HDSoftware_Documentation/class_d_kinematic_data.html

 - DHistogramActions.cc Source (DOxygen): http://www.jlab.org/Hall-D/software/HDSoftware_Documentation/_d_histogram_actions_8cc-source.html
 - DHistogramActions.h Source (DOxygen): http://www.jlab.org/Hall-D/software/HDSoftware_Documentation/_d_histogram_actions_8h-source.html

