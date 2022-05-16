#!/usr/bin/env python

# Alex Austregesilo
# Paul Mattione
# Built off of hdswif by Kei Moriya
#
# SWIF2 DOCUMENTATION:
# https://scicomp.jlab.org/docs/swif2

from optparse import OptionParser
import os.path
import rcdb
import sys
import time
import glob
import re
import time
from subprocess import Popen, PIPE

VERBOSE = False

####################################################### TRY COMMAND ######################################################

def try_command(command, sleeptime = 5):
	# Try an os command and if the exit code is non-zero then return an error
	return_code = -999
	while return_code != 0:
		process = Popen(command.split(), stdout=PIPE)
		output = process.communicate()[0] # is stdout. [1] is stderr
		print output
		return_code = process.returncode

		if return_code == 0:
			break #successful: leave

		# sleep for a few seconds between tries
		print 'sleeping for ' + str(sleeptime) + ' sec...'
		time.sleep(sleeptime)

####################################################### READ CONFIG ######################################################

def read_config(CONFIG_FILENAME):
	# Read in user config file
	config_dict = {}

	infile_config = open(CONFIG_FILENAME, 'r')
	for line in infile_config:

		# Ignore empty lines, and lines that begin with #
		if (len(line.split()) == 0) or (line.split()[0][0] == '#'):
			continue

		# Add new key/value pair into config_dict
		key = str(line.split()[0])
		value = line.split()[1]
		# if is string in quotes (e.g. RCDB query) continue reading until end-quote
		read_index = 2
		while(value[0] == "\"") and (value[-1:] != "\""):
			value += " " + line.split()[read_index]
			read_index += 1
		# now, strip leading and tail quotation marks (if present)
		if read_index > 2:
			value = value[1:-1]
		config_dict[key] = value
		if(VERBOSE == True):
			print "Job Config key, value = " + key + " " + value

	# Some of the keys may depend on other configuration parameters, so update the values
	# containing [key] within the values corresponding to those keys.
	#
	# Example:
	# OUTPUT_TOPDIR /volatile/halld/test/RunPeriod-[RUNPERIOD]/ver[VERSION]
	# depends on other config parameters RUNPERIOD and VERSION
	# 
	# NOTE: The method assumes there are no circular dependencies
	  
	# Iterate over key/value pairs in dictionary. If we find a replacement, we need to start over.
	# The parameter found keeps track of whether we found a replacement or not.
	found = 1
	while(found):
		for key, value in config_dict.items():
			found = 0

			# For each one see if any values contain [P] where P is a different value
			for other_key, other_value in config_dict.items():
				if str(value).find(str('[' + other_key + ']')) != -1:

					# Found replacement
					found = 1
					new_value = value.replace(str('[' + other_key + ']'),other_value)

					# Replace new key/value pair into config_dict
					new_pair = {key : new_value}
					config_dict.update(new_pair)
					del new_pair

					# Break out of loop over other_key, other_value
					break

			# Break out of loop over key, value, to restart loop again
			if found == 1:
				break

		# If we do not find a replacement we will finish the loop

	return config_dict

##################################################### VALIDATE CONFIG ####################################################

def validate_config(config_dict):

	# JOB ACCOUNTING
	if("PROJECT" not in config_dict) or ("TRACK" not in config_dict) or ("OS" not in config_dict):
		print "ERROR: JOB ACCOUNTING NOT FULLY SPECIFIED IN CONFIG FILE. ABORTING"
		sys.exit(1)

	# JOB RESOURCES
	if("NCORES" not in config_dict) or ("DISK" not in config_dict) or ("RAM" not in config_dict) or ("TIMELIMIT" not in config_dict):
		print "ERROR: JOB RESOURCES NOT FULLY SPECIFIED IN CONFIG FILE. ABORTING"
		sys.exit(1)

	# WORKFLOW DEFINITION
	if("WORKFLOW" not in config_dict):
		print "ERROR: WORKFLOW DEFINITION NOT FULLY SPECIFIED IN CONFIG FILE. ABORTING"
		sys.exit(1)

	# JOB, SCRIPT CONTROL
	if("ENVFILE" not in config_dict) or ("SCRIPTFILE" not in config_dict):
		print "ERROR: JOB, SCRIPT CONTROL NOT FULLY SPECIFIED IN CONFIG FILE. ABORTING"
		sys.exit(1)

	# FILE INPUT, OUTPUT BASE DIRECTORIES
	if("INDATA_TOPDIR" not in config_dict) or ("OUTDIR_LARGE" not in config_dict) or ("OUTDIR_SMALL" not in config_dict):
		print "ERROR: FILE INPUT, OUTPUT BASE DIRECTORIES NOT FULLY SPECIFIED IN CONFIG FILE. ABORTING"
		sys.exit(1)

	# CHECK FILE EXISTENCE
	if(not os.path.isfile(config_dict["ENVFILE"])): #Check if ENVFILE exists
		# Also accept ENVFILE if it is an xml file found in standard group disk location
		if(not os.path.isfile("/group/halld/www/halldweb/html/halld_versions/"+config_dict["ENVFILE"])): # 
			print "ERROR: ENVFILE does not exist (or is inaccessible) \n ENVFILE: " + config_dict["ENVFILE"]
			sys.exit(1)
	if(not os.path.isfile(config_dict["SCRIPTFILE"])): 
		print "ERROR: SCRIPTFILE does not exist (or is inaccessible) \n SCRIPTFILE: " + config_dict["SCRIPTFILE"]
		sys.exit(1)
	if("JANA_CONFIG" in config_dict):
		if(not os.path.isfile(config_dict["JANA_CONFIG"])): 
			print "ERROR: JANA_CONFIG specified but does not exist (or is inaccessible) \n JANA_CONFIG: " + config_dict["JANA_CONFIG"]
			sys.exit(1)
		
	# CHECK INPUT FOLDER EXISTENCE
	if(not os.path.isdir(config_dict["INDATA_TOPDIR"])): 
		print "ERROR: INDATA_TOPDIR does not exist! \n INDATA_TOPDIR: " + config_dict["INDATA_TOPDIR"]
		sys.exit(1)
		
	# CHECK OUTPUT (LARGE) FOLDER EXISTENCE
	if(not os.path.isdir(config_dict["OUTDIR_LARGE"])):
		# First try to create folder if it does not exist
		NEW_DIR = str(config_dict["OUTDIR_LARGE"])
		make_large_dir = "mkdir -p " + NEW_DIR
		try_command(make_large_dir)
		if(VERBOSE == True):
			print "OUTDIR_LARGE " + make_large_dir + " CREATED"
	# If creating OUTDIR_LARGE unsuccessful, we should exit
	if(not os.path.isdir(config_dict["OUTDIR_LARGE"])): 
		print "ERROR: OUTDIR_LARGE does not exist and could not be created \n OUTDIR_LARGE: " + config_dict["OUTDIR_LARGE"]
		sys.exit(1)

	# CHECK OUTPUT (SMALL) FOLDER EXISTENCE
	if(not os.path.isdir(config_dict["OUTDIR_SMALL"])): 
		# First try to create folder if it does not exist
		LOG_DIR = config_dict["OUTDIR_SMALL"] + "/log"
		make_log_dir = "mkdir -p " + LOG_DIR
		try_command(make_log_dir)
		if(VERBOSE == True):
			print "LOG DIRECTORY " + LOG_DIR + " CREATED"
	# If creating OUTDIR_SMALL unsuccessful, we should exit
	if(not os.path.isdir(config_dict["OUTDIR_SMALL"])): 
		print "ERROR: OUTDIR_SMALL does not exist and could not be created \n OUTDIR_SMALL: " + config_dict["OUTDIR_SMALL"]
		sys.exit(1)

####################################################### FIND FILES #######################################################

def find_files(INDATA_DIR, FORMATTED_RUN, FORMATTED_FILE):
	# Note: This won't work if the file names don't contain ".", or if there are directories that DO contain "."

	# If need specific file #
	if(FORMATTED_FILE != "*"):
		pathstring = INDATA_DIR + '/*' + FORMATTED_RUN + '*_*' + FORMATTED_FILE + '*.*'
		return glob.glob(pathstring)

	# Else just require run # in name
	pathstring = INDATA_DIR + '/*' + FORMATTED_RUN + '*.*'
	return glob.glob(pathstring)

######################################################## ADD JOB #########################################################

def find_num_threads(JANA_CONFIG_FILENAME):
	num_threads = "1"

	# Read in JANA config file
	infile_config = open(JANA_CONFIG_FILENAME, 'r')
	for line in infile_config:

		# Ignore empty lines, and lines that begin with #
		if (len(line.split()) == 0) or (line.split()[0][0] == '#'):
			continue

		# Save #-threads if correct key
		key = str(line.split()[0])
		if (key != "NTHREADS"):
			continue
		num_threads = line.split()[1]
		break;

	return num_threads

def add_job(WORKFLOW, FILEPATH, config_dict):

	# EXTRACT PATH, RUNNO, & FILE #: ASSUME THE FORM IS EITHER */*_RUNNO_FILENO.* OR */*_RUNNO.*
	match = re.search(r"(.*)/(.*)_(\d\d\d\d\d\d)_(\d\d\d).(.*)", FILEPATH)
	if(match is not None):
		INDATA_DIR = match.group(1)
		PREFIX = match.group(2)
		RUNNO = match.group(3)
		FILENO = match.group(4)
		EXTENSION = match.group(5)
	else: # Try with no file #
		match = re.search(r"(.*)/(.*)_(\d\d\d\d\d\d).(.*)", FILEPATH)
		if(match is None):
			print "WARNING: FILE " + FILEPATH + " DOESN'T MATCH EXPECTED NAME FORMAT. SKIPPING."
			return
		INDATA_DIR = match.group(1)
		PREFIX = match.group(2)
		RUNNO = match.group(3)
		FILENO = "-1"
		EXTENSION = match.group(4)
	if(VERBOSE == True):
		print "FILEPATH, COMPONENTS: " + FILEPATH + " " + INDATA_DIR + " " + PREFIX + " " + RUNNO + " " + FILENO + " " + EXTENSION

	# PREPARE NAMES
	DATE = time.strftime("%Y-%m-%d")
	STUBNAME = RUNNO if(FILENO == "-1") else RUNNO + "_" + FILENO
	FILENAME = PREFIX + "_" + RUNNO + "." + EXTENSION if(FILENO == "-1") else PREFIX + "_" + RUNNO + "_" + FILENO + "." + EXTENSION
	if(WORKFLOW.find("ver") == -1):
		JOBNAME = WORKFLOW + "_" + STUBNAME + "_" + DATE
	else:
		JOBNAME = WORKFLOW + "_" + STUBNAME

	# SETUP OTHER VARIABLES:
	INPUTDATA_TYPE = "mss" if(INDATA_DIR[:5] == "/mss/") else "file"
	CACHE_PIN_DAYS = config_dict["CACHE_PIN_DAYS"] if ("CACHE_PIN_DAYS" in config_dict) else "0"
	JANA_CONFIG = config_dict["JANA_CONFIG"] if ("JANA_CONFIG" in config_dict) else "NA"
	NUM_THREADS = find_num_threads(JANA_CONFIG) if ("JANA_CONFIG" in config_dict) else "1"

        # SETUP LOG DIRECTORY FOR SLURM
        if(FILENO != "-1"):
		LOG_DIR = config_dict["OUTDIR_SMALL"] + "/log/" + RUNNO
	else:
		LOG_DIR = config_dict["OUTDIR_SMALL"] + "/log"
        make_log_dir = "mkdir -p " + LOG_DIR
        try_command(make_log_dir)
        if(VERBOSE == True):
                print "LOG DIRECTORY " + LOG_DIR + " CREATED"

	# CREATE ADD-JOB COMMAND
	# job
	add_command = "swif2 add-job -workflow " + WORKFLOW + " -name " + JOBNAME
	# accounting
	add_command += " -account " + config_dict["PROJECT"] + " -partition " + config_dict["TRACK"] + " -os " + config_dict["OS"]
	# resources
	add_command += " -cores " + config_dict["NCORES"] + " -disk " + config_dict["DISK"] + " -ram " + config_dict["RAM"] + " -time " + config_dict["TIMELIMIT"]
	# inputs
	add_command += " -input " + FILENAME + " " + INPUTDATA_TYPE + ":" + INDATA_DIR + "/" + FILENAME
	# stdout, stderr
	if(FILENO != "-1"):
		add_command += " -stdout " + config_dict["OUTDIR_SMALL"] + "/log/" + RUNNO + "/stdout." + STUBNAME + ".out"
		add_command += " -stderr " + config_dict["OUTDIR_SMALL"] + "/log/" + RUNNO + "/stderr." + STUBNAME + ".err"
	else:
		add_command += " -stdout " + config_dict["OUTDIR_SMALL"] + "/log/stdout." + STUBNAME + ".out"
		add_command += " -stderr " + config_dict["OUTDIR_SMALL"] + "/log/stderr." + STUBNAME + ".err"

	# tags
	add_command += " -tag run_number " + RUNNO + " -tag num_threads " + NUM_THREADS
	# file # tag
	if(FILENO != "-1"):
		add_command += " -tag file_number " + FILENO
	# command + arguments
	add_command += " " + config_dict["SCRIPTFILE"] + " " + config_dict["ENVFILE"] + " " + FILENAME + " " + JANA_CONFIG
	# command arguments continued
	add_command += " " + config_dict["OUTDIR_LARGE"] + " " + config_dict["OUTDIR_SMALL"] + " " + RUNNO + " " + FILENO + " " + CACHE_PIN_DAYS

	# optional command arguments
	if('ROOT_SCRIPT' in config_dict):
		add_command += " " + config_dict["ROOT_SCRIPT"]
	if('TREE_NAME' in config_dict):
		add_command += " " + config_dict["TREE_NAME"]
	if('SELECTOR_NAME' in config_dict):
		add_command += " " + config_dict["SELECTOR_NAME"]
		add_command += " " + config_dict["NCORES"]
	if('WEBDIR_SMALL' in config_dict):
		add_command += " " + config_dict["WEBDIR_SMALL"]
	if('WEBDIR_LARGE' in config_dict):
		add_command += " " + config_dict["WEBDIR_LARGE"]

	if(VERBOSE == True):
		print "job add command is \n" + str(add_command)

	# ADD JOB
	status = try_command(add_command)

########################################################## MAIN ##########################################################

def main(argv):
	global VERBOSE # so can modify here

	# PARSER
	parser_usage = "launch.py job_configfile minrun maxrun\n\n"
	parser_usage += "optional: -f file_num: file_num must be 3 digits, with leading 0's if necessary)\n"
	parser_usage += "          but, it can be a search string for glob (e.g. first 5 files: -f '00[0-4]' (MUST include quotes!))\n\n"
	parser_usage += "optional: -v True: verbose output\n\n"
	parser = OptionParser(usage = parser_usage)

	# PARSER OPTIONS
	parser.add_option("-f", "--file", dest="file", help="specify file(s) to run over")
	parser.add_option("-v", "--verbose", dest="verbose", help="verbose output")

	# GET ARGUMENTS
	(options, args) = parser.parse_args(argv)
	if(len(args) < 3):
		parser.print_help()
		return

	# SET INPUT VARIABLES
	JOB_CONFIG_FILE = args[0]
	MINRUN = int(args[1])
	MAXRUN = int(args[2])
	VERBOSE = True if(options.verbose) else False
	INPUT_FILE_NUM = options.file if(options.file) else "*" #must be three digits, with leading 0's if necessary

	# READ CONFIG
	config_dict = read_config(JOB_CONFIG_FILE)
	validate_config(config_dict)

	# SET CONTROL VARIABLES
	WORKFLOW = config_dict["WORKFLOW"]
	RCDB_QUERY = config_dict["RCDB_QUERY"] if ("RCDB_QUERY" in config_dict) else ""
	INDATA_TOPDIR = config_dict["INDATA_TOPDIR"] if ("INDATA_TOPDIR" in config_dict) else ""

	# GET THE LIST OF GOOD RUNS
	db = rcdb.RCDBProvider("mysql://rcdb@hallddb/rcdb")
	good_runs = []
	if(VERBOSE == True):
		print "RCDB_QUERY = " + RCDB_QUERY
	if(RCDB_QUERY != ""):
		good_runs = db.select_runs(RCDB_QUERY, MINRUN, MAXRUN)
	if(VERBOSE == True):
		print str(len(good_runs)) + " good runs in range: " + str(MINRUN) + " - " + str(MAXRUN)

	# FIND & ADD JOBS
	for RUN in range(MINRUN, MAXRUN + 1):

		# See if is good run
		rcdb_run_info = db.get_run(int(RUN))
		if(RCDB_QUERY != "") and (rcdb_run_info not in good_runs):
			continue

		# Format run number
		FORMATTED_RUN = "%06d" % RUN

		# Find files for run number: First try separate folder for each run
		INDATA_DIR = INDATA_TOPDIR + "/*" + FORMATTED_RUN + "*/"
		file_list = find_files(INDATA_DIR, FORMATTED_RUN, INPUT_FILE_NUM)
		if(len(file_list) == 0): # No files. Now just try the input dir
			INDATA_DIR = INDATA_TOPDIR + "/"
			file_list = find_files(INDATA_DIR, FORMATTED_RUN, INPUT_FILE_NUM)

		if(VERBOSE == True):
			print str(len(file_list)) + " files found for run " + str(RUN)

		# Add jobs to workflow
		for FILEPATH in file_list:
                        add_job(WORKFLOW, FILEPATH, config_dict)

if __name__ == "__main__":
   main(sys.argv[1:])

