#ifndef INTT_INTTDRAW_H__
#define INTT_INTTDRAW_H__

#include <qahtml/QADraw.h>

#include <vector>

class QADB;
class QADBVar;
class TCanvas;
class TGraphErrors;
class TPad;
class TH1F;
class TH2F;

class INTTDraw : public QADraw
{
 public: 
  INTTDraw(const std::string &name = "INTTQA");
  ~INTTDraw() override;

  int Draw(const std::string &what = "ALL") override;
  int MakeHtml(const std::string &what = "ALL") override;
  int DBVarInit();

 private:
  int MakeCanvas(const std::string &name, int num);
  int DrawChipInfo();
  int DrawClusterInfo();
  TCanvas *TC[2]{};
  TPad *transparent[2]{};
  TPad *Pad[2][4]{};
  const char *histprefix;
};

#endif
