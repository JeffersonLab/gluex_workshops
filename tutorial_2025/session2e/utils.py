"""Collection of helper functions for the analysis of AmpTools fit results
"""

import itertools
import re
from typing import Dict

import numpy as np
import pandas as pd

def sort_input_files(input_files: list, position: int = -1) -> list:
    """Sort the input files based off the last number in the file name or path

    Args:
        input_files (list): input files to be sorted
        position (int, optional): Index position of the number to be sorted on in the
            full path. Defaults to -1, meaning the last number is used for sorting. Be
            careful using this, as it will assume all path names have the same amount of
            distinct numbers, and thus the same indices.

    Returns:
        list: sorted list of files
    """

    def extract_last_number(full_path: str) -> float:
        numbers = re.findall(r"(?:\d*\.*\d+)", full_path)
        return float(numbers[position]) if numbers else float("inf")

    return sorted(input_files, key=extract_last_number)


def parse_amplitude(amp: str) -> Dict[str, str]:
    """parse 'eJPmL' style amplitude string into its individual quantum numbers

    Args:
        amp (str): string like eJPmL, JPmL, JPL, eJPL, eJP, JP

    Returns:
        dict: keys = quantum numbers (e, J, P, m, l). values = found values from string,
        or "" if not found
    """
    re_dict = {
        "e": r"(?<![0-9]{1}[pm]{1})(?<!\d)[pm]",
        "jp": r"[0-9]{1}[pm]{1}",  # j and p always appear together
        "m": r"([0-9]{1}[pm]{1})+[qp0mn]",  # assumes always form of 'JPm'
        "l": r"[SPDF]",
    }

    result_dict = {"e": "", "j": "", "p": "", "m": "", "l": ""}

    for quantum_number, expression in re_dict.items():
        search = re.search(expression, amp)
        if search:
            # the search actually returns "JPm", so grab the last char if found
            if quantum_number == "m":
                result_dict["m"] = search.group()[-1]
            elif quantum_number == "jp":
                result_dict["j"] = search.group()[0]
                result_dict["p"] = search.group()[1]
            else:
                result_dict[quantum_number] = search.group()

    return result_dict


def get_coherent_sums(df: pd.DataFrame) -> Dict[str, set]:
    """Returns a dict of coherent sums from a fit results dataframe

    Args:
        df (pd.DataFrame): dataframe of fit results loaded from csv
    Returns:
        dict: Is of the form {Coherent sum string: set(amplitudes)}
        e.g. {"eJPmL": ["p1p0S", "m1mpP", ...]}
    """
    # create an empty list for every type of coherent sum
    sum_types = ["eJPmL", "JPmL", "eJPL", "JPL", "eJP", "JP", "e"]
    coherent_sums = {d: set() for d in sum_types}

    # grab all eJPml columns
    for column in df.columns:
        # skip the phase difference columns and any status columns
        if "_" in column or len(column) > 5:
            continue

        res = parse_amplitude(column)
        # only add to key if all elements of key are in the column
        for key in coherent_sums.keys():
            split_key = list(key.lower())
            if any(res[char] == "" for char in split_key):
                continue
            coh_sum = "".join([res[char] for char in split_key])
            coherent_sums[key].add(coh_sum)

    return {k: sorted(v) for k, v in coherent_sums.items()}  # sort each set


def get_phase_differences(df: pd.DataFrame) -> Dict[tuple, str]:
    """Returns dict of all the phase difference columns in the dataframe

    The keys are specifically like (eJPmL_1, eJPmL_2), and there is no way to
    know how those will be ordered in the dataframe a priori. We avoid this by
    creating keys for either ordering and setting both of their values to the order
    found in the dataframe.

    Args:
        df (pd.DataFrame): dataframe of fit results loaded from csv
    Returns:
        dict: key = tuple of both possible combinations of every amplitude, val = the
            phase difference combination found in the dataframe
    """
    phase_differences = {}

    # get all possible combinations of eJPmL columns, and add their phase difference
    # column name if it exists (handles reverse ordering i.e. p1ppS_p1pmS & p1pmS_p1ppS)
    all_combos = list(itertools.combinations(get_coherent_sums(df)["eJPmL"], 2))
    columns = df.columns.values.tolist()
    for combo in all_combos:
        name = "_".join(combo)
        reverse_name = "_".join(reversed(combo))

        if name in columns:
            phase_differences[combo] = name
            phase_differences[tuple(reversed(combo))] = name
        if reverse_name in columns:
            phase_differences[combo] = reverse_name
            phase_differences[tuple(reversed(combo))] = reverse_name

    return phase_differences


def wrap_phases(df: pd.DataFrame = None, series: pd.Series = None) -> None:
    """Wrap phase differences to be from (-pi, pi] & convert from radians to degrees

    Two options of passing either a pandas dataframe, or series. The dataframe case
    handles avoiding editing any non phase difference columns. The series case is much
    simpler, and just applies the wrapping to each value

    Args:
        df (pd.DataFrame, optional): dataframe of fit results loaded from csv
            Defaults to pd.DataFrame([]).
        series (pd.Series, optional): pandas series of phase difference values.
            Defaults to pd.Series([], dtype=float).

    Returns:
        None: Edits the df or series itself
    """

    if df is None and series is None:
        raise ValueError(
            "Both parameters are None. Provide either a dataframe or a series."
        )

    if df is not None and series is not None:
        raise ValueError("Only dataframe or series should be passed, NOT both.")

    # wraps phase (in radians) to -pi < x <= pi and convert to degrees
    def wrap(phase):
        return np.rad2deg(np.angle(np.exp(1j * phase)))

    if series is not None:
        series.apply(wrap, inplace=True)
        return

    phase_diffs = get_phase_differences(df)
    for col in set(phase_diffs.values()):
        df[col] = df[col].apply(wrap)
        df[f"{col}_err"] = df[f"{col}_err"].apply(wrap)

    return


def convert_amp_name(input_string: str) -> str:
    """Converts amplitude string to J^P L_m^(e) LaTeX style string

    Function can handle both amplitudes and phase differences. If input_string is not
    of eJPmL format, or subset of it i.e. eJPL, then the output will be undefined.

    Args:
        input_string (str): string in eJPmL format
    Returns:
        str: Prettier LaTeX style string in J^P L_m^(e) format. If it's a phase
            difference it's then J^P L_m^(e) - J^P L_m^(e)
    """

    pm_dict = {"m": "-", "p": "+", "": ""}
    m_proj_dict = {"m": -1, "0": 0, "p": +1, "": ""}

    # CASE 1: phase difference, always of form 'eJPmL_eJPmL'
    if "_" in input_string:
        amps = input_string.split("_")
        if len(amps) != 2:
            raise ValueError("Phase difference must be in 'eJPmL_eJPmL' format!")
        e1, j1, p1, m1, l1 = (
            pm_dict[amps[0][0]],
            amps[0][1],
            pm_dict[amps[0][2]],
            m_proj_dict[amps[0][3]],
            amps[0][-1],
        )
        e2, j2, p2, m2, l2 = (
            pm_dict[amps[1][0]],
            amps[1][1],
            pm_dict[amps[1][2]],
            m_proj_dict[amps[1][3]],
            amps[1][-1],
        )

        return (
            rf"${j1}^{{{p1}}}{l1}_{{{m1}}}^{{({e1})}}"
            rf" - {j2}^{{{p2}}}{l2}_{{{m2}}}^{{({e2})}}$"
        )

    # CASE 2: typical amplitude coherent sum
    amp_dict = parse_amplitude(input_string)

    # set each quantum number to its found value. If not found, denote it with a sum
    e = r"\Sigma\varepsilon" if amp_dict["e"] == "" else pm_dict[amp_dict["e"]]
    j = r"\Sigma J" if amp_dict["j"] == "" else amp_dict["j"]
    p = "" if amp_dict["p"] == "" else pm_dict[amp_dict["p"]]
    m = r"\Sigma m" if amp_dict["m"] == "" else m_proj_dict[amp_dict["m"]]
    l = r"\Sigma \ell" if amp_dict["l"] == "" else amp_dict["l"]

    return rf"${j}^{{{p}}}{l}_{{{m}}}^{{({e})}}$"