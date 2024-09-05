#ifndef INTT_RAW_HIT_CHIP_DRAWER_H
#define INTT_RAW_HIT_CHIP_DRAWER_H

#include <qahtml/SingleCanvasDrawer.h>

class TPad;

class INTTRawHitChipDrawer : public SingleCanvasDrawer
{
public:
  INTTRawHitChipDrawer(std::string const& = "intt_raw_hit_chip_info", int = 0);
  virtual ~INTTRawHitChipDrawer();

  int DrawCanvas() override;

protected:
  int MakeCanvas() override;

  using SingleCanvasDrawer::m_canvas; // base class owned
  using SingleCanvasDrawer::m_name;

private:
  TPad *transparent{nullptr};
  int m_which_intt{0};

  const char* histprefix;
};

#endif//INTT_RAW_HIT_CHIP_DRAWER_H
