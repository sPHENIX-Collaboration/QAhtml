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
class CaloGoodRunChecker;

class CaloDraw : public QADraw
{
 public:
  CaloDraw(const std::string &name = "CaloQA");
  ~CaloDraw() override;

  int Draw(const std::string &what = "ALL") override;
  int MakeHtml(const std::string &what = "ALL") override;
  int DBVarInit();
  void SetCemcSummary(TCanvas* c);
  void SetCemcChecker(CaloGoodRunChecker* ch) {cemc_checker = ch;}

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
  TCanvas* cemcSummary = nullptr;
  // add summary canvases for hcal etc later
  const char *histprefix;
  // for EMCal good run determination
  CaloGoodRunChecker* cemc_checker = nullptr;
};

#endif
