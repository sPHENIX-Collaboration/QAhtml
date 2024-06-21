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

class SiliconSeedsDraw : public QADraw
{
 public: 
  SiliconSeedsDraw(const std::string &name = "SILICONSEEDSQA");
  ~SiliconSeedsDraw() override;

  int Draw(const std::string &what = "ALL") override;
  int MakeHtml(const std::string &what = "ALL") override;
  int DBVarInit();

 private:
  int MakeCanvas(const std::string &name, int num);
  int DrawTrackletInfo();
  int DrawVertexInfo();
  TCanvas *TC[2]{};
  TPad *transparent[2]{};
  TPad *Pad[2][10]{};
  const char *histprefix;
};

#endif
