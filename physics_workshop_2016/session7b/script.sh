#!/bin/csh -f

# SET INPUTS
setenv ENVIRONMENT $1
setenv INPUTFILE $2
setenv CONFIG_FILE $3
setenv OUTDIR $4
setenv RUN_NUMBER $5
setenv FILE_NUMBER $6
setenv TREE_NAME_STRING $7 #comma-separated

# PRINT INPUTS
echo "ENVIRONMENT       = $ENVIRONMENT"
echo "INPUTFILE         = $INPUTFILE"
echo "CONFIG_FILE       = $CONFIG_FILE"
echo "OUTDIR            = $OUTDIR"
echo "RUN_NUMBER        = $RUN_NUMBER"
echo "FILE_NUMBER       = $FILE_NUMBER"
echo "TREE_NAME_STRING  = $TREE_NAME_STRING"

# ENVIRONMENT
source $ENVIRONMENT
echo pwd = $PWD
printenv

# COPY INPUT FILE TO WORKING DIRECTORY
# This step is necessary since the cache files will be created as soft links in the current directory, and we want to avoid large I/O processes.
# We first copy the input file to the current directory, then remove the link.
ls -l
cp $INPUTFILE ./tmp_file
rm -f $INPUTFILE
mv tmp_file $INPUTFILE
ls -l

# RUN JANA
hd_root $INPUTFILE --config=$CONFIG_FILE

# RETURN CODE
set RETURN_CODE = $?
echo "Return Code = " $RETURN_CODE
if ($RETURN_CODE != 0) then
	exit $RETURN_CODE
endif

# save output histograms
mkdir -p -m 775 ${OUTDIR}/hists/${RUN_NUMBER}/
if (-e hd_root.root) then
	cp -v hd_root.root ${OUTDIR}/hists/${RUN_NUMBER}/hd_root_${RUN_NUMBER}_${FILE_NUMBER}.root
	chmod 664 ${OUTDIR}/hists/${RUN_NUMBER}/hd_root_${RUN_NUMBER}_${FILE_NUMBER}.root
endif

# save output REST file (if any)
if (-e dana_rest.hddm) then
	mkdir -p -m 775 ${OUTDIR}/REST/${RUN_NUMBER}/
	cp -v dana_rest.hddm ${OUTDIR}/REST/${RUN_NUMBER}/dana_rest_${RUN_NUMBER}_${FILE_NUMBER}.hddm
	chmod 664 ${OUTDIR}/REST/${RUN_NUMBER}/dana_rest_${RUN_NUMBER}_${FILE_NUMBER}.hddm
endif

# save output trees
set tree_list = `echo $TREE_NAME_STRING:q | sed 's/,/ /g'`
foreach tree_var ($tree_list:q)
	set TREE_NAME = $tree_var:q
	echo "TREE_BASE_NAME = " $TREE_NAME
	mkdir -p -m 775 ${OUTDIR}/${TREE_NAME}/${RUN_NUMBER}/
	if (-e tree_${TREE_NAME}.root) then
		cp -v tree_${TREE_NAME}.root ${OUTDIR}/${TREE_NAME}/${RUN_NUMBER}/tree_${TREE_NAME}_${RUN_NUMBER}_${FILE_NUMBER}.root
		chmod 664 ${OUTDIR}/${TREE_NAME}/${RUN_NUMBER}/tree_${TREE_NAME}_${RUN_NUMBER}_${FILE_NUMBER}.root
	endif
end

