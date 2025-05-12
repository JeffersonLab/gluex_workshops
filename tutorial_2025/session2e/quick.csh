# Batch script to run the whole analysis process

source ../setup_gluex.csh
set home_dir = /w/halld-scshelf2101/kscheuer/gluex_workshops/tutorial_2025/session2e/
set parent_dir = /work/halld/gluex_workshop_data/tutorial_2025/session2e/

## BEGIN COPY TREE SECTION ###
set t_low = 0.1
set t_high = 0.6
set e_low = 8.2
set e_high = 8.8

set start = 1.100
set end = 1.300
set step = 0.025

while (`echo "$start < $end" | bc` == 1)
    set next = `echo "$start + $step" | bc`
    echo "Processing range: $start-$next"

    mkdir $parent_dir/mass_$start-$next

    cd $parent_dir/mass_$start-$next

    root -l -q -n -b '/w/halld-scshelf2101/kscheuer/gluex_workshops/tutorial_2025/session2e/scripts/copy_tree_with_cuts.C("../anglesOmegaPiAmplitude.root", "./", "'$t_low'", "'$t_high'", "'$e_low'", "'$e_high'", "'$start'", "'$next'")'
    root -l -q -n -b '/w/halld-scshelf2101/kscheuer/gluex_workshops/tutorial_2025/session2e/scripts/copy_tree_with_cuts.C("../anglesOmegaPiPhaseSpace.root", "./", "'$t_low'", "'$t_high'", "'$e_low'", "'$e_high'", "'$start'", "'$next'")'
    root -l -q -n -b '/w/halld-scshelf2101/kscheuer/gluex_workshops/tutorial_2025/session2e/scripts/copy_tree_with_cuts.C("../anglesOmegaPiPhaseSpaceAcc.root", "./", "'$t_low'", "'$t_high'", "'$e_low'", "'$e_high'", "'$start'", "'$next'")'


    set start = $next
end

# BEGIN FITTING SECTION ###

foreach dir (`ls -d $parent_dir/*/`)
    if ("`echo $dir | grep '1.1'`" != "") continue    
    cd $dir
    cp -f $home_dir/fit.cfg ./
    echo "Processing directory: $dir"    
    fit -c fit.cfg -m 1000000 >& log.out

    mkdir rand_fits
    
    mv omegapi_*.fit rand_fits/
    vecps_plotter omegapi.fit
    root -n -l -b -q '/w/halld-scshelf2101/kscheuer/gluex_workshops/tutorial_2025/session2e/scripts/angle_plotter.cc()'
end