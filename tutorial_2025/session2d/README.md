# Session 2d: Event Selection Using FSRoot

## :dart: Goals
- Develop a basic understanding of how event selection can be done using FSRoot
- Learn how to plot, cut, and skim using FSRoot
- Prepare flattened and skimmed root trees for AmpTools fits

## :clipboard: Prerequisites
- You need to have FSRoot set up on your machine. Instructions can be found in section 1 of the [FSRoot documentation](https://github.com/remitche66/FSRoot/blob/master/Documentation/FSRoot.pdf)

## :file_folder: Materials Provided
- ROOT files used in this session can be found in `/work/halld/gluex_workshop_data/tutorial_2025/session2d`
- Documentation for [FSRoot](https://github.com/remitche66/FSRoot/blob/master/Documentation/FSRoot.pdf)
- Documentation regarding the [FlattenForFSRoot](https://github.com/remitche66/FSRoot/blob/master/Documentation/FSRoot.pdf) tool

## :memo: Session Outline
1. Flatten analysis trees. The python scripts `flatten_data.py` and `flatten_mc.py` will generate a set of batch scripts that will submit the flattening jobs to the farm one by one. Usage: ```python flatten_data.py <directory> <file_prefix>```, where `<file_prefix>` is everything in the file name leading up to the run number. This will generate a set of scripts to flatten each file, plus another script that will submit each of the flattening scripts to the farm, and can be called using ```source dataSubmitJobs_flatten_trees_<runPeriod>.sh```. Be sure to do this from inside the `scripts` directory, to avoid clutter from `slurm.out` files. This flattening step should be performed once per dataset (data, MC, thrown MC).
2. Choose event selection cuts, plot their effects. Once you've flattened your trees, you should be ready to define and plot some cuts. The script `plots.C` has the cuts used in this tutorial already defined, but since we have this 2D sideband subtraction, we need to run `saveM3PI()` first, just to make our lives easier. First, start up root ```root -l```, load the script ```.L plots()```, and run ```.x saveM3PI()```. We only need to do that once, and then we can just run the script using ```root -l plots.C```.
3. Refine event selection cuts, if needed. The plots generated in step 2 can be found in the `plots` directory, and can be viewed using ```evince plots/plotName.pdf```. Once you've decided the cuts do what you want, you're ready to skim your trees. 
4. Skim flattened trees using finalized cuts. If you've changed the cuts used in `plots.C` at all, make sure those changes are copied to `skim_b1.C`. Then you should be able to just skim the data, MC, and thrown MC all at once ```root -l -q skim_b1.C```.
5. You should have a set of flattened and skimmed trees that are ready to be fit using AmpTools, and you should be ready to move on to the next session!

