#ifndef CALO_CALOGOODRUNCHECKER_H
#define CALO_CALOGOODRUNCHECKER_H

#include <string>
#include <TLatex.h>

class TCanvas;
class TPad;
class TH1;
class TH2;
class CaloGoodRunChecker

{
 public:
  CaloGoodRunChecker() {}
  ~CaloGoodRunChecker() {}

  void SetHistfile(std::string hfile) {histfile = hfile;}
  std::string MakeHotColdDeadMaps();
  void DeleteHotColdDeadMaps();
  void CemcCheckGoodRun();
  std::string CemcGetComments();
  TCanvas* CemcMakeSummary();
  void ihcalCheckGoodRun();
  std::string IhcalGetComments();
  TCanvas* ihcalMakeSummary();
  void ohcalCheckGoodRun();
  std::string OhcalGetComments();
  TCanvas* ohcalMakeSummary();
  void CaloWriteDB(std::string subsystem);
  void myText(double x, double y, int color, const char *text, double tsize = 0.04);


  std::string histfile = "";
  std::string histprefix = "h_CaloValid_";
  std::string mapsfile_prefix = "HotColdDeadMaps_";
  std::string mapsfile = "";
  TH2* cemc_hcdmap = nullptr;
  TH2* ihcal_hcdmap = nullptr;
  TH2* ohcal_hcdmap = nullptr;

  void SetRefHistfile(std::string ref_hfile) {ref_histfile = ref_hfile;}
  std::string MakeHotColdDeadMapsRef();
  std::string ref_histfile = "/sphenix/data/data02/sphnxpro/QAhtml/aggregated/HIST_CALOQA_run2auau_ana462_2024p010_v001-00054909-9000.root";
  std::string ref_histprefix = "h_CaloValid_";
  std::string ref_mapsfile_prefix = "HotColdDeadMaps_";
  std::string ref_mapsfile = "";
  TH2* ref_cemc_hcdmap = nullptr;
  TH2* ref_ihcal_hcdmap = nullptr;
  TH2* ref_ohcal_hcdmap = nullptr;

  int canvas_xsize = 1600;
  int canvas_ysize = 800;

  int n_events= 0;
  bool cemc_isgood = false;
  bool cemc_fails_events = false;
  bool cemc_fails_badtowers = false;
  bool cemc_fails_timing = false;
  bool cemc_fails_vertex = false;
  int cemc_hot_towers = 999999;
  int cemc_cold_towers = 999999;
  int cemc_dead_towers = 999999;
  float cemc_time_mean = 999.9;
  float cemc_time_sigma = 999.9;
  bool ihcal_isgood = false;
  bool ihcal_fails_events = false;
  bool ihcal_fails_badtowers = false;
  bool ihcal_fails_timing = false;
  bool ihcal_fails_vertex = false;
  int ihcal_hot_towers = 999999;
  int ihcal_cold_towers = 999999;
  int ihcal_dead_towers = 999999;
  float ihcal_time_mean = 999.9;
  float ihcal_time_sigma = 999.9;
  bool ohcal_isgood = false;
  bool ohcal_fails_events = false;
  bool ohcal_fails_badtowers = false;
  bool ohcal_fails_timing = false;
  bool ohcal_fails_vertex = false;
  int ohcal_hot_towers = 999999;
  int ohcal_cold_towers = 999999;
  int ohcal_dead_towers = 999999;
  float ohcal_time_mean = 999.9;
  float ohcal_time_sigma = 999.9;
  float vtxz_mean = 999.9;
  float vtxz_sigma = 999.9;

  // Reference tower information for each subsystem
  int ref_cemc_hot_towers = 999999;
  int ref_cemc_cold_towers = 999999;
  int ref_cemc_dead_towers = 999999;
  int ref_ihcal_hot_towers = 999999;
  int ref_ihcal_cold_towers = 999999;
  int ref_ihcal_dead_towers = 999999;
  int ref_ohcal_hot_towers = 999999;
  int ref_ohcal_cold_towers = 999999;
  int ref_ohcal_dead_towers = 999999;
};

#endif
