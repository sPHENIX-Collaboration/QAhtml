#ifndef MVTX_MVTXDRAW_H__
#define MVTX_MVTXDRAW_H__

#include <qahtml/QADraw.h>

#include <TColor.h>
#include <TLine.h>
#include <TCanvas.h>
#include <TDatime.h>
#include <TGraphErrors.h>
#include <TH1.h>
#include <TH2.h>
#include <TProfile.h>
#include <TPad.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TSystem.h>
#include <TText.h>
#include <TLatex.h>
#include <TLegend.h>
#include <TF1.h>
#include <TFrame.h>

#include <vector>

class QADB;
class QADBVar;
class TCanvas;
class TGraphErrors;
class TPad;
class TH1F;
class TH2F;
class TLatex;
class TF1;
class TColor;

class MVTXDraw : public QADraw
{
  public:
    MVTXDraw(const std::string &name = "MVTXQA");
    ~MVTXDraw() override;

    int Draw(const std::string &what = "ALL") override;
    int MakeHtml(const std::string &what = "ALL") override;
    int DBVarInit();

  private:
    int MakeCanvas(const std::string &name, int num);
    int DrawChipInfo();
    int DrawClusterInfo();
    int DrawSummaryInfo();
    TCanvas *TC[3]{};
    TPad *transparent[3]{};
    TPad *Pad[3][4]{};
    const char *histprefix;

    // good/ok/bad run threshold
    float scaledchi2ndf_good = 0.0015;
    float scaledchi2ndf_bad = 0.0025;
    float probOccupancygt0p003_95percentile = 2.58523e-05; // 95% of the runs have probability of chips with occupancy > 0.003 less than this value
    // Run summary text position
    TLatex *l;
    int textalign = 22;
    float textposx = 0.5;
    float titletextsize = 0.08;
    float stattextsize = 0.055;

    Color_t c_good;
    Color_t c_ok;
    Color_t c_bad;
};

#endif
