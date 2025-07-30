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
  int DrawdEdxInfo1();
  int DrawdEdxInfo2();
  int Drawsummary();
  TCanvas *TC[9]{};
  TPad *transparent[9]{};
  TPad *Pad[9][26]{};
  const char *histprefix;
  double xpos_z0 = -1;
  double xpos_z4 = -1;
  double xpos_z5 = -1;
  double xpos_z9 = -1;
  double z9_to_z5_ratio = -1;
  double z0_to_z4_ratio = -1;
};

#endif
