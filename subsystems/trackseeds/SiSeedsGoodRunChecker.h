#ifndef SILICONSEEDGOODRUNCHECKER_H
#define SILICONSEEDGOODRUNCHECKER_H

#include <string>

class TCanvas;
class TPad;
class TH1;
class TH2;
class TProfile2D;
class TString;
class TFile;

class SiSeedsGoodRunChecker
{
 public:
  SiSeedsGoodRunChecker() {}
  ~SiSeedsGoodRunChecker() {}

  void SetRefRunNumber(int runnum) {refrunnumber = runnum;}
  void SetRefProdTag(std::string tag) {prodtag = tag;}
  void SetHistfile(std::string hfile) {histfile = hfile;}
  bool SiSeedsGoodRun();
  TCanvas* SiSeedsMakeSummary(bool siseeds_goodrun=false);
  void myText(double x, double y, int color, const char *text, double tsize = 0.04);

  int refrunnumber = 47332; // For development purposes
  std::string prodtag = "2024p004"; // For development purposes
  std::string refhistbasedir = "/sphenix/data/data02/sphnxpro/QAhtml/aggregated"; // For development purposes
  TFile *reffile = nullptr;
  std::string histprefix = "h_SiliconSeedsQA_";

  std::string histfile = "";
  TFile *checkfile = nullptr;

  // Reference histogram for KS test
  TH1 *refh_ntrack1d = nullptr;
  TH2 *refh_ntrack = nullptr;
  TH2 *refh_nmaps_nintt = nullptr;
  TProfile2D *refh_avgnclus_eta_phi = nullptr;
  TH1 *refh_trackcrossing = nullptr;
  TH1 *refh_trackchi2ndf = nullptr;
  TH2 *refh_dcaxyorigin_phi = nullptr;
  TH2 *refh_dcaxyvtx_phi = nullptr;
  TH2 *refh_dcazorigin_phi = nullptr;
  TH2 *refh_dcazvtx_phi = nullptr;
  TH1 *refh_ntrack_isfromvtx = nullptr;
  TH1 *refh_trackpt_inclusive = nullptr;
  TH1 *refh_trackpt_pos = nullptr;
  TH1 *refh_trackpt_neg = nullptr;
  TH1 *refh_ntrack_IsPosCharge = nullptr;
  TH1 *refh_nvertex = nullptr;
  TH1 *refh_vz = nullptr;
  TH2 *refh_vx_vy = nullptr;
  TH1 *refh_vcrossing = nullptr;
  TH1 *refh_vchi2dof = nullptr;
  TH1 *refh_ntrackpervertex = nullptr;

  // Input histograms
  TH1 *h_ntrack1d = nullptr;
  TH2 *h_ntrack = nullptr;
  TH2 *h_nmaps_nintt = nullptr;
  TProfile2D *h_avgnclus_eta_phi = nullptr;
  TH1 *h_trackcrossing = nullptr;
  TH1 *h_trackchi2ndf = nullptr;

  // KS test score summary
  TH1 *h_KSSummary = nullptr;
};

#endif
