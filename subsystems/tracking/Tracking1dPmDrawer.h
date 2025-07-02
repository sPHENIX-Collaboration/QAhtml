#ifndef TRACKING_1D_PM_DRAWER_H
#define TRACKING_1D_PM_DRAWER_H

#include <qahtml/SingleCanvasDrawer.h>

class TPad;

// The TrackFittingQA module produces many histograms,
// where each histogram contains the 1D distributions
// of a variable by charge (+/-).
// 
// This module will draw the overlayed 1D distributions
// on the same plot, for each charge

class Tracking1dPmDrawer : public SingleCanvasDrawer
{
public:
	Tracking1dPmDrawer (std::string const&);
	virtual ~Tracking1dPmDrawer();

	int DrawCanvas() override;

protected:
	int MakeCanvas(int=-1, int=-1) override;

	using SingleCanvasDrawer::m_canvas; // base class owned
	using SingleCanvasDrawer::m_name;

private:
	TPad* transparent{};
	TPad* m_pad{};

	inline static std::string const m_pattern = "h_TrackFittingQA_%s_%s_charged_tracks"; // % m_name % ("positively" or "negatively")
};

#endif//TRACKING_1D_PM_DRAWER_H
