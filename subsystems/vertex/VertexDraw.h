#ifndef VERTEX_VERTEXDRAW_H__
#define VERTEX_VERTEXDRAW_H__

#include <qahtml/QADraw.h>

#include <vector>

class QADB;
class QADBVar;
class TCanvas;
class TGraphErrors;
class TPad;
class TH1F;
class TH2F;

class VertexDraw : public QADraw
{
 public: 
  VertexDraw(const std::string &name = "VertexQA");
  ~VertexDraw() override;

  int Draw(const std::string &what = "ALL") override;
  int MakeHtml(const std::string &what = "ALL") override;
  int DBVarInit();

 private:
  int MakeCanvas(const std::string &name, int num);
  int DrawVertexInfo();
  TCanvas *TC[3]{};
  TPad *transparent[3]{};
  TPad *Pad[3][4]{};
  const char *histprefix;
};

#endif
