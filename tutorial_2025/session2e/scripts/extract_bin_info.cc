/* Extract the bin information from a list of pre-cut ROOT data files used in fits

The csv file will have columns for:
    - The low and high bin edges for the t, E_beam, and mass histograms
    - The center, average, and RMS values for the t, E_beam, and mass histograms
    - The total number of events and the error on the total number of events

NOTE:
This script assumes that the original Flat Tree data files have been cut to their
respective bin, and that they contain the t, E_beam, and Weight branches. The
Weight branch is used for sideband subtraction, and so if a separate "background" file
is used, then it will need to be implemented here.
 */

#include <cmath>   // for power function
#include <fstream> // for writing csv
#include <iostream>
#include <sstream> // for std::istringstream
#include <string>
#include <vector>

// forward declarations
std::pair<double, double> get_hist_edges(TH1D *h, int round_to_decimals);

void extract_bin_info(std::string file_path, std::string csv_name, std::string mass_branch)
{
    // file path is a text file with a list of ROOT files, each on a newline
    std::vector<std::string> file_vector;
    std::ifstream infile(file_path);
    std::string line;
    while (std::getline(infile, line))
    {
        file_vector.push_back(line);
    }

    // Define header row and corresponding values
    std::vector<std::string> headers = {
        "file",
        "t_low",
        "t_high",
        "t_center",
        "t_avg",
        "t_rms",
        "e_low",
        "e_high",
        "e_center",
        "e_avg",
        "e_rms",
        "m_low",
        "m_high",
        "m_center",
        "m_avg",
        "m_rms",
        "events",
        "events_err",
    };
    std::vector<std::map<std::string, double>> values;

    for (const auto &file : file_vector)
    {
        std::map<std::string, double> value_map;
        // grab the file and tree
        std::unique_ptr<TFile> f(TFile::Open(file.c_str()));
        TTree *tree = f->Get<TTree>("kin");
        if (!tree)
        {
            std::cout << "'kin' tree could not be opened in file: " << file << "\n";
            exit(1);
        }

        TCanvas *c1 = new TCanvas("c1", "c1", 1800, 1800);

        // Assign histograms. These are required to get the mean and rms values
        tree->Draw("t>>h_t", "Weight");
        TH1D *h_t = (TH1D *)gPad->GetPrimitive("h_t");
        tree->Draw("E_Beam>>h_e", "Weight");
        TH1D *h_e = (TH1D *)gPad->GetPrimitive("h_e");
        tree->Draw((mass_branch + ">>h_m").c_str(), "Weight");
        TH1D *h_m = (TH1D *)gPad->GetPrimitive("h_m");

        // Fill the map. Round -t and E_beam to 2nd decimal, and the mass values to
        // the third decimal (1 MeV)
        double bin_contents_error;
        value_map["events"] = h_m->IntegralAndError(0, h_m->GetNbinsX() + 1, bin_contents_error);
        value_map["events_err"] = bin_contents_error;

        double t_low, t_high;
        std::tie(t_low, t_high) = get_hist_edges(h_t, 2);
        value_map["t_low"] = t_low;
        value_map["t_high"] = t_high;
        value_map["t_center"] = (t_high + t_low) / 2.0;
        value_map["t_avg"] = h_t->GetMean();
        value_map["t_rms"] = h_t->GetRMS();

        double e_low, e_high;
        std::tie(e_low, e_high) = get_hist_edges(h_e, 2);
        value_map["e_low"] = e_low;
        value_map["e_high"] = e_high;
        value_map["e_center"] = (e_high + e_low) / 2.0;
        value_map["e_avg"] = h_e->GetMean();
        value_map["e_rms"] = h_e->GetRMS();

        double m_low, m_high;
        std::tie(m_low, m_high) = get_hist_edges(h_m, 3);
        value_map["m_low"] = m_low;
        value_map["m_high"] = m_high;
        value_map["m_center"] = (m_high + m_low) / 2.0;
        value_map["m_avg"] = h_m->GetMean();
        value_map["m_rms"] = h_m->GetRMS();

        values.push_back(value_map);

        // Clean up
        delete h_t;
        delete h_e;
        delete h_m;
        delete c1;
    }

    // open csv file for writing
    std::ofstream csv_file;
    csv_file.open(csv_name);

    // Write the header line
    for (size_t i = 0; i < headers.size(); ++i)
    {
        csv_file << headers[i];
        if (i < headers.size() - 1)
        {
            csv_file << ",";
        }
    }
    csv_file << "\n";

    // Write values
    for (size_t i = 0; i < values.size(); ++i)
    {
        csv_file << file_vector[i] << ",";
        for (size_t j = 1; j < headers.size(); ++j)
        {
            csv_file << values[i].at(headers[j]);
            if (j < headers.size() - 1)
            {
                csv_file << ",";
            }
        }
        csv_file << "\n";
    }

    csv_file.close();
}

// Get the min / max non-zero bin edges of a histogram
std::pair<double, double> get_hist_edges(TH1D *h, int round_to_decimals)
{
    double min = std::numeric_limits<double>::max();
    double max = std::numeric_limits<double>::min();

    for (int i = 1; i <= h->GetNbinsX(); ++i)
    {
        double bin_content = h->GetBinContent(i);
        if (bin_content > 0 && h->GetXaxis()->GetBinLowEdge(i) < min)
        {
            min = h->GetXaxis()->GetBinLowEdge(i);
        }
        if (bin_content > 0 && h->GetXaxis()->GetBinUpEdge(i) > max)
        {
            max = h->GetXaxis()->GetBinUpEdge(i);
        }
    }
    // round values to requested decimal place
    min = std::round((min * std::pow(10, round_to_decimals))) / std::pow(10, round_to_decimals);
    max = std::round((max * std::pow(10, round_to_decimals))) / std::pow(10, round_to_decimals);
    return std::make_pair(min, max);
}