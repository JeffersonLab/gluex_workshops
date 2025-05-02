This session is directed at getting familiar with using a generator ( In this case using gen_amp_V2 ) with the correct parameter values to generate kinematics needed for your analysis. A config file with a few possible congigurations is shown to generate the GlueX reaction of interest along with a few plots to get a feeling of the key kinematic variables. Here are some general instructions to reproduce the plots given in the presentation.

# Getting Started 

## Items needed
  * The plots made in step 'Make Plots' uses the FSRoot package. You can obtain and read the Documaentation on how to load this package in ROOT by going to the GitHub repository: https://github.com/remitche66/FSRoot
  * halld_sim compiled ( No need if working on the farm).
  * config file (see `genPS.cfg`).  

## config file 
   The config file uses the same structure as a gen_amp config file. The most basic feature needed to simulate events are the reaction name: 
   * EX: `reaction OmegaPiProton Beam Proton Pi0 Omega`

## How to use
   * In this example we generate 10k three-body phase phase events where the ⍵ π<sup>0</sup> are in the upper vertex and the proton is in the lower vertex.  
   * run `gen_amp_V2 -ac genPS.cfg -o b1_omegapi_ps.root -uv 23 -lv 1 -f`.  
 
  Things to note:
   * Unless a beam config file is used, a local file will be produced within the coherent peak.
   * The values from the "uv & lv' flags indicate the particle index from the reaction name.
   * The `-f` flag skips the amplitude accept/reject process and generates phase space.
   * Two output files will be created, `test.root` and `gen_amp_diagnostic.root`.

   Getting more complex:
   There are more complex examples which are commented out in the first lines of the 'genPS.cfg' file for other specific analysis generation. For example the ' ' line includes a specific momentum transfer distribution as well as an important sampling around the B<sub>1</sub> mass range for the upper vertex.

## Make Plots
  In order to plot the key kinematic variables run the following code:
    * root -l plotPhaseSpace.C
## Documentation
    
  For further options and descriptions see `GlueX_Fixed_Target_Generator.pdf`.

## Using in MCWrapper
 
  When using this generator in MCWrapper one needs to place the flags in the first line of the config file.  
  Using the same example on the first line `# -uv 23 -lv 1 -f`.  
  The `-ac -bc -o -n -r -m -p -a -b` flags will be defined using the MCWrapper config file.

