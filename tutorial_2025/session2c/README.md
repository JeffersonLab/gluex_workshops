# Session Title / Description

## :dart: In this session, we will learn the following:
- Read and recognize the data stored in the output ROOT file of the ReactionFilter plugin
- Learn how to use a DSelector to analyze data or accepted Monte Carlo
- A quick overview of the analysis process

## :clipboard: Prerequisites
- Basic knowledge of C++ and ROOT 
- Make sure you have sourced your GlueX environment!
- Link to Tutorial Slides: `https://halldweb.jlab.org/doc-private/DocDB/ShowDocument?docid=6837`

## :file_folder: Materials Provided
- The DSelector examples to run on data (OmegaPi0) and thrown (thrown).
- Macro to run DSelector (runDSelector.cc) and shell script to redirect output (runDSelector.csh)
- [Links to any additional resources or documentation]

## :memo: Session Outline
0. **Introduction**: DSelectors are a tool designed for Hall D data analysis that allows access to all the information that is stored inside the output ROOT file of the ReactionFilter plugin. This session navigates some of the basics, such as plotting histograms, applying data selection, and outputting histogram and flat tree ROOT files.
1. **Step 1**: Create your DSelector using the command 'MakeDSelector'
      - MakeDSelector  tree_pi0pi0pippim__B4_051636.root  pi0pi0pippim__B4_Tree OmegaPi0
2. **Step 2**: Let's fill a histogram to see the FCAL shower quality factor
      - In DSelector_OmegaPi0.h declare the histogram `TH1D* dHist_ShowerQuality1;`
      - In DSelector_OmegaPi0.C initialize the histogram in the `Init()` function
        ```
        gDirectory->mkdir( "Showers" )->cd();
	      dHist_ShowerQuality1 = new TH1D( "ShowerQuality1", ";#gamma_{1} Shower Quality; Combos / .01", 100, 0, 1 )
        gDirectory->cd( ".." );
        ```
      - In the `Process()` function declare a set to track uniqueness	`set<Int_t> locUsedSoFar_Photon1;`
      - Also in the `Process()` function, but inside the combo loop, fill the histogram while checking for uniqueness
        ```
        if( locUsedSoFar_Photon1.find( locPhoton1NeutralID ) == locUsedSoFar_Photon1.end() ){
           // Checking if shower has FCAL timing as the shower quality is only defined for those photons
           if( dPhoton1Wrapper->Get_Detector_System_Timing() == SYS_FCAL )
              dHist_ShowerQuality1->Fill( dPhoton1Wrapper->Get_Shower_Quality() );
           locUsedSoFar_Photon1.insert( locPhoton1NeutralID );
        }
        ```
3. **Step 3**: Let's apply a chiSq selection
      - In `Process()` function, inside the combo loop get the chiSq/ndf from the combo wrapper
        `double reducedChiSq = dComboWrapper->Get_ChiSq_KinFit("") / dComboWrapper->Get_NDF_KinFit("");`
      - Assing an boolean to the selection `bool redChiSq = reducedChiSq > 2.5;`
      - Apply the selection by basically skipping the combo with a `continue` statement and mark the combo as cut
        ```
        if( redChiSq ){
           dComboWrapper->Set_IsComboCut(true);
           continue;
        }
        ```
4. **Step 4**: Let's fill an AmpTools-ready flat tree
      - At the beginning of the DSelector_OmegaPi.C file, in the `Init()` function, update the following lines of code:
        ```
	      dFlatTreeFileName = "OmegaPi0_tree.root"; //output flat tree (one combo per tree entry), "" for none
	      dFlatTreeName = "kin"; //if blank, default name will be chosen
        ```
        Make sure that the name is `kin` as this is the default name that AmpTools looks for in a tree
      - Also in the `Init()` function, call the following function to set up the AmpTools tree branches:
        `SetupAmpTools_FlatTree();`
      - In the `Process()` function, create a vector of TLorentzVectors which will be used to fill the branches:
        ```
         // set ordering of pions for amplitude analysis
		     vector<TLorentzVector> locFinalStateP4;
		     locFinalStateP4.push_back(locProtonP4);
		     locFinalStateP4.push_back(locPi0P4[abs(i - 1)]); // Bachelor Pi0
		     locFinalStateP4.push_back(locPi0P4[i]);
		     locFinalStateP4.push_back(locPiPlusP4);
		     locFinalStateP4.push_back(locPiMinusP4);
        ```
        Important! In the DSelector given here as an example, these lines are inside a loop that assigns a 3pi and an alternative 3pi
        hypothesis. This is why the bachelor pion is `locPi0P4[abs(i - 1)]`. This type of structure is different for simpler cases.
      - Once the vector is ready, we will fill the branches
        ```
        // Set weight from the 2D sidebands and accidentals
        dFlatTreeInterface->Fill_Fundamental<Float_t>("Weight", loc2Dweight * locHistAccidWeightFactor);
        // Fill the beam and the vector of TLorentz vectors from earlier
        FillAmpTools_FlatTree(locBeamP4, locFinalStateP4);
        // FILL FLAT TREE
        // This line is by default commented in the DSelector
        Fill_FlatTree(); //for the active combo
        ```
 5. **Step 4**: Run the DSelector
       - Make sure that the paths are properly set in the ROOT macro `runDSelector.cc`
       - Run the DSelector with the following script that redirects the output to two text files `source runDSelector.csh`
## :link: Additional Resources
- There is a copy of a DSelector to read a Thrown Tree as an example
