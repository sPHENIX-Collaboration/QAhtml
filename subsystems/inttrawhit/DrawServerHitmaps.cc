#include "DrawServerHitmaps.h"

#include <qahtml/QADrawClient.h>
#include <qahtml/QADrawDB.h>

#include <phool/phool.h>

#include <TCanvas.h>
#include <TH1.h>
#include <TH2D.h>
#include <TPad.h>
#include <TText.h>

#include <iostream>
#include <format>
#include <boost/format.hpp>

DrawServerHitmaps::DrawServerHitmaps (
	std::string const& name
) : SingleCanvasDrawer(name) {
	// Do nothing
}

int
DrawServerHitmaps::MakeCanvas (
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
		(boost::format("%s_hist_pad") % m_name).str().c_str(),
		(boost::format("%s_hist_pad") % m_name).str().c_str(),
		0.0, 0.0, 1.0, 0.9
	);
	m_hist_pad->SetFillStyle(4000);
	m_hist_pad->Range(0.0, 0.0, 1.0, 1.0);
	m_hist_pad->Divide(4,2);
	m_hist_pad->Draw();

	m_canvas->cd();
	m_transparent_pad = new TPad (
		(boost::format("%s_transparent_pad") % m_name).str().c_str(),
		(boost::format("%s_transparent_pad") % m_name).str().c_str(),
		0.0, 0.0, 1.0, 0.9
	);
	m_transparent_pad->SetFillStyle(4000);
	m_transparent_pad->Range(0.0, 0.0, 1.0, 1.0);
	m_transparent_pad->Divide(4,2);
	m_transparent_pad->Draw();
	// ...

	return 0;
}

int
DrawServerHitmaps::DrawCanvas (
) {
	MakeCanvas();
	m_canvas->SetEditable(true);

	QADrawClient *cl = QADrawClient::instance();

	m_title_pad->cd();
	TText title_text;
	title_text.SetTextAlign(22);
	title_text.SetTextSize(0.3);
	title_text.DrawText(0.5, 0.5, (boost::format("INTT Hitmaps, Run %08d") % cl->RunNumber()).str().c_str());

	for (int felix_server = 0; felix_server < 8; ++felix_server) {
		std::string hist_name = (boost::format("%s_hit_distribution_server%01d") % m_prefix % felix_server).str();
		auto* hist = dynamic_cast<TH2D*>(cl->getHisto(hist_name.c_str()));
		if (!hist) {
			std::cout
				<< PHWHERE
				<< "could not get hist '" << hist_name << "' from client"
				<< std::endl;
			return 1;
		}
		m_hist_pad->cd(felix_server + 1);
		gPad->SetTopMargin(0.1);
		gPad->SetRightMargin(0.15);
		gPad->SetFillStyle(4000);
		gPad->Range(0.0, 0.0, 1.0, 1.0);
		hist->SetTitle("THIS TEXT ISN'T APPEARING?????;chip;felix channel");
		hist->GetXaxis()->SetTitleSize(0.05);
		hist->GetYaxis()->SetTitleSize(0.05);
		hist->Draw("COLZ");

		// There is some kind of bug where doing hist->SetTitleSize(), SetTitleOffset()
		// modifies those paramters for the x axis, not the histogram title
		// I fucking hate ROOT
		m_transparent_pad->cd(felix_server + 1);
		gPad->SetFillStyle(4000);
		gPad->Range(0.0, 0.0, 1.0, 1.0);
		TText text;
		text.SetTextAlign(22);
		text.SetTextSize(0.075);
		text.DrawText(0.5, 0.95, (boost::format("intt%01d") % felix_server).str().c_str());
	}

	m_canvas->Update();
	m_canvas->Show();
	m_canvas->SetEditable(false);

	return 0;
}
