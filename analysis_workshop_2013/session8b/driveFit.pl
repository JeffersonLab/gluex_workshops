#!/usr/bin/perl

use Cwd;

# be sure that these settings agree with what was used in the divideData script
$nBins = 65;
$fitName = "threepi_fit";

# this directory can be adjusted if you want to do the fit elsewhere
# but it needs to be an explicit path
$workingDir = getcwd();

# this is the name of the file that will be used to store values used
# to see the parameters inthe fit
$seedFile = "param_init.cfg";

### things below here probably don't need to be modified

$fitDir = "$workingDir/$fitName";
$lastParams = "$fitDir/$seedFile";

for( $i = 0; $i < $nBins; ++$i ){
  
  chdir $fitDir;
  chdir "bin_$i";
  
  print "Fitting in bin $i...\n";
  
  system( "fit -c bin_$i.cfg -s $seedFile >& bin_$i.log" );
  
  if( -e "$seedFile" ){ system( "cp -f $seedFile .." ); }
  
  chdir $fitDir;
}

