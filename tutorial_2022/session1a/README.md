Session1a: Kinematic Fitting
============================

This session gives a very brief introduction to the kinematic fitting technique and how it is used in GlueX.
The slides dedicated to this session can be found in the folder "talk".

The folder "Scripts" contains four sub-folders, each representing a small example analysis of gp->pi+pi-pi0 p final states via 
a P4- / P4-Vertex kinematic fit. 

Example 1: Comparing reconstructed and fitted Observables
---------------------------------------------------------

When using a kinematic fit, it is very important to compare the kinematically fitted distributions to their
reconstructed counter part. This allows you to quickly check if the fitter is working properly or not. In this example, the proton missing mass,
three pion invariant mass and overall missing momentum / energy are calculated from the reconstucetd and fitted particle 4 momenta. 

Enter the sub-folder "run_example1" and execute the script:

root -x "run_ex1.C(val,name)"

where val can be any number between 0.0 and 1.0. "name" is a string which specifies the output name. 
After running this script, you will find four .png files: (where "name" takes the value that you just specified)

Mx_p_vs_IM_pippimgg_rec_name.png
Px_p_vs_Ex_rec_name.png
Mx_p_vs_IM_pippimgg_fit_name.png
Px_p_vs_Ex_fit_name.png

The files "Mx_p_vs_IM_pippimgg" show the proton missing mass as a function of the three pion final state invariant mass. 
The ending "_rec" or "_fit" indicates whether the reconstructed or kinematically fitted 4 momenta were used. 

The file "Px_vs_Ex" dispaly the reconstructed / fitted missing momentum as a function of the missing energy. 

Example 2: Monitoring the chi^2 and Probability Distribution
-------------------------------------------------------------

One of the very first things to check when using a kinematic fitter are the chi^2 and probability 
distributions. They might indicate right away that something is or is not working properly. This example comapares these distributions
for different probability cuts. Additonally, the effect of varying the probability is shown with respect to the three pion invariant mass.

Enter the sub-folder "run_example2" and execute the script:

root -x "run_ex2.C(idx)"

where idx is an inetger and can be either 0 or 1. This script will prodcue three pdf files, 
showing the kinematic fitprobability and chi^2, as well as the three pion invariant mass for different cuts on the kinematic fit probability. 

The results shown are obtained from a P4-Fit if idx = 0 and from a P4-Vertex-Fit, if idx = 1.

Example 3: Inspecting pi+ and pi- Pz-Pulls
------------------------------------------

The analysis of pull distributions helps to understand the "behavior" of the kinematic fit with respect to a given particle type 
or observable. This example focusses on the Pz-Pull of pi- / pi+ particles. 

Enter the sub-folder "run_example3" and execute the script:

root -x "run_ex3.C(idx)"

where idx is an inetger and can be either 0 or 1. Like in the previous example, this integer indicates if the results from a P4- or P4-Vertex fit are shown.
This script produces several .pdf files which compare the pi+ / pi- Pz-pull distributions under different conditions. 

Example 4: Run a Kinematic Fit Probability Scan
-----------------------------------------------

A frequntly asked question is: "Which probability cut should I choose for my analysis?" This example tries to highlight the idea behind a 
probability scan.

Enter the sub-folder "run_example4" and execute the script:

root -x "run_ex4.C()"

This scripts does not require any arguments. When executed, it will perform several cuts on the kinematic fit probability and monitor the
phi->pi+pi-pi0 yield as a function of this cut. This is done by fitting the corresponding three pion invariant mass 
with a gaussian (phi-signal) and a 3rd order polynomial (background). The results of this scan are:

i) The signal over background ratio as a function of the chosen probability cut

ii) The significance as a function of the chosen probability cut.

Ideally, the signifcane shows a maximum (best trade-off between background removal and signal loss) for a given probabiltiy cut. 


Data sets
---------

The .root files that are used for these examples are located at: /lustre19/expphy/volatile/halld/home/dlersch/KinFit_Workshop_2021_Data.
Each file contains a flat tree with gp->pi+ pi- pi0 p events.

