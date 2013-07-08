#!/usr/bin/perl

use Cwd;

$lowMass = 0.7;
$highMass = 2.0;
$nBins = 65;

$fitName = "threepi_fit";

# put a limit on the number of data events to process
# gen MC and acc MC smaples are not limited
$maxEvts = 1E9;

# this directory can be adjusted if you want to do the fit elsewhere
# but it needs to be an explicit path
$workingDir = getcwd();

# these files must exist in the working directory.  If you don't know how
# to generate them or don't have them, see the documentation in gen_3pi
# the Simulation area of the repository
$dataFile = "$workingDir/threepi_data.root";
$accMCFile = "$workingDir/threepi_acc.root";
$genMCFile = "$workingDir/threepi_gen.root";

# this file sould be used for partially polarized or unpolarized beam fits
#$cfgTempl = "$workingDir/threepi_unpol_TEMPLATE.cfg";

# this file should be used when there is 100% beam polarization
$cfgTempl = "$workingDir/threepi_pol_TEMPLATE.cfg";


### things below here probably don't need to be modified

# this is where the goodies for the fit will end up
$fitDir = "$workingDir/$fitName/";
mkdir $fitDir unless -d $fitDir;

chdir $fitDir;

# use the split_mass command line tool to divide up the
# data into bins of resonance mass

@dataParts = split /\//, $dataFile;
$dataTag = pop @dataParts;
$dataTag =~ s/\.root//;
system( "split_mass $dataFile $dataTag $lowMass $highMass $nBins $maxEvts" );

@accMCParts = split /\//, $accMCFile;
$accMCTag = pop @accMCParts;
$accMCTag =~ s/\.root//;
system( "split_mass $accMCFile $accMCTag $lowMass $highMass $nBins" );

@genMCParts = split /\//, $genMCFile;
$genMCTag = pop @genMCParts;
$genMCTag =~ s/\.root//;
system( "split_mass $genMCFile $genMCTag $lowMass $highMass $nBins" );

# make directories to perform the fits in
for( $i = 0; $i < $nBins; ++$i ){

  mkdir "bin_$i" unless -d "bin_$i";

  system( "mv *\_$i.root bin_$i" );

  chdir "bin_$i";

  open( CFGOUT, ">bin_$i.cfg" );
  open( CFGIN, $cfgTempl );

  while( <CFGIN> ){

    s/DATAFILE/$dataTag\_$i.root/;
    s/ACCMCFILE/$accMCTag\_$i.root/;
    s/GENMCFILE/$genMCTag\_$i.root/;
    s/NIFILE/bin_$i.ni/;
    s/FITNAME/bin_$i/;

    print CFGOUT $_;
  }

  close CFGOUT;
  close CFGIN;
  
  system( "touch param_init.cfg" );

  chdir $fitDir;
}

