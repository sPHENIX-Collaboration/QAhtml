#ifndef INTT_CHIP_DRAWER_H
#define INTT_CHIP_DRAWER_H

#include <qahtml/SingleCanvasDrawer.h>
#include <TFile.h>
#include<TH1F.h>

class TPad;

class INTTChipDrawer : public SingleCanvasDrawer
{
public:
  INTTChipDrawer(std::string const& = "intt_chip_info");
  virtual ~INTTChipDrawer();

  int DrawCanvas() override;

protected:
  int MakeCanvas() override;

  using SingleCanvasDrawer::m_canvas; // base class owned
  using SingleCanvasDrawer::m_name;

private:
  TPad *transparent{nullptr};
  TPad *Pad[4]{nullptr};
  const char* histprefix;

  TFile* file1;
  TFile* file2;
  TH1F* hist1;
  TH1F* hist2;
};

#endif//INTT_CHIP_DRAWER_H
