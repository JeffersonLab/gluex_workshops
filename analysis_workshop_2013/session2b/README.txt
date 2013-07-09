Purpose:
 - The purpose of these exercises is to familiarize users with performing track reconstruction and looking at basic DANA particle objects.

Data File:
 - $GLUEX_DATA/session2/hdgeant_smeared.hddm (pythia output)

Exercise 1:
 - Using hd_dump, print the DChargedTrackHypothesis, DNeutralShower, and DBeamPhoton objects from the first event to screen.  
   - Print the results using their default factories

Exercise 2:
 - Using a JANA plugin, print the following objects to screen for the first event in the file:
   - DChargedTrackHypothesis: PID, q, p, theta, phi, and vertex-z to screen for all of the "FinalState" DMCThrown objects.
   - DNeutralShower: Shower energy
   - DBeamPhoton: Beam energy
 - The plugin has already been setup; just modify the contents of the evnt() method.  
   - Note that DChargedTrackHypothesis & DBeamPhoton inherit from DKinematicData
 - Compare the results to the hd_dump output, and confirm that they agree.  

Helpful Links:
 - Workshop Wiki: https://halldweb1.jlab.org/wiki/index.php/July_2013_Tutorial
 - DNeutralShower (DOxygen): http://www.jlab.org/Hall-D/software/HDSoftware_Documentation/class_d_neutral_shower.html
 - DBeamPhoton (DOxygen): http://www.jlab.org/Hall-D/software/HDSoftware_Documentation/class_d_beam_photon.html
 - DChargedTrackHypothesis (DOxygen): http://www.jlab.org/Hall-D/software/HDSoftware_Documentation/class_d_charged_track_hypothesis.html
 - DKinematicData (DOxygen): http://www.jlab.org/Hall-D/software/HDSoftware_Documentation/class_d_kinematic_data.html

