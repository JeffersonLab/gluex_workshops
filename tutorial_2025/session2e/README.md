# How to Fit With Amptools and Plot the Results Using Python
:warning: This repo must be cloned to an `ifarm` node to ensure access to software and data files :warning:

## :dart: Goals
- Use a simple `.cfg` file to perform mass-independent fits
- Gain intuition of the intensity model by using the `vecps_plotter` for a fit result in one bin
- Aggregate fit results across multiple bins with AmpTools + Python
- Utilize Python to quickly plot fit results across $\omega\pi^0$ mass, and access other information.
---
## :memo: Session Instructions
If you are already familiar with amplitude analysis, then after you perform sections 1 - 3, you can skip to [section 5](#5-multi-bin-analysis-with-python) for how to use python.
### 1. Environment
#### 1.1 $\color{Turquoise}\sqrt{~}$ AmpTools and ROOT
Before we do anything, let's set up our environment. If you followed the main [README](../README.md), you have already setup `halld_sim` and `AmpTools` by running (from this directory):
```
source ../setup_gluex.csh
```

To run the python scripts, we'll need access to the AmpTools libraries in ROOT. To do this, copy the `.rootrc` file to your home directory with
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

#### 1.2 :snake: Python
We will be using very basic python virtual environments (venv) to keep our package versions consistent. At the time of writing this, the default version of python on the ifarm is `3.9.21`. Starting in the parent `PyAmpPlots` folder, execute
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
to download these packages to your virtual environment.

#### 1.3 Halld_sim info
AmpTools is written to be fairly generic for anyone interested in doing binned maximum likelihood analysis, so the collaboration has built up a set of libraries and programs over the years in the [halld_sim github](https://github.com/JeffersonLab/halld_sim/tree/master). The two main directories you'll most often consult will likely be [the amplitude analysis programs](https://github.com/JeffersonLab/halld_sim/tree/master/src/programs/AmplitudeAnalysis) and [the corresponding libraries](https://github.com/JeffersonLab/halld_sim/tree/master/src/libraries). As you just saw above, this is where our `fit` program comes from. Halld_sim is of course not limited to just Amplitude Analysis, and you'll find a whole host of other useful tools there. There's no need to deep dive into this repo yet, but it and [AmpTools](https://github.com/mashephe/AmpTools) are good ones to keep bookmarked.

### 2. Binning Data
We'll be using the data file `anglesOmegaPiAmplitude.root` produced by the DSelector from an earlier session, which is stored in `/work/halld/gluex_workshop_data/tutorial_2025/session2e`, along with the corresponding phasespace files labelled as `anglesOmegaPiPhaseSpace.root` and `anglesOmegaPiPhaseSpaceAcc.root` for the generated and accepted phasespace, respectively. For faster fits and compatability with our python scripts later, we'll split our files into bins of beam energy $(E_\gamma)$, four-momentum transfer $(-t)$, and  mass $(M_{\omega\pi^0})$:
- $8.2 < E_\gamma < 8.8$: coherent peak region
- $0.1 < -t < 0.6$: Nothing particularly special about this region, just done to demonstrate binning in $-t$
- 8 bins from $1.1 < M_{\omega\pi^0} < 1.3$: these are the bins we'll perform independent fits in

Try creating a binned file now with the following command:
```bash
root -l -b -q -n 'scripts/copy_tree_with_cuts.C("/work/halld/gluex_workshop_data/tutorial_2025/session2e/anglesOmegaPiAmplitude.root","./", "0.1", "0.2", "8.2", "8.8", "1.100", "1.120")'
```
and viewing it in a TBrowser window.

Lucky for you the selection process for all the mass bins has already been done, and you can find the pre-selected files in `/work/halld/gluex_workshop_data/tutorial_2025/session2e/mass_X-Y/` where X and Y will denote the lower and upper bound of each mass bin.

#### 2.1 A note about binning
You don't *have* to pre-bin your data for all amplitude analyses. You can use a special `ROOTDataReaderTEM` in your `.cfg` file, which selects the $-t$, $E_\gamma$, and $M_{\omega\pi^0}$ bins for you, so all you have to give the fitter is the single large data file. The reason we pre-bin the data here is primarily for our python scripts to read in the binned data. Doing this has another benefit though. You'll often iterate on a fit multiple times, and by doing so time is wasted having to read in and cut the large data files each time. If we pre-bin the data though, the cuts are done once and this often leads to significant time saves.

### 3. Fits
#### 3.1. The config file
The configuration file [`fit.cfg`](./fit.cfg) is the main file we'll use to actually run the fit. Whenever you want to change the waveset (the amplitudes you're fitting with) this is the file to edit. Conveniently the [halld_sim AmpTools libraries](https://github.com/JeffersonLab/halld_sim/tree/master/src/libraries/AMPTOOLS_AMPS) have plenty of amplitudes that others have written. This is why we can specify the `Vecp_ps_refl` amplitude in the config file and it will already know what to do.

#### 3.2 Running the fit
Now we're finally at the point where we can start running some actual fits! Notice that the [fit.cfg](./fit.cfg) is already setup to read in files for a particular mass bin (look under the `DATA, REACTIONS, AND SUMS` comment). We can thus run the command
```bash
fit -c ./fit.cfg -m 100000 > log.out
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

The `.fit` files for all 10 bins have already been done for you, and you can find these in the same directory bins as before `/work/halld/gluex_workshop_data/tutorial_2025/session2e/mass_X-Y/`.

TODO: Mention random fits in new section here

### 4. Analysis of one bin using vecps_plotter
1. Lets take a look at the fit you just did above, using the below command to bring up the interactive plotter view. You'll see two windows. The blank one will contain the plots we wish to see, and the other is a GUI for selecting the plots and amplitudes we wish to view. 
  ```
    vecps_plotter omegapi.fit -g
  ```
2. Let's try looking at one of the 5 kinematic variables we're fitting to, the $\cos\theta$ distribution. In the top left of the GUI open the drop-down and select `cos#theta`. Make sure all amplitudes and coherent sums are selected. 
3. Click **Plot** and in the previously empty window, you should now see black data points and <span style="color:#879D8E;">a green fit result histogram</span>. 
   * Because we have selected all the coherent sums and amplitudes, we are viewing the total fit result to this distribution. 
   * Just as we might hope, the fit looks to do a relatively good job fitting to the data. If there were large gaps then that would indicate we don't have a good fit and we need to adjust our model. 
     * For example, real data will have baryonic backgrounds like a $\Delta^+$ that our model doesn't account for, and as the strength of those backgrounds increases, the fit to the $\cos\theta$ distribution becomes increasingly worse.

#### 4.1 Understanding interference
So we've seen the total fit result to the $\cos\theta$ distribution, but we can break this down into its individual contributions.

##### 4.1.1 Reflectivity
4. While keeping all the amplitudes selected, under **Coherent Sums** make sure only `ImagPosSign` and `RealNegSign` are selected. Plot it. What we are looking at now is only positive reflectivity $\varepsilon=+1$ contribution to $\cos\theta$. 
5. Now do the inverse, only selecting the $\varepsilon=-1$ coherent sums. What did you notice? You likely saw that the positive reflectivity contribution completely dominates, while the negative reflectivity is very small.

If you can't remember which is which it helps to look at the model 
I = 2∑ₖ { (1-Pₐ)[|∑ₘₖ c⁻ Im(Zₘ)|² + |∑ₘₖ c⁺ Re(Zₘ)|²] + (1+Pₐ)[|∑ₘₖ c⁺ Im(Zₘ)|² + |∑ₘₖ c⁻ Re(Zₘ)|²] }
and remember that a sum like `ImagPosSign` refers to the sum containing Im(Z) and the positive sign of the polarization fraction (1+Pₐ), which we see carries the positive reflectivity (denoted by the $+$ sign in the exponent of the production parameter c⁺). It's very important to remember that because the reflectivities live in different coherent sums, that **destructive interferences and phase differences don't occur across reflectivities**.  

##### 4.1.2 Amplitudes
Now let's try looking at the coherent sum of some of the amplitudes. 

6. With only the positive reflectivity coherent sums selected, select all the $S$-wave amplitudes i.e. those end with `s`. 
   * If you can't remember the meaning of the notation, reference the block comment in [fit.cfg](./fit.cfg). 
7. Now try plotting the sum of the $D$-waves seperately. You'll likely notice that they don't both constructively add to the total. This is not a failure of the fit, but rather we are witnessing the built-in interference effects of the model! As we saw earlier, the total fit result performs quite well, but that total fit is the constructive/destructive interference of all the waves in the model. 

#### 4.2 Other Plots
When viewing results, the mass distribution is what's most often shown, as that's where we are primarily looking for signatures of resonances. That's not to say the other plots are useless though. Below I've summarized some of the ones you'll find in the plotter and what they're useful for
* `Invariant Mass of Vec+Ps`: or $M_{\omega\pi^0}$. This is the distribution most often shown when looking for hints of resonances.
* `Prod_ang`: the production angle $\Phi$, one of the 5 fit variables.
* `cos#theta`, `phi`: Two of the 5 fit variables describing the angular distribution of the decay of a resonance $X\rightarrow \omega\pi^0$. 
* `cos#theta_H`, `$phi_H`: The last two of the 5 fit variables describing the angular distribution of the decay of $\omega\rightarrow\pi^+\pi^-\pi^0$. 
  * We typically refer to these as the "helicity" angles, denoted by `_H`, since most analyses use the helicity frame to measure them. Note that we typically also use the helicity frame for the resonance angles $\cos\theta$ and $\phi$, so if someone says "helicity angles" assume they mean the former.
* `-t`: the four-momentum transfer. Good to check that the selected range is right, and that the overall exponential decay shape is there.
* `Invariant Mass of proton and bachelor Ps`: or $M_{p'\pi^0}$. This is most often where baryonic backgrounds show up. For now, it's nothing to worry about, as they aren't generated in the MC.

I'd highly recommend going through these plots and toying around with different combinations of amplitudes and sums to get a sense of the plotter and try to understand the physical implications of what you're plotting. For example, what does it mean to have a strong $1^+$ contribution to the $\omega\pi^0$ mass?

#### 4.3 vecps_fitPars.txt & vecps_plot.root
When running the plotter you'll get the above two output files. `vecps_fitPars.txt` is quite similar to the .fit file, but in a more human readable format. It provides:
* **Fit fractions**: If you were to divide an individual amplitude's value by the total fit result (a.k.a. the intensity) in that bin then you would get that amplitude's fractional contribution to the intensity, the fit fraction.
  * The fit fractions of coherent sums are also provided. So the fit fraction of `PosRefl 1p` is the fractional contribution of the coherent sum of every positive reflectivity wave with $J^P=1^+$.
* **Phase Differences**: in the search for resonances this is the other key component alongside the mass distribution.

The `vecps_plot.root` file is a ROOT file that essentially saves every single plot you could've made in the `vecps_plotter` GUI. It's very useful for our next step below.

#### 4.4 Angle Plotter
The plotter GUI is a nice interactive tool to understand the amplitudes, but impractical to run on every single fit result. If we run the plotter without the `-g` flag then no GUI is presented, and it simply returns the two output files. This let's us grab the histograms from `vecps_plot.root` and make a simplified version useful for diagnosing fit results. To do this we'll run 
```
root -l -n -q -b ../scripts/angle_plotter.C
```
Take a look at the output `fit.pdf` and you can see we've quickly obtained some plots and their angular distributions without having to run the plotter GUI.


### 5. Multi-bin analysis with python


### Summary 
---
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

## :question: FAQ
### How can I adopt the python tools for my own analysis?
This tutorial uses a vector-pseudoscalar process $\gamma p \rightarrow \omega\pi^0$ for its amplitude analysis example. As such, the scripts will require some modification to be adapted for other channels / processes. Below discusses what needs to be modified.

#### Amplitude Naming
When naming amplitudes like `reaction::sum_type::amp_name` there is unfortunately no single standard that enforces what `amp_name` looks like. This repo follows the vector-pseudoscalar inspired format, where `amp_name = JPml`, explained in the table below.

| Quantum Number | Description | Allowed Characters |
| :------------: | :---------- | :----------------: | 
| e              | reflectivity | p (+), m (-) |
| J              | total spin   | positive integers including 0 |
| P              | parity       | p (+), m (-) |
| m              | spin-projection | p (+1), 0, m (-1) |
| L              | orbital angular momentum | standard letter convention: s, p, d, f, ... |

If your config files, and therefore `.fit` result files, don't follow this format then you must edit the `parse_amplitude` function within [extract_fit_results.cc](./scripts/extract_fit_results.cc) to properly convert your amplitude naming scheme into `eJPmL` format. This is because the csv headers use this format, and so all the analysis scripts *heavily* depend on this for interpreting the results. You can, of course, choose to keep your `amp_name` scheme and instead rewrite all the analysis scripts to interpret your format instead.

#### Data File Format
The [extract_bin_info.cc](./scripts/extract_bin_info.cc) script also makes 2 basic assumptions:
1. The flat ROOT trees (typically output by a DSelector) that serve as the input to AmpTools have cuts already applied to them in a mass bin. For example, the `M4Pi` branch in any of the mass bins in [data](./data/) is already cut to its alloted range. 
2. The following branches exist in each file:
    * `t`: the squared four momentum transfer
    * `E_Beam`: beam photon energy
    * `M4Pi`: the invariant mass spectrum of interest. There is an optional argument in the scripts to specify the name of this branch, but it still assumes that *a* mass branch exists.
    * `Weight`: tracks a weight value for each event so that sideband subtraction is properly implemented. This may fail when using a separate `background` file in the AmpTools config files.