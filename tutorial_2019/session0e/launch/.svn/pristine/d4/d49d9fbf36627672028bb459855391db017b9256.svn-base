#!/bin/bash
#
# This should be placed in the "launch" directory in the NERSC
# project directory used for the job. e.g.
#
#     /global/project/projectdirs/m3120/launch/script_nersc.sh
#
# This script will wake up in shifter container in the job directory
# created by swif2. GlueX software is available via CVMFS.
#
# Arguments:
#
# arg 1:  JANA config file
# arg 2:  sim-recon version
# arg 3:  run     <--+ run and file number used to name job_info
# arg 4:  file    <--+ directory only.

JANA_CONFIG=$1
HALLD_RECON_VERSION=$2
RUN=$3
FILE=$4

ulimit -c unlimited

# Setup environment for the specified sim-recon version
setenv_file=/cvmfs/oasis.opensciencegrid.org/gluex/group/halld/Software/builds/Linux_CentOS7-x86_64-gcc4.8.5-cntr/${HALLD_RECON_VERSION}/Linux_CentOS7-x86_64-gcc4.8.5-cntr/setenv.sh
source $setenv_file

# Use CCDB and RCDB from CVMFS. Make a temporary local
# copy so that we don't interfere with other jobs locking
# the same file
cp /group/halld/www/halldweb/html/dist/ccdb.sqlite /tmp
cp /group/halld/www/halldweb/html/dist/rcdb.sqlite /tmp
export JANA_CALIB_URL=sqlite:////tmp/ccdb.sqlite
export CCDB_CONNECTION=$JANA_CALIB_URL
export RCDB_CONNECTION=sqlite:////tmp/rcdb.sqlite

# Use JANA_RESOURCE_DIR from CVMFS
export JANA_RESOURCE_DIR=/cvmfs/oasis.opensciencegrid.org/gluex/group/halld/www/halldweb/html/resources

# Record some info about the node and environment
rm -f top.out
top -b -n 1 > top.out

rm -f cpuinfo.out
cat /proc/cpuinfo > cpuinfo.out

rm -f env.out
env > env.out

rm -f hostname.out
hostname > hostname.out

# Run script in background that will send abort signal after 8.5 hours
/launch/assassin.sh &> assassin.out  &


# Run hd_root
hd_root --config=${JANA_CONFIG} hd_rawdata_??????_???.evio 2> std.err 1> std.out

# Move small files into a directory and make a tarball
JOB_INFO=$(printf "job_info_%06d_%03d" $RUN $FILE)
mkdir $JOB_INFO
mv top.out cpuinfo.out env.out hostname.out std.err std.out $JOB_INFO
tar czf ${JOB_INFO}.tgz $JOB_INFO
rm -rf $JOB_INFO

# The converted_random.hddm file may or may not exist, depending on
# whether the file had random trigger events in it. In order for
# swif2 to copy it if it does, but ignore it if not, we have to use
# the "match:" feature. This requires the destination be a directory.
# Thus, we need to rename the file here to it's final name since swif2
# won't rename it while copying it.
if [ -f converted_random.hddm ]; then
	mv converted_random.hddm $(printf "converted_random_%06d_%03d.hddm" $RUN $FILE)
fi

# Remove link to input file.
# The swif2 job will copy all files in this directory back
# to JLab so we don't want the raw data file to be copied back.
rm -f hd_rawdata_??????_???.evio

# Remove ccdb.sqlite and rcdb.sqlit files
rm -f /tmp/ccdb.sqlite /tmp/rcdb.sqlite
