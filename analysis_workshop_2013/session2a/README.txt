Purpose:
 - The purpose of these exercises is to familiarize users with using hd_dump and the basics of JANA.  

Data File:
 - $GLUEX_DATA/session2/hdgeant_smeared.hddm (pythia output)

Exercise 1:
 - Using hd_dump, print the thrown monte-carlo particle data (DMCThrown) from the first event to screen.  
   - Print the results using three different DMCThrown factories: the default factory (all), the tag="FinalState" factory, and the tag="Decaying" factory.

Exercise 2:
 - Using a JANA plugin and hd_root, print PID, q, p, theta (convert to degrees!), phi (convert to degrees!), and vertex-z to screen for all of the "FinalState" DMCThrown objects.
   - The "workshop_s2a" plugin has already been created in this folder; just modify the contents of its evnt() method and compile it. 
   - Note that DMCThrown inherits from DKinematicData
   - Only print the results from the first event in the file.  
 - Compare the results to the hd_dump "FinalState" factory tag output, and confirm that they agree.  

Helpful Links:
 - Workshop Wiki: https://halldweb1.jlab.org/wiki/index.php/July_2013_Tutorial
 - C++ vectors: http://www.cplusplus.com/reference/vector/vector/
 - DMCThrown (DOxygen): http://www.jlab.org/Hall-D/software/HDSoftware_Documentation/class_d_m_c_thrown.html
 - DKinematicData (DOxygen): http://www.jlab.org/Hall-D/software/HDSoftware_Documentation/class_d_kinematic_data.html

 - GlueX Libraries Source Code: on virtual machine at: $HALLD_HOME/src/libraries/
