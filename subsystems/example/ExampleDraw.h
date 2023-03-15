#ifndef EXAMPLEDRAW_H__
#define EXAMPLEDRAW_H__

#include <qahtml/OnlProdDraw.h>

#include <vector>

class OnlProdDB;
class OnlProdDBVar;
class TCanvas;
class TGraphErrors;
class TPad;

class ExampleDraw: public OnlProdDraw
{

 public: 
  ExampleDraw(const std::string &name = "EXAMPLE");
  virtual ~ExampleDraw();

  int Draw(const std::string &what = "ALL");
  int MakeHtml(const std::string &what = "ALL");
  int DBVarInit();

 protected:
  int MakeCanvas(const std::string &name);
  int DrawFirst(const std::string &what = "ALL");
  int DrawGraph(TPad *pad, const std::vector<OnlProdDBVar> &history, const time_t begin, const time_t end);
  OnlProdDB *db;
  TCanvas *TC[1];
  TPad *transparent[1];
  TPad *Pad[4];
  TGraphErrors *gr[1];
};

#endif
