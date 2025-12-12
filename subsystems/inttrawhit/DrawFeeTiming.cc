#include "DrawFeeTiming.h"

#include <qahtml/QADrawClient.h>
#include <qahtml/QADrawDB.h>

#include <phool/phool.h>

#include <TCanvas.h>
#include <TH1.h>
#include <TLine.h>
#include <TPad.h>
#include <TPolyLine.h>
#include <TText.h>

#include <iostream>
#include <format>
#include <boost/format.hpp>

namespace { // anonymous
	Color_t get_color (
		int felix_channel
	) {
	    switch (felix_channel % 7) {
	    	case 1:
	    	    return kRed;
	    	case 2:
	    	    return kGreen;
	    	case 3:
	    	    return kYellow;
	    	case 4:
	    	    return kBlue;
	    	case 5:
	    	    return kMagenta;
	    	case 6:
	    	    return kCyan;
	    	default:
	    	    return kBlack;
	    }
	}
}

DrawFeeTiming::DrawFeeTiming (
	std::string const& name, int felix_server
) : SingleCanvasDrawer(name), m_felix_server{felix_server} {
	// Do nothing
}

int
DrawFeeTiming::MakeCanvas (
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

	m_south_north_pad = new TPad (
		(boost::format("%s_south_north_pad") % m_name).str().c_str(),
		(boost::format("%s_south_north_pad") % m_name).str().c_str(),
		0.0, 0.0, 0.05, 0.9
	);
	m_south_north_pad->Draw();

	for (int south_north = 0; south_north < 2; ++south_north) {
		m_canvas->cd();
		m_hist_pad[south_north] = new TPad (
			(boost::format("%s_hist_pad_%01d") % m_name % south_north).str().c_str(),
			(boost::format("%s_hist_pad_%01d") % m_name % south_north).str().c_str(),
			0.05, 0.45 * south_north, 0.9, 0.45 + 0.45 * south_north
		);
		m_hist_pad[south_north]->Range(0.0, 0.0, 1.0, 1.0);
		m_hist_pad[south_north]->SetTopMargin(0.01);
		m_hist_pad[south_north]->SetBottomMargin(0.01);
		m_hist_pad[south_north]->SetLeftMargin(0.01);
		m_hist_pad[south_north]->SetRightMargin(0.01);
		m_hist_pad[south_north]->SetFillStyle(4000);
		m_hist_pad[south_north]->Draw();
	}

	m_canvas->cd();
	m_legend_pad = new TPad (
		(boost::format("%s_legend_pad") % m_name).str().c_str(),
		(boost::format("%s_legend_pad") % m_name).str().c_str(),
		0.9, 0.0, 1.0, 0.9
	);
	m_legend_pad->Draw();

	return 0;
}

int
DrawFeeTiming::DrawCanvas (
) {
	MakeCanvas();
	m_canvas->SetEditable(true);

	QADrawClient *cl = QADrawClient::instance();

	m_title_pad->cd();
	TText title_text;
	title_text.SetTextAlign(22);
	title_text.SetTextSize(0.5);
	title_text.DrawText(0.5, 0.5, (boost::format("intt%01d Timing, Run %08d") % m_felix_server % cl->RunNumber()).str().c_str());

	m_south_north_pad->cd();
	TText south_north_text;
	south_north_text.SetTextSize(0.3);
	south_north_text.SetTextAlign(22);
	south_north_text.SetTextAngle(90);
	south_north_text.DrawText(0.5, 0.25, "South");
	south_north_text.DrawText(0.5, 0.75, "North");

	// Initial pass over all server histograms to get maximum count and peak positions
	double max{1.0}; // (logscale, start at 1 not 0)
	std::array<int, 14> peak_positions;
	std::map<int, int> peak_counts;
	for (int felix_channel = 0; felix_channel < 14; ++felix_channel) {
		std::string hist_name = (boost::format("%s_bco_distribution_server%01d_channel%02d") % m_prefix % m_felix_server % felix_channel).str();
		auto* hist = dynamic_cast<TH1*>(cl->getHisto(hist_name.c_str()));
		if (!hist) {
			std::cout
				<< PHWHERE
				<< "Could not get hist '" << hist_name << "' from client"
				<< std::endl;
			return 1;
		}

		// Do not consider felix channels which are completely masked
		if (hist->GetEntries() == 0) {
			peak_positions[felix_channel] = -1;
			continue;
		}

		double peak_bin = hist->GetMaximumBin();
		++peak_counts[hist->GetBinCenter(peak_bin)];
		peak_positions[felix_channel] = hist->GetBinCenter(peak_bin);

		double this_max = hist->GetBinContent(peak_bin);
		if (this_max < max) continue;
		max = this_max;
	}

	// Choose the server peak as the mode
	int peak{}, max_count{0};
	for (auto const& [peak_position, count] : peak_counts) {
		if (count < max_count) continue;
		max_count = count;
		peak = peak_position;
	}

	for (int felix_channel = 0; felix_channel < 14; ++felix_channel) {
		std::string hist_name = (boost::format("%s_bco_distribution_server%01d_channel%02d") % m_prefix % m_felix_server % felix_channel).str();
		auto* hist = dynamic_cast<TH1*>(cl->getHisto(hist_name.c_str()));
		if (!hist) {
			std::cout
				<< PHWHERE
				<< "Could not get hist '" << hist_name << "' from client"
				<< std::endl;
			return 1;
		}
		int south_north = (int)std::floor(felix_channel / 7.0);
		m_hist_pad[south_north]->cd();
		m_hist_pad[south_north]->SetLogy(true);

		hist->GetYaxis()->SetRangeUser(1, 10 * max);

		hist->SetMarkerStyle(0);
		hist->SetLineStyle(1);
		hist->SetLineWidth(1);
        hist->SetFillStyle(3002);
        hist->SetLineColor(get_color(felix_channel));
        hist->SetFillColor(get_color(felix_channel));
		hist->SetLineColor(get_color(felix_channel));

		hist->Draw("HIST AH same");

		// Picks out the cases where felix_channel == 6, 13, ...
		// (last channel per south_north)
        if ((felix_channel + 1) % 7) continue;

        TLine line;
        line.SetLineColor(kBlack);
        line.SetLineWidth(2);
        line.SetLineStyle(1);
        line.DrawLine(peak, 1.0, peak, 10 * max);

        TText text;
        text.SetTextColor(kBlack);
        text.SetTextSize(0.06);
        text.SetTextAlign(22);
        double text_pos = (peak < 64) ? peak - 1.75 : peak + 1.75;
        text.DrawText(text_pos, sqrt(10.0) * max, Form("%0d", peak));

        line.SetLineWidth(1);
        line.SetLineStyle(2);
        text.SetTextSize(0.04);
        text.SetTextAlign(12);
        for (int p = 0; p < 127; p += 8) {
            if (abs(text_pos - p) < 4) continue;
            line.DrawLine(p, 1.0, p, 10 * max);
            text.DrawText(p + 0.5, sqrt(10) * max, Form("%d", p));
        }
	}

	m_legend_pad->cd();
	for (int felix_channel = 0; felix_channel < 14; ++felix_channel) {
		double xs[] = { -1.0, +1.0, +1.0, -1.0 };
		double ys[] = { -1.0, -1.0, +1.0, +1.0 };

		for (auto& x : xs) {
			x *= 0.15; // width / 2
			x += 0.15; // x0 (center)
		}
		for (auto& y : ys) {
			y *= 0.01; // height / 2
			y += (felix_channel + 0.5) / 14; // y0 (center)
		}

		TPolyLine box;
		box.SetFillColor(get_color(felix_channel));
		box.SetLineColor(kBlack);
		box.SetLineWidth(1);
		box.DrawPolyLine(4, xs, ys, "F");

		TText text;
		text.SetTextAlign(22);
		text.SetTextSize(0.1);

		if (peak_positions[felix_channel] != peak) {
			text.SetTextColor(kRed+1);
		}

		std::string peak_position_str = peak_positions[felix_channel] == -1 ?
			"empty":
			(boost::format("%d") % peak_positions[felix_channel]).str();
		text.DrawText (
			0.6, (felix_channel + 0.5) / 14,
			(boost::format("FCh %02d (%s)") % felix_channel % peak_position_str).str().c_str()
		);
	}

	m_canvas->Update();
	m_canvas->Show();
	m_canvas->SetEditable(false);

	return 0;
}
