Purpose:
 - The purpose of this exercise is to familiarize users with the creation of DReaction's and the concept and content of DParticleCombo.

Data File:
 - $GLUEX_DATA/session3_and_4_and_5a/rest_60001.hddm (REST file of g, p -> pi+, pi+, pi-, n generated signal data)

Exercise:
 - Using a JANA plugin ("workshop_3a" in this folder) and hd_root:
   - Create a DReaction for g, p -> (n), pi+, pi-, pi+
   - Using the DParticleCombo objects, histogram:
     - # of Particle Combinations for each event
     - Missing mass
     - pi+, pi-, pi+ Invariant Mass
     - If time: p vs. theta of pi+'s and pi-'s (separately)
     - If time: vertex-z of pi+'s and pi-'s (separately)

Helpful Links:
 - Workshop Wiki: https://halldweb1.jlab.org/wiki/index.php/July_2013_Tutorial

 - particleType.h Source (DOxygen): http://www.jlab.org/Hall-D/software/HDSoftware_Documentation/particle_type_8h-source.html
 - particleType.h Reference (DOxygen): http://www.jlab.org/Hall-D/software/HDSoftware_Documentation/particle_type_8h.html

 - DReaction (DOxygen): http://www.jlab.org/Hall-D/software/HDSoftware_Documentation/class_d_reaction.html
 - DParticleCombo (DOxygen): http://www.jlab.org/Hall-D/software/HDSoftware_Documentation/class_d_particle_combo.html

 - DKinematicData (DOxygen): http://www.jlab.org/Hall-D/software/HDSoftware_Documentation/class_d_kinematic_data.html

