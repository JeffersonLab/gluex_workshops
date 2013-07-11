#!/bin/sh

# combine signal and background trees to similate "data"
hadd tree_n3piBdtCut0.1_data.root $GLUEX_DATA/session7a/tree_n3piBdtCut0.1_bggen_eval.root $GLUEX_DATA/session7a/tree_n3piBdtCut0.1_signal_eval.root

# convert "data" tree which pass BDT cut to AmpTools input format
tree_to_amptools tree_n3piBdtCut0.1_data.root n3pi_Tree
mv AmpToolsInputTree.root threepi_data.root

# convert acceptance tree which pass BDT cut to AmpTools input format 
tree_to_amptools $GLUEX_DATA/session7a/tree_n3piBdtCut0.1_signal_acc_eval.root n3pi_Tree
mv AmpToolsInputTree.root threepi_acc.root

#cleanup 
rm tree_n3piBdtCut0.1_data.root

