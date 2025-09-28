#pragma once

#include <qahtml/SingleCanvasDrawer.h>

#include <array>
#include <string>

class TPad;

class DrawTimingOkay : public SingleCanvasDrawer {
public:
	DrawTimingOkay(std::string const&);
	virtual ~DrawTimingOkay() = default;

	int DrawCanvas() override;

protected:
	int MakeCanvas(int=-1, int=-1) override;

	using SingleCanvasDrawer::m_canvas; // base class owned
	using SingleCanvasDrawer::m_name;

private:
	static std::string constexpr m_prefix = "h_InttQa";

	TPad* m_title_pad{};
	TPad* m_text_pad{};

	// The number of unique peaks among a server's felix channel bco distribution
	// before we say there is a problem
	// This must be at least 1, as there is always at least 1 unique peak
	static int const m_max_acceptable_peaks = 3; // At most 2 felix channels can have different peaks
};
