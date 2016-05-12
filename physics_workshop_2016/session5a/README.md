#!/bin/csh -f
goto test

Session 5a: Fitting SDMEs
============

You can just `./README.md` to run the example from beginning to end - but you should go step by step!

The goal of this session is to generate omega->3pi events with the angular distribution presented
in Schilling, et al., Nucl. Phy. B, 15 397 (1970) using the AmpTools framework. These input data
are then be processed through sim-recon to model experimental acceptance and reconstruction
efficiency. Finally, an acceptance corrected fit is preformed on the data using AmpTools to extract the SDMEs.

We have already done most of the heavy lifting. One million omega->3pi events have been generated,
simulated, smeared and reconstructed. The results at the end of this process in a format suitable for 
Amptools can be found in the $WORKSHOP/session5a/amtoolstrees/ directory.

As an example, let's generate just 25 events and take all the steps necessary to produce AmpTools input files.

First, we will use gen_omega_3pi to determine the thrown 4-vectors. You can run `gen_omega_3pi -h` to
print the command line options to screen. Use the following command to generate 100 events, saving the
4-vectors to both hddm format for input to hdgeant, and AmpTools root format.

    gen_omega_3pi -c $WORKSHOP/session5a/gen_omega_3pi.cfg -o AmpToolsFormatThrown.root -hd HDDMFormatThrown.hddm -n 25 -r 10000

Now, we want to simulate the passage of the thrown particles through the GlueX detector. The configuration
for this step is contained in the 'control.in' file in this directory and is loaded automatically by hdgeant. 
We want to use the ccdb variation 'mc_sim1', so we will set our environment, then launch hdgeant.

on VM (bash):
    export JANA_CALIB_CONTEXT="variation=mc_sim1"
on ifarm (csh):
    setenv JANA_CALIB_CONTEXT "variation=mc_sim1"

    hdgeant

Next we need to smear the output of hdgeant to simulate the detector response.

    mcsmear hdgeant.hddm

We can look at these smeared events on the viewer

    hdview2 hdgeant_smeared.hddm

Now we can reconstruct the smeared events, and produce some generic histograms to look at results of reconstruction.

    hd_root hdgeant_smeared.hddm -PPLUGINS=danarest,monitoring_hists

There are two output files at the end of this step: 'hd_root.root' and 'danarest.hddm'. We will look at the root
file later to view various histograms related to the quality of the resonstruction. Now we will produce a root tree 
using the very loose cuts developed in session 2.

    hd_root dana_rest.hddm -PPLUGINS=omega_ref

The next step is to select events from the root file that are consistent with omega->3pi events. This is done
using the DSelector you developed in session 3b (solutions are in this folder. This can be run over the data with 
the following command.

    root -l tree_omega_skim.root ${ROOT_ANALYSIS_HOME}/scripts/Load_DSelector.C
    [0] omega_Tree->Process(gSystem->ExpandPathName("${WORKSHOP}/session5a/DSelector_p3pi_workshop.C+"))

Finally, we need to convert the tree passing all of our cuts to a format suitable for AmpTools. To do this you can
issue the following command

    tree_to_amptools tree_p3pi_omega.root omega_Tree

We can look at some of the results for the full 1M event data set:

    cd $WORKSHOP/session5a/sim_1M_omega/histograms/
    root -l *.root
    cd $WORKSHOP/session5a/

This includes a sample of 187.5 million events from sim1 with the omega signal removed. It is clear from these plots that the ammount of background
that would enter the fit is small. For this reason, we won't account for it in our fit (DONT DO THIS IN YOUR ANALYSIS).

You now have the input file suitable for use in AmpTools. At this point you have gone through all of the steps before the 
final fit. We have already processed 1M events for you to use in a fit. This includes a sample with the Schilling angular
distributions, and a seperate sample flat in angles to be used for the acceptance correction. The fit configuration file 
is found in 'fit_omega_3pi.cfg'. The following command will fit the data.

    fit -c fit_omega_3pi.cfg

Now you can adjust the parameters for the simulation, and the fit. Its a toy model, so play around!

Now that you have completed the exercise, you can always cheat if you want to do it again:

    ./README.md

test:

# Make some utility scripts
    echo '{ TBrowser g; }' > ! tbr.C
    echo '{ omega_Tree->Process(gSystem->ExpandPathName("${WORKSHOP}/session5a/DSelector_p3pi_workshop.C+"));}' > ! RunDSelector.C

# Run the code
    gen_omega_3pi -c ${WORKSHOP}/session5a/gen_omega_3pi.cfg -o AmpToolsFormatThrown.root -hd HDDMFormatThrown.hddm -n 25 -r 10000
    setenv JANA_CALIB_CONTEXT "variation=mc_sim1"
    hdgeant
    mcsmear hdgeant.hddm -PTHREAD_TIMEOUT_FIRST_EVENT=300 -PTHREAD_TIMEOUT=300
    hdview2 hdgeant_smeared.hddm
    hd_root hdgeant_smeared.hddm -PPLUGINS=danarest,monitoring_hists -PTHREAD_TIMEOUT_FIRST_EVENT=300 -PTHREAD_TIMEOUT=300
    hd_root -PPLUGINS=omega_ref dana_rest.hddm
    root -l -q tree_omega_skim.root ${ROOT_ANALYSIS_HOME}/scripts/Load_DSelector.C RunDSelector.C
    tree_to_amptools tree_p3pi_omega.root omega_Tree
    fit -c fit_omega_3pi.cfg

# Open a browser
    cd $WORKSHOP/session5a/sim_1M_omega/histograms/
    root -l *.root
    cd $WORKSHOP/session5a/

# Remove the scripts
    rm tbr.C
    rm RunDSelector.C

exit
