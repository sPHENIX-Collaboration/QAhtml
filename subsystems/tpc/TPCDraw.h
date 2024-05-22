#ifndef TPC_TPCDRAW_H__
#define TPC_TPCDRAW_H__

#include <qahtml/QADraw.h>

#include <vector>

class QADB;
class QADBVar;
class TCanvas;
class TGraphErrors;
class TPad;
class TH1F;
class TH2F;

class TPCDraw : public QADraw
{
 public: 
  TPCDraw(const std::string &name = "TPCQA");
  ~TPCDraw() override;

  int Draw(const std::string &what = "ALL") override;
  int MakeHtml(const std::string &what = "ALL") override;
  int DBVarInit();

 private:
  int MakeCanvas(const std::string &name, int num);
  int DrawChannelHits();
  int DrawChannelADCs();
  int DrawClusterInfo();
  int DrawRegionInfo();
  TCanvas *TC[25]{};
  TPad *transparent[25]{};
  TPad *Pad[25][4]{};
};

#endif
