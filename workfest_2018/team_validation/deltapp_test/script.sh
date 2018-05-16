#!/bin/bash

############################################################# SETUP #############################################################

Setup_Script()
{
	# PWD, STATUS OF MOUNTED DISKS
	echo "pwd =" $PWD
	#echo "df -h:"
	#df -h

	# ENVIRONMENT
	source $ENVIRONMENT
	printenv
	echo "PERL INCLUDES: "
	perl -e "print qq(@INC)"
	echo ""

    if [ "$INTERACTIVE" -ne "1" ]; then
	    # COPY INPUT FILE TO WORKING DIRECTORY
	    # This step is necessary since the cache files will be created as soft links in the current directory, and we want to avoid large I/O processes.
	    # We first copy the input file to the current directory, then remove the link.
	    echo "LOCAL FILES PRIOR TO INPUT COPY"
	    ls -l
	    cp $INPUTFILE ./tmp_file
	    rm -f $INPUTFILE
	    mv tmp_file $INPUTFILE
    fi

	echo "LOCAL FILES AFTER INPUT COPY"
	ls -l
}

####################################################### SAVE OUTPUT FILES #######################################################

Save_OutputFiles()
{
	# SEE WHAT FILES ARE PRESENT
	echo "FILES PRESENT PRIOR TO SAVE:"
	ls -l

	# REMOVE INPUT FILE: so that it's easier to determine which remaining files are skims
    if [ "$INTERACTIVE" -ne "1" ]; then
	    rm -f $INPUTFILE
    fi

	# BUILD TAPEDIR, IF $OUTDIR_LARGE STARTS WITH "/cache/"
	# AND CACHE_PIN_DAYS WAS GIVEN AND GREATER THAN 0  && [ "$CACHE_PIN_DAYS" -gt "0" ]
	# If so, output files are pinned & jcache put.  If not, then they aren't. 
	local TAPEDIR=""
	local OUTDIR_LARGE_BASE=`echo $OUTDIR_LARGE | awk '{print substr($0,1,7)}'`
	# first strip /cache/, then insert /mss/
	if [ "$OUTDIR_LARGE_BASE" == "/cache/" ]; then
		local OUTPATH=`echo $OUTDIR_LARGE | awk '{print substr($0,8)}'`
		TAPEDIR=/mss/${OUTPATH}/
	fi

	# CALL SAVE FUNCTIONS
	Save_Histograms
	Save_REST
	Save_JANADot

	# SEE WHAT FILES ARE LEFT
	echo "FILES REMAINING AFTER SAVING:"
	ls -l
}

Save_Histograms()
{
	# SAVE ROOT HISTOGRAMS
	if [ -e hd_root.root ]; then
		echo "Saving histogram file"

		# setup output dirs
		local OUTDIR_THIS=${OUTDIR_LARGE}/hists/${RUN_NUMBER}/
		mkdir -p -m 755 ${OUTDIR_THIS}

		# save it to web dir
		mkdir -p -m 755 ${WEBDIR_LARGE}
		cp hd_root.root ${WEBDIR_LARGE}/hd_root_${RUN_NUMBER}_${FILE_NUMBER}.root

		# save it
		local OUTPUT_FILE=${OUTDIR_THIS}/hd_root_${RUN_NUMBER}_${FILE_NUMBER}.root
		mv -v hd_root.root $OUTPUT_FILE
		chmod 644 $OUTPUT_FILE

		# force save to tape & pin
		if [ "$TAPEDIR" != "" ]; then
			echo jcache pin $OUTPUT_FILE -D $CACHE_PIN_DAYS
			jcache pin $OUTPUT_FILE -D $CACHE_PIN_DAYS
			echo jcache put $OUTPUT_FILE
			jcache put $OUTPUT_FILE
		fi
	fi
}

Save_REST()
{
	# SAVE REST FILE
	if [ -e dana_rest.hddm ]; then
		echo "Saving REST file"

		# setup output dirs
		local OUTDIR_THIS=${OUTDIR_LARGE}/REST/${RUN_NUMBER}/
		mkdir -p -m 755 $OUTDIR_THIS

		# save it
		local OUTPUT_FILE=${OUTDIR_THIS}/dana_rest_${RUN_NUMBER}_${FILE_NUMBER}.hddm
		mv -v dana_rest.hddm $OUTPUT_FILE
		chmod 644 $OUTPUT_FILE

		# force save to tape & pin
		if [ "$TAPEDIR" != "" ]; then
		        echo jcache pin $OUTPUT_FILE -D $CACHE_PIN_DAYS
			jcache pin $OUTPUT_FILE -D $CACHE_PIN_DAYS
			echo jcache put $OUTPUT_FILE
			jcache put $OUTPUT_FILE
		fi
	fi
}

Save_JANADot()
{
	# SAVE JANADOT FILE
	if [ -e jana.dot ]; then
		echo "Saving JANADOT file"
		dot -Tps2 jana.dot -o jana.ps
		ps2pdf jana.ps

		# setup output dir
		local OUTDIR_THIS=${OUTDIR_SMALL}/log/${RUN_NUMBER}/
		mkdir -p -m 755 $OUTDIR_THIS

		# save it
		local OUTPUT_FILE=${OUTDIR_THIS}/janadot_${RUN_NUMBER}_${FILE_NUMBER}.pdf
		mv -v jana.pdf $OUTPUT_FILE
		chmod 644 $OUTPUT_FILE
	fi
}

########################################################### RUN DSELECTOR ##########################################################

Run_DSelector()
{
    root -b -q runDSelector.C
    
}


########################################################### MAKE PLOTS ##########################################################

Make_Plots()
{
	#cp $MONITORING_HOME/recon_test/Make_Plots.C .
	#root -b -q hd_root.root 'Make_Plots.C("$HALLD_HOME/src/plugins/Analysis/monitoring_hists/HistMacro_NumHighLevelObjects.C", "HistMacro_NumHighLevelObjects.png")'
	#root -b -q hd_root.root 'Make_Plots.C("$HALLD_HOME/src/plugins/Analysis/monitoring_hists/HistMacro_EventInfo.C", "HistMacro_EventInfo.png")'
	#root -b -q hd_root.root 'Make_Plots.C("$HALLD_HOME/src/plugins/monitoring/highlevel_online/HistMacro_Kinematics.C", "HistMacro_Kinematics.png")'
	#root -b -q hd_root.root 'Make_Plots.C("$HALLD_HOME/src/plugins/Analysis/p2pi_hists/HistMacro_p2pi.C", "HistMacro_p2pi.png")'
	#root -b -q hd_root.root 'Make_Plots.C("$HALLD_HOME/src/plugins/Analysis/p3pi_hists/HistMacro_p3pi.C", "HistMacro_p3pi.png")'
    root -b -q Make_Delta_Plots.C

	# setup output dir
	local OUTDIR_THIS=${WEBDIR_SMALL}/${RUN_NUMBER}/
	mkdir -p -m 755 $OUTDIR_THIS

	# save it
	cp *.png ${OUTDIR_THIS}
	chmod 644 ${OUTDIR_THIS}/*.png

	# copy html
	cp $MONITORING_HOME/recon_test/index.html ${OUTDIR_THIS}
}

########################################################### CCDB SQLITE #########################################################

Create_SQLite()
{
	$CCDB_HOME/scripts/mysql2sqlite/mysql2sqlite.sh -hhallddb.jlab.org -uccdb_user ccdb | sqlite3 ccdb.sqlite
}

########################################################## MAIN FUNCTION ########################################################

Run_Script()
{
	Setup_Script
    if [ "$INTERACTIVE" -eq "1" ]; then
        export JANA_CALIB_URL=mysql://ccdb_user@hallddb.jlab.org/ccdb
    else
	    Create_SQLite
    fi

	# RUN JANA
	hd_root $INPUTFILE --config=$CONFIG_FILE

	# RETURN CODE
	RETURN_CODE=$?
	echo "Return Code = " $RETURN_CODE
	if [ $RETURN_CODE -ne 0 ]; then
		exit $RETURN_CODE
	fi

	# SAVE OUTPUTS
    Run_DSelector
	Make_Plots
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
CACHE_PIN_DAYS=$8
WEBDIR_SMALL=$9
WEBDIR_LARGE=${10}
INTERACTIVE=${11}

if [ -z $INTERACTIVE ]; then
    INTERACTIVE=0
fi

# PRINT INPUTS
echo "HOSTNAME          = $HOSTNAME"
echo "ENVIRONMENT       = $ENVIRONMENT"
echo "INPUTFILE         = $INPUTFILE"
echo "CONFIG_FILE       = $CONFIG_FILE"
echo "OUTDIR_LARGE      = $OUTDIR_LARGE"
echo "OUTDIR_SMALL      = $OUTDIR_SMALL"
echo "RUN_NUMBER        = $RUN_NUMBER"
echo "FILE_NUMBER       = $FILE_NUMBER"
echo "CACHE_PIN_DAYS    = $CACHE_PIN_DAYS"
echo "WEBDIR_SMALL      = $WEBDIR_SMALL"
echo "WEBDIR_LARGE      = $WEBDIR_LARGE"
echo "INTERACTIVE       = $INTERACTIVE"

if [ "$INTERACTIVE" -ne "1" ]; then
    exit 0
fi

# RUN
Run_Script

