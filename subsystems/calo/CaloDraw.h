#ifndef CALO_CALODRAW_H
#define CALO_CALODRAW_H

#include <qahtml/QADraw.h>

#include <vector>

class QADB;
class QADBVar;
class TCanvas;
class TGraphErrors;
class TPad;
class TH1;
class TH2;

class CaloDraw : public QADraw
{
 public:
  CaloDraw(const std::string &name = "CaloQA");
  ~CaloDraw() override;

  int Draw(const std::string &what = "ALL") override;
  int MakeHtml(const std::string &what = "ALL") override;
  int DBVarInit();
  bool CemcGoodRun();

 private:
  int MakeCanvas(const std::string &name, int num);
  int DrawCemc();
  int DrawIhcal();
  int DrawOhcal();
  int DrawZdcMbd();
  int DrawCorr();
  TH1 *proj(TH2 *h2);
  TH1 *FBratio(TH1 *h);
  void myText(double x, double y, int color, const char *text, double tsize = 0.04);
  //  QADB *db {nullptr};
  const static int ncanvases = 8;
  const static int maxpads = 6;
  TCanvas *TC[ncanvases]{};
  TPad *transparent[ncanvases]{};
  TPad *Pad[ncanvases][maxpads]{};
  const char *histprefix;
  // for EMCal good run determination
  int n_events = 0;
  int hot_towers = 999999;
  int cold_towers = 999999;
  int dead_towers = 999999;
  float cemc_time_mean = 999.9;
  float cemc_time_sigma = 999.9;
  float vtxz_mean = 999.9;
  float vtxz_sigma = 999.9;
};

#endif
