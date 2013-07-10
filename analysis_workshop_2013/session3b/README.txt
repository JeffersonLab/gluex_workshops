Purpose:
 - The purpose of this exercise is to familiarize users with particle ID and kinematic fitting. 

Data File:
 - $GLUEX_DATA/session3_and_4_and_5a/rest_60001.hddm (REST file of g, p -> pi+, pi+, pi-, n generated signal data)

Exercise:
 - Using a JANA plugin ("workshop_3b" in this folder) and hd_root:
   - Configure the settings of the DReaction
     - Enable loose cuts on the combined tracking & PID FOMs
     - Enable p4 & vertex kinematic fitting
   - Using the DParticleCombo objects, histogram:
     - PID FOM of pi+'s and pi-'s (separately)
       - Must cast combo particles (DKinematicData) to DChargedTrackHypothesis first!!!
     - Missing Mass before kinematic fit
     - Kinematic Fit Confidence Level
       - Beware kinematic fits that don't converge!! (DParticleCombo::Get_KinFitResults() == NULL)
     - Missing Mass using MEASURED data after placing a 1% confidence level cut on the kinematic fit
     - If time: DC dE/dx FOM of pi+'s and pi-'s (separately)

Helpful Links:
 - Workshop Wiki: https://halldweb1.jlab.org/wiki/index.php/July_2013_Tutorial

 - particleType.h Source (DOxygen): http://www.jlab.org/Hall-D/software/HDSoftware_Documentation/particle_type_8h-source.html
 - particleType.h Reference (DOxygen): http://www.jlab.org/Hall-D/software/HDSoftware_Documentation/particle_type_8h.html

 - DParticleCombo (DOxygen): http://www.jlab.org/Hall-D/software/HDSoftware_Documentation/class_d_particle_combo.html
 - DReaction (DOxygen): http://www.jlab.org/Hall-D/software/HDSoftware_Documentation/class_d_reaction.html

 - DKinematicData (DOxygen): http://www.jlab.org/Hall-D/software/HDSoftware_Documentation/class_d_kinematic_data.html
 - DKinFitResults (DOxygen): http://www.jlab.org/Hall-D/software/HDSoftware_Documentation/class_d_kin_fit_results.html
 - DChargedTrackHypothesis (DOxygen): http://www.jlab.org/Hall-D/software/HDSoftware_Documentation/class_d_charged_track_hypothesis.html

