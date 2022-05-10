#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <cassert>
#include <cstdlib>
#include <unistd.h>

#include "IUAmpTools/FitResults.h"

using namespace std;

int main( int argc, char* argv[] ){
    
    // these params should probably come in on the command line
    double lowMass = 0.7;
    double highMass = 2.05;
    enum{ kNumBins = 30 };
    string fitDir( "EtaPi_fit" );
    
    // set default parameters
    
    string outfileName("");
    
    // parse command line
    
    for (int i = 1; i < argc; i++){
        
        string arg(argv[i]);
        
        if (arg == "-o"){
            if ((i+1 == argc) || (argv[i+1][0] == '-')) arg = "-h";
            else  outfileName = argv[++i]; }
        if (arg == "-h"){
            cout << endl << " Usage for: " << argv[0] << endl << endl;
            cout << "\t -o <file>\t Ouput text file" << endl;
            exit(1);}
    }
    
    if (outfileName.size() == 0){
        cout << "No output file specified" << endl;
        exit(1);
    }
    
    double step = ( highMass - lowMass ) / kNumBins;
    
    ofstream outfile;
    outfile.open( outfileName.c_str() );
    
    // descend into the directory that contains the bins
    chdir( fitDir.c_str() );
    
    for( int i = 0; i < kNumBins; ++i ){
        cout<<"bin "<<i<<endl;
        
        ostringstream dir;
        dir << "bin_" << i;
        chdir( dir.str().c_str() );
        
        ostringstream resultsFile;
        resultsFile << "bin_" << i << ".fit";
        
        FitResults results( resultsFile.str() );
        /*
        if( !results.valid() ){
            
            chdir( ".." );
            continue;
        }
        */
        // print out the bin center
        outfile << lowMass + step * i + step / 2. << "\t";
        
        vector<string> etaPiS0;
        etaPiS0.push_back("EtaPi::Negative::S0-");
        pair< double, double > etaPiS0Int = results.intensity( etaPiS0 );
        outfile << etaPiS0Int.first << "\t" << etaPiS0Int.second << "\t";
        
        vector<string> etaPiP0;
        etaPiP0.push_back("EtaPi::Negative::P0-");
        pair< double, double > etaPiP0Int = results.intensity( etaPiP0 );
        outfile << etaPiP0Int.first << "\t" << etaPiP0Int.second << "\t";
        
        vector<string> etaPiP1m;
        etaPiP1m.push_back("EtaPi::Negative::P1-");
        pair< double, double > etaPiP1mInt = results.intensity( etaPiP1m );
        outfile << etaPiP1mInt.first << "\t" << etaPiP1mInt.second << "\t";

        vector<string> etaPiD0;
        etaPiD0.push_back("EtaPi::Negative::D0-");
        pair< double, double > etaPiD0Int = results.intensity( etaPiD0 );
        outfile << etaPiD0Int.first << "\t" << etaPiD0Int.second << "\t";
        
        vector<string> etaPiD1;
        etaPiD1.push_back("EtaPi::Negative::D1-");
        pair< double, double > etaPiD1Int = results.intensity( etaPiD1 );
        outfile << etaPiD1Int.first << "\t" << etaPiD1Int.second << "\t";
        
        vector<string> etaPiD1p;
        etaPiD1p.push_back("EtaPi::Positive::D1+");
        pair< double, double > etaPiD1pInt = results.intensity( etaPiD1p );
        outfile << etaPiD1pInt.first << "\t" << etaPiD1pInt.second << "\t";
    
        vector<string> etaPiP1p;
        etaPiP1p.push_back("EtaPi::Positive::P1+");
        pair< double, double > etaPiP1pInt = results.intensity( etaPiP1p );
        outfile << etaPiP1pInt.first << "\t" << etaPiP1pInt.second << "\t";
        
        vector< string > all;
        all.push_back( "EtaPi::Negative::S0-" );
        all.push_back( "EtaPi::Negative::P0-" );
        all.push_back( "EtaPi::Negative::P1-" );
        all.push_back( "EtaPi::Negative::D0-" );
        all.push_back( "EtaPi::Negative::D1-" );
        all.push_back( "EtaPi::Positive::D1+" );
        all.push_back( "EtaPi::Positive::P1+" );

        pair< double, double > allInt = results.intensity( all );
        outfile << allInt.first << "\t" << allInt.second << "\t";
        
        pair< double, double > phaseD0 =
        results.phaseDiff( "EtaPi::Negative::S0-",
                          "EtaPi::Negative::D0-");
        
        outfile << phaseD0.first << "\t" << phaseD0.second << "\t";
        
        pair< double, double > phaseP0 =
        results.phaseDiff( "EtaPi::Negative::S0-",
                          "EtaPi::Negative::P0-");
        
        outfile << phaseP0.first << "\t" << phaseP0.second << "\t";
        
        pair< double, double > phaseP1 =
        results.phaseDiff( "EtaPi::Negative::S0-",
                          "EtaPi::Negative::P1-");
        
        outfile << phaseP1.first << "\t" << phaseP1.second << "\t";
        
        pair< double, double > phaseD1 =
        results.phaseDiff( "EtaPi::Negative::S0-",
                          "EtaPi::Negative::D1-");
        outfile << phaseD1.first << "\t" << phaseD1.second << "\t";
        
        
        pair< double, double > phaseP0D1 =
        results.phaseDiff( "EtaPi::Negative::P0-",
                          "EtaPi::Negative::D1-");
        outfile << phaseP0D1.first << "\t" << phaseP0D1.second << "\t";
        
        pair< double, double > phaseP1D1 =
        results.phaseDiff( "EtaPi::Negative::P1-",
                          "EtaPi::Negative::D1-");
        outfile << phaseP1D1.first << "\t" << phaseP1D1.second << "\t";
        
        pair< double, double > phaseP1D1p =
        results.phaseDiff( "EtaPi::Positive::P1+",
                          "EtaPi::Positive::D1+");
        outfile << phaseP1D1p.first << "\t" << phaseP1D1p.second << "\t";
        
        
        /*
        pair< double, double > phaseDS =
        results.phaseDiff( "Pi+Pi-Pi+::xpol::J2_rhopi_D",
                          "Pi+Pi-Pi+::xpol::J2_f2pi_S" );
        
        outfile << phaseDS.first << "\t" << phaseDS.second << "\t";
        */
        outfile << endl;
        
        chdir( ".." );
    }
    
    return 0;
}
