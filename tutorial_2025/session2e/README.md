# How to Fit With Amptools and Plot the Results Using Python
:warning: This repo must be cloned to an `ifarm` node to ensure access to software and data files :warning:

## :dart: Goals
- Use a simple `.cfg` file to perform mass-independent fits
- Gain intuition of the intensity model by using the `vecps_plotter` for a fit result in one bin
- Aggregate fit results across multiple bins with AmpTools + Python
- Utilize Python to quickly plot fit results across $\omega\pi^0$ mass, and access other information.

## :memo: Session Instructions
If you are already familiar with amplitude analysis, then after you perform sections 1-3, you can skip to [section 5](#5-multi-bin-analysis-with-python) for how to use python, but note that if you want to run the scripts within this repo you will need to perform the fits detailed in [section 4](#4-analysis-of-one-bin-using-vecps_plotter) to obtain `.fit` files.
### 1. Environment
#### $\color{Turquoise}\sqrt{~}$ AmpTools and ROOT
Before we do anything, let's set up our environment. If you followed the main [README](../README.md), you have already setup `halld_sim` and `AmpTools` by running (from this directory):
```
source ../setup_gluex.csh
```
and ensuring that the setup is verified via:
```
bash verify_setup.sh
```

To run the python scripts, we'll need access to the AmpTools libraries from ROOT. To do this, copy the `.rootrc` file to your home directory with
```bash
cp .rootrc ~/
```
This file simply adds a path that will allow us to utilize the AmpTools `FitResults` class. Execute `root` in a terminal window, then run `.x loadAmpTools.C` in the ROOT session. If this is successful, you should see the following output:
```
--------------------------------------------------
------      Loading AmpTools Modules        ------
--------------------------------------------------
Loading IUAmpTools/report.cc..
Loading IUAmpTools/Kinematics.cc..
Loading IUAmpTools/ConfigurationInfo.cc..
Loading IUAmpTools/ConfigFileParser.cc..
Loading IUAmpTools/NormIntInterface.cc..
Loading IUAmpTools/FitResults.cc..
--------------------------------------------------
------  Finished Loading AmpTools Modules   ------
--------------------------------------------------
```

#### :snake: Python
We will be using very basic python virtual environments (venv) to keep our package versions consistent. At the time of writing this, the default version of python on the ifarm `3.9.21`. Starting in the parent `PyAmpPlots` folder, execute
```
python -m venv .venv
``` 
to create a virtual environment with the same python version as the ifarm default. Activate the environment by running 
```
source .venv/bin/activate
``` 
(use `activate.csh` for csh/tcsh shells). With our new virtual environment, we can now download the required python packages to perform our analysis. Run
```
python -m pip install -r requirements.txt
```
to download these packages to the virtual environment.

#### Halld_sim
AmpTools is written to be fairly generic for anyone interested in doing binned maximum likelihood analysis, so the collaboration has built up a set of libraries and programs over the years in the [halld_sim github](https://github.com/JeffersonLab/halld_sim/tree/master). The two main directories you'll most often consult will likely be [the amplitude analysis programs](https://github.com/JeffersonLab/halld_sim/tree/master/src/programs/AmplitudeAnalysis) and [the corresponding libraries](https://github.com/JeffersonLab/halld_sim/tree/master/src/libraries). As you just saw above, this is where our `fit` program comes from. Halld_sim is of course not limited to just Amplitude Analysis, and you'll find a whole host of other useful tools there. There's no need to deep dive into this repo yet, but it and [AmpTools](https://github.com/mashephe/AmpTools) are good ones to keep bookmarked.

### 2. Binning Data
We'll be using the data file produced by the DSelector from the previous session, which is stored in `/work/halld/gluex_workshop_data/tutorial_2025/session2e`, along with the corresponding phasespace files labelled as `anglesOmegaPiPhaseSpace.root` and `anglesOmegaPiPhaseSpaceAcc.root` for the generated and accepted phasespace, respectively. For faster fits and compatability with our python scripts later, we'll split our files into bins of 
beam energy $(E_\gamma)$, four-momentum transfer $(-t)$, and  mass $(M_{\omega\pi^0})$ to perform independent fits in. 
- $8.2 < E_\gamma < 8.8$: coherent peak region
- $0.1 < -t < 0.2$: Nothing special about this region, just done to demonstrate binning in $-t$
- 10 bins from $1.1 < M_{\omega\pi^0} < 1.3$: we'll perform 10 independent fits in these 10 mass bins

This selection was done using the [copy_tree_with_cuts.cc script](./scripts/copy_tree_with_cuts.C). The data file has been renamed to `anglesOmegaPiAmplitude.root` and the phasespace files to `anglesOmegaPiPhaseSpace.root` and `anglesOmegaPiPhaseSpaceAcc.root`. This is mainly convention, we could name these files whatever so long as they match what's in our .cfg files.

### 3. Fits
#### The config file
The configuration file [`fit.cfg`](./fit.cfg) is the main file we'll use to actually run the fit. Whenever you want to change the waveset (the amplitudes you're fitting with) this is the file to edit. Conveniently the [halld_sim AmpTools libraries](https://github.com/JeffersonLab/halld_sim/tree/master/src/libraries/AMPTOOLS_AMPS) have plenty of amplitudes that others have written. This is why we can specify the `Vecp_ps_refl` amplitude in the config file and it will already know what to do.

#### Running the fit
Now we're finally at the point where we can start running some actual fits! `cd` into MASS BIN and run the following from the terminal:
```
fit -c ../fit.cfg -m 100000 > log.out
```
The `-c` flag specifies what .cfg file we want to use, and `-m` determines the maximum number of fit iterations before it gives up. Fits can output thousands of lines, so we've piped it to a `log.out` file. You can see what other options are available by running
```
fit -h
```
When the fit completes you should have the following:
- `log.out`
  - The output of the AmpTools fit. Describes AmpTools setup and a log of MINUIT's attempt to converge to a solution and obtain a good error matrix. Crucial as a debugging tool if fits fail.
- `omegapi.fit`
  - Contains all the amplitude info that results from a successful fit to data. Its main purpose is to be fed into other programs that we'll use like the [`vecps_plotter`](https://github.com/JeffersonLab/halld_sim/blob/bcff7a5c4e8453b1e75e83facdc6f2ad95ba5719/src/programs/AmplitudeAnalysis/vecps_plotter/vecps_plotter.cc#L40), which under-the-hood utilizes [AmpTools' FitResults class](https://github.com/JeffersonLab/halld_sim/blob/bcff7a5c4e8453b1e75e83facdc6f2ad95ba5719/src/programs/AmplitudeAnalysis/vecps_plotter/vecps_plotter.cc#L40).

### 4. Analysis of one bin using vecps_plotter
While still in the MASS BIN directory, run the following:
```
vecps_plotter omegapi.fit -g
```
to bring up the interactive plotter view. You'll see two windows. The blank one will contain the plots we wish to see, and the other is a GUI for selecting the plots and amplitudes we wish to view. Let's try looking at one of the 5 kinematic variables we're fitting to, the $\cos\theta$ distribution. In the top left of the GUI open the drop-down and select `cos#theta`. Make sure all amplitudes and coherent sums are selected. Click **Plot** and in the previously empty window, you should now see black data points and <span style="color:#879D8E;">a green fit result histogram</span>. 

Because we have selected all the coherent sums and amplitudes, we are viewing the total fit result to this distribution. Just as we might hope, the fit looks to do a relatively good job fitting to the data. If there were large gaps then that would indicate we don't have a good fit and we need to adjust our model. For example, real data will have baryonic backgrounds like a $\Delta^+$ that our model doesn't account for, and as the strength of those backgrounds increases, the fit to the $\cos\theta$ distribution becomes increasingly worse.

#### Understanding interference
So we've seen the total fit result to the $\cos\theta$ distribution, but we can break this down into its individual contributions.

##### Reflectivity
While keeping all the amplitudes selected, under **Coherent Sums** make sure only `ImagPosSign` and `RealNegSign` are selected. Plot it. What we are looking at now is only positive reflectivity $\varepsilon=+1$ contribution to $\cos\theta$. Now do the inverse, only selecting the $\varepsilon=-1$ coherent sums. What did you notice? You likely saw that the positive reflectivity contribution completely dominates, while the negative reflectivity is very small.

If you can't remember which is which it helps to look at the model 
$$
\begin{equation*}
\begin{split}
    I = 
    2\sum_k \Bigg\{
        (1-P_\gamma)&\Bigg[
            \bigg|\sum_{i,m}[c^i]_{m,k}^{-} \mathrm{Im}(Z_m^i)\bigg|^2 + 
            \bigg|\sum_{i,m}[c^i]_{m,k}^{+} \mathrm{Re}(Z_m^i)\bigg|^2
        \Bigg]
        \\
        + (1+P_\gamma)&\Bigg[
            \bigg|\sum_{i,m}[c^i]_{m,k}^{+} \mathrm{Im}(Z_m^i)\bigg|^2 + 
            \bigg|\sum_{i,m}[c^i]_{m,k}^{-} \mathrm{Re}(Z_m^i)\bigg|^2
        \Bigg]
    \Bigg\}
    \,,
\end{split}
\end{equation*}
$$
and remember that a sum like `ImagPosSign` refers to the sum containing $\mathrm{Im}(Z)$ and the positive sign of the polarization fraction $(1+P_\gamma)$, which we see is the bottom left sum that carries the positive reflectivity (denoted by the $+$ sign in the exponent of the production parameter $[c^i]_{m,k}^+$). It's very important to remember that because the reflectivities live in different coherent sums, that **destructive interferences and phase differences don't occur across reflectivities**.  

##### Amplitudes
Now let's try looking at the coherent sum of some of the amplitudes. With only the positive reflectivity coherent sums selected, select all the $J^P=1^+$ amplitudes i.e. those that start with `1p`. They should all end with either an `s` or `d` denoting their angular momentum value. If you can't remember the meaning of the notation, reference the block comment in [fit.cfg](./fit.cfg). If you plot these you'll notice something interesting, that the fit is larger than the data in the low $\cos\theta$ region. This is not a failure of the fit, but rather we are witnessing the built-in interference effects of the model! As we saw earlier, the total fit result performs quite well, but that total fit is the constructive/destructive interference of all the waves in the model. 

#### Other Plots
When viewing results, the mass distribution is what's most often shown, as that's where we are primarily looking for signatures of resonances. That's not to say the other plots are useless though. Below I've summarized some of the ones you'll find in the plotter and what they're useful for
* `Invariant Mass of Vec+Ps`: or $M_{\omega\pi^0}$. This is the distribution most often shown when looking for hints of resonances.
* `Prod_ang`: the production angle $\Phi$, one of the 5 fit variables.
* `cos#theta`, `phi`: Two of the 5 fit variables describing the angular distribution of the decay of a resonance $X\rightarrow \omega\pi^0$. 
* `cos#theta_H`, `$phi_H`: The last two of the 5 fit variables describing the angular distribution of the decay of $\omega\rightarrow\pi^+\pi^-\pi^0$. We typically refer to these as the "helicity" angles, denoted by `_H`, since most analyses use the helicity frame to measure them. Note that we typically also use the helicity frame for the resonance angles $\cos\theta$ and $\phi$, so if someone says "helicity angles" assume they mean the former.
* `-t`: the four-momentum transfer. Good to check that the selected range is right, and that the overall exponential decay shape is there.
* `Invariant Mass of proton and bachelor Ps`: or $M_{p'\pi^0}$. This is most often where baryonic backgrounds show up. For now, it's nothing to worry about, as they aren't generated in the MC.

I'd highly recommend going through these plots and toying around with different combinations of amplitudes and sums to get a sense of the plotter and try to understand the physical implications of what you're plotting. For example, what does it mean to have a strong $1^+$ contribution to the $\omega\pi^0$ mass?

#### vecps_fitPars.txt & vecps_plot.root
When running the plotter you'll get the above two output files. `vecps_fitPars.txt` is quite similar to the .fit file, but in a more human readable format. It provides:
* **Fit fractions**: If you were to divide an individual amplitude's value by the total fit result (a.k.a. the intensity) in that bin then you would get that amplitude's fractional contribution to the intensity, the fit fraction.
  * The fit fractions of coherent sums are also provided. So the fit fraction of `PosRefl 1p` is the fractional contribution of the coherent sum of every positive reflectivity wave with $J^P=1^+$.
* **Phase Differences**: in the search for resonances this is the other key component alongside the mass distribution.

The `vecps_plot.root` file is a ROOT file that essentially saves every single plot you could've made in the `vecps_plotter` GUI. It's very useful for our next step below.

#### Angle Plotter
The plotter GUI is a nice interactive tool to understand the amplitudes, but impractical to run on every single fit result. If we run the plotter without the `-g` flag then no GUI is presented, and it simply returns the two output files. This let's us grab the histograms from `vecps_plot.root` and make a simplified version useful for diagnosing fit results. To do this we'll `cd mass_1.175-1.200/` if not already there, and run
```
root -l -n -q -b ../scripts/angle_plotter.C
```
Take a look at the output `fit.pdf` and you can see we've quickly obtained some plots and their angular distributions without having to run the plotter GUI.


### 5. Multi-bin analysis with python

### Summary 

## :link: Additional Resources
- [Tired of emacs/vim? Check out this guide for using Visual Studio Code on the ifarm](https://halldweb.jlab.org/wiki/index.php/How_to_Setup_Visual_Studio_Code_for_the_ifarm)
- Working on Partial Wave Analysis and need help?
  - Post questions on the `halld_amplitude_analysis` slack channel (in the Jefferson Lab slack group).
  - Attend and present at the [Amplitude Analysis working group meetings](https://halldweb.jlab.org/wiki-private/index.php/Amplitude_Analysis_Working_Group_Meetings)
    - There's also a subgroup of us that attend those meetings who also meet in a [vector-pseudoscalar specific working group meeting](https://halldweb.jlab.org/wiki-private/index.php/Vector_Pseudoscalar_Working_Group_Meetings).
  - 
- [The AmpTools user guide](https://github.com/mashephe/AmpTools/blob/master/AmpTools_User_Guide.pdf)
- [Previous analysis tutorial, with great AmpTools examples by Matt](https://halldweb.jlab.org/wiki/index.php/GlueX_Tutorial_2022)
- [TODO: add link here to my analysis, and maybe some presentations to show what can be done with it. Hao also has some really nice studies.]
- [Optional: FAQs or troubleshooting tips]