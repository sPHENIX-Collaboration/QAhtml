
#include "CaloFittingDraw.h"

#include <array>
#include <cstring>
#include <sPhenixStyle.C>

#include <TString.h>
#include <TVirtualPad.h>
#include <TLine.h>
#include <qahtml/QADrawClient.h>
#include <qahtml/QADrawDB.h>

#include <TCanvas.h>
#include <TGraphErrors.h>
#include <TH1.h>
#include <TH2.h>
#include <TProfile2D.h>
#include <TLatex.h>
#include <TPad.h>
#include <TROOT.h>
#include <TSystem.h>
#include <TText.h>
#include <TGaxis.h>
#include <TLegend.h>

#include <cmath>
#include <ctime>
#include <iostream>
#include <sstream>
#include <cstdio>
#include <chrono>
#include <utility>
#include <string>

const std::array<  int, CaloFittingDraw::packets_per_seb > CaloFittingDraw::seb00_packet_ids {{ 6067 , 6068 , 6071 , 6072 , 6075 , 6076 , 6079 , 6080 }};
const std::array<  int, CaloFittingDraw::packets_per_seb > CaloFittingDraw::seb01_packet_ids {{ 6083 , 6084 , 6087 , 6088 , 6091 , 6092 , 6095 , 6096 }};
const std::array<  int, CaloFittingDraw::packets_per_seb > CaloFittingDraw::seb02_packet_ids {{ 6115 , 6116 , 6119 , 6120 , 6123 , 6124 , 6127 , 6128 }};
const std::array<  int, CaloFittingDraw::packets_per_seb > CaloFittingDraw::seb03_packet_ids {{ 6099 , 6100 , 6103 , 6104 , 6107 , 6108 , 6111 , 6112 }};
const std::array<  int, CaloFittingDraw::packets_per_seb > CaloFittingDraw::seb04_packet_ids {{ 6035 , 6036 , 6039 , 6040 , 6043 , 6044 , 6047 , 6048 }};
const std::array<  int, CaloFittingDraw::packets_per_seb > CaloFittingDraw::seb05_packet_ids {{ 6051 , 6052 , 6055 , 6056 , 6059 , 6060 , 6063 , 6064 }};
const std::array<  int, CaloFittingDraw::packets_per_seb > CaloFittingDraw::seb06_packet_ids {{ 6019 , 6020 , 6023 , 6024 , 6027 , 6028 , 6031 , 6032 }};
const std::array<  int, CaloFittingDraw::packets_per_seb > CaloFittingDraw::seb07_packet_ids {{ 6003 , 6004 , 6007 , 6008 , 6011 , 6012 , 6015 , 6016 }};
const std::array<  int, CaloFittingDraw::packets_per_seb > CaloFittingDraw::seb08_packet_ids {{ 6065 , 6066 , 6069 , 6070 , 6073 , 6074 , 6077 , 6078 }};
const std::array<  int, CaloFittingDraw::packets_per_seb > CaloFittingDraw::seb09_packet_ids {{ 6081 , 6082 , 6085 , 6086 , 6089 , 6090 , 6093 , 6094 }};
const std::array<  int, CaloFittingDraw::packets_per_seb > CaloFittingDraw::seb10_packet_ids {{ 6121 , 6122 , 6125 , 6126 , 6113 , 6114 , 6117 , 6118 }};
const std::array<  int, CaloFittingDraw::packets_per_seb > CaloFittingDraw::seb11_packet_ids {{ 6105 , 6106 , 6109 , 6110 , 6097 , 6098 , 6101 , 6102 }};
const std::array<  int, CaloFittingDraw::packets_per_seb > CaloFittingDraw::seb12_packet_ids {{ 6033 , 6034 , 6037 , 6038 , 6041 , 6042 , 6045 , 6046 }};
const std::array<  int, CaloFittingDraw::packets_per_seb > CaloFittingDraw::seb13_packet_ids {{ 6049 , 6050 , 6053 , 6054 , 6057 , 6058 , 6061 , 6062 }};
const std::array<  int, CaloFittingDraw::packets_per_seb > CaloFittingDraw::seb14_packet_ids {{ 6025 , 6026 , 6029 , 6030 , 6017 , 6018 , 6021 , 6022 }};
const std::array<  int, CaloFittingDraw::packets_per_seb > CaloFittingDraw::seb15_packet_ids {{ 6009 , 6010 , 6013 , 6014 , 6001 , 6002 , 6005 , 6006 }};
const std::array<  int, CaloFittingDraw::packets_per_seb > CaloFittingDraw::seb16_packet_ids {{ 8001 , 8002 , 8007 , 8008 , 7001 , 7002 , 7007 , 7008 }};
const std::array<  int, CaloFittingDraw::packets_per_seb > CaloFittingDraw::seb17_packet_ids {{ 8003 , 8004 , 8005 , 8006 , 7003 , 7004 , 7005 , 7006 }};
const std::pair < int, int > CaloFittingDraw::emcal_packet_range = { 6000 , 6999 };
const std::pair < int, int > CaloFittingDraw::hcal_packet_range = { 7000, 8999 };
const std::pair < int, int > CaloFittingDraw::inner_hcal_packet_range = { 7000, 7999 };
const std::pair < int, int > CaloFittingDraw::outer_hcal_packet_range = { 8000, 8999 };
const std::array<  int, CaloFittingDraw::packets_per_seb > CaloFittingDraw::inner_hcal_packet_ids {{7001 , 7002 , 7007 , 7008 , 7003 , 7004 , 7005 , 7006 }};
const std::array<  int, CaloFittingDraw::packets_per_seb > CaloFittingDraw::outer_hcal_packet_ids {{8001 , 8002 , 8007 , 8008 , 8003 , 8004 , 8005 , 8006 }};
const std::array<  int, CaloFittingDraw::nsebs_hcal*CaloFittingDraw::packets_per_seb > CaloFittingDraw::hcal_packet_ids {};
const std::array<  int, CaloFittingDraw::npackets_total > CaloFittingDraw::valid_packet_ids {{
  6067 , 6068 , 6071 , 6072 , 6075 , 6076 , 6079 , 6080 ,
  6083 , 6084 , 6087 , 6088 , 6091 , 6092 , 6095 , 6096 ,
  6115 , 6116 , 6119 , 6120 , 6123 , 6124 , 6127 , 6128 ,
  6099 , 6100 , 6103 , 6104 , 6107 , 6108 , 6111 , 6112 ,
  6035 , 6036 , 6039 , 6040 , 6043 , 6044 , 6047 , 6048 ,
  6051 , 6052 , 6055 , 6056 , 6059 , 6060 , 6063 , 6064 ,
  6019 , 6020 , 6023 , 6024 , 6027 , 6028 , 6031 , 6032 ,
  6003 , 6004 , 6007 , 6008 , 6011 , 6012 , 6015 , 6016 ,
  6065 , 6066 , 6069 , 6070 , 6073 , 6074 , 6077 , 6078 ,
  6081 , 6082 , 6085 , 6086 , 6089 , 6090 , 6093 , 6094 ,
  6121 , 6122 , 6125 , 6126 , 6113 , 6114 , 6117 , 6118 ,
  6105 , 6106 , 6109 , 6110 , 6097 , 6098 , 6101 , 6102 ,
  6033 , 6034 , 6037 , 6038 , 6041 , 6042 , 6045 , 6046 ,
  6049 , 6050 , 6053 , 6054 , 6057 , 6058 , 6061 , 6062 ,
  6025 , 6026 , 6029 , 6030 , 6017 , 6018 , 6021 , 6022 ,
  6009 , 6010 , 6013 , 6014 , 6001 , 6002 , 6005 , 6006 ,
  8001 , 8002 , 8007 , 8008 , 7001 , 7002 , 7007 , 7008 ,
  8003 , 8004 , 8005 , 8006 , 7003 , 7004 , 7005 , 7006 }
};
  
CaloFittingDraw::CaloFittingDraw( const std::string &name ) 
  : QADraw(name)
{
  memset(TC, 0, sizeof(TC));
  memset(transparent, 0, sizeof(transparent));
  memset(Pad, 0, sizeof(Pad));
  // DBVarInit();
  
  return;
}

int CaloFittingDraw::Draw( const std::string &what )
{

  int iret = 0;
  int idraw = 0;
  if (what == "ALL" || what == "CEMC")
  {
    iret += DrawCalo(0);
    idraw++;
  }
  if (what == "ALL" || what == "IHCAL")
  {
    iret += DrawCalo(1);
    idraw++;
  }
  if (what == "ALL" || what == "OHCAL")
  {
    iret += DrawCalo(2);
    idraw++;
  }
  if (what == "ALL" || what == "PACKETSSUM")
  {
    GetPacketInfo();
    iret += MakeAlignmentSummary("packets1", 3);
    idraw++;
  }
  if (what == "ALL" || what == "PACKETSDETS")
  {
    iret += MakeDets("packets2", 4);
    idraw++;
  }
  if (!idraw)   {
    std::cout << " Unimplemented Drawing option: " << what << std::endl;
    iret = -1;
  }
  return iret;
}

int CaloFittingDraw::MakeCanvas( const std::string &name, int num )
{
  TC[num] = new TCanvas(name.c_str(), Form("CaloFittingDraw Plots %d", num), -1, 0, canvas_xsize, canvas_ysize);
  TC[num]->UseCurrentStyle();
  gSystem->ProcessEvents();
  Pad[num][0] = new TPad(Form("mypad%d0", num), "put", 0.02, 0.02, 0.49, 0.98, 0);
  Pad[num][1] = new TPad(Form("mypad%d1", num), "a", 0.51, 0.02, 0.98, 0.98, 0);
  Pad[num][0]->Draw();
  Pad[num][1]->Draw();
  transparent[num] = new TPad(Form("transparent%d", num), "this does not show", 0, 0, 1, 1);
  transparent[num]->SetFillStyle(4000);
  transparent[num]->Draw();
  return 0;
}

int CaloFittingDraw::DrawCalo( int num )
{

  std::string calo_string_lower;
  std::string calo_string_upper;
  if (num == 0) {
    calo_string_lower = "cemc";
    calo_string_upper = "EMCal";
  } else if (num == 1) {
    calo_string_lower = "ihcal";
    calo_string_upper = "iHCal";
  } else if (num == 2) {
    calo_string_lower = "ohcal";
    calo_string_upper = "oHCal";
  } else {
    std::cout << "CaloFittingDraw::DrawCalo: Invalid calo number: " << num << std::endl;
    return -1;
  }

  if (!gROOT->FindObject(Form("%s1", calo_string_lower.c_str()))) {
    MakeCanvas(Form("%s1", calo_string_lower.c_str()), num);
  }
  std::vector< std::string > tprof2_names = {"etaphi_ZScrosscalib"};
  std::vector< std::string > tprof2_titles = {"ZS Cross Calibration"};
  std::string  tprof2_xtitle = "#it{#eta}_{i}^{EMCal}";
  std::string  tprof2_ytitle = "#it{#phi}_{i}^{EMCal}";

  QADrawClient *cl = QADrawClient::instance();

  for (size_t i = 0; i < tprof2_names.size(); ++i) {
    TProfile2D * h_calofitting_prof2 = dynamic_cast<TProfile2D *>(cl->getHisto(histprefix + calo_string_lower + std::string("_") + tprof2_names[i]));
    Pad[num][i]->cd();
    gPad->UseCurrentStyle();
    gPad->SetLeftMargin(0.15);
    gPad->SetBottomMargin(0.15);
    gPad->SetTopMargin(0.1);
    gPad->SetRightMargin(0.15);
    if ( h_calofitting_prof2 ) {
      h_calofitting_prof2->GetXaxis()->SetTitle(tprof2_xtitle.c_str());
      h_calofitting_prof2->GetYaxis()->SetTitle(tprof2_ytitle.c_str());
      h_calofitting_prof2->GetXaxis()->SetNdivisions(505);
      h_calofitting_prof2->DrawCopy("COLZ");
    } else {
        std::cout << "CaloFittingDraw::DrawCalo: Histogram " << Form("%s%s_%s", histprefix, calo_string_lower.c_str(), tprof2_names[i].c_str()) << " not found!" << std::endl;
        TProfile2D * h_calofitting_prof2_empty = new TProfile2D(Form("%s%s_%s", histprefix, calo_string_lower.c_str(), tprof2_names[i].c_str()), "Empty Histogram", 1, 0, 1, 1, 0, 1);
        h_calofitting_prof2_empty->SetXTitle(tprof2_xtitle.c_str());
        h_calofitting_prof2_empty->SetYTitle(tprof2_ytitle.c_str());
        h_calofitting_prof2_empty->SetTitle(Form("%s %s", calo_string_upper.c_str(), tprof2_titles[i].c_str()));
        h_calofitting_prof2_empty->GetXaxis()->SetNdivisions(505);
        h_calofitting_prof2_empty->DrawCopy("COLZ");
        myText(0.52, 0.6, kRed, Form("%s%s_%s", histprefix, calo_string_lower.c_str(), tprof2_names[i].c_str()), 0.03);
        myText(0.52, 0.55, kRed, "Not found", 0.03);

    }
    myText(0.5, 0.93, kBlack, Form("%s %s", calo_string_upper.c_str(), tprof2_titles[i].c_str()), 0.04);
  }

  std::vector< std::string > th2_names = {"zs_frac_vs_multiplicity"};
  std::vector< std::string > th2_titles = {"ZS Fraction vs Multiplicity"};
  std::string  th2_xtitle = "Multiplicity";
  std::string  th2_ytitle = "ZS Fraction";

  for (size_t i = 0; i < th2_names.size(); ++i) {
    TH2F * h_calofitting_th2 = dynamic_cast<TH2F *>(cl->getHisto(histprefix + calo_string_lower + std::string("_") + th2_names[i]));
    Pad[num][i + tprof2_names.size()]->cd();
    gPad->UseCurrentStyle();
    gPad->SetLeftMargin(0.15);
    gPad->SetBottomMargin(0.15);
    gPad->SetTopMargin(0.1);
    gPad->SetRightMargin(0.15);
    if ( h_calofitting_th2 ) {
      h_calofitting_th2->SetTitle(Form("%s %s", calo_string_upper.c_str(), th2_titles[i].c_str()));
      h_calofitting_th2->SetXTitle(th2_xtitle.c_str());
      h_calofitting_th2->SetYTitle(th2_ytitle.c_str());
      h_calofitting_th2->DrawCopy("COLZ");
      gPad->SetLogx();
      gPad->SetLogz();
    } else {
      std::cout << "CaloFittingDraw::DrawCalo: Histogram " << Form("%s%s_%s", histprefix, calo_string_lower.c_str(), th2_names[i].c_str()) << " not found!" << std::endl;
      TH2F * h_calofitting_th2_empty = new TH2F(Form("%s%s_%s", histprefix, calo_string_lower.c_str(), th2_names[i].c_str()), "Empty Histogram", 1, 0, 1, 1, 0, 1);
      h_calofitting_th2_empty->SetXTitle(th2_xtitle.c_str());
      h_calofitting_th2_empty->SetYTitle(th2_ytitle.c_str());
      h_calofitting_th2_empty->SetTitle(Form("%s %s", calo_string_upper.c_str(), th2_titles[i].c_str()));
      h_calofitting_th2_empty->GetXaxis()->SetNdivisions(505);
      h_calofitting_th2_empty->DrawCopy("COLZ");
      myText(0.52, 0.6, kRed, Form("%s%s_%s", histprefix, calo_string_lower.c_str(), th2_names[i].c_str()), 0.03);
      myText(0.52, 0.55, kRed, "Not found", 0.03);
    }
    myText(0.5, 0.93, kBlack, Form("%s %s", calo_string_upper.c_str(), th2_titles[i].c_str()), 0.04);
  }

  TText PrintRun;
  PrintRun.SetTextFont(42);
  PrintRun.SetTextSize(0.04);
  PrintRun.SetNDC();          // set to normalized coordinates
  PrintRun.SetTextAlign(23);  // center/top alignment
  std::ostringstream runnostream1;
  std::string runstring1;
  runnostream1 << Name() << " " << calo_string_upper << " ZS " << cl->RunNumber();
  runstring1 = runnostream1.str();
  transparent[num]->cd();
  PrintRun.DrawText(0.5, 1., runstring1.c_str());
 
  TC[num]->Update();


  return 0;
}

std::vector< int > CaloFittingDraw::FilterPackets( CaloFittingDraw::PacketStatus status ) const
{
  std::vector< int > filtered_packets {};
  for ( const auto &packet : packet_status_map ) {
    if ( packet.second == status ) {
      filtered_packets.push_back(packet.first);
    }
  }
  return filtered_packets;
}

bool CaloFittingDraw::gl1daq_dropped() const { 
    auto good = FilterPackets(CaloFittingDraw::PacketStatus::GOOD);
    auto events = std::count_if(packet_events_map.begin(), packet_events_map.end(),
      [](const std::pair<int, int>& p) { return p.second > 0; });
    return ( good.size() == 0 && events > 0 );
}

int CaloFittingDraw::GetPacketInfo()
{

  QADrawClient *cl = QADrawClient::instance();

  n_events_db = cl->EventsInRun();
  run_number = cl->RunNumber();
  run_time = cl->RunTime();
  n_events = 0;

  time_t endruntime_t = cl->EndRunUnixTime();
  std::tm *endtime_tm = std::localtime(&endruntime_t);
  char endtime_buf[100];
  std::strftime(endtime_buf, sizeof(endtime_buf), "%Y-%m-%d%H:%M:%S", endtime_tm);
  end_time = endtime_buf;

  time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  std::tm *now_tm = std::localtime(&now);
  char now_buf[100];
  std::strftime(now_buf, sizeof(now_buf), "%Y-%m-%d %H:%M:%S", now_tm);
  std::string current_time(now_buf);  
  qa_time = current_time;

  // init status variables
  packet_status_map.clear();
  packet_events_map.clear();
  for ( int ip = 0; ip < npackets_total; ip++ ) {
    int packet_id = valid_packet_ids[ip];
    packet_status_map[packet_id] = PacketStatus::VOID; // initialize all packets to VOID status
    packet_events_map[packet_id] = -1;
  } 
  
  // get histograms
  TH1 * h_CaloFittingQA_empty_packets = dynamic_cast< TH1 *>(cl->getHisto(histprefix + std::string("empty_packets")));
  TH1 * h_CaloFittingQA_packet_events = dynamic_cast<  TH1 *>(cl->getHisto(histprefix + std::string("packet_events")));
  TH1 * h_CaloFittingQA_missing_packets = dynamic_cast< TH1 *>(cl->getHisto(histprefix + std::string("missing_packets")));

  if ( !h_CaloFittingQA_empty_packets || !h_CaloFittingQA_packet_events )   {  
    std::cout << "CaloFittingDraw::MakeSummary: Histograms not found: " 
              << histprefix + std::string("empty_packets") << " or "
              << histprefix + std::string("packet_events") << std::endl;
    return -1; 
  }

  // loop bins 
  for ( int ip = 0; ip < npackets_total; ip++ ) {

    int bin = h_CaloFittingQA_empty_packets->FindBin(valid_packet_ids[ip]);
    if ( bin < 1 || bin > h_CaloFittingQA_empty_packets->GetNbinsX() ) { continue; } // invalid bin
    int const empty_event_count = h_CaloFittingQA_empty_packets->GetBinContent(bin);
    
    bin = h_CaloFittingQA_packet_events->FindBin(valid_packet_ids[ip]);
    if ( bin < 1 || bin > h_CaloFittingQA_packet_events->GetNbinsX() ) { continue; } // invalid bin
    int const total_event_count = h_CaloFittingQA_packet_events->GetBinContent(bin);

    int const reco_event_count = total_event_count - empty_event_count;

    if ( h_CaloFittingQA_missing_packets ) {
      int missing_bin = h_CaloFittingQA_missing_packets->FindBin(valid_packet_ids[ip]);
      if ( missing_bin > 0 &&  missing_bin < h_CaloFittingQA_missing_packets->GetNbinsX()+1 ) {
        int const is_missing = h_CaloFittingQA_missing_packets->GetBinContent(missing_bin);
        if ( is_missing > 0 )  {
          packet_status_map[valid_packet_ids[ip]] = PacketStatus::MISSING;
        }
      } 
    }

    packet_events_map[valid_packet_ids[ip]] = reco_event_count;
    if ( empty_event_count == total_event_count ) {
      packet_status_map[valid_packet_ids[ip]] = PacketStatus::EMPTY;
    } else if ( reco_event_count == 0 ) {
      packet_status_map[valid_packet_ids[ip]] = PacketStatus::EMPTY;
    } else if ( reco_event_count < total_event_count && reco_event_count > 0 ) {
      packet_status_map[valid_packet_ids[ip]] = PacketStatus::INCOMPLETE;
    } else if ( reco_event_count == total_event_count ) {
      packet_status_map[valid_packet_ids[ip]] = PacketStatus::GOOD;
    }
    if ( total_event_count  > n_events ) {
      n_events = total_event_count;
    }

  } // update all packets

  if ( n_events == 0 ) {
    std::cout << "CaloFittingDraw::GetPacketInfo: No events found in run " << run_number << std::endl;
    return -1;
  }

  return 0;
} 

int CaloFittingDraw::MakeAlignmentSummary( const std::string &name, int num )
{


  TC[num] = new TCanvas(name.c_str(), Form("CaloFittingDraw Summary %s", name.c_str()), -1, 0, canvas_xsize, canvas_ysize);
  TC[num]->UseCurrentStyle();
  gSystem->ProcessEvents();
  Pad[num][0] = new TPad(Form("mypad%d0", num),  "top", 0.02, 0.72, 0.98, 0.98, 0);
  Pad[num][1] = new TPad(Form("mypad%d1", num), "bottom", 0.02, 0.02, 0.98, 0.72, 0);
  Pad[num][0]->Draw();
  Pad[num][1]->Draw();
  transparent[num] = new TPad("transparent1", "this does not show", 0, 0, 1, 1);
  transparent[num]->SetFillStyle(4000);
  transparent[num]->Draw();

  TLegend * leg = new TLegend(0.1, 0.9, 0.9, 0.95);
  leg->SetFillColor(0);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.03);
  leg->SetTextFont(42);
  leg->SetNColumns(5);

  Pad[num][1]->cd();
  gPad->UseCurrentStyle();
  gPad->SetLeftMargin(0.15);
  gPad->SetBottomMargin(0.2);  // More space for labels
  gPad->SetTopMargin(0.1);
  gPad->SetRightMargin(0.05);

  TGraphErrors * h_CaloFitting_eventFrac[4];
  std::vector< std::string > h_CaloFitting_eventFrac_names = {
    "goodSebs", 
    "incompleteSebs",
    "emptySebs",
    "missingSebs"
  };
  std::vector< std::string  > leg_labs = {
    "Complete Packets",
    "Incomplete Packets",
    "Empty Packets",
    "Missing Packets"
  };

  const int colors[4] = { kGreen, kOrange, kRed, kBlue };
  std::vector<int> valid_packet_vec{};
  for (unsigned int i = 0; i < npackets_total; ++i) {
    valid_packet_vec.push_back(valid_packet_ids[i]);
  }


  std::sort(valid_packet_vec.begin(), valid_packet_vec.end());
  for (int i = 0; i < 4; ++i) {

    h_CaloFitting_eventFrac[i] = new TGraphErrors(npackets_total);
    h_CaloFitting_eventFrac[i]->SetName(Form("%s%s", histprefix, h_CaloFitting_eventFrac_names[i].c_str()));
    h_CaloFitting_eventFrac[i]->SetMarkerStyle(20);
    h_CaloFitting_eventFrac[i]->SetMarkerColor(colors[i]);
    h_CaloFitting_eventFrac[i]->SetLineColor(colors[i]);
    h_CaloFitting_eventFrac[i]->SetLineWidth(2);

    
    for (int j = 0; j < npackets_total; ++j) {
      h_CaloFitting_eventFrac[i]->SetPoint(j, j, -1.0); // initialize with -1 (unset)
      h_CaloFitting_eventFrac[i]->SetPointError(j, 0, 0);
      int packet_id = valid_packet_vec[j];
      if ( packet_status_map[packet_id] == static_cast<PacketStatus>(i) ) {
        float y = packet_events_map[packet_id] / static_cast<float>(n_events);
        h_CaloFitting_eventFrac[i]->SetPoint(j, j, y);
      }
    }
    h_CaloFitting_eventFrac[i]->GetXaxis()->SetLimits(-0.5, npackets_total - 0.5);
    h_CaloFitting_eventFrac[i]->GetYaxis()->SetRangeUser(-0.05, 1.05);
    h_CaloFitting_eventFrac[i]->GetYaxis()->SetTitle("Event Efficiency");
    h_CaloFitting_eventFrac[i]->GetYaxis()->SetTitleSize(0.04);
    h_CaloFitting_eventFrac[i]->GetYaxis()->SetTitleOffset(0.7);

    h_CaloFitting_eventFrac[i]->GetXaxis()->SetNdivisions(505);
    h_CaloFitting_eventFrac[i]->GetXaxis()->SetTickSize(0.0);
    h_CaloFitting_eventFrac[i]->GetXaxis()->SetLabelSize(0.0);

    h_CaloFitting_eventFrac[i]->GetXaxis()->SetTitleSize(0.04);
    h_CaloFitting_eventFrac[i]->GetXaxis()->SetTitleOffset(1.9);
    h_CaloFitting_eventFrac[i]->GetXaxis()->SetTitle("Packet ID");

    auto vec = FilterPackets(static_cast<CaloFittingDraw::PacketStatus>(i));
    int num_packets = vec.size();

    Pad[num][1]->cd();
    if (i == 0) {
      h_CaloFitting_eventFrac[i]->Draw("AP");
    } else {
      h_CaloFitting_eventFrac[i]->Draw("P SAME");
    }
    leg->AddEntry(h_CaloFitting_eventFrac[i], Form("%s : %d" , leg_labs[i].c_str(), num_packets), "p");
    gPad->Update();
  }

  Pad[num][1]->cd();
  if (gl1daq_dropped()) {
    int max_event = std::max_element(packet_events_map.begin(), packet_events_map.end(),
      [](const std::pair<int, int>& a, const std::pair<int, int>& b) { return a.second < b.second; })->second;
    
    TLine * gl1_dropped_line = new TLine(-0.5, static_cast<float>(max_event) / static_cast<float>(n_events), 
                                          npackets_total - 0.5, static_cast<float>(max_event) / static_cast<float>(n_events));
    gl1_dropped_line->SetLineColor(kMagenta);
    gl1_dropped_line->SetLineStyle(2);
    gl1_dropped_line->SetLineWidth(2);
    gl1_dropped_line->Draw("SAME");
    leg->AddEntry(gl1_dropped_line, Form("GL1 DAQ dropped at %d events", max_event), "l");

  }
  leg->Draw("SAME");
  gPad->Update();

  double const axisY = gPad->GetUymin();
  double const axisXmin = -0.5;
  double const axisXmax = npackets_total - 0.5;
  TGaxis *xaxis = new TGaxis(axisXmin, axisY, axisXmax, axisY, axisXmin, axisXmax, npackets_total, "-");
  xaxis->SetTickSize(0);
  xaxis->SetLabelSize(0.0);
  xaxis->Draw("same");

  // Draw vertical TLatex labels
  TLatex * latex2 = new TLatex();
  latex2->SetTextAlign(22);  // center both horizontally and vertically
  latex2->SetTextSize(0.04);
  latex2->SetTextFont(42);
  latex2->SetTextAngle(50);  // vertical
  double labelY = axisY - 0.07 * (gPad->GetY2() - gPad->GetY1());  // offset slightly below axis
  for (int i = 0; i < npackets_total; ++i) {
      if (i % 5 != 0) { continue; } // Skip every other label
      double x = i;
      latex2->DrawLatex(x, labelY, Form("%d", valid_packet_vec[i]));
  }
  gPad->Update();


  Pad[num][0]->cd();
  gPad->UseCurrentStyle();
  gPad->SetLeftMargin(0.1);
  gPad->SetBottomMargin(0.05);  // More space for labels
  gPad->SetTopMargin(0.05);
  gPad->SetRightMargin(0.1);

  double const yy = 0.13;
  double ya = 0.85;
  latex2->SetTextSize(yy);
  latex2->SetTextFont(42);
  latex2->SetTextAlign(22);  // center both horizontally and vertically
  latex2->SetTextColor(kBlack);
  latex2->SetTextAngle(0);  // horizontal text
  latex2->DrawLatex(0.5, ya, Form("Run %d,  %s", run_number, run_time.c_str()));
  int max_reco = std::max_element(packet_events_map.begin(), packet_events_map.end(),
      [](const std::pair<int, int>& a, const std::pair<int, int>& b) { return a.second < b.second; })->second;
    
  std::string gl1status;
  std::string runstatus;
  if (gl1daq_dropped()) {
    gl1status = "Dropped";
    runstatus = "Bad";
  } else {
    gl1status = "OK";
  }
  int nmissing = FilterPackets(PacketStatus::MISSING).size();
  int nincomplete = FilterPackets(PacketStatus::INCOMPLETE).size();
  int nempty = FilterPackets(PacketStatus::EMPTY).size();
  if (nmissing > 0 ){
    runstatus = "Bad"; 
  } 
  else if (nincomplete > 0 || nempty > 0) {
    runstatus = "Incomplete" + std::string(" (") + std::to_string(nincomplete) + std::string(" incomplete, ") + std::to_string(nempty) + std::string(" empty packets)");
  } else {
    runstatus = "Good";
  }
  ya-=yy;
  if (runstatus == "Good") {
    latex2->SetTextColor(kGreen);
  } else if (runstatus.find("Incomplete") != std::string::npos) {
    latex2->SetTextColor(kOrange);
  } else {
    latex2->SetTextColor(kRed);
  }
  latex2->SetTextFont(62);
  latex2->DrawLatex(0.5, ya, Form("Run Status: %s", runstatus.c_str()));
  latex2->SetTextColor(kBlack);
  
  if (gl1daq_dropped()) {
    latex2->SetTextColor(kMagenta);
    latex2->SetTextFont(62);
  } else {
    latex2->SetTextColor(kBlack);
    latex2->SetTextFont(42);
  }

  latex2->DrawLatex(0.3, ya-yy, Form("GL1 DAQ Status: %s", gl1status.c_str()));
  latex2->SetTextColor(kBlack);
  latex2->DrawLatex(0.6, ya-yy, Form("Reconstructed Events: %d/%d ( %.1f%%)", max_reco, n_events,
      static_cast<float>(max_reco) / static_cast<float>(n_events) * 100.0));
  ya-=yy;
  
  latex2->SetTextFont(42);
  latex2->SetTextAlign(22);  // center both horizontally and vertically 
  if( FilterPackets(PacketStatus::GOOD).size() > 0 ) {
    latex2->SetTextColor(kBlack);
  } else {
    latex2->SetTextColor(kRed);
  }
  latex2->DrawLatex(0.5, ya-yy, Form("Complete Packets: %zu /144", FilterPackets(PacketStatus::GOOD).size()));

  if ( FilterPackets(PacketStatus::INCOMPLETE).size() > 0 ) {
    latex2->SetTextColor(kRed);
  } else {
    latex2->SetTextColor(kBlack);
  }
 ya-=yy;
  latex2->DrawLatex(0.5, ya-yy, Form("Incomplete Packets: %zu /144", FilterPackets(PacketStatus::INCOMPLETE).size()));
  latex2->SetTextColor(kRed);
  
  if ( FilterPackets(PacketStatus::EMPTY).size() > 0 ) {
    latex2->SetTextColor(kRed);
  } else {
    latex2->SetTextColor(kBlack);
  }
  ya-=yy;
  latex2->SetTextFont(42);
  latex2->DrawLatex(0.5, ya-yy, Form("Empty Packets: %zu / 144", FilterPackets(PacketStatus::EMPTY).size()));
  if ( FilterPackets(PacketStatus::MISSING).size() > 0 ) {
    latex2->SetTextColor(kRed);
  } else {
    latex2->SetTextColor(kBlack);
  }
  ya-=yy;
  latex2->DrawLatex(0.5, ya-yy, Form("Missing Packets: %zu /144", FilterPackets(PacketStatus::MISSING).size()));
  latex2->SetTextColor(kBlack);
  ya-=yy;
  gPad->Update();

  TText PrintRun;
  PrintRun.SetTextFont(42);
  PrintRun.SetTextSize(0.04);
  PrintRun.SetNDC();          // set to normalized coordinates
  PrintRun.SetTextAlign(23);  // center/top alignment
  std::ostringstream runnostream1;
  std::string runstring1;
  runnostream1 << Name() << " : Packet Summary Run " << run_number;
  runstring1 = runnostream1.str();
  transparent[num]->cd();
  PrintRun.DrawText(0.5, 1., runstring1.c_str());
  TC[num]->Update();

  return 0;

}

int CaloFittingDraw::MakeDets( const std::string &name, int num )
{

  TC[num] = new TCanvas(name.c_str(), Form("CaloFittingDraw Summary %s", name.c_str()), -1, 0, canvas_xsize, canvas_ysize);
  TC[num]->UseCurrentStyle();
  gSystem->ProcessEvents();
  Pad[num][0] = new TPad(Form("mypad%d0", num),  "top", 0.02, 0.02, 0.98, 0.98, 0);
  Pad[num][0]->Draw();
  transparent[num] = new TPad("transparent1", "this does not show", 0, 0, 1, 1);
  transparent[num]->SetFillStyle(4000);
  transparent[num]->Draw();
 
  const double dx_margin = 0.05;
  const double dy_margin = 0.02;
  const double xpad = (0.98 - 0.02) - 2 * dx_margin; // total width of the pad
  const double ypad = (0.98 - 0.02) - 2 * dy_margin; // total height of the pad


  Pad[num][0]->cd();
  gPad->UseCurrentStyle();
  gPad->SetLeftMargin(dx_margin);  // More space for labels
  gPad->SetBottomMargin(dy_margin);  // More space for labels
  gPad->SetTopMargin(dy_margin);
  gPad->SetRightMargin(dx_margin);

  double const dy = 0.03; // height of each row
  double const dx_min = 0.1 + dx_margin; // minimum width of each column
  double yi = 1.0 - 0.02 - dy_margin; // start from the top of the pad
  const int nrows_max = int( int( ypad*100) / int(dy*100) ); // number of rows that fit in the pad height
  const int ncols_max = int( int(xpad*100) / int(dx_min*100) ); // number of columns that fit in the pad width

  auto incomplete_packets = FilterPackets(CaloFittingDraw::PacketStatus::INCOMPLETE);
  auto missing_packets = FilterPackets(CaloFittingDraw::PacketStatus::MISSING);
  auto empty_packets = FilterPackets(CaloFittingDraw::PacketStatus::EMPTY);
  int nincomplete = incomplete_packets.size();
  int nmissing = missing_packets.size();
  int nempty = empty_packets.size();

  int ncols = 0;
  if (nincomplete % nrows_max !=0 ){
    ncols += (nincomplete / nrows_max) + 1; // add an extra column if not evenly divisible
  } else {
    ncols += nincomplete / nrows_max;
  } 
  if (nmissing % nrows_max !=0 ){
    ncols += (nmissing / nrows_max) + 1; // add an extra column if not evenly divisible
  } else {
    ncols += nmissing / nrows_max;
  }
  if (nempty % nrows_max !=0 ){
    ncols += (nempty / nrows_max) + 1; // add an extra column if not evenly divisible
  } else {
    ncols += nempty / nrows_max;
  }
  if (ncols > ncols_max) {
    ncols = ncols_max; // limit to maximum number of columns that fit in the pad
  }
  double dx = xpad / ncols; // width of each column based on the number of columns
  if (dx < dx_min) {
    dx = dx_min; // ensure minimum width
    ncols = int(xpad / dx_min); // recalculate number of columns based on minimum width
  }
  
  const int width = 11;
  const int  bufsize = 128;  // safely covers 3 fields + commas + newline
  char buffer[bufsize];
  std::snprintf(buffer, bufsize, "%*s,%*s,%*s\n",
                  width, "Packet ID", width, "Drop Event", width, "Run %");

  auto thisline = [&](int packet_id, int final_event, float percent) -> std::string {
    char row[bufsize];
    std::snprintf(row, bufsize, "%*d,%*d,%*.1f\n",
                  width, packet_id,
                  width, final_event,
                  width, percent);
    return std::string(row);
  };

  TLatex * tex = new TLatex();
  tex->SetTextFont(42);
  tex->SetTextSize(0.02);
  tex->SetNDC();          // set to normalized coordinates
  tex->SetTextAlign(22);  // right-align horizontally, center vertically

  for (int i = 0; i < ncols; ++i) {
    // draw column header at the top of each column
    double const x1 = dx_margin + i * dx;
    double const x2 = dx_margin + (i + 1) * dx;
    double const y1 = yi - dy; // start from the top of the pad
    double const y2 = yi;
    std::string header = buffer; // use the buffer for the header
    tex->SetTextColor(kBlack);
    tex->DrawLatex((x1 + x2) / 2.0, (y1 + y2) / 2.0, header.c_str());
    
  }

  // now fill the columns with packet information
  
  // bool gl1daq_dropped = gl1daq_dropped();
  std::vector< int > incomplete_packets_gl1tagged {}; 
  int max_reco = std::max_element(packet_events_map.begin(), packet_events_map.end(),
      [](const std::pair<int, int>& a, const std::pair<int, int>& b) { return a.second < b.second; })->second;
    

  if (gl1daq_dropped()) {
    // Find the maximum dropped event count
    std::vector<int> incomplete_packets_copy{};
    for (const auto &packet_id : incomplete_packets) {
      incomplete_packets_copy.push_back(packet_id);
    }
    for (const auto &packet_id : incomplete_packets_copy) {
      if (packet_events_map[packet_id] == max_reco) {
        incomplete_packets_gl1tagged.push_back(packet_id);
        // Remove the packet from the incomplete_packets vector
        incomplete_packets.erase(std::remove(incomplete_packets.begin(), incomplete_packets.end(), packet_id), incomplete_packets.end());
      }
    }
    // Remove the gl1daq_dropped packets from the incomplete_packets vector

  }

  // sort all 
  std::sort(incomplete_packets.begin(), incomplete_packets.end());
  std::sort(incomplete_packets_gl1tagged.begin(), incomplete_packets_gl1tagged.end());
  std::sort(missing_packets.begin(), missing_packets.end());
  std::sort(empty_packets.begin(), empty_packets.end());
  


  int col = 0;
  int row = 0;
  for (const auto &packet_id : incomplete_packets_gl1tagged) {
    if (row >= nrows_max) {
      row = 0; // reset row for next column
      col++; // move to next column
    }
    if (col >= ncols) {
      break; // no more columns available
    }
    double x1 = dx_margin + col * dx;
    double x2 = dx_margin + (col + 1) * dx;
    double y1 = yi - row * dy - dy; // adjust for row height
    double y2 = yi - row * dy;
    row++; // increment row for next packet
    
    auto packetline = thisline(packet_id, packet_events_map[packet_id], 
        static_cast<float>(packet_events_map[packet_id]) / static_cast<float>(n_events) * 100.0);
    tex->SetTextColor(kMagenta);
    tex->DrawLatex((x1 + x2) / 2.0, (y1 + y2) / 2.0, packetline.c_str());
  }
  for (const auto &packet_id : incomplete_packets) {
    if (row >= nrows_max) {
      row = 0; // reset row for next column
      col++; // move to next column
    }
    if (col >= ncols) {
      break; // no more columns available
    }
    double x1 = dx_margin + col * dx;
    double x2 = dx_margin + (col + 1) * dx;
    double y1 = yi - row * dy - dy; // adjust for row height
    double y2 = yi - row * dy;
    row++; // increment row for next packet
    
    auto packetline = thisline(packet_id, packet_events_map[packet_id], 
        static_cast<float>(packet_events_map[packet_id]) / static_cast<float>(n_events) * 100.0);
    tex->SetTextColor(kOrange);
    tex->DrawLatex((x1 + x2) / 2.0, (y1 + y2) / 2.0, packetline.c_str());
  }
  col++; // increment column for next packet
  row = 0; // reset row for next column
  for (const auto &packet_id : empty_packets) {
    if (row >= nrows_max) {
      row = 0; // reset row for next column
      col++; // move to next column
    }
    if (col >= ncols) {
      break; // no more columns available
    }
    double x1 = dx_margin + col * dx;
    double x2 = dx_margin + (col + 1) * dx;
    double y1 = yi - row * dy - dy; // adjust for row height
    double y2 = yi - row * dy;
    row++; // increment row for next packet
    
    auto packetline = thisline(packet_id, packet_events_map[packet_id], 
        static_cast<float>(packet_events_map[packet_id]) / static_cast<float>(n_events) * 100.0);
    tex->SetTextColor(kRed);
    tex->DrawLatex((x1 + x2) / 2.0, (y1 + y2) / 2.0, packetline.c_str());
  
  }
  col++; // increment column for next packet
  row = 0; // reset row for next column
  for (const auto &packet_id : missing_packets) {
    if (row >= nrows_max) {
      row = 0; // reset row for next column
      col++; // move to next column
    }
    if (col >= ncols) {
      break; // no more columns available
    }
    double x1 = dx_margin + col * dx;
    double x2 = dx_margin + (col + 1) * dx;
    double y1 = yi - row * dy - dy; // adjust for row height
    double y2 = yi - row * dy;
    row++; // increment row for next packet
    
    auto packetline = thisline(packet_id, packet_events_map[packet_id],
        static_cast<float>(packet_events_map[packet_id]) / static_cast<float>(n_events) * 100.0);
    tex->SetTextColor(kBlue);
    tex->DrawLatex((x1 + x2) / 2.0,
                  (y1 + y2) / 2.0, packetline.c_str());
  }

  // draw legend above column headers
  // double legend_x1 = dx_margin;
  // double legend_x2 = dx_margin + 1* dx; // width of the legend box
  // double legend_y1 = yi + dy; // position legend above the last row
  // double legend_y2 = yi + 2*dy; // height of the

  // if (gl1daq_dropped()) {
  //   tex->SetTextColor(kMagenta);
  //   tex->DrawLatex((legend_x1 + legend_x2) / 2.0, (legend_y1 + legend_y2) / 2.0, "GL1 DAQ Dropped");
  //   legend_x2 += dx; // move legend to the right 
  //   legend_x1 += dx; // reset legend x1 to the new position
  // }
  // if (incomplete_packets.size() > 0) {
  //   tex->SetTextColor(kOrange);
  //   tex->DrawLatex((legend_x1 + legend_x2) / 2.0, (legend_y1 + legend_y2) / 2.0, "Incomplete Packets");
  //   legend_x2 += dx; // move legend to the right 
  //   legend_x1 += dx; // reset legend x1 to the new position
  // }
  // if (empty_packets.size() > 0) {
  //   tex->SetTextColor(kRed);
  //   tex->DrawLatex((legend_x1 + legend_x2) / 2.0, (legend_y1 + legend_y2) / 2.0, "Empty Packets");
  //   legend_x2 += dx; // move legend to the righ
  //   legend_x1 += dx; // reset legend x1 to the new position
  // }
  // if (missing_packets.size() > 0) {
  //   tex->SetTextColor(kBlue);
  //   tex->DrawLatex((legend_x1 + legend_x2) / 2.0, (legend_y1 + legend_y2) / 2.0, "Missing Packets");
  //   legend_x2 += dx; // move legend to the right
  //   legend_x1 += dx; // reset legend x1 to the new position
  // }
  // TLine * legend_line = new TLine(legend_x1, legend_y2-0.01, legend_x2, legend_y1+0.01);
  // legend_line->SetLineColor(kBlack);
  // legend_line->SetLineWidth(2);
  // legend_line->Draw("SAME");
  gPad->Update();

  
  TText PrintRun;
  PrintRun.SetTextFont(42);
  PrintRun.SetTextSize(0.04);
  PrintRun.SetNDC();          // set to normalized coordinates
  PrintRun.SetTextAlign(23);  // center/top alignment
  std::ostringstream runnostream1;
  std::string runstring1;
  runnostream1 << Name() << " : Packet Details Run " << run_number;
  runstring1 = runnostream1.str();
  transparent[num]->cd();
  PrintRun.DrawText(0.5, 1., runstring1.c_str());
  TC[num]->Update();

  return 0;

}

int CaloFittingDraw::MakeHtml( const std::string &what )
{
    std::cout << "CaloFittingDraw::MakeHtml: Making HTML for " << what << std::endl;
    int iret = Draw(what);
    std::cout << "CaloFittingDraw::MakeHtml: Draw returned " << iret << std::endl;
    if (iret) {  return iret; }

    std::cout << "CaloFittingDraw::MakeHtml: Registering HTML pages" << std::endl;
    QADrawClient *cl = QADrawClient::instance();
    std::cout << "CaloFittingDraw::MakeHtml: Registering summary pages" << std::endl;

    std::string pngfile = cl->htmlRegisterPage(*this, "ZS/EMCal", "cemc1", "png");
    cl->CanvasToPng(TC[0], pngfile);
    pngfile = cl->htmlRegisterPage(*this, "ZS/IHCal", "ihcal1", "png");
    cl->CanvasToPng(TC[1], pngfile);
    pngfile = cl->htmlRegisterPage(*this, "ZS/OHCal", "ohcal1", "png");
    cl->CanvasToPng(TC[2], pngfile);
    pngfile = cl->htmlRegisterPage(*this, "Alignment/Summary", "packets1", "png");
    cl->CanvasToPng(TC[3], pngfile);
    pngfile = cl->htmlRegisterPage(*this, "Alignment/Details", "packets2", "png");
    cl->CanvasToPng(TC[4], pngfile);
    return 0;
}

// int CaloFittingDraw::DBVarInit()
// {
//   /* db = new QADrawDB(this); */
//   /* db->DBInit(); */
//   return 0;
// }

void CaloFittingDraw::myText( double x, double y, int color, const char *text, double tsize )
{
  TLatex l;
  l.SetTextAlign(22);
  l.SetTextSize(tsize);
  l.SetNDC();
  l.SetTextColor(color);
  l.DrawLatex(x, y, text);
}
