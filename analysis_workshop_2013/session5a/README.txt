Purpose:
 - The purpose of this exercise is to familiarize users with outputing data to ROOT, and using a TSelector. 

Data File:
 - $GLUEX_DATA/session3_and_4_and_5a/rest_60001.hddm (REST file of g, p -> pi+, pi+, pi-, n generated signal data)

Exercise:
 - Using a JANA plugin ("workshop_5a" in this folder) and hd_root:
   - Using DEventWriterROOT, save the surviving particle combos to a ROOT TTree
 - Create a TSelector called "n3pi" from the output TTree
 - Using the TSelector, histogram and view 
   - The invariant mass of pi+, pi-, pi+: using measured & kinfit data
   - A dalitz plot (invariant mass squared of pi+_1 & pi- vs. invariant mass squared of pi+_2 & pi-): using measured & kinfit data

Helpful Links:
 - Workshop Wiki: https://halldweb1.jlab.org/wiki/index.php/July_2013_Tutorial

 - particleType.h Source (DOxygen): http://www.jlab.org/Hall-D/software/HDSoftware_Documentation/particle_type_8h-source.html
 - particleType.h Reference (DOxygen): http://www.jlab.org/Hall-D/software/HDSoftware_Documentation/particle_type_8h.html

 - DReaction (DOxygen): http://www.jlab.org/Hall-D/software/HDSoftware_Documentation/class_d_reaction.html

 - ROOT TTree Format: https://halldweb1.jlab.org/wiki/index.php/Analysis_TTreeFormat

