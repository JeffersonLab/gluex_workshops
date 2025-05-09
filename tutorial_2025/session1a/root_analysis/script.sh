#!/bin/bash

############################################################# SETUP #############################################################

Setup_Script()
{
	# PWD, STATUS OF MOUNTED DISKS
	echo "pwd =" $PWD
	if [[ $PWD != /scratch/slurm/${SLURM_JOB_ID} ]] ; then
	    echo "LOCAL DIRECTORY " $PWD " NOT SUPPORTED"
	    exit 1
	fi
	echo "df -h:"
	df -h

	# ENVIRONMENT (shell script or xml?)
	ENV_EXT="${ENVIRONMENT##*.}"
	ENV_DIR=`dirname $ENVIRONMENT`
	if [[ $ENV_EXT == "sh" ]] ; then
	    source $ENVIRONMENT
	elif [[ $ENV_EXT == "xml" ]] ; then
	    if [[ $ENV_DIR == "." ]] ; then
		source /group/halld/Software/build_scripts/gluex_env_jlab.sh /group/halld/www/halldweb/html/halld_versions/$ENVIRONMENT
	    else
		source /group/halld/Software/build_scripts/gluex_env_jlab.sh $ENVIRONMENT
	    fi
	else
	    echo "ENVIRONMENT " $ENVIRONMENT " not supported"
	    exit 1
	fi
	printenv
	echo "PERL INCLUDES: "
	perl -e "print qq(@INC)"

	# COPY CCDB SQLITE FILE TO LOCAL DISK
	if [[ $CCDB_CONNECTION  == *"sqlite"* ]] ; then
	    local SCRATCH=/scratch/slurm/${SLURM_JOB_ID}
	    ls -l $SCRATCH
	    local NEW_SQLITE=${SCRATCH}/ccdb.sqlite
            cp -v ${CCDB_CONNECTION:10} $NEW_SQLITE
            export CCDB_CONNECTION=sqlite:///$NEW_SQLITE
            export JANA_CALIB_URL=sqlite:///$NEW_SQLITE
            echo "JANA_CALIB_URL: " $JANA_CALIB_URL
	fi

	# LIST WORKING DIRECTORY
	echo "LOCAL FILES"
	ls -l
}

####################################################### UTILITY FUNCTIONS #######################################################

Extract_BaseName()
{
	# base name is everything before the last period
	local INPUT_FILE=$1
	local LENGTH=`echo $INPUT_FILE | awk '{print index($0,".")}'`
	let LENGTH-=1
	local LOCAL_BASE_NAME=`echo $INPUT_FILE | awk -v size="$LENGTH" '{print substr($0,1,size)}'`
	echo "BASE_NAME: " $LOCAL_BASE_NAME

	#return the result "by reference"
	local __result=$2
	eval $__result="'$LOCAL_BASE_NAME'"
}


Extract_FileName()
{
	# file name is everything after the last slash
	local INPUT_PATH=$1
	local SLASH_INDEX=`echo $INPUT_PATH | awk -F"/" '{print length($0)-length($NF)}'`
	local LOCAL_FILE_NAME=`echo ${INPUT_PATH:$SLASH_INDEX}`
	echo "FILE_NAME: " $LOCAL_FILE_NAME

	#return the result "by reference"
	local __result=$2
	eval $__result="'$LOCAL_FILE_NAME'"
}

####################################################### SAVE OUTPUT FILES #######################################################

Save_OutputFiles()
{
	# SEE WHAT FILES ARE PRESENT
	echo "FILES PRESENT PRIOR TO SAVE:"
	ls -l

	# REMOVE INPUT FILE: so that it's easier to determine which remaining files are skims
	rm -f $INPUTFILE

	# CALL SAVE FUNCTIONS
	Save_ROOTFiles
}

Save_ROOTFiles()
{
	# SAVE OTHER ROOT FILES
	local NUM_FILES=`ls *.root 2>/dev/null | wc -l`
	if [ $NUM_FILES -eq 0 ] ; then
		echo "No additional ROOT files produced"
		exit 1
	fi

	echo "Saving other ROOT files"
	for ROOT_FILE in `ls *.root`; do
		Extract_BaseName $ROOT_FILE BASE_NAME

		# setup output
		if [ "${FILE_NUMBER}" != "-1" ] ; then
			local OUTDIR_THIS=${OUTDIR_LARGE}/${BASE_NAME}/${RUN_NUMBER}/
			local OUTPUT_FILE=${OUTDIR_THIS}/${BASE_NAME}_${RUN_NUMBER}_${FILE_NUMBER}.root
		else
			local OUTDIR_THIS=${OUTDIR_LARGE}/${BASE_NAME}/
			local OUTPUT_FILE=${OUTDIR_THIS}/${BASE_NAME}_${RUN_NUMBER}.root
		fi

		# save it
		echo "Adding $ROOT_FILE to swif2 output: $OUTPUT_FILE"
		swif2 output $ROOT_FILE $OUTPUT_FILE
	done
}

########################################################## MAIN FUNCTION ########################################################

Run_Script()
{
	Setup_Script

	# RUN ROOT
	Extract_FileName $SELECTOR_NAME SELECTOR_FILE
	cp ${SELECTOR_NAME}.* .
	root -b -q $ROOT_ANALYSIS_HOME/scripts/Load_DSelector.C $ROOT_SCRIPT'("'$INPUTFILE'", "'$TREE_NAME'", "'${SELECTOR_FILE}.C+'", '${NUM_THREADS}')'

	# RETURN CODE
	RETURN_CODE=$?
	echo "Return Code = " $RETURN_CODE
	if [ $RETURN_CODE -ne 0 ]; then
		exit $RETURN_CODE
	fi

	# SAVE OUTPUTS
	Save_OutputFiles
}

######################################################### EXECUTE SCRIPT ########################################################

# SET INPUTS
ENVIRONMENT=$1
INPUTFILE=$2
CONFIG_FILE=$3
OUTDIR_LARGE=$4
OUTDIR_SMALL=$5
RUN_NUMBER=$6
FILE_NUMBER=$7
ROOT_SCRIPT=$8
TREE_NAME=$9
SELECTOR_NAME=${10}
NUM_THREADS=${11}

# PRINT INPUTS
echo "HOSTNAME          = $HOSTNAME"
echo "ENVIRONMENT       = $ENVIRONMENT"
echo "INPUTFILE         = $INPUTFILE"
echo "CONFIG_FILE       = $CONFIG_FILE"
echo "OUTDIR_LARGE      = $OUTDIR_LARGE"
echo "OUTDIR_SMALL      = $OUTDIR_SMALL"
echo "RUN_NUMBER        = $RUN_NUMBER"
echo "FILE_NUMBER       = $FILE_NUMBER"
echo "ROOT_SCRIPT       = $ROOT_SCRIPT"
echo "TREE_NAME         = $TREE_NAME"
echo "SELECTOR_NAME     = $SELECTOR_NAME"
echo "NUM_THREADS       = $NUM_THREADS"

# RUN
Run_Script

