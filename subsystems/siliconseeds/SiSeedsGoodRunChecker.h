#ifndef SILICONSEEDGOODRUNCHECKER_H
#define SILICONSEEDGOODRUNCHECKER_H

#include <string>

class TCanvas;
class TPad;
class TH1;
class TH1D;
class TH2;
class TProfile2D;
class TString;
class TFile;
class TStyle;

class SiSeedsGoodRunChecker
{
 public: 
  SiSeedsGoodRunChecker() {}
  ~SiSeedsGoodRunChecker() {}

  void SetKSTestSummary(TH1D *h) {h_KSSummary = h;}
  bool SiSeedsGoodRun();
  TCanvas* SiSeedsMakeSummary(int run, bool siseeds_goodrun=false);
  void myText(double x, double y, int color, const char *text, double tsize = 0.04);

  TH1D *h_KSSummary = nullptr;
};

#endif
