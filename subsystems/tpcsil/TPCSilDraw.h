#ifndef TPCSIL_TPCSILDRAW_H__
#define TPCSIL_TPCSILDRAW_H__

#include <qahtml/QADraw.h>

#include <vector>

class QADB;
class QADBVar;
class TCanvas;
class TGraphErrors;
class TPad;
class TH1F;
class TH2F;

class TPCSilDraw : public QADraw
{
  public:
    TPCSilDraw(const std::string &name = "TpcSiliconQA");
    ~TPCSilDraw() override;

    int Draw(const std::string &what = "ALL") override;
    int MakeHtml(const std::string &what = "ALL") override;
    int DBVarInit();

  private:
    int MakeCanvas(const std::string &name, int num);
    int DrawMatchingInfo();
    int DrawPositionInfo();
    TCanvas *TC[2]{};
    TPad *transparent[2]{};
    TPad *Pad[2][6]{};
    const char *histprefix;
};

#endif
