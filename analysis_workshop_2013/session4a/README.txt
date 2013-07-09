Purpose:
 - The purpose of this exercise is to familiarize users with using analysis actions. 

Data File:
 - $GLUEX_DATA/session3_and_4_and_5a/rest_60001.hddm (REST file of g, p -> pi+, pi+, pi-, n generated signal data)

Exercise:
 - Using a JANA plugin ("workshop_4a" in this folder) and hd_root:
   - Add analysis actions to the DReaction to make histograms & place cuts:
     - REMEMBER: when creating two different actions of the same type, give at least one of them a unique (string) name!!!!
     - histogram PID & True PID
     - place a 1% cut on the combined PID FOM
     - histogram kinematic fit results
     - place a 1% cut on the kinematic fit confidence level
     - histogram missing mass: before and after the kinematic fit confidence level cut
     - histogram particle combon kinematics: both at the beginning and at the end
   - Create and execute reaction-independent actions in the event processor to histogram:
     - thrown & detected particle kinematics
     - track multiplicity for the event
     - comparison between generated and reconstructed data
 - View these histograms in ROOT
   - Look at all of the histograms; compare the distributions of the missing mass before and after cutting on the kinematic fit confidence level
   - Determine approximately what % of the remaining events & particle combinations were removed by each cut

Helpful Links:
 - Workshop Wiki: https://halldweb1.jlab.org/wiki/index.php/July_2013_Tutorial

 - DReaction (DOxygen): http://www.jlab.org/Hall-D/software/HDSoftware_Documentation/class_d_reaction.html

 - DHistogramActions.h Reference (DOxygen): http://www.jlab.org/Hall-D/software/HDSoftware_Documentation/_d_histogram_actions_8h.html
 - DHistogramActions.h Source (DOxygen): http://www.jlab.org/Hall-D/software/HDSoftware_Documentation/_d_histogram_actions_8h-source.html

 - DCutActions.h Reference (DOxygen): http://www.jlab.org/Hall-D/software/HDSoftware_Documentation/_d_cut_actions_8h.html
 - DCutActions.h Source (DOxygen): http://www.jlab.org/Hall-D/software/HDSoftware_Documentation/_d_cut_actions_8h-source.html

