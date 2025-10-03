#include "Tracking2dPmDrawer.h"

#include <qahtml/QADrawClient.h>
#include <qahtml/QADrawDB.h>

#include <TCanvas.h>
#include <TDatime.h>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TFitResult.h>
#include <TH1.h>
#include <TH2.h>
#include <TProfile.h>
#include <TPad.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TSystem.h>
#include <TLatex.h>
#include <TLatex.h>
#include <TColor.h>
#include <TLegend.h>

#include <boost/format.hpp>

#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <limits>
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
	m_pos_pad->SetRightMargin(0.2);
	m_pos_pad->Draw();
	m_pos_overlay_pad = new TPad("pad", "pad", 0.0, 0.0, 0.5, 1.0);
	m_pos_overlay_pad->SetFillStyle(4000);
	m_pos_overlay_pad->Draw();

	m_canvas->cd();	
	m_neg_pad = new TPad("pad", "pad", 0.5, 0.0, 1.0, 1.0);
	m_neg_pad->SetRightMargin(0.2);
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

	auto h_pos = dynamic_cast<TH2*>(cl->getHisto((boost::format(m_pattern) % m_name % "positively").str()));
	auto h_neg = dynamic_cast<TH2*>(cl->getHisto((boost::format(m_pattern) % m_name % "negatively").str()));

	if (!h_pos || !h_neg) {
		std::cout
			<< WHERE << "\n"
			<< "\tCould not get \"positive\" or \"negative\" variant of " << m_pattern << "\n"
			<< "\tReturning\n"
			<< std::endl;
		return -1;
	}

	m_canvas->Clear("D");
	std::string const fit_str = "[0] + [1]*x*x"; // "[0]*(exp([1]*x)-1)";

	// Used with fits later
	float slope = std::numeric_limits<float>::max();
	float error = std::numeric_limits<float>::max();

	m_pos_pad->cd();
	h_pos->SetLineColor(kRed);
	h_pos->SetMarkerColor(kRed);
	//...
	h_pos->Draw("COLZ");
	if (auto h_pos_profile = dynamic_cast<TProfile*>(cl->getHisto((boost::format(m_pattern) % m_name % "positively").str() + "_profile"))) {
		// Some 2D quantities include a profile (pt_err), which has the same name with "_profile" appended
		// See if that exists and if so, draw it and fit it
		h_pos_profile->Draw("same");
		TF1 pos_fit("pos_fit", fit_str.c_str(), h_pos_profile->GetXaxis()->GetXmin(), h_pos_profile->GetXaxis()->GetXmax());
		pos_fit.SetParameters(1, 1);
		auto fit_result_ptr = h_pos_profile->Fit("pos_fit", "SQ");
		if (fit_result_ptr->Status() == 0 || fit_result_ptr->Status() == 1) {
			slope = fit_result_ptr->GetParams()[1];
			error = fit_result_ptr->GetErrors()[1];
		}
	}
	m_pos_overlay_pad->cd();
	TLatex pos_text;
	pos_text.SetTextSize(0.04);
	pos_text.DrawLatex(0.1, 0.05, (boost::format("Positive tracks (%d)") % h_pos->GetEntries()).str().c_str());
	if (slope != std::numeric_limits<float>::max()) {
		pos_text.DrawLatex(0.2, 0.85, (boost::format("Param = %.3E#pm%.3E") % slope % error).str().c_str());
	}

	m_neg_pad->cd();
	h_neg->SetLineColor(kBlue);
	h_neg->SetMarkerColor(kBlue);
	//...
	h_neg->Draw("COLZ");
	slope = std::numeric_limits<float>::max();
	error = std::numeric_limits<float>::max();
	if (auto h_neg_profile = dynamic_cast<TProfile*>(cl->getHisto((boost::format(m_pattern) % m_name % "negatively").str() + "_profile"))) {
		// Some 2D quantities include a profile (pt_err), which has the same name with "_profile" appended
		// See if that exists and if so, draw it and fit it
		h_neg_profile->Draw("same");
		TF1 neg_fit("neg_fit", fit_str.c_str(), h_neg_profile->GetXaxis()->GetXmin(), h_neg_profile->GetXaxis()->GetXmax());
		neg_fit.SetParameters(1, 1);
		auto fit_result_ptr = h_neg_profile->Fit("neg_fit", "SQ");
		if (fit_result_ptr->Status() == 0 || fit_result_ptr->Status() == 1) {
			slope = fit_result_ptr->GetParams()[1];
			error = fit_result_ptr->GetErrors()[1];
		}
	}
	m_neg_overlay_pad->cd();
	TLatex neg_text;
	neg_text.SetTextSize(0.04);
	neg_text.DrawLatex(0.1, 0.05, (boost::format("Negative tracks (%d)") % h_neg->GetEntries()).str().c_str());
	if (slope != std::numeric_limits<float>::max()) {
		neg_text.DrawLatex(0.2, 0.85, (boost::format("Param = %.3E#pm%.3E") % slope % error).str().c_str());
	}

	TLatex print_run;
	print_run.SetTextFont(62);
	print_run.SetTextSize(0.04);
	print_run.SetNDC();	// set to normalized coordinates
	print_run.SetTextAlign(23); // center/top alignment

	transparent->cd();
	std::ostringstream run_num_stream;
	run_num_stream << "TrackFittingQA Info Run " << cl->RunNumber() << ", build " << cl->build();
	std::string run_num_string = run_num_stream.str();

	print_run.DrawLatex(0.5, 1., run_num_string.c_str());

	m_canvas->Update();

	return 0;
}

#undef WHERE
