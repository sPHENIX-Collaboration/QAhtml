#ifndef INTT_CHIP_DRAWER_H
#define INTT_CHIP_DRAWER_H

#include <qahtml/SingleCanvasDrawer.h>

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
};

#endif//INTT_CHIP_DRAWER_H
