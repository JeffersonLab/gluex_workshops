// Note:  for info about possible function calls refer to this header file
#include "IUAmpTools/FitResults.h"
#include "IUAmpTools/ConfigurationInfo.h"

#include <iostream>
#include <string>

using namespace std;

// a helper function to fetch amplitude names from a vector of other objects
vector< string > namesFromAmpInfo( const vector< AmplitudeInfo* >& ampInfoVec );


int main( int argc, char* argv[] ){

  if( argc != 2 ){

    cout << "Usage:  displayFitResults [fitResultsFile]" << endl;
  }


  FitResults results( argv[1] );

  // make this function call here just to trigger the warning about free parameters so
  // it doesn't make a mess of the table formatting
  results.intensity();
   
  // hard code some lists of amplitudes we care about -- we'll want sum these up
  // over all sums and all reactions

  vector<string> dWaves;
  dWaves.push_back( "a2_D0-" );
  dWaves.push_back( "a2_D0+" );
  dWaves.push_back( "a2_D1-" );
  dWaves.push_back( "a2_D1+" );
  dWaves.push_back( "a2_D-1-" );
  dWaves.push_back( "a2_D2+" );
  
  vector<string> allAmps = dWaves;
  allAmps.push_back( "S0+" );
  allAmps.push_back( "S0-" );
  
  // print some information at the reaction level
  
  vector< string > reactions = results.reactionList();
  cout << "====== REACTIONS ======" << endl;
  cout << " Number of reactions:  " << reactions.size() << endl;
  cout << "   (reaction name):  (observed signal events from fit)  [contribution to -2 ln L]" << endl;

  for( auto itr = reactions.begin(); itr != reactions.end(); ++itr ){

    pair< double, double > evts = results.intensity( results.ampList( *itr ), false );
    
    cout << "   " << *itr << ":  "  << evts.first << " +/- " << evts.second
	 << "  [" << results.likelihood( *itr ) << "]" << endl;
  }
  cout << "     TOTAL:  " << results.intensity(false).first << " +/- "
       << results.intensity(false).second << " [" << results.likelihood() << "]" << endl << endl;


  // for some operations it helpful to use the functionality of the configuration info object
  const ConfigurationInfo* cfgInfo = results.configInfo();

  // note -- it makes no sense to look at the intensity of an 
  
  cout << "====== AMPLITUDES ======" << endl;
  cout << "   (amplitude name):  (acceptance corrected yield) (fit fraction) [detection efficiency]" << endl;

  for( auto itr = allAmps.begin(); itr != allAmps.end(); ++itr ){

    // first get the list of all amplitudes across sums and reactions
    vector< string > thisAmps = namesFromAmpInfo( cfgInfo->amplitudeList( "", "", *itr ) );

    pair< double, double > evtsObserved = results.intensity( thisAmps, false );
    pair< double, double > evtsCorrected = results.intensity( thisAmps );

    cout << "   " << *itr << "\t:  " << evtsCorrected.first << " +/- " << evtsCorrected.second << "  ("
	 << evtsCorrected.first/results.intensity().first << ") ["
	 << evtsObserved.first/evtsCorrected.first << "]" << endl;
  }

  vector< string > fullDWaveNames;
  for( auto itr = dWaves.begin(); itr != dWaves.end(); ++itr ){

    // get the list of full names for each m projection of the D wave and then add
    // them to the full list of all D wave amplitudes
    vector< string > thisAmps = namesFromAmpInfo( cfgInfo->amplitudeList( "", "", *itr ) );
    fullDWaveNames.insert( fullDWaveNames.end(), thisAmps.begin(), thisAmps.end() );
  }
  cout << "------" << endl;
  
  // now print out:
   pair< double, double > a2Observed = results.intensity( fullDWaveNames, false );
   pair< double, double > a2Corrected = results.intensity( fullDWaveNames );

   cout << "     ALL a2(1320)\t:  " <<  a2Corrected.first << " +/- " << a2Corrected.second << "  ("
	 << a2Corrected.first/results.intensity().first << ") ["
	 << a2Observed.first/a2Corrected.first << "]" << endl;

  cout << endl;
  
  cout << "====== MINUIT DIAGNOSTICS ======" << endl;
  cout << "  Error Matrix Status ( 3 = full/accurate ):  " << results.eMatrixStatus() << endl;
  cout << "  Last Command Status ( 0 = normal ):  " << results.lastMinuitCommandStatus() << endl;
  cout << "  Estimated Distance to Minimum:  " << results.estDistToMinimum() << endl;
}


vector< string >
namesFromAmpInfo( const vector< AmplitudeInfo* >& ampInfoVec ){

  vector< string > result;

  for( auto itr = ampInfoVec.begin(); itr != ampInfoVec.end(); ++itr ){

    result.push_back( (*itr)->fullName() );
  }

  return result;
}
