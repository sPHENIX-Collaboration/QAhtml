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

class TpcSeedsDraw : public QADraw
{
 public: 
  TpcSeedsDraw(const std::string &name = "TPCSEEDSQA");
  ~TpcSeedsDraw() override;

  int Draw(const std::string &what = "ALL") override;
  int MakeHtml(const std::string &what = "ALL") override;
  int DBVarInit();

 private:
  int MakeCanvas(const std::string &name, int num);
  int DrawTrackletInfo();
  int DrawClusterInfo1();
  int DrawClusterInfo2();
  int DrawDCAInfo1();
  int DrawDCAInfo2();
  int DrawVertexInfo();
  TCanvas *TC[6]{};
  TPad *transparent[6]{};
  TPad *Pad[6][26]{};
  const char *histprefix;
};

#endif
