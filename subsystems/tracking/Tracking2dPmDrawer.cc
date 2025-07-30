#include "Tracking2dPmDrawer.h"

#include <qahtml/QADrawClient.h>
#include <qahtml/QADrawDB.h>

#include <TCanvas.h>
#include <TDatime.h>
#include <TGraphErrors.h>
#include <TH1.h>
#include <TH2.h>
#include <TProfile.h>
#include <TPad.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TSystem.h>
#include <TText.h>
#include <TLatex.h>
#include <TColor.h>
#include <TLegend.h>

#include <boost/format.hpp>

#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>

#define WHERE __FILE__ << ":" << __LINE__

Tracking2dPmDrawer::Tracking2dPmDrawer (
	std::string const& name
) :
	SingleCanvasDrawer(name)
{
	// ...
}

Tracking2dPmDrawer::~Tracking2dPmDrawer (
) {
	// ...
}

int
Tracking2dPmDrawer::MakeCanvas (
	int width,
	int height
) {
	if(SingleCanvasDrawer::MakeCanvas(width, height))
	{
		return 0;
	}

	m_canvas->SetTitle((boost::format("TrackFittingQA_%s") % m_name).str().c_str());

	m_canvas->cd();	
	m_pos_pad = new TPad("pad", "pad", 0.0, 0.0, 0.5, 1.0);
	m_pos_pad->Draw();
	m_pos_overlay_pad = new TPad("pad", "pad", 0.0, 0.0, 0.5, 1.0);
	m_pos_overlay_pad->SetFillStyle(4000);
	m_pos_overlay_pad->Draw();

	m_canvas->cd();	
	m_neg_pad = new TPad("pad", "pad", 0.5, 0.0, 1.0, 1.0);
	m_neg_pad->Draw();
	m_neg_overlay_pad = new TPad("pad", "pad", 0.5, 0.0, 1.0, 1.0);
	m_neg_overlay_pad->SetFillStyle(4000);
	m_neg_overlay_pad->Draw();
	
	// this one is used to plot the run number on the canvas
	m_canvas->cd();	
	transparent = new TPad("transparent", "transparent", 0.0, 0.0, 1.0, 1.0);
	transparent->SetFillStyle(4000);
	transparent->Draw();

	return 0;
}

int
Tracking2dPmDrawer::DrawCanvas (
) {
	MakeCanvas(1600, 600);

	QADrawClient *cl = QADrawClient::instance();

	TH1* h_pos = dynamic_cast<TH1*>(cl->getHisto((boost::format(m_pattern) % m_name % "positively").str()));
	TH1* h_neg = dynamic_cast<TH1*>(cl->getHisto((boost::format(m_pattern) % m_name % "negatively").str()));

	if (!h_pos || !h_neg) {
		std::cout
			<< WHERE << "\n"
			<< "\tCould not get \"positive\" or \"negative\" variant of " << m_pattern << "\n"
			<< "\tReturning\n"
			<< std::endl;
		return -1;
	}

	m_canvas->Clear("D");


	m_pos_pad->cd();
	h_pos->SetLineColor(kRed);
	h_pos->SetMarkerColor(kRed);
	//...
	h_pos->Draw("COLZ");
	m_pos_overlay_pad->cd();
	TText pos_text;
	pos_text.SetTextSize(0.04);
	pos_text.DrawText(0.1, 0.05, "Positive tracks");


	m_neg_pad->cd();
	h_neg->SetLineColor(kBlue);
	h_neg->SetMarkerColor(kBlue);
	//...
	h_neg->Draw("COLZ");
	m_neg_overlay_pad->cd();
	TText neg_text;
	neg_text.SetTextSize(0.04);
	neg_text.DrawText(0.1, 0.05, "Negative tracks");

	TText print_run;
	print_run.SetTextFont(62);
	print_run.SetTextSize(0.04);
	print_run.SetNDC();	// set to normalized coordinates
	print_run.SetTextAlign(23); // center/top alignment

	transparent->cd();
	std::ostringstream run_num_stream;
	run_num_stream << "TrackFittingQA Info Run " << cl->RunNumber() << ", build " << cl->build();
	std::string run_num_string = run_num_stream.str();

	print_run.DrawText(0.5, 1., run_num_string.c_str());

	m_canvas->Update();

	return 0;
}

#undef WHERE
