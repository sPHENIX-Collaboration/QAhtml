#ifndef INTT_RAW_HIT_SUMMARY_DRAWER_H
#define INTT_RAW_HIT_SUMMARY_DRAWER_H

#include <qahtml/SingleCanvasDrawer.h>

class TPad;

class INTTRawHitSummaryDrawer : public SingleCanvasDrawer
{
public:
  INTTRawHitSummaryDrawer(std::string const& = "intt_raw_hit_summary_info");
  virtual ~INTTRawHitSummaryDrawer();

  int DrawCanvas() override;

protected:
  int MakeCanvas(int=-1, int=-1) override;

  using SingleCanvasDrawer::m_canvas; // base class owned
  using SingleCanvasDrawer::m_name;

private:
  TPad* Pad[8];
  TPad* transparent{nullptr};

  const char* histprefix;
};

#endif//INTT_RAW_HIT_SUMMARY_DRAWER_H
