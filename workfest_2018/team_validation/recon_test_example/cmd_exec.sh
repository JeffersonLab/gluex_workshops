#!/bin/bash
# executes analysis in local directory, mainly for testing
# usage:  ./cmd_exec.sh [RUN] [FILE]

USAGE="./cmd_exec.sh [RUN] [FILE]"

# HANDLE COMMAND LINE ARGS

RUN=$1
if [ -z $RUN ]; then
    echo $USAGE
    exit 1
fi
RUN=$2
if [ -z $RUN ]; then
    echo $USAGE
    exit 1
fi

RUN_NUMBER=`echo ${RUN} | awk '{printf "%06d\n",$0;}'`
FILE_NUMBER=`echo ${FILE} | awk '{printf "%03d\n",$0;}'`

# SET UP INPUT FILE
#INPUTFILE=/cache/halld/RunPeriod-2017-01/rawdata/Run${RUN_NUMBER}/dana_rest_${RUN_NUMBER}_${FILE_NUMBER}.hddm data.evio
INPUTFILE=/cache/halld/offline_monitoring/RunPeriod-2017-01/ver33/REST/${RUN_NUMBER}/dana_rest_${RUN_NUMBER}_${FILE_NUMBER}.hddm data.evio

# SET UP TEST OUTPUT DIRECTORIES
if [ ! -d out ]; then 
    mkdir out
fi
if [ ! -d web ]; then 
    mkdir web
fi

# SET UP PARAMETERS

ENVIRONMENT=env_recon_test.sh
#INPUTFILE=data.evio
CONFIG_FILE=jana_recon_test.config
OUTDIR_LARGE=out
OUTDIR_SMALL=out
CACHE_PIN_DAYS=14
WEBDIR_SMALL=web
WEBDIR_LARGE=web


#### RUN COMMAND ####

./script.sh $ENVIRONMENT $INPUTFILE $CONFIG_FILE $CONFIG_FILE $OUTDIR_LARGE $OUTDIR_SMALL $RUN_NUMBER $FILE_NUMBER $CACHE_PIN_DAYS $WEBDIR_SMALL $WEBDIR_LARGE