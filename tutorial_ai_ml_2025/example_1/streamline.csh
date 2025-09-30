#!/bin/csh -f                                                                                                                                                                                   
set echo

# k+k- MC for training (signal)
#root -b -q /work/halld/home/acschick/channels/p2k/tree_kpkm__B4_gen_amp_051284_000.root 'call_DSelector_KTree.C("DSelector_kpi_TMVA.C")' >! kpkm.list

# pi+pi- MC for training (background)
root -b -q /cache/halld/gluex_simulations/REQUESTED_MC/PWA_201701_ver04_100M_3279/root/trees/tree_pippim__B4_gen_amp/tree_pippim__B4_gen_amp_030567.root 'call_DSelector_PiTree.C("DSelector_kpkm.C+")' >! pippim.list

# data to classify with our BDT
#root -b -q /work/halld/home/acschick/channels/p2pi/RunPeriod-2018-01/ver02/FullRun/Tree_pippim_2018-01_ver02/Tree_pippim_2018-01_ver02_merged_small.root 'call_DSelector_PiTree.C("DSelector_kpkm.C+")' >! pippim.list
unset echo

    
                                                                                            
