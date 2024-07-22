#ifndef SILICONSEEDSDRAW_H__
#define SILICONSEEDSDRAW_H__

#include <qahtml/QADraw.h>

#include <vector>

class QADB;
class QADBVar;
class TCanvas;
class TGraphErrors;
class TPad;
class TH1F;
class TH2F;
class TProfile2D;
class TStyle;

class SiliconSeedsDraw : public QADraw
{
 public: 
  SiliconSeedsDraw(const std::string &name = "SILICONSEEDSQA");
  ~SiliconSeedsDraw() override;

  int Draw(const std::string &what = "ALL") override;
  int MakeHtml(const std::string &what = "ALL") override;
  int DBVarInit();
  void SetSiSeedsSummary(int run, TCanvas* c);

 private:
  int MakeCanvas(const std::string &name, int num);
  int DrawTrackBasicInfo();
  int DrawTrackDCAInfo();
  int DrawTrackQualityInfo();
  int DrawTrackChargeInfo();
  int DrawVertexInfo();
  int DrawVertexQualityInfo();
  TCanvas *TC[6]{};
  TPad *transparent[6]{};
  TPad *Pad[6][6]{};
  const char *histprefix;
  // Summary
  TCanvas* siseedsSummary = nullptr;
};

#endif
