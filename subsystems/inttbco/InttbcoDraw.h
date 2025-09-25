#ifndef EXAMPLEDRAW_H__
#define EXAMPLEDRAW_H__

//#include <qahtml/QADraw.h>
#include <qahtml/SingleCanvasDrawer.h>
#include <vector>

//class QADrawDB;
//class QADrawDBVar;
//class TCanvas;
//class TGraphErrors;
//class TPad;
class InttbcoDraw : public SingleCanvasDrawer
{
public:
  InttbcoDraw(std::string const& = "intt_cluster_info");
  virtual ~InttbcoDraw();

protected:
  int DrawCanvas() override;
  int MakeCanvas(int=-1, int=-1) override;

  using SingleCanvasDrawer::m_canvas; // base class owned
  using SingleCanvasDrawer::m_name;

private:
  TPad *transparent{nullptr};
  TPad *Pad[8]{nullptr};
  //  const char* histprefix;
};

#endif
