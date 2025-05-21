#!/usr/bin/env python

##########################################################################################################################

from optparse import OptionParser
import os.path
import os
import sys
import re
import subprocess
import glob

#################################################### GLOBAL VARIABLES ####################################################

VERBOSE = False

####################################################### FIND FILES #######################################################

def find_files(SIGNATURE):

    # SEARCH FOR THE FILES
    file_signature = SIGNATURE
    file_list = glob.glob(file_signature)
    if(VERBOSE == True):
        print ("size of file_list is " + str(len(file_list)))

    return file_list

def main(argv):
    parser_usage = "flatten_data.py directory filePrefix"
    parser = OptionParser(usage = parser_usage)
    (options, args) = parser.parse_args(argv)

    if(len(args) != 2):
        parser.print_help()
        return

    # GET ARGUMENTS
    DIR = args[0]
    PREFIX = args[1]
    
    RUN_PERIOD = "2018-08"

    SUFFIX = ".root"

    # GET LIST OF FILES ON DISK
    os.chdir(DIR)
    FOUND_TREE_FILES = find_files( PREFIX + "*.root" )

    totalScriptName = "/work/halld2/home/aschertz/gluex_workshops/tutorial_2025/session2d/scripts/dataSubmitJobs_flatten_trees_%s.sh" % RUN_PERIOD

    outBaseDir = "/work/halld2/home/aschertz/gluex_workshops/tutorial_2025/session2d/dataTrees/RunPeriod-%s/" % RUN_PERIOD

    if not os.path.exists(outBaseDir):
        os.makedirs(outBaseDir)

    # FIND/ADD JOBS
    for TREE_FILE in FOUND_TREE_FILES:
        if PREFIX not in TREE_FILE:
            continue
        if SUFFIX not in TREE_FILE:
            continue
#        print( TREE_FILE )

        # EXTRACT RUN NUMBER FOR INDEXING
        TEMP_FILE_INDEX = TREE_FILE.replace( PREFIX, '' )
#        print( TEMP_FILE_INDEX )
        FILE_INDEX = TEMP_FILE_INDEX.replace( SUFFIX, '' )
#        print( FILE_INDEX )

        # Write script to submit all jobs
        f = open(totalScriptName,'a')
        f.write("sbatch /work/halld2/home/aschertz/gluex_workshops/tutorial_2025/session2d/scripts/subScripts/job_trees_flatten_data_%s.sh\n" % FILE_INDEX)
        f.close() 

        # Write individual scripts for each job
        f2 = open( "/work/halld2/home/aschertz/gluex_workshops/tutorial_2025/session2d/scripts/subScripts/job_trees_flatten_data_%s.sh" % FILE_INDEX, "w")
        f2.write( "#!/bin/sh\n" )
        f2.write( "#SBATCH -q phi\n" )
        f2.write( "#SBATCH -t 00:50:00\n" )
        f2.write( "/work/halld2/home/aschertz/hd_utilities/FlattenForFSRoot/flatten -in %s/%s -out %s/tree_pi0pip0pippim__B4_flat_%s.root -chi2 20 -numUnusedTracks 1 -numUnusedNeutrals 2 -shQuality 0.5 -usePolarization 1 -combos 2" % (DIR,TREE_FILE,outBaseDir,FILE_INDEX) )
        f2.close()      


if __name__ == "__main__":
    main(sys.argv[1:])

