
root -l -b -q 'runSelector.C("3","/work/halld/home/jrsteven/tutorial_2022/bggen/merged/")'
mv hist_etapi_3.acc.root hist_etapi_bggen.acc.root
mv tree_etapi.acc.root tree_etapi_bggen.acc.root
mv tree_etapi_flat.acc.root tree_etapi_flat_bggen.acc.root

root -l -b -q 'runSelector.C("3","/cache/halld/RunPeriod-2017-01/analysis/ver52/tree_pi0eta__B4_M17_M7/merged/")'
mv hist_etapi_3.acc.root hist_etapi_data.acc.root
mv tree_etapi.acc.root tree_etapi_data.acc.root
mv tree_etapi_flat.acc.root tree_etapi_flat_data.acc.root

#root -l -b -q 'runSelector.C("5","/work/halld/home/jrsteven/tutorial_2022/signal/merged/")'
