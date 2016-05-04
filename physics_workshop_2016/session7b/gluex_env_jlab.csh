#!/bin/bash
set VERSION_XML=/group/halld/Software/workshops/physics_workshop_2016/session7b/version_workshop.xml   ## change this if you modify the software versions
setenv BUILD_SCRIPTS /group/halld/Software/build_scripts
setenv BMS_OSNAME `$BUILD_SCRIPTS/osrelease.pl`
setenv GLUEX_TOP /group/halld/Software/builds/$BMS_OSNAME
# perl on the cue
setenv PATH /apps/perl/bin:$PATH
# finish the rest of the environment
source $BUILD_SCRIPTS/gluex_env_version.csh $VERSION_XML
setenv JANA_CALIB_URL $CCDB_CONNECTION
setenv JANA_RESOURCE_DIR /group/halld/www/halldweb/html/resources
# python on the cue
setenv PATH /apps/python/PRO/bin:$PATH
setenv LD_LIBRARY_PATH /apps/python/PRO/lib:$LD_LIBRARY_PATH
