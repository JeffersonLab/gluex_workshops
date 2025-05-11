/* Copy ROOT Tree to new directory, with data cuts applied to TEM regions

T = mandelstam t variable
E = beam energy (typically 8.2 - 8.8)
M = invariant mass
*/

void copy_tree_with_cuts(TString file, TString target_dir,                         
                         TString t_low, TString t_high,
                         TString E_low, TString E_high,
                         TString m_low, TString m_high)
{
    gROOT->Reset();

    cout << "Copying Tree for: " << file << "\n";

    // open original file and get tree
    TFile f(file);
    TTree *T = (TTree *)f.Get("kin");

    // get basename by splitting the file path by the "/" delimeter and getting the
    // last element
    TString tok;
    TString file_name;
    Ssiz_t delimiter_position = 0;
    while (file.Tokenize(tok, delimiter_position, "/"))
    {
        file_name = tok;
    }

    // open new file at new path
    TString new_file = target_dir + "/" + file_name;
    TFile *f2 = new TFile(new_file, "new");

    // setup selection regions    
    TString t_cut = "t>" + t_low + " && t<" + t_high;
    TString E_cut = "E_Beam>" + E_low + " && E_Beam<" + E_high;
    TString m_cut = "M4Pi>" + m_low + " && M4Pi<" + m_high;

    TString selection = t_cut + " && " + E_cut + " && " + m_cut;

    TTree *T2 = T->CopyTree(selection.Data());

    T2->Write();
    f2->Close();
}
