
#include <iostream>
#include <fstream>
#include <complex>
#include <string>
#include <vector>
#include <utility>
#include <map>
#include <cassert>
#include <cstdlib>

#include "particleType.h"

#include "AMPTOOLS_DATAIO/ROOTDataWriter.h"
#include "AMPTOOLS_DATAIO/HDDMDataWriter.h"

#include "AMPTOOLS_AMPS/ThreePiAngles.h"
#include "AMPTOOLS_AMPS/TwoPiAngles.h"
#include "AMPTOOLS_AMPS/TwoPiAngles_amp.h"
#include "AMPTOOLS_AMPS/TwoPSHelicity.h"
#include "AMPTOOLS_AMPS/TwoPSAngles.h"
#include "AMPTOOLS_AMPS/BreitWigner.h"
#include "AMPTOOLS_AMPS/BreitWigner3body.h"
#include "AMPTOOLS_AMPS/ThreePiAnglesSchilling.h"
#include "AMPTOOLS_AMPS/Lambda1520Angles.h"
#include "AMPTOOLS_AMPS/omegapiAngAmp.h"

#include "AMPTOOLS_MCGEN/ProductionMechanism.h"
#include "AMPTOOLS_MCGEN/GammaPToNPartP.h"

#include "IUAmpTools/AmpToolsInterface.h"
#include "IUAmpTools/ConfigFileParser.h"
#include "IUAmpTools/PlotGenerator.h"
#include "IUAmpTools/FitResults.h"
//#include "/w/halld-scifs17exp/home/gleasonc/Software/AmpTools/AmpTools-0.9.4/AmpTools/IUAmpTools/AmpToolsInterface.h"
//#include "/w/halld-scifs17exp/home/gleasonc/Software/AmpTools/AmpTools-0.9.4/AmpTools/IUAmpTools/ConfigFileParser.h"
//#include "/w/halld-scifs17exp/home/gleasonc/Software/AmpTools/AmpTools-0.9.4/AmpTools/IUAmpTools/PlotGenerator.h"
//#include "/w/halld-scifs17exp/home/gleasonc/Software/AmpTools/AmpTools-0.9.4/AmpTools/IUAmpTools/FitResults.h"

#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TLorentzVector.h"
#include "TLorentzRotation.h"
#include "TRandom3.h"

using std::complex;
using namespace std;

int main( int argc, char* argv[] ){
  
	string  configfile("");
	string  outname("");
	string  hddmname("");
	
	bool diag = false;
	bool genFlat = false;
	
	// default upper and lower bounds 
	double lowMass = 0.2;
	double highMass = 2.0;

	double beamMaxE   = 12.0;
	double beamPeakE  = 9.0;
	double beamLowE   = 2.0;
	double beamHighE  = 12.0;

	int runNum = 30731;
	unsigned int seed = 0;

	double lowT = 0.0;
	double highT = 12.0;
	double slope = 6.0;

	int nEvents = 10000;
	int batchSize = 10000;
	
	//parse command line:
	for (int i = 1; i < argc; i++){
		
		string arg(argv[i]);
		
		if (arg == "-c"){  
			if ((i+1 == argc) || (argv[i+1][0] == '-')) arg = "-h";
			else  configfile = argv[++i]; }
		if (arg == "-o"){  
			if ((i+1 == argc) || (argv[i+1][0] == '-')) arg = "-h";
			else  outname = argv[++i]; }
		if (arg == "-hd"){
			if ((i+1 == argc) || (argv[i+1][0] == '-')) arg = "-h";
			else  hddmname = argv[++i]; }
		if (arg == "-l"){
			if ((i+1 == argc) || (argv[i+1][0] == '-')) arg = "-h";
			else  lowMass = atof( argv[++i] ); }
		if (arg == "-u"){
			if ((i+1 == argc) || (argv[i+1][0] == '-')) arg = "-h";
			else  highMass = atof( argv[++i] ); }
		if (arg == "-n"){  
			if ((i+1 == argc) || (argv[i+1][0] == '-')) arg = "-h";
			else  nEvents = atoi( argv[++i] ); }
		if (arg == "-m"){
			if ((i+1 == argc) || (argv[i+1][0] == '-')) arg = "-h";
			else  beamMaxE = atof( argv[++i] ); }
		if (arg == "-p"){
			if ((i+1 == argc) || (argv[i+1][0] == '-')) arg = "-h";
			else beamPeakE = atof( argv[++i] ); }
		if (arg == "-a"){
			if ((i+1 == argc) || (argv[i+1][0] == '-')) arg = "-h";
                        else  beamLowE = atof( argv[++i] ); }
                if (arg == "-b"){
                        if ((i+1 == argc) || (argv[i+1][0] == '-')) arg = "-h";
                        else  beamHighE = atof( argv[++i] ); }
		if (arg == "-r"){
                        if ((i+1 == argc) || (argv[i+1][0] == '-')) arg = "-h";
                        else  runNum = atoi( argv[++i] ); }
		if (arg == "-s"){
                        if ((i+1 == argc) || (argv[i+1][0] == '-')) arg = "-h";
                        else  seed = atoi( argv[++i] ); }
		if (arg == "-t"){
                        if ((i+1 == argc) || (argv[i+1][0] == '-')) arg = "-h";
                        else  slope = atof( argv[++i] ); }
		if (arg == "-tmin"){
                        if ((i+1 == argc) || (argv[i+1][0] == '-')) arg = "-h";
                        else  lowT = atof( argv[++i] ); }
		if (arg == "-tmax"){
                        if ((i+1 == argc) || (argv[i+1][0] == '-')) arg = "-h";
                        else  highT = atof( argv[++i] ); }
		if (arg == "-d"){
			diag = true; }
		if (arg == "-f"){
			genFlat = true; }
		if (arg == "-h"){
			cout << endl << " Usage for: " << argv[0] << endl << endl;
			cout << "\t -c    <file>\t Config file" << endl;
			cout << "\t -o    <name>\t ROOT file output name" << endl;
			cout << "\t -hd   <name>\t HDDM file output name [optional]" << endl;
			cout << "\t -l    <value>\t Low edge of mass range (GeV) [optional]" << endl;
			cout << "\t -u    <value>\t Upper edge of mass range (GeV) [optional]" << endl;
			cout << "\t -n    <value>\t Minimum number of events to generate [optional]" << endl;
			cout << "\t -m  <value>\t Electron beam energy (or photon energy endpoint) [optional]" << endl;
                        cout << "\t -p  <value>\t Coherent peak photon energy [optional]" << endl;
                        cout << "\t -a  <value>\t Minimum photon energy to simulate events [optional]" << endl;
                        cout << "\t -b  <value>\t Maximum photon energy to simulate events [optional]" << endl;
			cout << "\t -r    <value>\t Run number assigned to generated events [optional]" << endl;
			cout << "\t -s    <value>\t Random number seed initialization [optional]" << endl;
			cout << "\t -t    <value>\t Momentum transfer slope [optional]" << endl;
			cout << "\t -tmin <value>\t Minimum momentum transfer [optional]" << endl;
			cout << "\t -tmax <value>\t Maximum momentum transfer [optional]" << endl;
			cout << "\t -f \t\t Generate flat in M(X) (no physics) [optional]" << endl;
			cout << "\t -d \t\t Plot only diagnostic histograms [optional]" << endl << endl;
			exit(1);
		}
	}
	
	if( configfile.size() == 0 || outname.size() == 0 ){
		cout << "No config file or output specificed:  run gen_amp -h for help" << endl;
		exit(1);
	}
	
	// open config file and be sure only one reaction is specified
	ConfigFileParser parser( configfile );
	ConfigurationInfo* cfgInfo = parser.getConfigurationInfo();
	assert( cfgInfo->reactionList().size() == 1 );
	ReactionInfo* reaction = cfgInfo->reactionList()[0];
	
	// use particletype.h to convert reaction particle names
	vector<Particle_t> Particles;
	vector<double> childMasses;
	double threshold = 0;
	for (unsigned int i = 0; i < reaction->particleList().size(); i++){
	  Particle_t locEnum = ParticleEnum(reaction->particleList()[i].c_str());
	  // Beam particle is always photon
	  if (locEnum == 0 && i > 0)
	    cout << "ConfigFileParser WARNING:  unknown particle type \"" << reaction->particleList()[i] << "\"" << endl;
	  Particles.push_back(ParticleEnum(reaction->particleList()[i].c_str()));
	  if (i>1){
	    childMasses.push_back(ParticleMass(Particles[i]));
	    threshold += ParticleMass(Particles[i]);
	  }
	}

	// loop to look for resonance in config file
	// currently only one at a time is supported 
	const vector<ConfigFileLine> configFileLines = parser.getConfigFileLines();
	double resonance[]={1.0, 1.0};
	bool foundResonance = false;
	bool isKaonRecoil = false;
	bool isPionRecoil = false;
	for (vector<ConfigFileLine>::const_iterator it=configFileLines.begin(); it!=configFileLines.end(); it++) {
	  if ((*it).keyword() == "define") {
	    if ((*it).arguments()[0] == "rho" || (*it).arguments()[0] == "omega" || (*it).arguments()[0] == "phi" || (*it).arguments()[0] == "b1" || (*it).arguments()[0] == "a1" || (*it).arguments()[0] == "Lambda1520"){
	      if ( (*it).arguments().size() != 3 )
		continue;
	      resonance[0]=atof((*it).arguments()[1].c_str());
	      resonance[1]=atof((*it).arguments()[2].c_str());
	      cout << "Distribution seeded with resonance " << (*it).arguments()[0] << " : mass = " << resonance[0] << "GeV , width = " << resonance[1] << "GeV" << endl; 
	      if((*it).arguments()[0] == "Lambda1520")
		 isKaonRecoil = true;
	      foundResonance = true;
	      break;
	    }
	  }
	}
	if (!foundResonance)
	  cout << "ConfigFileParser WARNING:  no known resonance found, seed with mass = width = 1GeV" << endl; 

	// random number initialization (set to 0 by default)
	TRandom3* gRandom = new TRandom3();
	gRandom->SetSeed(seed);
	seed = gRandom->GetSeed();
	cout << "TRandom3 Seed : " << seed << endl;

	// setup AmpToolsInterface
	AmpToolsInterface::registerAmplitude( ThreePiAngles() );
	AmpToolsInterface::registerAmplitude( TwoPiAngles() );
	AmpToolsInterface::registerAmplitude( TwoPiAngles_amp() );
	AmpToolsInterface::registerAmplitude( TwoPSHelicity() );
	AmpToolsInterface::registerAmplitude( TwoPSAngles() );
	AmpToolsInterface::registerAmplitude( BreitWigner() );
	AmpToolsInterface::registerAmplitude( BreitWigner3body() );
	AmpToolsInterface::registerAmplitude( ThreePiAnglesSchilling() );
	AmpToolsInterface::registerAmplitude( Lambda1520Angles() );
	AmpToolsInterface::registerAmplitude( omegapiAngAmp() );
	AmpToolsInterface ati( cfgInfo, AmpToolsInterface::kMCGeneration );

	// loop to look for beam configuration file
        TString beamConfigFile;
        const vector<ConfigFileLine> configFileLinesBeam = parser.getConfigFileLines();
        for (vector<ConfigFileLine>::const_iterator it=configFileLinesBeam.begin(); it!=configFileLinesBeam.end(); it++) {
                if ((*it).keyword() == "define") {
                        TString beamArgument =  (*it).arguments()[0].c_str();
                        if(beamArgument.Contains("beamconfig")) {
                                beamConfigFile = (*it).arguments()[1].c_str();
                        }
                }
        }
	if(beamConfigFile.Length() == 0) {
		cout<<"WARNING: Couldn't find beam configuration file -- write local version"<<endl;

		beamConfigFile = "local_beam.conf";
		ofstream locBeamConfigFile;
		locBeamConfigFile.open(beamConfigFile.Data());
		locBeamConfigFile<<"ElectronBeamEnergy "<<beamMaxE<<endl;       // electron beam energy
		locBeamConfigFile<<"CoherentPeakEnergy "<<beamPeakE<<endl;      // coherent peak energy
		locBeamConfigFile<<"PhotonBeamLowEnergy "<<beamLowE<<endl;      // photon beam low energy
		locBeamConfigFile<<"PhotonBeamHighEnergy "<<beamHighE<<endl;    // photon beam high energy
		locBeamConfigFile.close();
	}

	ProductionMechanism::Type type =
		( genFlat ? ProductionMechanism::kFlat : ProductionMechanism::kResonant );

	// generate over a range of mass
	// start with threshold or lowMass, whichever is higher
	GammaPToNPartP resProd;
	if(isKaonRecoil)
		resProd = GammaPToNPartP( threshold<lowMass ? lowMass : threshold, highMass, childMasses, ProductionMechanism::kKaon, type, slope, lowT, highT, seed, beamConfigFile );
	else if(isPionRecoil)
		resProd = GammaPToNPartP( threshold<lowMass ? lowMass : threshold, highMass, childMasses, ProductionMechanism::kPion, type, slope, lowT, highT, seed, beamConfigFile );
	else
		resProd = GammaPToNPartP( threshold<lowMass ? lowMass : threshold, highMass, childMasses, ProductionMechanism::kProton, type, slope, lowT, highT, seed, beamConfigFile );
	
	if (childMasses.size() < 2){
	  cout << "ConfigFileParser ERROR:  single particle production is not yet implemented" << endl; 
	  return 1;
	}
		
	// seed the distribution with a sum of noninterfering Breit-Wigners
	// we can easily compute the PDF for this and divide by that when
	// doing accept/reject -- improves efficiency if seeds are picked well
cout<<"here 1"<<endl;
	
	if( !genFlat ){
		
		// the lines below should be tailored by the user for the particular desired
		// set of amplitudes -- doing so will improve efficiency.  Leaving as is
		// won't make MC incorrect, it just won't be as fast as it could be
		
		resProd.addResonance( resonance[0], resonance[1],  1.0 );
	}
	
	vector< int > pTypes;
	for (unsigned int i=0; i<Particles.size(); i++)
	  pTypes.push_back( Particles[i] );
	
	HDDMDataWriter* hddmOut = NULL;
	if( hddmname.size() != 0 ) hddmOut = new HDDMDataWriter( hddmname, runNum, seed);
	ROOTDataWriter rootOut( outname );
	
	TFile* diagOut = new TFile( "gen_amp_diagnostic.root", "recreate" );
	ostringstream locStream;
	ostringstream locIsobarStream;
	for (unsigned int i=2; i<Particles.size(); i++){
	  locStream << ParticleName_ROOT(Particles[i]);
	  if ( i> 2 )
	    locIsobarStream << ParticleName_ROOT(Particles[i]);
	}
	string locHistTitle = string("Resonance Mass ;") + locStream.str() + string(" Invariant Mass (GeV/c^{2});");
	string locIsobarTitle = string("Isobar Mass ;") + locIsobarStream.str() + string(" Invariant Mass (GeV/c^{2});");

	TH1F* mass = new TH1F( "M", locHistTitle.c_str(), 180, lowMass, highMass );
	TH1F* massW = new TH1F( "M_W", ("Weighted "+locHistTitle).c_str(), 180, lowMass, highMass );
	massW->Sumw2();
	TH1F* intenW = new TH1F( "intenW", "True PDF / Gen. PDF", 1000, 0, 100 );
	TH2F* intenWVsM = new TH2F( "intenWVsM", "Ratio vs. M", 100, lowMass, highMass, 1000, 0, 10 );
	
	TH1F* t = new TH1F( "t", "-t Distribution", 200, 0, 2 );

	TH1F* M_isobar = new TH1F( "M_isobar", locIsobarTitle.c_str(), 200, 0, 2 );

	TH2F* CosTheta_psi = new TH2F( "CosTheta_psi", "cos#theta vs. #psi", 180, -3.14, 3.14, 100, -1, 1);
	TH2F* M_CosTheta = new TH2F( "M_CosTheta", "M vs. cos#vartheta", 180, lowMass, highMass, 200, -1, 1);
	TH2F* M_Phi = new TH2F( "M_Phi", "M vs. #varphi", 180, lowMass, highMass, 200, -3.14, 3.14);
	
	int eventCounter = 0;
	while( eventCounter < nEvents ){
		
		if( batchSize < 1E4 ){
			
			cout << "WARNING:  small batches could have batch-to-batch variations\n"
			     << "          due to different maximum intensities!" << endl;
		}
		
		cout << "Generating four-vectors..." << endl;
		
		ati.clearEvents();
		for( int i = 0; i < batchSize; ++i ){
			
			Kinematics* kin = resProd.generate();
			ati.loadEvent( kin, i, batchSize );
			delete kin;
		}
		
		cout << "Processing events..." << endl;
		
		// include factor of 1.5 to be safe in case we miss peak -- avoid
		// intensity calculation of we are generating flat data
		double maxInten = ( genFlat ? 1 : 1.5 * ati.processEvents( reaction->reactionName() ) );
		
		
		for( int i = 0; i < batchSize; ++i ){
			
			Kinematics* evt = ati.kinematics( i );
			TLorentzVector resonance;
            TLorentzVector isobar;

            TLorentzVector ProtonP4=evt->particle(1);
            TLorentzVector EtaP4=evt->particle(2);
            TLorentzVector PiMinusP4=evt->particle(3);
            TLorentzVector PiPlusP4=evt->particle(4);

            resonance=EtaP4+PiMinusP4;
            isobar=ProtonP4+PiPlusP4;
            
            /*
			for (unsigned int i=2; i<Particles.size(); i++)
			  resonance += evt->particle( i );

			TLorentzVector isobar;
			for (unsigned int i=1; i<Particles.size(); i++){
				if(Particles.size()>=5){
                 		   if (i==1 || i==4){
                        		isobar += evt->particle( i );
                    		   }
                		}
                		else{
                    			if (i==1){
                        			isobar += evt->particle( i );
                    			}
                		}
			}
          */
			
			double genWeight = evt->weight();
			
			// cannot ask for the intensity if we haven't called process events above
			double weightedInten = ( genFlat ? 1 : ati.intensity( i ) ); 
			// cout << " i=" << i << "  intensity_i=" << weightedInten << endl;

			if( !diag ){
				
				// obtain this by looking at the maximum value of intensity * genWeight
				double rand = gRandom->Uniform() * maxInten;
				
				if( weightedInten > rand || genFlat ){

					mass->Fill( resonance.M() );
					massW->Fill( resonance.M(), genWeight );
					
					intenW->Fill( weightedInten );
					intenWVsM->Fill( resonance.M(), weightedInten );

					M_isobar->Fill( isobar.M() );
					
					// calculate angular variables
					TLorentzVector beam = evt->particle ( 0 );
					TLorentzVector recoil = isobar;
					TLorentzVector p1 = evt->particle ( 2 );
					TLorentzVector target(0,0,0,ProtonP4.M());
					
					if(isKaonRecoil || isPionRecoil)
						t->Fill(-1*(beam-evt->particle(1)).M2());
					else
						t->Fill(-1*(evt->particle(1)-target).M2());


                    
                     ///CoM Boost
                     TLorentzVector locCoMP4=beam + target;
                     TVector3 boostCoM=-(locCoMP4.Vect())*(1.0/locCoMP4.E());
                     //Boost in CoM
                     TLorentzVector locBeamP4_CM=beam;
                     TLorentzVector locEtaPiMinusP4_CM=resonance;
                     TLorentzVector locDeltaPPP4_CM=isobar;
                    TLorentzVector locEtaP4_CM=EtaP4;
                    
                     locDeltaPPP4_CM.Boost(boostCoM);
                     locEtaPiMinusP4_CM.Boost(boostCoM);
                     locBeamP4_CM.Boost(boostCoM);
                     locEtaP4_CM.Boost(boostCoM);
                     //GJ Boost
                     TVector3 boostGJ=-(locEtaPiMinusP4_CM.Vect())*(1.0/locEtaPiMinusP4_CM.E());
                     
                     //Define GJ frame vectors
                     TLorentzVector locEtaPiMinus_GJ=locEtaPiMinusP4_CM;
                     TLorentzVector locEtaP4_GJ=locEtaP4_CM;
                     TLorentzVector locBeamP4GJ=locBeamP4_CM;
                     
                     //Boost in GJ
                     locEtaPiMinus_GJ.Boost(boostGJ);
                     locBeamP4GJ.Boost(boostGJ);
                     locEtaP4_GJ.Boost(boostGJ);
                    
                     TVector3 z_GJ;
                     z_GJ.SetXYZ(locBeamP4GJ.X(),locBeamP4GJ.Y(),locBeamP4GJ.Z());//z GJ
                     TVector3 z_hat_GJ=z_GJ.Unit();
                     TVector3 y_GJ=locBeamP4_CM.Vect().Cross(locEtaPiMinusP4_CM.Vect());
                     TVector3 y_hat_GJ=y_GJ.Unit();
                     TVector3 x_hat_GJ=y_hat_GJ.Cross(z_hat_GJ);//x hat GJ
                     
                     TVector3 v(locEtaP4_GJ.Vect()*x_hat_GJ, locEtaP4_GJ.Vect()*y_hat_GJ,
                     locEtaP4_GJ.Vect()*z_hat_GJ);
                     double cosTheta = v.CosTheta();
                     //double theta = v.Theta();
                    double phi = v.Phi();//*180./TMath::Pi();
                     
                    
					M_CosTheta->Fill( resonance.M(), cosTheta);
					M_Phi->Fill( resonance.M(), phi);
                    
					/*
                    TLorentzRotation resonanceBoost( -resonance.BoostVector() );
                    
                     TLorentzVector beam_res = resonanceBoost * beam;
                     TLorentzVector recoil_res = resonanceBoost * recoil;
                     TLorentzVector p1_res = resonanceBoost * p1;
                     
                     // normal to the production plane
                     TVector3 y = (beam.Vect().Unit().Cross(-recoil.Vect().Unit())).Unit();
                     
                     // choose helicity frame: z-axis opposite recoil proton in rho rest frame
                     TVector3 z = -1. * recoil_res.Vect().Unit();
                     TVector3 x = y.Cross(z).Unit();
                     TVector3 angles( (p1_res.Vect()).Dot(x),
                     (p1_res.Vect()).Dot(y),
                     (p1_res.Vect()).Dot(z) );
                     
                     //double cosTheta = angles.CosTheta();
                     double phi = angles.Phi();
                     
                    M_CosTheta->Fill( resonance.M(), cosTheta);
                    M_Phi->Fill( resonance.M(), phi);
                    
					TVector3 eps(1.0, 0.0, 0.0); // beam polarization vector
                                        double Phi = atan2(y.Dot(eps), beam.Vect().Unit().Dot(eps.Cross(y)));

                                        GDouble psi = phi - Phi;
                                        if(psi < -1*PI) psi += 2*PI;
                                        if(psi > PI) psi -= 2*PI;
					
					CosTheta_psi->Fill( psi, cosTheta);
					*/
					// we want to save events with weight 1
					evt->setWeight( 1.0 );
					
					if( hddmOut ) hddmOut->writeEvent( *evt, pTypes );
					rootOut.writeEvent( *evt );
					++eventCounter;
					if(eventCounter >= nEvents) break;
				}
			}
			else{
				
				mass->Fill( resonance.M() );
				massW->Fill( resonance.M(), genWeight );
				
				intenW->Fill( weightedInten );
				intenWVsM->Fill( resonance.M(), weightedInten );
				TLorentzVector recoil = evt->particle ( 1 );
				
				++eventCounter;
			}
			
			delete evt;
		}
		
		cout << eventCounter << " events were processed." << endl;
	}
	
	mass->Write();
	massW->Write();
	intenW->Write();
	intenWVsM->Write();
	M_isobar->Write();
	t->Write();
	CosTheta_psi->Write();
	M_CosTheta->Write();
	M_Phi->Write();

	diagOut->Close();
	
	if( hddmOut ) delete hddmOut;
	
	return 0;
}

