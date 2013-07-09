Purpose:
 - The purpose of this exercise is for users to combine everything they've learned to do a full DANA analysis. 

Data Files:
 - $GLUEX_DATA/session5b/dana_rest_skim_*.hddm (skimmed REST files of g, p -> K*0, K0, p; K*0 -> K+, pi-; K0 -> pi+, pi- generated signal data)

Exercise:
 - Create a JANA plugin (using the "MakeReactionPlugin.pl" script linked in "Helpful Links") to:
   - Do a full analysis of the reaction:
 
    g, p -> K*0, K0, (p)
        K*0 -> K+, pi-
           K0 -> pi+, pi-

   - This entails:
     - performing a vertex-p4 kinematic fit
     - histogramming kinematics, pid, kinematic fit results, missing/invariant mass peaks before & after kinematic fit
     - cutting on the combined pid FOM & the kinematic fit FOM
     - filling the reaction-independent histograms (thrown & detected kinematics, track multiplicity, gen-recon comparison)
     - saving the results to an output ROOT TTree
 - View the output histograms and TTree in ROOT: compare mass histograms before/after kinematic fit, etc.

Helpful Links:
 - Workshop Wiki: https://halldweb1.jlab.org/wiki/index.php/July_2013_Tutorial
 - MakeReactionPlugin.pl: $ANALYSIS_SCRIPTS/MakeAnalysisAction.pl (in svn, located at: trunk/scripts/analysis/)

 - DHistogramActions.h Reference (DOxygen): http://www.jlab.org/Hall-D/software/HDSoftware_Documentation/_d_histogram_actions_8h.html
 - DHistogramActions.h Source (DOxygen): http://www.jlab.org/Hall-D/software/HDSoftware_Documentation/_d_histogram_actions_8h-source.html

 - DCutActions.h Reference (DOxygen): http://www.jlab.org/Hall-D/software/HDSoftware_Documentation/_d_cut_actions_8h.html
 - DCutActions.h Source (DOxygen): http://www.jlab.org/Hall-D/software/HDSoftware_Documentation/_d_cut_actions_8h-source.html

 - DReaction (DOxygen): http://www.jlab.org/Hall-D/software/HDSoftware_Documentation/class_d_reaction.html
 - ROOT TTree Format: https://halldweb1.jlab.org/wiki/index.php/Analysis_TTreeFormat

