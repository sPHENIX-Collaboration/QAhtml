#ifndef CALO_CALOGOODRUNCHECKER_H
#define CALO_CALOGOODRUNCHECKER_H

#include <string>

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
  bool CemcGoodRun();
  TCanvas* CemcMakeSummary(bool cemc_goodrun=false);
  void myText(double x, double y, int color, const char *text, double tsize = 0.04);

  std::string histfile = "";
  std::string histprefix = "h_CaloValid_";
  std::string mapsfile_prefix = "HotColdDeadMaps_";
  std::string mapsfile = "";
  TH2* cemc_hcdmap = nullptr;
  TH2* ihcal_hcdmap = nullptr;
  TH2* ohcal_hcdmap = nullptr;

  int n_events= 0;
  int cemc_hot_towers = 999999;
  int cemc_cold_towers = 999999;
  int cemc_dead_towers = 999999;
  float cemc_time_mean = 999.9;
  float cemc_time_sigma = 999.9;
  int ihcal_hot_towers = 999999;
  int ihcal_cold_towers = 999999;
  int ihcal_dead_towers = 999999;
  float ihcal_time_mean = 999.9;
  float ihcal_time_sigma = 999.9;
  int ohcal_hot_towers = 999999;
  int ohcal_cold_towers = 999999;
  int ohcal_dead_towers = 999999;
  float ohcal_time_mean = 999.9;
  float ohcal_time_sigma = 999.9;
  float vtxz_mean = 999.9;
  float vtxz_sigma = 999.9;
};

#endif
