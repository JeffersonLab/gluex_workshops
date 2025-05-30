"""This script converts AmpTools .fit file(s) or its associated ROOT files into a csv.

This script is used for two fit result purposes:
1. To aggregate the AmpTools .fit files into a single .csv file for easier analysis.
2. To convert the ROOT files that the .fit files are based off of into a .csv file.
Behind the scenes, this script calls a ROOT macro for either situation.
"""

import argparse
import os
import subprocess
import sys
import tempfile

sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), "../")))

import utils

# get the script directory to properly call the script with the right path
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))

def main(args: dict) -> None:

    # ERROR / VALUE HANDLING
    if not os.environ["ROOTSYS"]:
        raise EnvironmentError(
            "ROOTSYS path is not loaded. Make sure to run 'source setup_gluex.csh'\n"
        )

    if args["output"] and not args["output"].endswith(".csv"):
        args["output"] = args["output"] + ".csv"

    # Check if args["input"] is a file containing a list of result files, and save
    # the list of files to input_files
    input_files = []
    if (
        len(args["input"]) == 1
        and os.path.isfile(args["input"][0])
        and not args["input"][0].endswith(".fit")
        and not args["input"][0].endswith(".root")
    ):
        with open(args["input"][0], "r") as file:
            input_files = [line.strip() for line in file if line.strip()]
    else:
        input_files = args["input"]

    # Check if all input files exist, and expand its full path if just a file name
    print("Checking if all input files exist...")
    for file in input_files:
        if not os.path.exists(file):
            raise FileNotFoundError(f"The file {file} does not exist")
        if not os.path.isabs(file):
            input_files[input_files.index(file)] = os.path.abspath(file)

    if all(file.endswith(".fit") for file in input_files):
        file_type = "fit"
    elif all(file.endswith(".root") for file in input_files):
        file_type = "root"
    else:
        raise ValueError(
            "All input files must be of the same type: either .fit or .root files"
        )

    # sort the input files
    input_files = (
        utils.sort_input_files(input_files, args["sort_index"])
        if args["sorted"]
        else input_files
    )

    if args["preview"]:
        print("Files that will be processed:")
        for file in input_files:
            print(f"\t{file}")
        return    

    # create a tempfile that contains the list of input files
    # this seems to improve the speed of subprocess.Popen
    with tempfile.NamedTemporaryFile(delete=False, mode="w") as temp_file:
        temp_file.write("\n".join(input_files))
        temp_file_path = temp_file.name
    print(f"Temp file created at {temp_file_path}")

    # convert this flag into bool integers for the ROOT macro to interpret
    is_acceptance_corrected = 1 if args["acceptance_corrected"] else 0

    # setup ROOT command with appropriate arguments
    package = ""
    if file_type == "fit":
        output_file_name = "fits.csv" if not args["output"] else args["output"]
        command = [
            (
                f'{SCRIPT_DIR}/extract_fit_results.cc("{temp_file_path}",'
                f' "{output_file_name}", {is_acceptance_corrected})'
            )
        ]
        package = "loadAmpTools.C"

        if args["correlation"]:
            corr_output_name = output_file_name.replace(".csv", "_corr.csv")
            command.append(
                f'{SCRIPT_DIR}/extract_corr_matrix.cc("{temp_file_path}",'
                f' "{corr_output_name}")'
            )
        if args["covariance"]:
            cov_output_name = output_file_name.replace(".csv", "_cov.csv")
            command.append(
                f'{SCRIPT_DIR}/extract_cov_matrix.cc("{temp_file_path}",'
                f' "{cov_output_name}")'
            )
    elif file_type == "root":
        output_file_name = "data.csv" if not args["output"] else args["output"]
        if args["fsroot"]:
            command = [
                (
                    f'{SCRIPT_DIR}/extract_bin_info_fsroot.cc("{temp_file_path}",'
                    f" \"{output_file_name}\", \"{args['tree_name']}\","
                    f" \"{args['meson_index']}\")\n "
                )
            ]
            package = "$FSROOT/rootlogon.FSROOT.C"

        command = [
            (
                f'{SCRIPT_DIR}/extract_bin_info.cc("{temp_file_path}",'
                f" \"{output_file_name}\", \"{args['mass_branch']}\")"
            )
        ]
    else:
        raise ValueError("Invalid type. Must be either 'fit' or 'root'")

    proc_command = ["root", "-n", "-l", "-b", "-q"]
    proc_command.append(package)
    proc_command.extend(command)

    print(f"Running ROOT macro with command: {proc_command}")
    # call the ROOT macro
    proc = subprocess.Popen(
        proc_command,
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
    )
    # print the output of the ROOT macro as it runs
    if args["verbose"]:
        for line in iter(proc.stdout.readline, ""):
            print(line, end="")
    proc.wait()  # wait for the process to finish and update the return code
    if proc.returncode != 0:
        print("Error while running ROOT macro:")
        for line in iter(proc.stderr.readline, ""):
            print(line, end="")
    else:
        print("ROOT macro completed successfully")

    return


def parse_args() -> dict:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "-i",
        "--input",
        help=(
            "Input file(s). Also accepts path(s) with a wildcard '*' and finds all"
            " matching files. Can also accept a file containing a list of files"
        ),
        nargs="+",
    )
    parser.add_argument(
        "-o", "--output", default="", help="File name of output .csv file"
    )
    parser.add_argument(
        "-s",
        "--sorted",
        type=bool,
        default=True,
        help=(
            "Sort the input files by last number in the file name or path. Defaults"
            " to True, so that the index of each csv row matches the ordering of the"
            " input files"
        ),
    )
    parser.add_argument(
        "--sort-index",
        type=int,
        default=-1,
        help=(
            "Determines what number in the file path is used for sorting. Defaults to"
            " -1, so that the last number in the path is used. See "
            " utils.sort_input_files for details."
        ),
    )
    parser.add_argument(
        "-a",
        "--acceptance-corrected",
        action="store_true",
        help=(
            "When passed, the amplitude intensities are corrected for acceptance. These"
            " are the true 'generated' values with no detector effects. Defaults to"
            " False, or the 'reconstructed' values"
        ),
    )
    parser.add_argument(
        "-m",
        "--mass-branch",
        type=str,
        default="M4Pi",
        help=(
            "Name of branch for the final invariant mass combo of interest in the"
            " Amplitude Analysis. Note this is only applicable when attempting to"
            " create csv's for ROOT data files. Defaults to M4Pi"
        ),
    )
    parser.add_argument(
        "--correlation",
        type=bool,
        default=True,
        help=(
            "When passed, the correlation matrix of each fit is included in a separate"
            " csv file. Defaults to True."
        ),
    )
    parser.add_argument(
        "--covariance",
        type=bool,
        default=True,
        help=(
            "When passed, the covariance matrix of each fit is included in a separate"
            " csv file. Defaults to True."
        ),
    )
    parser.add_argument(
        "-f",
        "--fsroot",
        action="store_true",
        help=(
            "Pass to indicate that the data input file(s) are in FSRoot format. Needs"
            " to be used in conjunction with -nt and -mi arguments."
        ),
    )
    parser.add_argument(
        "-nt",
        "--tree-name",
        type=str,
        default="ntFSGlueX_100_221",
        help=("FSRoot tree name. Defaults to ntFSGlueX_100_221"),
    )
    parser.add_argument(
        "-mi",
        "--meson-index",
        type=str,
        default="2,3,4,5",
        help=(
            "Indices of the particles coming from the meson vertex. Only relevant for"
            " FSRoot formatted data files. Defaults to 2,3,4,5"
        ),
    )
    parser.add_argument(
        "-p",
        "--preview",
        action="store_true",
        help=("When passed, print out the files that will be processed and exit."),
    )
    parser.add_argument(
        "-v",
        "--verbose",
        action="store_true",
        help="Print out more information while running the script",
    )
    return vars(parser.parse_args())


if __name__ == "__main__":
    args = parse_args()
    main(args)
