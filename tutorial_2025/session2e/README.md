# How to Fit With Amptools and Plot the Results Using Python

## :dart: Goals
- Use a simple `.cfg` file to perform mass-independent fits
- Gain intuition of the intensity model by using the `vecps_plotter` for a fit result in one bin
- Aggregate fit results across multiple bins with AmpTools + Python
- Utilize Python to quickly plot fit results across $\omega\pi^0$ mass, and access other information.

## :clipboard: Prerequisites
- [I'll need to have a simple python venv and instructions how to use it]
- [List any prior knowledge, tools, or setup required for this session]
- [Include links to any resources or instructions for setup]

## :file_folder: Materials Provided
- [List of files, scripts, or datasets provided for this session. Large datasets should be located in `/work/halld/gluex_workshop_data/tutorial_2025` and referenced]

## :memo: Session Instructions
### 1. Environment
Before we do anything, let's set up our environment. To run anything we'll need up-to-date versions of `halld_sim` and `AmpTools`, which can be sourced running the following line:
```
source setup_gluex.csh
```
In the terminal, execute `echo $AMPTOOLS_HOME`, `which root` and `which fit` to check that you get the following outputs (ignoring version numbers), respectively:
```
/group/halld/Software/builds/Linux_Alma9-x86_64-gcc11.4.1/amptools/AmpTools-0.15.3
/group/halld/Software/builds/Linux_Alma9-x86_64-gcc11.4.1/root/root-6.24.04/bin/root
/group/halld/Software/builds/Linux_Alma9-x86_64-gcc11.4.1/halld_sim/halld_sim-4.53.0/Linux_Alma9-x86_64-gcc11.4.1/bin/fit
```
#### Halld_sim
AmpTools is written to be fairly generic for anyone interested in doing binned maximum likelihood analysis, so the collaboration has built up a set of libraries and programs over the years in the [halld_sim github](https://github.com/JeffersonLab/halld_sim/tree/master). The two main directories you'll most often consult will likely be [the amplitude analysis programs](https://github.com/JeffersonLab/halld_sim/tree/master/src/programs/AmplitudeAnalysis) and [the corresponding libraries](https://github.com/JeffersonLab/halld_sim/tree/master/src/libraries). As you just saw above, this is where our `fit` program comes from. Halld_sim is of course not limited to just Amplitude Analysis, and you'll find a whole host of other useful tools there. There's no need to deep dive into this repo yet, but it and [AmpTools](https://github.com/mashephe/AmpTools) are good ones to keep bookmarked.

### 2. Binning data
With our event selected data files from the previous sessions in [LINK TO DIR HERE], we can now split our data into independent bins of beam energy $(E_\gamma)$, four-momentum transfer $(-t)$, and  mass $(M_{\omega\pi^0})$ to perform independent fits in. [DESCRIPTION HERE of t-range and mass bins we want to do].

This selection was done using the [copy_tree_with_cuts.cc script](./scripts/copy_tree_with_cuts.C). The data file has been renamed to `anglesOmegaPiAmplitude.root` and the phasespace files to `anglesOmegaPiPhaseSpace.root` and `anglesOmegaPiPhaseSpaceAcc.root`. This is mainly convention, we could name these files whatever so long as they match what's in our .cfg files.

[mention TEM reader and that we want to use this instead to prep for python analysis]

### 3. Running fits

### 4. Analysis of one bin using vecps_plotter

### 5. Multi-bin analysis with python

### Summary 

## :link: Additional Resources
- [TODO: add link here to my analysis, and maybe some presentations to show what can be done with it. Hao also has some really nice studies.]
- [Optional: FAQs or troubleshooting tips]