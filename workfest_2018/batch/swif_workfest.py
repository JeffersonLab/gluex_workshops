#!/usr/bin/env python

##########################################################################################################################
#
# 2018/05/11 Justin Stevens
# Heavily based off of work by Paul Mattione and Kei Moriya at:
# https://halldsvn.jlab.org/repos/trunk/home/pmatt/jobs/analysis/swif_gluex_analysis.py
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

import time

#################################################### RCDB ENVIRONMENT ####################################################
os.environ["RCDB_HOME"] = "/group/halld/www/halldweb/html/rcdb_home"
sys.path.append("/group/halld/www/halldweb/html/rcdb_home/python")
import rcdb
db = rcdb.RCDBProvider("mysql://rcdb@hallddb/rcdb")

#################################################### GLOBAL VARIABLES ####################################################

# DEBUG
VERBOSE    = False

# PROJECT INFO
PROJECT    = "gluex"          # http://scicomp.jlab.org/scicomp/#/projects
TRACK      = "analysis"		   # https://scicomp.jlab.org/docs/batch_job_tracks

# RESOURCES
NCORES     = "8"               # Number of CPU cores
DISK       = "10GB"            # Max Disk usage
RAM        = "10000MB"            # Max RAM usage
TIMELIMIT  = "300minutes"      # Max walltime
OS         = "centos7"        # Specify CentOS65 machines

# SOURCE DATA INFORMATION
# FILES ARE SEARCHED-FOR WITH THE PATH: DATA_SOURCE_BASE_DIR + "/RunPeriod-" + RUN_PERIOD + "/" + VERSION + "/REST/" + FORMATTED_RUN + "/dana_rest_*.hddm"
# Where FORMATTED_RUN is the run number you chose, with leading zeros. 
RUN_PERIOD            = "2017-01"
VERSION               = "ver33"
DATA_SOURCE_TYPE      = "file" #"mss" or "file"
DATA_SOURCE_BASE_DIR  = "/cache/halld/offline_monitoring/"

# OUTPUT DATA LOCATION
DATA_OUTPUT_BASE_DIR    = "/volatile/halld/home/%s/workfest/data/"%(os.environ['USER'])   ## CHANGE IF YOU WANT TO

# JOB EXECUTION (SET WITH YOUR OWN PATH)
SCRIPTFILE        = "/work/halld2/home/%s/workfest/builds/gluex_workshops/workfest_2018/batch/script.csh"
ENVFILE           = "/work/halld2/home/%s/workfest/builds/gluex_workshops/workfest_2018/setup.csh"
CONFIG_FILE_PATH  = "/work/halld2/home/%s/workfest/builds/gluex_workshops/workfest_2018/analysis_workfest.conf"
TREE_NAMES        = "eta__eta_gg__M17_B4,eta__eta_pi0pi0pi0__M17_B4,eta__eta_pi0pi0pi0__M17_M7_B4,etapr__etapr_pi0pi0eta__eta_gg__M35_B4,etapr__etapr_pi0pi0eta__eta_gg__M35_M17_B4,etapr__etapr_pimpipeta__eta_gg__M35_B4,etapr__etapr_pimpipeta__eta_gg__M35_M17_B4,etapr__etapr_pimpipg__M35_B4,kmkp__B4,pi0g__B4,pi0g__M7_B4,pi0pimmisspip__U1_B4,pi0pimpip__B4,pi0pimpip__M7_B4,pi0pimpipmissprot__U1_B4,pi0pimpip__pi0_gmissg__U1_B4,pi0pipmisspim__U1_B4,pimmisspip__U1_B4,pimpimpipmisspip__U1_B4,pimpip__B4,pimpipmissprot__U1_B4,pimpippimpipmissprot__U1_B4,pipmisspim__U1_B4,pippimpipmisspim__U1_B4,trackeff"

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
	#time.sleep(120)
	status = subprocess.call(add_command.split(" "))


########################################################## MAIN ##########################################################
	
def main(argv):
	parser_usage = "swif_workfest.py workflow minrun maxrun"
	parser = OptionParser(usage = parser_usage)
	(options, args) = parser.parse_args(argv)

	if(len(args) != 3):
		parser.print_help()
		return

	# GET ARGUMENTS
	WORKFLOW = args[0]
	MINRUN = int(args[1])
	MAXRUN = int(args[2])

	# CREATE WORKFLOW IF IT DOESN'T ALREADY EXIST
	WORKFLOW_LIST = subprocess.check_output(["swif", "list"])
	if WORKFLOW not in WORKFLOW_LIST:
	    status = subprocess.call(["swif", "create", "-workflow", WORKFLOW])

	# GET LISTS OF GOOD RUNS TO CHOOSE FROM
	RCDB_RUNS = db.select_runs("@is_production and @status_approved")
	RCDB_RUN_NUMBERS = [ run.number for run in RCDB_RUNS ]

	# FIND/ADD JOBS
	for RUN in range(MINRUN, MAXRUN + 1):

		# Check RCDB status for each run number
		if RUN not in RCDB_RUN_NUMBERS:
			continue

		# Format run and file numbers
		FORMATTED_RUN = "%06d" % RUN

		# Find files for run number
		DATA_SOURCE_DIR = DATA_SOURCE_BASE_DIR + "/RunPeriod-" + RUN_PERIOD + "/" + VERSION + "/REST/" + FORMATTED_RUN + "/"
		print DATA_SOURCE_DIR
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

