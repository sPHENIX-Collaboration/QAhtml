#ifndef TPC_TPCDRAW_H__
#define TPC_TPCDRAW_H__

#include <qahtml/QADraw.h>

#include <vector>

class QADB;
class QADBVar;
class TCanvas;
class TPad;
class TH1;
class TH2;

class TPCLasersDraw : public QADraw
{
 public: 
  TPCLasersDraw(const std::string &name = "TpcLasersQA");
  ~TPCLasersDraw() override;

  int Draw(const std::string &what = "ALL") override;
  int MakeHtml(const std::string &what = "ALL") override;
  int DBVarInit();

 private:
  int MakeCanvas(const std::string &name);
  int DrawLaserInfo();
  TCanvas *TC{nullptr};
  TPad *transparent{nullptr};
  TPad *Pad[2]{nullptr};
  const char *histprefix;
};

#endif
