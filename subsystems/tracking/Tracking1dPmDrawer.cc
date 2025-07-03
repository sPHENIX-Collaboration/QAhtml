#include "Tracking1dPmDrawer.h"

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

Tracking1dPmDrawer::Tracking1dPmDrawer (
	std::string const& name
) :
	SingleCanvasDrawer(name)
{
	// ...
}

Tracking1dPmDrawer::~Tracking1dPmDrawer (
) {
	// ...
}

int
Tracking1dPmDrawer::MakeCanvas (
	int width,
	int height
) {
	if(SingleCanvasDrawer::MakeCanvas(width, height))
	{
		return 0;
	}

	m_canvas->SetTitle((boost::format("TrackFittingQA_%s") % m_name).str().c_str());
	
	m_pad = new TPad("pad", "pad", 0.0, 0.0, 1.0, 1.0);
	m_pad->Draw();
	
	// this one is used to plot the run number on the canvas
	transparent = new TPad("transparent", "transparent", 0.0, 0.0, 1.0, 1.0);
	transparent->SetFillStyle(4000);
	transparent->Draw();

	return 0;
}

int
Tracking1dPmDrawer::DrawCanvas (
) {
	MakeCanvas(800, 600);

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

	m_pad->cd();

	h_pos->SetLineColor(kRed);
	h_pos->SetMarkerColor(kRed);
	//...
	h_pos->Draw();

	h_neg->SetLineColor(kBlue);
	h_neg->SetMarkerColor(kBlue);
	//...
	h_neg->Draw("same");

	TLegend* legend = new TLegend (0.7, 0.8, 0.9, 0.9);
	legend->AddEntry(h_pos, "Positive Tracks");
	legend->AddEntry(h_neg, "Negative Tracks");
	legend->SetTextSize(0.04);
	legend->Draw();

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
