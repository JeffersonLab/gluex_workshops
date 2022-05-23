 # GlueX Tutorial 2022, Session 2f
## _Efficiency Correction and Flux Normalization_
##### Jon Zarling, jzarling@jlab.org

At this stage, we assume that you have an efficiency corrected yield of events. We only need to determine flux/luminosity and incorporate any relevant branching fractions to convert to a cross section.

Word of caution: before adapting for your analysis, be careful that your selection of target length, beam E, runs, and any other selection is applied to all data samples! It's very easy to forget to apply these to your thrown MC sample in particular.

## Getting Flux / Luminosity From RCDB 
_The following commands extract the flux/luminosity separately for each diamond orientation (and skipping AMO runs), then combines in a single file named flux_AllDiamondRuns_30274_31057.root_



- `$HD_UTILITIES_HOME/psflux/plot_flux_ccdb.py --pol PARA --angle 0.0   --target-length 26.0 --energy-min 8.2 --energy-max 8.8 --begin-run 30274 --end-run 31057 --rest-ver 3`
- `$HD_UTILITIES_HOME/psflux/plot_flux_ccdb.py --pol PERP --angle 90.0  --target-length 26.0 --energy-min 8.2 --energy-max 8.8 --begin-run 30274 --end-run 31057 --rest-ver 3`
- `$HD_UTILITIES_HOME/psflux/plot_flux_ccdb.py --pol PARA --angle 135.0 --target-length 26.0 --energy-min 8.2 --energy-max 8.8 --begin-run 30274 --end-run 31057 --rest-ver 3`
- `$HD_UTILITIES_HOME/psflux/plot_flux_ccdb.py --pol PERP --angle 45.0  --target-length 26.0 --energy-min 8.2 --energy-max 8.8 --begin-run 30274 --end-run 31057 --rest-ver 3`
- `hadd flux_AllDiamondRuns_30274_31057.root flux_P???_*_30274_31057.root `

## Determining Cross Section
Print to console with the following command:
```sh
root -b -q 'CalculateCrossSec.C("flux_AllDiamondRuns_30274_31057.root",29178,1416)'
```
(where 29178 and 1416 are a_2(1320) yield and yield uncertainty respectively)
