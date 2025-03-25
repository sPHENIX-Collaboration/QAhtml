#ifndef CALO_CALODRAW_H
#define CALO_CALODRAW_H

#include <qahtml/QADraw.h>

#include <vector>
#include <string>

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
  void SetihcalSummary(TCanvas* c);  // Declaration for ihcal summary setter
  void SetohcalSummary(TCanvas* c);  // Declaration for ohcal summary setter
  void SetCaloChecker(CaloGoodRunChecker* ch) {calo_checker = ch;}

  // New function to set reference file path
  //  void SetReferenceFile(const std::string &filepath) { refFilePath = filepath; }

 private:
  int MakeCanvas(const std::string &name, int num);
  int DrawCemc();
  int DrawIhcal();
  int DrawOhcal();
  int DrawZdcMbd();
  int DrawCorr();
  TH1 *proj(TH2 *h2);
  TH1 *projRef(TH2 *h2);  // New function for reference projection
  TH1 *FBratio(TH1 *h);
  void myText(double x, double y, int color, const char *text, double tsize = 0.04);
  //  QADB *db {nullptr};
  const static int ncanvases = 14;
  const static int maxpads = 6;
  TCanvas *TC[ncanvases]{};
  TPad *transparent[ncanvases]{};
  TPad *Pad[ncanvases][maxpads]{};
  TCanvas* cemcSummary = nullptr;
  TCanvas* ihcalSummary = nullptr;
  TCanvas* ohcalSummary = nullptr;

  int canvas_xsize = 1600;
  int canvas_ysize = 800;

  const char *histprefix;
  // for good run determination
  CaloGoodRunChecker* calo_checker = nullptr;

  // New member variable to store reference file path
  //std::string refFilePath = "/sphenix/data/data02/sphnxpro/QAhtml/aggregated/HIST_CALO_run2pp_new_2024p006-00049125-9000.root";

};

#endif
