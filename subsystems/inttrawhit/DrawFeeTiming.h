#pragma once

#include <qahtml/SingleCanvasDrawer.h>

#include <array>
#include <string>

class TPad;

class DrawFeeTiming : public SingleCanvasDrawer {
public:
	DrawFeeTiming(std::string const&, int);
	virtual ~DrawFeeTiming() = default;

	int DrawCanvas() override;

protected:
	int MakeCanvas(int=-1, int=-1) override;

	using SingleCanvasDrawer::m_canvas; // base class owned
	using SingleCanvasDrawer::m_name;

private:
	int m_felix_server{0};
	static std::string constexpr m_prefix = "h_InttQa";

	TPad* m_title_pad{};
	TPad* m_south_north_pad{};
	std::array<TPad*, 2> m_hist_pad{};
	TPad* m_legend_pad{};
};
