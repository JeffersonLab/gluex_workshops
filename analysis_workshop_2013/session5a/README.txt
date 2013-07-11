Purpose:
 - The purpose of this exercise is to familiarize users with outputing data to ROOT, and using a TSelector. 

Data File:
 - $GLUEX_DATA/session3_and_4_and_5a/rest_60001.hddm (REST file of g, p -> pi+, pi+, pi-, n generated signal data)

Exercise:
 - Using a JANA plugin ("workshop_s5a" in this folder) and hd_root:
   - Using DEventWriterROOT, save the surviving particle combos to a ROOT TTree
 - Create a TSelector called "n3pi" (n3pi.C, n3pi.h) from the output TTree (call the following:)
   - root -l tree_workshop_s5a.root
   - workshop_s5a_Tree->MakeSelector("n3pi");
 - Editing the TSelector, create & fill histograms for:
   - The invariant mass of pi+, pi-, pi+: using measured & kinfit data
   - A dalitz plot (invariant mass squared of pi+_1 & pi- vs. invariant mass squared of pi+_2 & pi-): using measured & kinfit data
 - To execute the TSelector on your tree:
   - root -l tree_workshop_s5a.root
   - workshop_s5a_Tree->Process("n3pi.C");

Helpful Links:
 - Workshop Wiki: https://halldweb1.jlab.org/wiki/index.php/July_2013_Tutorial

 - particleType.h Source (DOxygen): http://www.jlab.org/Hall-D/software/HDSoftware_Documentation/particle_type_8h-source.html
 - particleType.h Reference (DOxygen): http://www.jlab.org/Hall-D/software/HDSoftware_Documentation/particle_type_8h.html

 - DReaction (DOxygen): http://www.jlab.org/Hall-D/software/HDSoftware_Documentation/class_d_reaction.html

 - ROOT TTree Format: https://halldweb1.jlab.org/wiki/index.php/Analysis_TTreeFormat

