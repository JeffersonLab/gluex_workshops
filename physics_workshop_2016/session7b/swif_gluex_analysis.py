#!/usr/bin/env python

##########################################################################################################################
#
# 2015/07/24 Paul Mattione
# Heavily based off of work by Kei Moriya at:
# https://halldsvn.jlab.org/repos/trunk/scripts/monitoring/hdswif/hdswif.py
#
# SWIF DOCUMENTATION:
# https://scicomp.jlab.org/docs/swif
# https://scicomp.jlab.org/docs/swif-cli
# https://scicomp.jlab.org/help/swif/add-job.txt #consider phase!
#
##########################################################################################################################

from optparse import OptionParser
import os.path
import os
import sys
import re
import subprocess
import glob

#################################################### GLOBAL VARIABLES ####################################################

# DEBUG
VERBOSE    = False

# PROJECT INFO
PROJECT    = "gluex"          # http://scicomp.jlab.org/scicomp/#/projects
TRACK      = "analysis"		   # https://scicomp.jlab.org/docs/batch_job_tracks

# RESOURCES
NCORES     = "8"               # Number of CPU cores
DISK       = "10GB"            # Max Disk usage
RAM        = "12GB"            # Max RAM usage
TIMELIMIT  = "300minutes"      # Max walltime
OS         = "centos65"        # Specify CentOS65 machines

# SOURCE DATA INFORMATION
# FILES ARE SEARCHED-FOR WITH THE PATH: DATA_SOURCE_BASE_DIR + "/RunPeriod-" + RUN_PERIOD + "/" + VERSION + "/REST/" + FORMATTED_RUN + "/dana_rest_*.hddm"
# Where FORMATTED_RUN is the run number you chose, with leading zeros. 
RUN_PERIOD            = "2016-02"
VERSION               = "ver05"
#DATA_SOURCE_TYPE      = "mss" #"mss" for tape files, "file" for disk files
#DATA_SOURCE_BASE_DIR  = "/mss/halld/offline_monitoring/"
DATA_SOURCE_TYPE      = "file" #"mss" or "file"
DATA_SOURCE_BASE_DIR  = "/cache/halld/offline_monitoring/"

# OUTPUT DATA LOCATION
DATA_OUTPUT_BASE_DIR    = "/volatile/halld/home/%s/analysis_output/"%(os.environ['USER'])

# JOB EXECUTION
#SCRIPTFILE        = "/work/halld2/home/sdobbs/jobs/analysis/script.sh"
SCRIPTFILE        = "/work/halld/home/gluex/analysis_jobs/script.sh"            ## CHANGE
ENVFILE           = "/work/halld/home/gluex/analysis_jobs/gluex_env_jlab.csh"   ## CHANGE
#CONFIG_FILE_PATH  = "/work/halld2/home/sdobbs/jobs/analysis/analysis.conf"
CONFIG_FILE_PATH  = "/work/halld/home/gluex/analysis_jobs/analysis.conf"        ## CHANGE
TREE_NAMES        = "omega"

# CONFIG FILE CONTENTS
CONFIG_DICT = {}
CONFIG_DICT["PLUGINS"] =            "omega_ref"
CONFIG_DICT["NTHREADS"] =           "8" #Ncores if you have the whole node
CONFIG_DICT["THREAD_TIMEOUT"] =     "300"
#CONFIG_DICT["JANA_CALIB_CONTEXT"] = '""' #'"variation=,calibtime="'

################################################## GENERATE CONFIG FILE ##################################################

def generate_config():
	config_file = open(CONFIG_FILE_PATH, 'w')
	config_file.truncate() #wipe it out

	for key in CONFIG_DICT:
		value = CONFIG_DICT[key]
		config_file.write(key + " " + value + "\n")

	config_file.close()

####################################################### FIND FILES #######################################################

def find_files(DATA_SOURCE_DIR):

	# CHANGE TO THE DIRECTORY CONTAINING THE INPUT FILES
	current_dir = os.getcwd()
        if not os.path.isdir(DATA_SOURCE_DIR):
                return []
	os.chdir(DATA_SOURCE_DIR)

	# SEARCH FOR THE FILES
	file_signature = "dana_rest_*.hddm"
	file_list = glob.glob(file_signature)
	if(VERBOSE == True):
		print "size of file_list is " + str(len(file_list))

	# CHANGE BACK TO THE PREVIOUS DIRECTORY
	os.chdir(current_dir)
	return file_list

######################################################## ADD JOB #########################################################

def add_job(WORKFLOW, DATA_SOURCE_DIR, FILENAME, RUNNO, FILENO):

	# PREPARE NAMES
	STUBNAME = RUNNO + "_" + FILENO
	JOBNAME = WORKFLOW + "_" + STUBNAME

	# CREATE ADD-JOB COMMAND
	# job
	add_command = "swif add-job -workflow " + WORKFLOW + " -name " + JOBNAME
	# project/track
	add_command += " -project " + PROJECT + " -track " + TRACK
	# resources
	add_command += " -cores " + NCORES + " -disk " + DISK + " -ram " + RAM + " -time " + TIMELIMIT + " -os " + OS
	# inputs
	add_command += " -input " + FILENAME + " " + DATA_SOURCE_TYPE + ":" + DATA_SOURCE_DIR + "/" + FILENAME
	# stdout
	add_command += " -stdout " + DATA_OUTPUT_BASE_DIR + "/log/" + RUNNO + "/stdout." + STUBNAME + ".out"
	# stderr
	add_command += " -stderr " + DATA_OUTPUT_BASE_DIR + "/log/" + RUNNO + "/stderr." + STUBNAME + ".err"
	# tags
	add_command += " -tag run_number " + RUNNO
	# tags
	add_command += " -tag file_number " + FILENO
	# command
	add_command += " " + SCRIPTFILE + " " + ENVFILE + " " + FILENAME + " " + CONFIG_FILE_PATH + " " + DATA_OUTPUT_BASE_DIR + " " + RUNNO + " " + FILENO + " " + TREE_NAMES

	if(VERBOSE == True):
		print "job add command is \n" + str(add_command)

	# ADD JOB
	status = subprocess.call(add_command.split(" "))


########################################################## MAIN ##########################################################
	
def main(argv):
	parser_usage = "swif_g13data.py workflow minrun maxrun"
	parser = OptionParser(usage = parser_usage)
	(options, args) = parser.parse_args(argv)

	if(len(args) != 3):
		parser.print_help()
		return

	# GET ARGUMENTS
	WORKFLOW = args[0]
	MINRUN = int(args[1])
	MAXRUN = int(args[2])

	# CREATE WORKFLOW
	status = subprocess.call(["swif", "create", "-workflow", WORKFLOW])

	# GENERATE CONFIG
	generate_config()

	# FIND/ADD JOBS
	for RUN in range(MINRUN, MAXRUN + 1):

		# Format run and file numbers
		FORMATTED_RUN = "%06d" % RUN

		# Find files for run number
		DATA_SOURCE_DIR = DATA_SOURCE_BASE_DIR + "/RunPeriod-" + RUN_PERIOD + "/" + VERSION + "/REST/" + FORMATTED_RUN + "/"
		if(os.path.exists(DATA_SOURCE_DIR)):
			file_list = find_files(DATA_SOURCE_DIR)
		else:
			continue
		if(len(file_list) == 0):
			continue

		# Add jobs to workflow
		for FILENAME in file_list:
			FILENO = FILENAME[-8:-5] #e.g. dana_rest_003185_015.hddm #Cheat!
			add_job(WORKFLOW, DATA_SOURCE_DIR, FILENAME, FORMATTED_RUN, FILENO)

if __name__ == "__main__":
   main(sys.argv[1:])

