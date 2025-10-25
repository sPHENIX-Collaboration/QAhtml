#include "DrawBarrelHitmaps.h"

#include <qahtml/QADrawClient.h>
#include <qahtml/QADrawDB.h>

#include <phool/phool.h>

#include <TCanvas.h>
#include <TH1.h>
#include <TLine.h>
#include <TPad.h>
#include <TText.h>

#include <iostream>
#include <format>
#include <map>
#include <set>
#include <sstream>
#include <boost/format.hpp>

DrawBarrelHitmaps::DrawBarrelHitmaps (
	std::string const& name
) : SingleCanvasDrawer(name) {
	// Do nothing
}

int
DrawBarrelHitmaps::MakeCanvas (
	int width,
	int height
) {
	if (SingleCanvasDrawer::MakeCanvas(width, height)) return 0;

	m_canvas->cd();
	m_title_pad = new TPad (
		(boost::format("%s_title_pad") % m_name).str().c_str(),
		(boost::format("%s_title_pad") % m_name).str().c_str(),
		0.0, 0.9, 1.0, 1.0
	);
	m_title_pad->SetFillStyle(4000);
	m_title_pad->Range(0.0, 0.0, 1.0, 1.0);
	m_title_pad->Draw();

	m_canvas->cd();
	m_hist_pad = new TPad (
		(boost::format("%s_text_pad") % m_name).str().c_str(),
		(boost::format("%s_text_pad") % m_name).str().c_str(),
		0.0, 0.0, 1.0, 0.9
	);
	m_hist_pad->SetFillStyle(4000);
	m_hist_pad->Range(0.0, 0.0, 1.0, 1.0);
	m_hist_pad->Divide(2, 1);
	m_hist_pad->Draw();

	return 0;
}

int
DrawBarrelHitmaps::DrawCanvas (
) {
	MakeCanvas();
	m_canvas->SetEditable(true);

	QADrawClient *cl = QADrawClient::instance();

	m_title_pad->cd();
	TText title_text;
	title_text.SetTextAlign(22);
	title_text.SetTextSize(0.5);
	title_text.DrawText(0.5, 0.5, (boost::format("INTT Barrel Hitmaps, Run %08d") % cl->RunNumber()).str().c_str());

	for (int barrel = 0; barrel < 2; ++barrel) {
		m_hist_pad->cd(barrel+1);
		gPad->SetFillStyle(4000);
		gPad->Range(0.0, 0.0, 1.0, 1.0);
		gPad->SetLeftMargin(0.1);
		gPad->SetRightMargin(0.2);

		std::string hist_name = (boost::format("%s_hit_distribution_barrel_%01d") % m_prefix % barrel).str();
		auto* hist = dynamic_cast<TH1*>(cl->getHisto(hist_name.c_str()));
		if (!hist) {
			std::cout
				<< PHWHERE
				<< "Could not get hist '" << hist_name << "' from client"
				<< std::endl;
			return 1;
		}
		hist->GetXaxis()->SetTitle("z index");
		hist->GetYaxis()->SetTitle(barrel ?
			"Outer Barrel Ladder Nominal #phi" :
			"Inner Barrel Ladder Nominal #phi"
		);
		hist->GetXaxis()->SetTitleOffset(0.8);
		hist->GetYaxis()->SetTitleOffset(0.6);

		hist->Draw("COLZ");
	}

	m_canvas->Update();
	m_canvas->Show();
	m_canvas->SetEditable(false);

	return 0;
}
