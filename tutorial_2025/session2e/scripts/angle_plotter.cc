/*Plot fit results for angular distributions and other variables of interest

The primary use of these is for diagnostic purposes, to check that the grey "Fit Result"
reasonably matches the black data points. Each total J^P contribution is also plotted,
to observe any interference effects those waves create.
*/

#include <iostream>
#include <regex>

#include "glueXstyle.C"
#include "TCanvas.h"
#include "TH1.h"
#include "TString.h"
#include "TFile.h"
#include "TLegend.h"

// struct to hold the properties of each JP contribution
struct JP_props
{
    TString legend;
    TColor *color;
    int marker;
};

// forward declarations
void plot1D(TFile *f, TString dir, TString data_title, TString reac);
std::vector<TColor *> create_custom_colors();
const std::map<TString, JP_props> create_jp_map();

void angle_plotter(TString file_name = "vecps_plot.root",
                   TString dir = "./", TString data_title = "GlueX Data",
                   TString reac = "")
{
    gluex_style(); // semi-standard style for GlueX root plots
    gStyle->SetOptStat(0);

    TFile *f = TFile::Open(dir + file_name);
    if (!f || f->IsZombie())
    {
        cout << "File" << dir + file_name << "doesn't exist or is corrupted! Exiting" << "\n";
        exit(1);
    }

    plot1D(f, dir, data_title, reac);

    return;
}

// Make the 1D histograms of interest from vecps_plotter
void plot1D(TFile *f, TString dir, TString data_title, TString reac)
{
    std::vector<TString> distributions = {"CosTheta", "Phi", "CosTheta_H", "Phi_H",
                                          "Prod_Ang", "MVecPs", "MProtonPs"};
    const std::map<TString, JP_props> jp_properties = create_jp_map();

    // iterate through the histograms in the file and find the JP contributions
    std::set<TString> jp_keys_found;
    TList *keys = f->GetListOfKeys();
    for (int i = 0; i < keys->GetSize(); i++)
    {
        TString key_name = keys->At(i)->GetName();

        // regex pattern for capturing a JP key (number + letter after an "_")
        std::regex regex_pattern(".*_(\\d[a-zA-Z]).*");
        std::smatch match;
        std::string key_name_str = key_name.Data();
        if (std::regex_match(key_name_str, match, regex_pattern))
        {
            TString jp_key = match[1].str();
            jp_keys_found.insert(jp_key);
        }
    }

    // create canvas and setup some plot parameters
    TCanvas *cc = new TCanvas("cc", "cc", 1800, 1000);
    cc->Divide(3, 3);

    double textSize = 0.10;
    TLegend *leg1 = new TLegend(0.1, 0.1, 0.5, 0.9);
    leg1->SetEntrySeparation(0.01);
    leg1->SetNColumns(2);
    leg1->SetColumnSeparation(1.0);
    leg1->SetMargin(0.2);
    leg1->SetFillColor(0);
    leg1->SetTextSize(textSize);
    leg1->SetBorderSize(0);
    leg1->SetLineColor(kWhite);

    int plot_count = 0;
    for (auto distribution : distributions)
    {
        // avoid plotting on the first subplot, since it will be used for the legend
        cc->cd(plot_count + 2);

        // first plot the data for this distribution
        TH1F *hdat = (TH1F *)f->Get(reac + distribution + "dat");
        if (!hdat)
        {
            cout << Form("hdat Plot %s doesn't exist! exiting", (reac + distribution + "dat").Data()) << "\n";
            exit(1);
        }

        // since its the first in the subplot, setup all the aesthetics
        hdat->SetLineColor(kBlack);
        hdat->SetLabelSize(0.06, "xy");
        hdat->SetTitleSize(0.08, "xy");
        hdat->SetTitleOffset(0.88, "x");
        hdat->SetTitleOffset(0.9, "y");
        hdat->SetMinimum(0);
        hdat->SetMarkerStyle(20);
        hdat->SetMarkerSize(0.5);
        hdat->Draw();
        if (plot_count == 0)
            leg1->AddEntry(hdat, data_title, "ep");

        // now plot the fit result, which is a special case
        TH1F *hfit = (TH1F *)f->Get(reac + distribution + "acc");
        if (!hfit)
        {
            cout << Form("hfit Plot %s doesn't exist! exiting", (reac + distribution + "acc").Data()) << "\n";
            exit(1);
        }
        hfit->SetFillStyle(1001);
        hfit->SetFillColorAlpha(16, 0.2);
        hfit->SetLineColor(16);
        hfit->Draw("same HIST");
        if (plot_count == 0)
            leg1->AddEntry(hfit, "Fit Result", "f");

        // now we can loop through the JP contributions and plot them
        for (const TString &jp : jp_keys_found)
        {
            TString hist_name = reac + distribution + "acc" + "_" + jp;
            TH1F *hjp = (TH1F *)f->Get(hist_name);

            if (!hjp)
            {
                cout << Form("Plot %s doesn't exist! exiting", hist_name.Data()) << "\n";
                exit(1);
            }
            hjp->SetMarkerColor(jp_properties.at(jp).color->GetNumber());
            hjp->SetLineColor(jp_properties.at(jp).color->GetNumber());
            hjp->SetMarkerSize(0.6);
            hjp->SetMarkerStyle(jp_properties.at(jp).marker);
            hjp->Draw("same ep");

            if (plot_count == 0)
                leg1->AddEntry(hjp, jp_properties.at(jp).legend, "ep");
        }
        plot_count += 1;
    }

    // finally draw the legend on the 1st subplot
    cc->cd(1);
    leg1->Draw();

    // save file
    cc->Print(dir + "fit.pdf");

    return;
}


// Mimic the colors of matplotlib's Dark2 colormap for consistency between plots
std::vector<TColor *> create_custom_colors()
{
    // rgb values from matplotlib's Dark2 colormap
    std::vector<std::vector<float>> mpl_Dark2_rgb_values = {
        {0.10588235294117647, 0.6196078431372549, 0.4666666666666667},
        {0.8509803921568627, 0.37254901960784315, 0.00784313725490196},
        {0.4588235294117647, 0.4392156862745098, 0.7019607843137254},
        {0.9058823529411765, 0.1607843137254902, 0.5411764705882353},
        {0.4, 0.6509803921568628, 0.11764705882352941},
        {0.9019607843137255, 0.6705882352941176, 0.00784313725490196},
        {0.6509803921568628, 0.4627450980392157, 0.11372549019607843},
        {0.4, 0.4, 0.4}};

    // Create TColor objects for each RGB value
    // and store them in a vector
    std::vector<TColor *> custom_colors;
    for (const auto &rgb : mpl_Dark2_rgb_values)
    {
        int color_index = TColor::GetFreeColorIndex();
        TColor *color = new TColor(color_index, rgb[0], rgb[1], rgb[2]);
        custom_colors.push_back(color);
    }
    return custom_colors;
}

// Create a map of the JP contributions to their properties
const std::map<TString, JP_props> create_jp_map()
{
    std::vector<TColor *> custom_colors = create_custom_colors();

    const std::map<TString, JP_props> jp_map = {
        {"0m", JP_props{"#[]{0^{#minus}}^{(#pm)} (P)", custom_colors[1], 5}},
        {"1p", JP_props{"#[]{1^{#plus}}^{(#pm)} (S+D)", custom_colors[2], 20}},
        {"1m", JP_props{"#[]{1^{#minus}}^{(#pm)} (P)", custom_colors[3], 21}},
        {"2m", JP_props{"#[]{2^{#minus}}^{(#pm)} (P+F)", custom_colors[4], 29}},
        {"2p", JP_props{"#[]{2^{#plus}}^{(#pm)} (D)", custom_colors[5], 34}},
        {"3m", JP_props{"#[]{3^{#minus}}^{(#pm)} (F)", custom_colors[6], 33}},
    };

    return jp_map;
}