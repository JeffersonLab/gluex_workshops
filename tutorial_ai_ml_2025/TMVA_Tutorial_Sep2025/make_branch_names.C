// make_branch_names.C
#include <TFile.h>
#include <TTree.h>
#include <cstdio>
#include <TBranch.h>
#include <TObjArray.h>
#include <TObject.h>
#include <TIterator.h>

#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <algorithm>

// Choose what branches to include in your flat tree
// We're making a kaon/pion classifier so we don't really care about NeutralHypo's
static bool INCLUDE_ChargedHypo = true; // Basically only interested in this for our TMVA analysis
static bool INCLUDE_NeutralHypo = false;
static bool INCLUDE_Thrown     = false;
static bool INCLUDE_KinFit     = true;  // add the "KinFit" P4 and X4 to analysis

// You can add anything else you don't care about here.
static const std::unordered_set<std::string> EXCLUDE_EXACT = {
    "ChargedHypo__ExtrapolatedX_DIRC",
    "ChargedHypo__ExtrapolatedY_DIRC",
    "ChargedHypo__Lele_DIRC",
    "ChargedHypo__Lk_DIRC",
    "ChargedHypo__Lp_DIRC",
    "ChargedHypo__Lpi_DIRC",
    "ChargedHypo__NumPhotons_DIRC",
    "ChargedHypo__ThetaC_DIRC",
    "ChargedHypo__Energy_CCAL",
    "ChargedHypo__PIDFOM",
    "NumUnusedShowers_Quality",
    "SumP3_UnusedTracks",
    "SumPMag_UnusedTracks",
    "X4_Production",
    "NumBeam",
    "NumChargedHypos",
    "NumNeutralHypos",
    "NumPIDThrown_FinalState",
    "Beam__IsGenerator",
    "ComboBeam__BeamIndex",
    "IsComboCut",
    "IsThrownTopology",
    "IsTrueCombo",
    "L1TriggerBits",
    "IsBDTSignalCombo",
    "MCWeight",
    "NumPIDThrown_FinalState",
    "Energy_UnusedShowers_Quality",
    "ChargedHypo__Beta_Timing_KinFit",
    "KPlus__ChargedIndex",
    "KPlus__Beta_Timing_Measured",
    "KPlus__ChiSq_Timing_Measured",
    "KPlus__Beta_Timing_KinFit",
    "KPlus__ChiSq_Timing_KinFit",
    "KMinus__ChargedIndex",
    "KMinus__Beta_Timing_Measured",
    "KMinus__ChiSq_Timing_Measured",
    "KMinus__Beta_Timing_KinFit",
    "KMinus__ChiSq_Timing_KinFit",
    "Proton__ChargedIndex",
    "Proton__Beta_Timing_Measured",
    "Proton__ChiSq_Timing_Measured",
    "Proton__Beta_Timing_KinFit",
    "Proton__ChiSq_Timing_KinFit",
};



// Make a text file with the branch names you will use in your TMVA analysis
// root -l -b -q 'make_branch_names.C+("tree_kpkm__B4_gen_amp_051284_000.root","kpkm__B4_Tree","branch_names.txt")'


























static bool contains(const std::string& s, const char* sub){
    return s.find(sub) != std::string::npos;
}

static bool keep_by_section(const std::string& n){
    if (!INCLUDE_ChargedHypo && contains(n, "ChargedHypo__")) return false;
    if (!INCLUDE_NeutralHypo && contains(n, "NeutralHypo__")) return false;
    if (!INCLUDE_Thrown     && (contains(n, "Thrown__") || contains(n, "ThrownBeam__"))) return false;
    if (!INCLUDE_KinFit     && contains(n, "KinFit")) return false;
    return true;
}

// Suffix-based exclusion: names that end exactly with "__PID"
static inline bool exclude_pid_suffix(const std::string& n) {
    const std::string suf = "_PID";
    return n.size() >= suf.size() && n.compare(n.size()-suf.size(), suf.size(), suf) == 0;
    // Note: "PIDFOM" WILL NOT match (no "__PID" at end)
}


// central exclusion predicate
static bool should_exclude(const std::string& n){
    // exact-name drop
    if (EXCLUDE_EXACT.count(n)) return true;

    // ends-with "__PID"
    if (exclude_pid_suffix(n)) return true;

    // dependent drops when Thrown is off
    if (!INCLUDE_Thrown) {
        if (n == "NumThrown") return true;
        if (n == "PIDThrown_Decaying") return true;
        // after ChargedHypo duplication you may see these:
        if (n == "Positive_ThrownIndex" || n == "Negative_ThrownIndex") return true;
    }

    return false;
}


static void expand_X4_P4(const std::string& base, std::vector<std::string>& out){
  // TMVA can only take in floats, so we need a feature for each component of a TLorentzVector
  // If base contains "X4" or "P4", append compo
  // Components go at the very end: base_<comp>
  if (contains(base, "X4")){
    static const char* comps[] = {"T","X","Y","Z"};
    for (auto c : comps) out.push_back(base + "_" + c);
  } else if (contains(base, "P4")){
    static const char* comps[] = {"E","X","Y","Z"};
    for (auto c : comps) out.push_back(base + "_" + c);
  } else {
    out.push_back(base);
  }
}

// Replace a single prefix if present (helper)
static bool replace_prefix(std::string& s, const std::string& from, const std::string& to){
    if (s.rfind(from, 0) == 0){ // starts with
        s.replace(0, from.size(), to);
        return true;
    }
    return false;
}

static void process_name(const std::string& raw, std::vector<std::string>& out){
    // 1) section filter
    if (!keep_by_section(raw)) return;
    if (should_exclude(raw))   return;
    
    // 2) ChargedHypo__ → Positive_/Negative_
    if (contains(raw, "ChargedHypo__")){
        std::string stem = raw;
        // drop the "ChargedHypo__"
        stem.erase(0, std::string("ChargedHypo__").size());
        expand_X4_P4("Positive_" + stem, out);
        expand_X4_P4("Negative_" + stem, out);
        return;
    }

    // 3) Particle prefixes: generate both Positive_/Negative_ variants
    //    (KPlus, KMinus, PiPlus, PiMinus, Electron, Positron -> both signs)
    const char* pos_particlePrefixes[] = {
      "KPlus__", "PiPlus__", "Positron__", "MuPlus__"
    };
    for (auto pfx : pos_particlePrefixes){
        if (raw.rfind(pfx, 0) == 0){
            std::string stem = raw.substr(std::string(pfx).size());
            expand_X4_P4("Positive_" + stem, out);

            return;
        }
    }
    
    const char* neg_particlePrefixes[] = {
      "KMinus__", "PiMinus__", "Electron__", "MuMinus__"
    };
    for (auto pfx : neg_particlePrefixes){
      if (raw.rfind(pfx, 0) == 0){
            std::string stem = raw.substr(std::string(pfx).size());
	    expand_X4_P4("Negative_" + stem, out);
            return;
	}
    }

    // 4) Default: keep original name and expand vectors if present
    expand_X4_P4(raw, out);
}


  
void make_branch_names(TTree* t, const char* outfile = "branch_names.txt"){
    if (!t){ printf("No TTree provided.\n"); return; }

    std::vector<std::string> tmp;
    tmp.reserve(2048);

    // Walk branches
    TObjArray* brs = t->GetListOfBranches();
    TIterator* it = brs->MakeIterator();
    while (TObject* o = it->Next()){
        const char* nm = o->GetName();
        if (!nm) continue;
        process_name(nm, tmp);
    }
    delete it;

    // De-dup and sort
    std::unordered_set<std::string> seen;
    std::vector<std::string> final;
    final.reserve(tmp.size());
    for (auto& s : tmp){
        if (seen.insert(s).second) final.push_back(s);
    }
    std::sort(final.begin(), final.end());

    // Write file
    std::ofstream out(outfile);
    for (auto& s : final) out << s << "\n";
    out.close();

    printf("Wrote %zu names to %s\n", final.size(), outfile);
}

void make_branch_names(const char* rootfile,
                       const char* treename = "kpkm__B4_Tree",
                       const char* outfile  = "branch_names.txt")
{
    TFile f(rootfile, "READ");
    if (f.IsZombie()) { printf("Cannot open %s\n", rootfile); return; }
    TTree* t = (TTree*)f.Get(treename);
    if (!t) { printf("Tree %s not found in %s\n", treename, rootfile); return; }
    make_branch_names(t, outfile);   // calls your existing implementation
}
