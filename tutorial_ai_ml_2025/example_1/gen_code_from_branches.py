#!/usr/bin/env python3
import sys, re
from pathlib import Path

# -------- Always-create + custom-fill mapping --------
# key = branch name, value = RHS expression (full, including wrapper if needed)
HARDCODED = {
    "RunNumber": "Get_RunNumber()",
    "EventNumber": "Get_EventNumber()",
    "Combo": "loc_i",
    "kinfit_CL": 'dComboWrapper->Get_ConfidenceLevel_KinFit("")',
    "AccWeight": "AccWeight",
}

POS_PREFIXES = ("Positive_",)
NEG_PREFIXES = ("Negative_",)
PARTICLE_PREFIXES = (
    "Proton__", "KPlus__", "KMinus__", "PiPlus__", "PiMinus__",
    "Electron__", "Positron__", "MuPlus__", "MuMinus__"
)

def is_measured(n: str) -> bool:
    return "_meas" in n or "Measured" in n

def last_comp(n: str):
    m = re.search(r'_(E|X|Y|Z|T)$', n)
    return m.group(1) if m else None

def wrapper_for(n: str) -> str:
    if n.startswith(POS_PREFIXES): return "dPositiveWrapper"
    if n.startswith(NEG_PREFIXES): return "dNegativeWrapper"
    if n.startswith("Proton__"):   return "dRecoilWrapper"
    if n.startswith("Recoil"):     return "dRecoilWrapper"
    if n.startswith("BeamX4") or n.startswith("BeamP4") or n.startswith("ComboBeam__P4") or n.startswith("ComboBeam__X4"):
        return "dComboBeamWrapper"
    if n.startswith("Beam"):       return "dComboBeamWrapper"
    if n.startswith("RFTime_"):    return "dComboWrapper"
    return "dComboWrapper"

def strip_known_prefix(n: str) -> str:
    for pre in POS_PREFIXES + NEG_PREFIXES + ("Beam", "Recoil") + PARTICLE_PREFIXES:
        if n.startswith(pre):
            core = n[len(pre):]
            if pre in ("Beam","Recoil") and core.startswith("_"):
                core = core[1:]
            return core
    return n

def getter_kind(n: str):
    nn = strip_known_prefix(n)
    if "X4" in nn: return "X4"
    if "P4" in nn: return "P4"
    if nn.startswith("RFTime"): return "RFTime"
    return None

def make_fill_line(name: str) -> str:
    # Force-included custom mapping wins
    if name in HARDCODED:
        return f'dFlatTreeInterface->Fill_Fundamental<Float_t>("{name}", {HARDCODED[name]});'

    wrap = wrapper_for(name)
    kind = getter_kind(name)
    comp = last_comp(name)

    # 4-vectors
    if kind in ("X4","P4"):
        if comp is None:
            return f'// TODO: {name}: missing vector component suffix'
        g = f'Get_{kind}_Measured()' if is_measured(name) else f'Get_{kind}()'
        if (kind == "X4" and comp not in ("T","X","Y","Z")) or \
           (kind == "P4" and comp not in ("E","X","Y","Z")):
            return f'// TODO: {name}: unknown {kind} component {comp}'
        return f'dFlatTreeInterface->Fill_Fundamental<Float_t>("{name}", {wrap}->{g}.{comp}());'

    # RFTime scalars
    if name.startswith("RFTime_"):
        g = "Get_RFTime_Measured()" if is_measured(name) else "Get_RFTime()"
        return f'dFlatTreeInterface->Fill_Fundamental<Float_t>("{name}", dComboWrapper->{g});'

    # General scalars
    core = strip_known_prefix(name)
    return f'dFlatTreeInterface->Fill_Fundamental<Float_t>("{name}", {wrap}->Get_{core}());'

# ------------- vars.ini: simple rule -------------
def is_spectator(name: str) -> bool:
    # Training = names starting with Positive_ or Negative_; else spectators
    return not (name.startswith("Positive_") or name.startswith("Negative_"))
# -------------------------------------------------

def main():
    if len(sys.argv) < 2:
        print("usage: gen_code_from_branches.py branch_names.txt", file=sys.stderr)
        sys.exit(1)

    # read names from file (skip blanks), then FORCE include HARDCODED keys
    from_file = [ln.strip() for ln in Path(sys.argv[1]).read_text().splitlines() if ln.strip()]
    names = list(dict.fromkeys(from_file + list(HARDCODED.keys())))  # preserve input order; append forced

    creates = [f'dFlatTreeInterface->Create_Branch_Fundamental<Float_t>("{n}");' for n in names]
    fills   = [make_fill_line(n) for n in names]

    print("// ===== Branch creation =====")
    print("\n".join(creates))
    print("\n// ===== Branch filling =====")
    print("\n".join(fills))

    # Write vars.ini
    training, spectators = [], []
    for n in names:
        (spectators if is_spectator(n) else training).append(n)

    with open("vars.ini", "w") as f:
        f.write("[training]\n")
        for n in training:
            f.write(n + "\n")
        f.write("\n[spectators]\n")
        for n in spectators:
            f.write(n + "\n")

if __name__ == "__main__":
    main()
