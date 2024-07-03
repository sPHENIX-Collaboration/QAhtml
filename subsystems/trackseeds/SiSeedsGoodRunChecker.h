#ifndef SILICONSEEDGOODRUNCHECKER_H
#define SILICONSEEDGOODRUNCHECKER_H

#include <string>

class TCanvas;
class TPad;
class TH1;
class TH2;

class SiSeedsGoodRunChecker
{
 public:
  SiSeedsGoodRunChecker() {}
  ~SiSeedsGoodRunChecker() {}

  TCanvas* SiSeedsMakeSummary(bool siseeds_goodrun=false);
  void myText(double x, double y, int color, const char *text, double tsize = 0.04);
};

#endif
