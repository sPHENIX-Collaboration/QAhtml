#pragma once

#include <qahtml/SingleCanvasDrawer.h>

#include <array>
#include <string>

class TPad;

class DrawServerHitmaps : public SingleCanvasDrawer {
public:
	DrawServerHitmaps(std::string const&);
	virtual ~DrawServerHitmaps() = default;

	int DrawCanvas() override;

protected:
	int MakeCanvas(int=-1, int=-1) override;

	using SingleCanvasDrawer::m_canvas; // base class owned
	using SingleCanvasDrawer::m_name;

private:
	static std::string constexpr m_prefix = "h_InttQa";

	TPad* m_title_pad{};
	TPad* m_hist_pad{};
	TPad* m_transparent_pad{};
};
