#include "TpcSeedsDraw.h"

#include <sPhenixStyle.C>

#include <qahtml/QADrawClient.h>
#include <qahtml/QADrawDB.h>

#include <TCanvas.h>
#include <TDatime.h>
#include <TGraphErrors.h>
#include <TH1.h>
#include <TH2.h>
#include <TLatex.h>
#include <TPad.h>
#include <TProfile.h>
#include <TProfile2D.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TSystem.h>
#include <TText.h>
#include <TLegend.h>

#include <boost/format.hpp>

#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

TpcSeedsDraw::TpcSeedsDraw(const std::string &name)
    : QADraw(name)
{
    gStyle->SetOptStat(0);
    memset(TC, 0, sizeof(TC));
    memset(transparent, 0, sizeof(transparent));
    memset(Pad, 0, sizeof(Pad));
    DBVarInit();
    histprefix = "h_TpcSeedsQA_";
    return;
}

TpcSeedsDraw::~TpcSeedsDraw()
{
    /* delete db; */
    return;
}

int TpcSeedsDraw::Draw(const std::string &what)
{
    /* SetsPhenixStyle(); */
    int iret = 0;
    int idraw = 0;
    if (what == "ALL" || what == "TRACKLET")
    {
        iret += DrawTrackletInfo();
        idraw++;
    }
    if (what == "ALL" || what == "CLUSTER")
    {
        iret += DrawClusterInfo();
        idraw++;
    }
    if (what == "ALL" || what == "DCA")
    {
        iret += DrawDCAInfo();
        idraw++;
    }
    if (what == "ALL" || what == "VERTEX")
    {
        iret += DrawVertexInfo();
        idraw++;
    }
    if (!idraw)
    {
        std::cout << " Unimplemented Drawing option: " << what << std::endl;
        iret = -1;
    }
    return iret;
}

int TpcSeedsDraw::MakeCanvas(const std::string &name, int num)
{
    std::cout << "Tpc Seeds MakeCanvas() Beginning" << std::endl;
    QADrawClient *cl = QADrawClient::instance();
    int xsize = cl->GetDisplaySizeX();
    int ysize = cl->GetDisplaySizeY();
    // xpos (-1) negative: do not draw menu bar
    TC[num] = new TCanvas(name.c_str(), (boost::format("Tpc Seeds Plots %d") % num).str().c_str(), -1, 0, (int)(xsize), (int)(ysize * 2.2));
    TC[num]->SetCanvasSize(xsize, ysize * 2.2);
    gSystem->ProcessEvents();

    int nrow = 4;
    double yoffset = 0.02;
    double ywidth=(1.-yoffset-yoffset)/(double)nrow;
    double x1=0, y1=0, x2=0, y2=0;
    for (int i=0; i<2*nrow; i++)
    {
        if (i%2==0)
        {
          x1=0.5;
          x2=0.95;
        }
        else if (i%2==1)
        {
          x1=0.05;
          x2=0.5;
        }
        y1=0.02+(i/2)*ywidth;
        y2=0.02+(i/2+1)*ywidth;

        Pad[num][2*nrow-1-i] = new TPad(
                                (boost::format("mypad_%1%_%2%") % num % i).str().c_str(),
                                "pad",
                                x1, y1, x2, y2
                                );
    }

    for (int i=0; i<2*nrow; i++) Pad[num][2*nrow-1-i]->Draw();

    // this one is used to plot the run number on the canvas
    transparent[num] = new TPad((boost::format("transparent%d") % num).str().c_str(), "this does not show", 0, 0, 1, 1);
    transparent[num]->SetFillStyle(4000);
    transparent[num]->Draw();

    return 0;
}

int TpcSeedsDraw::DrawTrackletInfo()
{
    std::cout << "Tpc Seeds DrawTrackletInfo() Beginning" << std::endl;
    QADrawClient *cl = QADrawClient::instance();

    TH1F *h_ntrack1d = dynamic_cast<TH1F *>(cl->getHisto(histprefix + std::string("nrecotracks1d")));
    TH1F *h_ntrack1d_pos = dynamic_cast<TH1F *>(cl->getHisto(histprefix + std::string("nrecotracks1d_pos")));
    TH1F *h_ntrack1d_neg = dynamic_cast<TH1F *>(cl->getHisto(histprefix + std::string("nrecotracks1d_neg")));
    TH1F *h_ntrack1d_ptg1 = dynamic_cast<TH1F *>(cl->getHisto(histprefix + std::string("nrecotracks1d_ptg1")));
    TH1F *h_ntrack1d_ptg1_pos = dynamic_cast<TH1F *>(cl->getHisto(histprefix + std::string("nrecotracks1d_ptg1_pos")));
    TH1F *h_ntrack1d_ptg1_neg = dynamic_cast<TH1F *>(cl->getHisto(histprefix + std::string("nrecotracks1d_ptg1_neg")));
    TH1F *h_pt = dynamic_cast<TH1F *>(cl->getHisto(histprefix + std::string("pt")));
    TH1F *h_pt_pos = dynamic_cast<TH1F *>(cl->getHisto(histprefix + std::string("pt_pos")));
    TH1F *h_pt_neg = dynamic_cast<TH1F *>(cl->getHisto(histprefix + std::string("pt_neg")));
    TH2F *h_ntrack_pos = dynamic_cast<TH2F *>(cl->getHisto(histprefix + std::string("nrecotracks_pos")));
    TH2F *h_ntrack_neg = dynamic_cast<TH2F *>(cl->getHisto(histprefix + std::string("nrecotracks_neg")));
    //TH1F *h_trackcrossing_pos = dynamic_cast<TH1F *>(cl->getHisto(histprefix + std::string("trackcrossing_pos")));
    //TH1F *h_trackcrossing_neg = dynamic_cast<TH1F *>(cl->getHisto(histprefix + std::string("trackcrossing_neg")));
    TH1F *h_ntrack_isfromvtx_pos = dynamic_cast<TH1F *>(cl->getHisto(histprefix + std::string("ntrack_isfromvtx_pos")));
    TH1F *h_ntrack_isfromvtx_neg = dynamic_cast<TH1F *>(cl->getHisto(histprefix + std::string("ntrack_isfromvtx_neg")));

    if (!gROOT->FindObject("track_info"))
    {
        MakeCanvas("track_info", 0);
    }
    TC[0]->Clear("D");

    Pad[0][0]->cd();
    if (h_ntrack1d && h_ntrack1d_pos && h_ntrack1d_neg)
    {
        h_ntrack1d->SetTitle("Number of tpc tracks");
        h_ntrack1d->SetXTitle("Number of tpc tracks");
        h_ntrack1d->SetYTitle("Entries");
        h_ntrack1d->SetMarkerColor(kBlack);
        h_ntrack1d->SetLineColor(kBlack);
        auto ymax = h_ntrack1d->GetMaximum();
        auto ymax_pos = h_ntrack1d_pos->GetMaximum();
        auto ymax_neg = h_ntrack1d_neg->GetMaximum();
        h_ntrack1d->SetMaximum(1.2*std::max({ymax, ymax_pos, ymax_neg}));
        h_ntrack1d->DrawCopy("");
        h_ntrack1d_pos->SetMarkerColor(kRed);
        h_ntrack1d_pos->SetLineColor(kRed);
        h_ntrack1d_pos->DrawCopy("same");
        h_ntrack1d_neg->SetMarkerColor(kBlue);
        h_ntrack1d_neg->SetLineColor(kBlue);
        h_ntrack1d_neg->DrawCopy("same");
        auto legend = new TLegend(0.55, 0.7, 0.83, 0.9);
        legend->AddEntry(h_ntrack1d, "Positive + Negative", "pl");
        legend->AddEntry(h_ntrack1d_pos, "Positive charged", "pl");
        legend->AddEntry(h_ntrack1d_neg, "Negative charged", "pl");
        legend->Draw();
        gPad->SetRightMargin(0.15);
    }
    else
    {
        // histogram is missing
        return -1;
    }

    Pad[0][1]->cd();
    Pad[0][1]->SetLogy(1);
    if (h_ntrack1d_ptg1 && h_ntrack1d_ptg1_pos && h_ntrack1d_ptg1_neg)
    {
        h_ntrack1d_ptg1->SetTitle("Number of tpc tracks with p_{T}>1GeV");
        h_ntrack1d_ptg1->SetXTitle("Number of tpc tracks");
        h_ntrack1d_ptg1->SetYTitle("Entries");
        h_ntrack1d_ptg1->SetMarkerColor(kBlack);
        h_ntrack1d_ptg1->SetLineColor(kBlack);
        auto ymax = h_ntrack1d_ptg1->GetMaximum();
        auto ymax_pos = h_ntrack1d_ptg1_pos->GetMaximum();
        auto ymax_neg = h_ntrack1d_ptg1_neg->GetMaximum();
        h_ntrack1d_ptg1->SetMaximum(1.2*std::max({ymax, ymax_pos, ymax_neg}));
        h_ntrack1d_ptg1->DrawCopy("");
        h_ntrack1d_ptg1_pos->SetMarkerColor(kRed);
        h_ntrack1d_ptg1_pos->SetLineColor(kRed);
        h_ntrack1d_ptg1_pos->DrawCopy("same");
        h_ntrack1d_ptg1_neg->SetMarkerColor(kBlue);
        h_ntrack1d_ptg1_neg->SetLineColor(kBlue);
        h_ntrack1d_ptg1_neg->DrawCopy("same");
        auto legend = new TLegend(0.55, 0.7, 0.83, 0.9);
        legend->AddEntry(h_ntrack1d_ptg1, "Positive + Negative", "pl");
        legend->AddEntry(h_ntrack1d_ptg1_pos, "Positive charged", "pl");
        legend->AddEntry(h_ntrack1d_ptg1_neg, "Negative charged", "pl");
        legend->Draw();
        gPad->SetRightMargin(0.15);
    }
    else
    {
        // histogram is missing
        return -1;
    }

    Pad[0][2]->cd();
    Pad[0][2]->SetLogy(1);
    if (h_pt && h_pt_pos && h_pt_neg)
    {
        h_pt->SetTitle("p_{T} distribution for p_{T}>1GeV tracks");
        h_pt->SetXTitle("p_{T} (GeV)");
        h_pt->SetYTitle("Entries");
        h_pt->SetMarkerColor(kBlack);
        h_pt->SetLineColor(kBlack);
        auto ymax = h_pt->GetMaximum();
        auto ymax_pos = h_pt_pos->GetMaximum();
        auto ymax_neg = h_pt_neg->GetMaximum();
        h_pt->SetMaximum(1.2*std::max({ymax, ymax_pos, ymax_neg}));
        h_pt->DrawCopy("");
        h_pt_pos->SetMarkerColor(kRed);
        h_pt_pos->SetLineColor(kRed);
        h_pt_pos->DrawCopy("same");
        h_pt_neg->SetMarkerColor(kBlue);
        h_pt_neg->SetLineColor(kBlue);
        h_pt_neg->DrawCopy("same");
        auto legend = new TLegend(0.55, 0.7, 0.83, 0.9);
        legend->AddEntry(h_pt, "Positive + Negative", "pl");
        legend->AddEntry(h_pt_pos, "Positive charged", "pl");
        legend->AddEntry(h_pt_neg, "Negative charged", "pl");
        legend->Draw();
        gPad->SetRightMargin(0.15);
    }
    else
    {
        // histogram is missing
        return -1;
    }

/*
    Pad[0][3]->cd();
    if (h_trackcrossing_pos && h_trackcrossing_neg)
    {
        h_trackcrossing_pos->SetXTitle("Track crossing");
        h_trackcrossing_pos->SetYTitle("Entries");
        h_trackcrossing_pos->SetMarkerColor(kRed);
        h_trackcrossing_pos->SetLineColor(kRed);
        auto ymax_pos = h_trackcrossing_pos->GetMaximum();
        auto ymax_neg = h_trackcrossing_neg->GetMaximum();
        h_trackcrossing_pos->SetMaximum(1.2*std::max({ymax_pos, ymax_neg}));
        h_trackcrossing_pos->DrawCopy();
        h_trackcrossing_neg->SetMarkerColor(kBlue);
        h_trackcrossing_neg->SetLineColor(kBlue);
        h_trackcrossing_neg->DrawCopy("same");
        auto legend = new TLegend(0.55, 0.7, 0.83, 0.9);
        legend->AddEntry(h_trackcrossing_pos, "Positive charged", "pl");
        legend->AddEntry(h_trackcrossing_neg, "Negative charged", "pl");
        legend->Draw();
        gPad->SetRightMargin(0.17);
    }
    else
    {
        // histogram is missing
        return -1;
    }
*/

    Pad[0][4]->cd();
    if (h_ntrack_pos)
    {
        h_ntrack_pos->SetXTitle("#eta");
        h_ntrack_pos->SetYTitle("#phi [rad]");
        h_ntrack_pos->SetZTitle("Number of positive tracks with p_{T}>1GeV");
        h_ntrack_pos->DrawCopy("colz");
        gPad->SetRightMargin(0.17);
    }
    else
    {
        // histogram is missing
        return -1;
    }

    Pad[0][5]->cd();
    if (h_ntrack_neg)
    {
        h_ntrack_neg->SetXTitle("#eta");
        h_ntrack_neg->SetYTitle("#phi [rad]");
        h_ntrack_neg->SetZTitle("Number of negative tracks with p_{T}>1GeV");
        h_ntrack_neg->DrawCopy("colz");
        gPad->SetRightMargin(0.17);
    }
    else
    {
        // histogram is missing
        return -1;
    }

    Pad[0][6]->cd();
    if (h_ntrack_isfromvtx_pos)
    {
        h_ntrack_isfromvtx_pos->SetTitle("Is positive track from a vertex");
        h_ntrack_isfromvtx_pos->SetXTitle("Is positive track associated to a vertex");
        h_ntrack_isfromvtx_pos->SetYTitle("Normalized Entries");
        h_ntrack_isfromvtx_pos->Scale(1. / h_ntrack_isfromvtx_pos->Integral());
        h_ntrack_isfromvtx_pos->SetMarkerSize(1.5);
        h_ntrack_isfromvtx_pos->DrawCopy("histtext0");
        gPad->SetRightMargin(0.15);
    }
    else
    {
        // histogram is missing
        return -1;
    }

    Pad[0][7]->cd();
    if (h_ntrack_isfromvtx_neg)
    {
        h_ntrack_isfromvtx_neg->SetTitle("Is negative track from a vertex");
        h_ntrack_isfromvtx_neg->SetXTitle("Is negative track associated to a vertex");
        h_ntrack_isfromvtx_neg->SetYTitle("Normalized Entries");
        h_ntrack_isfromvtx_neg->Scale(1. / h_ntrack_isfromvtx_neg->Integral());
        h_ntrack_isfromvtx_neg->SetMarkerSize(1.5);
        h_ntrack_isfromvtx_neg->DrawCopy("histtext0");
        gPad->SetRightMargin(0.15);
    }
    else
    {
        // histogram is missing
        return -1;
    }

    TText PrintRun;
    PrintRun.SetTextFont(62);
    PrintRun.SetTextSize(0.03);
    PrintRun.SetNDC();         // set to normalized coordinates
    PrintRun.SetTextAlign(23); // center/top alignment
    std::ostringstream runnostream1;
    std::string runstring1;
    runnostream1 << Name() << "_tpcseeds Track Info Run " << cl->RunNumber();
    runstring1 = runnostream1.str();
    transparent[0]->cd();
    PrintRun.DrawText(0.5, 1., runstring1.c_str());

    TC[0]->Update();

    std::cout << "DrawTrackletInfo Ending" << std::endl;
    return 0;
}

int TpcSeedsDraw::DrawClusterInfo()
{
    std::cout << "Tpc Seeds DrawClusterInfo() Beginning" << std::endl;
    QADrawClient *cl = QADrawClient::instance();

    TH1F *h_ntpc_pos = dynamic_cast<TH1F *>(cl->getHisto(histprefix + std::string("ntpc_pos")));
    TH1F *h_ntpc_neg = dynamic_cast<TH1F *>(cl->getHisto(histprefix + std::string("ntpc_neg")));
    TH1F *h_ntpot_pos = dynamic_cast<TH1F *>(cl->getHisto(histprefix + std::string("ntpot_pos")));
    TH1F *h_ntpot_neg = dynamic_cast<TH1F *>(cl->getHisto(histprefix + std::string("ntpot_neg")));
    TProfile2D *h_avgnclus_eta_phi_pos = dynamic_cast<TProfile2D *>(cl->getHisto(histprefix + std::string("avgnclus_eta_phi_pos")));
    TProfile2D *h_avgnclus_eta_phi_neg = dynamic_cast<TProfile2D *>(cl->getHisto(histprefix + std::string("avgnclus_eta_phi_neg")));
    TH1F *h_cluster_phisize1_fraction_pos = dynamic_cast<TH1F *>(cl->getHisto(histprefix + std::string("cluster_phisize1_fraction_pos")));
    TH1F *h_cluster_phisize1_fraction_neg = dynamic_cast<TH1F *>(cl->getHisto(histprefix + std::string("cluster_phisize1_fraction_neg")));

    if (!gROOT->FindObject("cluster_info"))
    {
        MakeCanvas("cluster_info", 1);
    }
    TC[1]->Clear("D");

    Pad[1][0]->cd();
    if (h_ntpc_pos && h_ntpc_neg)
    {
        h_ntpc_pos->SetTitle("Number of TPC clusters for track with p_{T}>1GeV");
        h_ntpc_pos->SetXTitle("Number of TPC clusters");
        h_ntpc_pos->SetYTitle("Entries");
        h_ntpc_pos->SetMarkerColor(kRed);
        h_ntpc_pos->SetLineColor(kRed);
        auto ymax_pos = h_ntpc_pos->GetMaximum();
        auto ymax_neg = h_ntpc_neg->GetMaximum();
        h_ntpc_pos->SetMaximum(1.2*std::max({ymax_pos, ymax_neg}));
        h_ntpc_pos->DrawCopy("");
        h_ntpc_neg->SetMarkerColor(kBlue);
        h_ntpc_neg->SetLineColor(kBlue);
        h_ntpc_neg->DrawCopy("same");
        auto legend = new TLegend(0.55, 0.7, 0.83, 0.9);
        legend->AddEntry(h_ntpc_pos, "Positive charged", "pl");
        legend->AddEntry(h_ntpc_neg, "Negative charged", "pl");
        legend->Draw();
        gPad->SetRightMargin(0.15);
    }
    else
    {
        // histogram is missing
        return -1;
    }

    Pad[1][1]->cd();
    if (h_ntpot_pos && h_ntpot_neg)
    {
        h_ntpot_pos->SetTitle("Number of TPOT clusters for track with p_{T}>1GeV");
        h_ntpot_pos->SetXTitle("Number of TPOT clusters");
        h_ntpot_pos->SetYTitle("Entries");
        h_ntpot_pos->SetMarkerColor(kRed);
        h_ntpot_pos->SetLineColor(kRed);
        auto ymax_pos = h_ntpot_pos->GetMaximum();
        auto ymax_neg = h_ntpot_neg->GetMaximum();
        h_ntpot_pos->SetMaximum(1.2*std::max({ymax_pos, ymax_neg}));
        h_ntpot_pos->DrawCopy("");
        h_ntpot_neg->SetMarkerColor(kBlue);
        h_ntpot_neg->SetLineColor(kBlue);
        h_ntpot_neg->DrawCopy("same");
        auto legend = new TLegend(0.55, 0.7, 0.83, 0.9);
        legend->AddEntry(h_ntpc_pos, "Positive charged", "pl");
        legend->AddEntry(h_ntpc_neg, "Negative charged", "pl");
        legend->Draw();
        gPad->SetRightMargin(0.15);
    }
    else
    {
        // histogram is missing
        return -1;
    }

    Pad[1][2]->cd();
    if (h_avgnclus_eta_phi_pos)
    {
        h_avgnclus_eta_phi_pos->SetXTitle("#eta");
        h_avgnclus_eta_phi_pos->SetYTitle("#phi [rad]");
        h_avgnclus_eta_phi_pos->SetZTitle("Average number of clusters per positive track with p_{T}>1GeV");
        h_avgnclus_eta_phi_pos->DrawCopy("colz");
        gPad->SetRightMargin(0.17);
    }
    else
    {
        // histogram is missing
        return -1;
    }

    Pad[1][3]->cd();
    if (h_avgnclus_eta_phi_neg)
    {
        h_avgnclus_eta_phi_neg->SetXTitle("#eta");
        h_avgnclus_eta_phi_neg->SetYTitle("#phi [rad]");
        h_avgnclus_eta_phi_neg->SetZTitle("Average number of clusters per negative track with p_{T}>1GeV");
        h_avgnclus_eta_phi_neg->DrawCopy("colz");
        gPad->SetRightMargin(0.17);
    }
    else
    {
        // histogram is missing
        return -1;
    }

    Pad[1][4]->cd();
    if (h_cluster_phisize1_fraction_pos && h_cluster_phisize1_fraction_neg)
    {
        h_cluster_phisize1_fraction_pos->SetTitle("Fraction of TPC clusters per p_{T}>1GeV track with phi size of 1");
        h_cluster_phisize1_fraction_pos->SetXTitle("Fraction of TPC clusters per p_{T}>1GeV track with phi size of 1");
        h_cluster_phisize1_fraction_pos->SetYTitle("Entries");
        h_cluster_phisize1_fraction_pos->SetMarkerColor(kRed);
        h_cluster_phisize1_fraction_pos->SetLineColor(kRed);
        auto ymax_pos = h_cluster_phisize1_fraction_pos->GetMaximum();
        auto ymax_neg = h_cluster_phisize1_fraction_neg->GetMaximum();
        h_cluster_phisize1_fraction_pos->SetMaximum(1.2*std::max({ymax_pos, ymax_neg}));
        h_cluster_phisize1_fraction_pos->DrawCopy();
        h_cluster_phisize1_fraction_neg->SetMarkerColor(kBlue);
        h_cluster_phisize1_fraction_neg->SetLineColor(kBlue);
        h_cluster_phisize1_fraction_neg->DrawCopy("same");
        auto legend = new TLegend(0.55, 0.7, 0.83, 0.9);
        legend->AddEntry(h_ntpc_pos, "Positive charged", "pl");
        legend->AddEntry(h_ntpc_neg, "Negative charged", "pl");
        legend->Draw();
        gPad->SetRightMargin(0.15);
    }
    else
    {
        // histogram is missing
        return -1;
    }

    TText PrintRun;
    PrintRun.SetTextFont(62);
    PrintRun.SetTextSize(0.03);
    PrintRun.SetNDC();         // set to normalized coordinates
    PrintRun.SetTextAlign(23); // center/top alignment
    std::ostringstream runnostream1;
    std::string runstring1;
    runnostream1 << Name() << "_tpcseeds Cluster Info Run " << cl->RunNumber();
    runstring1 = runnostream1.str();
    transparent[1]->cd();
    PrintRun.DrawText(0.5, 1., runstring1.c_str());

    TC[1]->Update();

    std::cout << "DrawClusterInfo Ending" << std::endl;
    return 0;
}

int TpcSeedsDraw::DrawDCAInfo()
{
    std::cout << "Tpc Seeds DrawDCAInfo() Beginning" << std::endl;
    QADrawClient *cl = QADrawClient::instance();

    TH2F *h_dcaxyorigin_phi_pos = dynamic_cast<TH2F *>(cl->getHisto(histprefix + std::string("dcaxyorigin_phi_pos")));
    TH2F *h_dcaxyorigin_phi_neg = dynamic_cast<TH2F *>(cl->getHisto(histprefix + std::string("dcaxyorigin_phi_neg")));
    TH2F *h_dcaxyvtx_phi_pos = dynamic_cast<TH2F *>(cl->getHisto(histprefix + std::string("dcaxyvtx_phi_pos")));
    TH2F *h_dcaxyvtx_phi_neg = dynamic_cast<TH2F *>(cl->getHisto(histprefix + std::string("dcaxyvtx_phi_neg")));
    TH2F *h_dcazorigin_phi_pos = dynamic_cast<TH2F *>(cl->getHisto(histprefix + std::string("dcazorigin_phi_pos")));
    TH2F *h_dcazorigin_phi_neg = dynamic_cast<TH2F *>(cl->getHisto(histprefix + std::string("dcazorigin_phi_neg")));
    TH2F *h_dcazvtx_phi_pos = dynamic_cast<TH2F *>(cl->getHisto(histprefix + std::string("dcazvtx_phi_pos")));
    TH2F *h_dcazvtx_phi_neg = dynamic_cast<TH2F *>(cl->getHisto(histprefix + std::string("dcazvtx_phi_neg")));

    if (!gROOT->FindObject("dca_info"))
    {
        MakeCanvas("dca_info", 2);
    }
    TC[2]->Clear("D");

    Pad[2][0]->cd();
    if (h_dcaxyorigin_phi_pos)
    {
        h_dcaxyorigin_phi_pos->SetXTitle("#phi [rad]");
        h_dcaxyorigin_phi_pos->SetYTitle("Positive track DCA_{xy} wrt origin [cm]");
        h_dcaxyorigin_phi_pos->SetZTitle("Entries");
        h_dcaxyorigin_phi_pos->DrawCopy("colz");
        gPad->SetRightMargin(0.17);
    }
    else
    {
        // histogram is missing
        return -1;
    }

    Pad[2][1]->cd();
    if (h_dcaxyorigin_phi_neg)
    {
        h_dcaxyorigin_phi_neg->SetXTitle("#phi [rad]");
        h_dcaxyorigin_phi_neg->SetYTitle("Negative track DCA_{xy} wrt origin [cm]");
        h_dcaxyorigin_phi_neg->SetZTitle("Entries");
        h_dcaxyorigin_phi_neg->DrawCopy("colz");
        gPad->SetRightMargin(0.17);
    }
    else
    {
        // histogram is missing
        return -1;
    }

    Pad[2][2]->cd();
    if (h_dcaxyvtx_phi_pos)
    {
        h_dcaxyvtx_phi_pos->SetXTitle("#phi [rad]");
        h_dcaxyvtx_phi_pos->SetYTitle("Positive track DCA_{xy} wrt vertex [cm]");
        h_dcaxyvtx_phi_pos->SetZTitle("Entries");
        h_dcaxyvtx_phi_pos->DrawCopy("colz");
        gPad->SetRightMargin(0.17);
    }
    else
    {
        // histogram is missing
        return -1;
    }

    Pad[2][3]->cd();
    if (h_dcaxyvtx_phi_neg)
    {
        h_dcaxyvtx_phi_neg->SetXTitle("#phi [rad]");
        h_dcaxyvtx_phi_neg->SetYTitle("Negative track DCA_{xy} wrt vertex [cm]");
        h_dcaxyvtx_phi_neg->SetZTitle("Entries");
        h_dcaxyvtx_phi_neg->DrawCopy("colz");
        gPad->SetRightMargin(0.17);
    }
    else
    {
        // histogram is missing
        return -1;
    }

    Pad[2][4]->cd();
    if (h_dcazorigin_phi_pos)
    {
        h_dcazorigin_phi_pos->SetXTitle("#phi [rad]");
        h_dcazorigin_phi_pos->SetYTitle("Positive track DCA_{z} wrt origin [cm]");
        h_dcazorigin_phi_pos->SetZTitle("Entries");
        h_dcazorigin_phi_pos->DrawCopy("colz");
        gPad->SetRightMargin(0.17);
    }
    else
    {
        // histogram is missing
        return -1;
    }

    Pad[2][5]->cd();
    if (h_dcazorigin_phi_neg)
    {
        h_dcazorigin_phi_neg->SetXTitle("#phi [rad]");
        h_dcazorigin_phi_neg->SetYTitle("Negative track DCA_{z} wrt origin [cm]");
        h_dcazorigin_phi_neg->SetZTitle("Entries");
        h_dcazorigin_phi_neg->DrawCopy("colz");
        gPad->SetRightMargin(0.17);
    }
    else
    {
        // histogram is missing
        return -1;
    }

    Pad[2][6]->cd();
    if (h_dcazvtx_phi_pos)
    {
        h_dcazvtx_phi_pos->SetXTitle("#phi [rad]");
        h_dcazvtx_phi_pos->SetYTitle("Positive track DCA_{z} wrt vertex [cm]");
        h_dcazvtx_phi_pos->SetZTitle("Entries");
        h_dcazvtx_phi_pos->DrawCopy("colz");
        gPad->SetRightMargin(0.17);
    }
    else
    {
        // histogram is missing
        return -1;
    }

    Pad[2][7]->cd();
    if (h_dcazvtx_phi_neg)
    {
        h_dcazvtx_phi_neg->SetXTitle("#phi [rad]");
        h_dcazvtx_phi_neg->SetYTitle("Negative track DCA_{z} wrt vertex [cm]");
        h_dcazvtx_phi_neg->SetZTitle("Entries");
        h_dcazvtx_phi_neg->DrawCopy("colz");
        gPad->SetRightMargin(0.17);
    }
    else
    {
        // histogram is missing
        return -1;
    }

    TText PrintRun;
    PrintRun.SetTextFont(62);
    PrintRun.SetTextSize(0.03);
    PrintRun.SetNDC();         // set to normalized coordinates
    PrintRun.SetTextAlign(23); // center/top alignment
    std::ostringstream runnostream1;
    std::string runstring1;
    runnostream1 << Name() << "_tpcseeds DCA Info Run " << cl->RunNumber();
    runstring1 = runnostream1.str();
    transparent[2]->cd();
    PrintRun.DrawText(0.5, 1., runstring1.c_str());

    TC[2]->Update();

    std::cout << "DrawDCAInfo Ending" << std::endl;
    return 0;
}

int TpcSeedsDraw::DrawVertexInfo()
{
    std::cout << "Tpc Seeds DrawVertexInfo() Beginning" << std::endl;
    QADrawClient *cl = QADrawClient::instance();

    TH1F *h_nvertex = dynamic_cast<TH1F *>(cl->getHisto(histprefix + std::string("nrecovertices")));
    TH2F *h_vx_vy = dynamic_cast<TH2F *>(cl->getHisto(histprefix + std::string("vx_vy")));
    TH1F *h_vz = dynamic_cast<TH1F *>(cl->getHisto(histprefix + std::string("vz")));
    TH1F *h_vt = dynamic_cast<TH1F *>(cl->getHisto(histprefix + std::string("vt")));
    //TH1F *h_vcrossing = dynamic_cast<TH1F *>(cl->getHisto(histprefix + std::string("vertexcrossing")));
    TH1F *h_vchi2dof = dynamic_cast<TH1F *>(cl->getHisto(histprefix + std::string("vertexchi2dof")));
    TH1F *h_ntrackpervertex = dynamic_cast<TH1F *>(cl->getHisto(histprefix + std::string("ntrackspervertex")));

    if (!gROOT->FindObject("vertex_info"))
    {
        MakeCanvas("vertex_info", 3);
    }
    TC[3]->Clear("D");

    Pad[3][0]->cd();
    if (h_nvertex)
    {
        h_nvertex->SetXTitle("Number of vertices");
        h_nvertex->SetYTitle("Entries");
        h_nvertex->DrawCopy();
        gPad->SetRightMargin(0.15);
    }
    else
    {
        // histogram is missing
        return -1;
    }

    Pad[3][1]->cd();
    if (h_vx_vy)
    {
        h_vx_vy->SetXTitle("Vertex x [cm]");
        h_vx_vy->SetYTitle("Vertex y [cm]");
        h_vx_vy->SetZTitle("Entries");
        h_vx_vy->DrawCopy("colz");
        gPad->SetRightMargin(0.17);
    }
    else
    {
        // histogram is missing
        return -1;
    }

    Pad[3][2]->cd();
    if (h_vz)
    {
        h_vz->SetXTitle("Vertex z [cm]");
        h_vz->SetYTitle("Entries");
        h_vz->DrawCopy();
        gPad->SetRightMargin(0.15);
    }
    else
    {
        // histogram is missing
        return -1;
    }

    Pad[3][3]->cd();
    if (h_vt)
    {
        h_vt->SetXTitle("Vertex t [ns]");
        h_vt->SetYTitle("Entries");
        h_vt->DrawCopy("colz");
        gPad->SetRightMargin(0.15);
    }
    else
    {
        // histogram is missing
        return -1;
    }

/*
    Pad[3][4]->cd();
    if (h_vcrossing)
    {
        h_vcrossing->SetXTitle("Vertex crossing ");
        h_vcrossing->SetYTitle("Entries");
        h_vcrossing->DrawCopy();
        gPad->SetRightMargin(0.15);
    }
    else
    {
        // histogram is missing
        return -1;
    }
*/

    Pad[3][4]->cd();
    if (h_vchi2dof)
    {
        h_vchi2dof->SetXTitle("Vertex #chi2/ndof");
        h_vchi2dof->SetYTitle("Entries");
        h_vchi2dof->DrawCopy();
        gPad->SetRightMargin(0.15);
    }
    else
    {
        // histogram is missing
        return -1;
    }

    Pad[3][5]->cd();
    if (h_ntrackpervertex)
    {
        h_ntrackpervertex->SetXTitle("Number of tracks per vertex");
        h_ntrackpervertex->SetYTitle("Entries");
        h_ntrackpervertex->DrawCopy();
        gPad->SetRightMargin(0.15);
    }
    else
    {
        // histogram is missing
        return -1;
    }

    TText PrintRun;
    PrintRun.SetTextFont(62);
    PrintRun.SetTextSize(0.03);
    PrintRun.SetNDC();         // set to normalized coordinates
    PrintRun.SetTextAlign(23); // center/top alignment
    std::ostringstream runnostream1;
    std::string runstring1;
    runnostream1 << Name() << "_tpcseeds Vertex Info Run " << cl->RunNumber();
    runstring1 = runnostream1.str();
    transparent[3]->cd();
    PrintRun.DrawText(0.5, 1., runstring1.c_str());

    TC[3]->Update();

    std::cout << "DrawVertexInfo Ending" << std::endl;
    return 0;
}

int TpcSeedsDraw::MakeHtml(const std::string &what)
{
    int iret = Draw(what);
    if (iret) // on error no html output please
    {
        return iret;
    }

    QADrawClient *cl = QADrawClient::instance();
    std::string pngfile;
    
    // Register the 1st canvas png file to the menu and produces the png file.
    if (what == "ALL" || what == "TRACKLET")
    {
        pngfile = cl->htmlRegisterPage(*this, "track_info", "1", "png");
        cl->CanvasToPng(TC[0], pngfile);
    }
    
    if (what == "ALL" || what == "CLUSTER")
    {
        pngfile = cl->htmlRegisterPage(*this, "cluster_info", "2", "png");
        cl->CanvasToPng(TC[1], pngfile);
    }
 
    if (what == "ALL" || what == "DCA")
    {
        pngfile = cl->htmlRegisterPage(*this, "dca_info", "3", "png");
        cl->CanvasToPng(TC[2], pngfile);
    }
 
    if (what == "ALL" || what == "VERTEX")
    {
        pngfile = cl->htmlRegisterPage(*this, "vertex_info", "4", "png");
        cl->CanvasToPng(TC[3], pngfile);
    }
    return 0;
}

int TpcSeedsDraw::DBVarInit()
{
    /* db = new QADrawDB(this); */
    /* db->DBInit(); */
    return 0;
}
