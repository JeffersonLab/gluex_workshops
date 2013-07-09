Purpose:
 - The purpose of these exercises is to familiarize users with performing track reconstruction and looking at basic DANA particle objects.

Data File:
 - $GLUEX_DATA/session2/hdgeant_smeared.hddm (pythia output)

Exercise 1:
 - Using hd_root and the danarest plugin, perform track reconstruction on the provided pythia data (output file will be dana_rest.hddm).

Exercise 2:
 - Using hd_dump on the newly created REST file, print the DChargedTrackHypothesis, DNeutralShower, and DBeamPhoton objects from the first event to screen.  
   - Print the results using their default factories

Exercise 3:
 - Using a JANA plugin and hd_root on the newly created REST file, print the following objects to screen for the first event in the file:
   - DChargedTrackHypothesis: PID, q, p, theta, phi, and vertex-z
   - DNeutralShower: Shower energy
   - DBeamPhoton: Beam energy
 - The "workshop_2b" plugin has already been created in this folder; just modify the contents of its evnt() method and compile it. 
   - Note that DChargedTrackHypothesis & DBeamPhoton inherit from DKinematicData
 - Compare the results to the hd_dump output, and confirm that they agree.  

Helpful Links:
 - Workshop Wiki: https://halldweb1.jlab.org/wiki/index.php/July_2013_Tutorial
 - DNeutralShower (DOxygen): http://www.jlab.org/Hall-D/software/HDSoftware_Documentation/class_d_neutral_shower.html
 - DBeamPhoton (DOxygen): http://www.jlab.org/Hall-D/software/HDSoftware_Documentation/class_d_beam_photon.html
 - DChargedTrackHypothesis (DOxygen): http://www.jlab.org/Hall-D/software/HDSoftware_Documentation/class_d_charged_track_hypothesis.html
 - DKinematicData (DOxygen): http://www.jlab.org/Hall-D/software/HDSoftware_Documentation/class_d_kinematic_data.html

