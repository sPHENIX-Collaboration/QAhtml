#include "DrawTimingOkay.h"

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

DrawTimingOkay::DrawTimingOkay (
	std::string const& name
) : SingleCanvasDrawer(name) {
	// Do nothing
}

int
DrawTimingOkay::MakeCanvas (
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
	m_text_pad = new TPad (
		(boost::format("%s_text_pad") % m_name).str().c_str(),
		(boost::format("%s_text_pad") % m_name).str().c_str(),
		0.0, 0.0, 1.0, 0.9
	);
	m_text_pad->SetFillStyle(4000);
	m_text_pad->Range(0.0, 0.0, 1.0, 1.0);
	m_text_pad->Draw();

	return 0;
}

int
DrawTimingOkay::DrawCanvas (
) {
	MakeCanvas();
	m_canvas->SetEditable(true);

	QADrawClient *cl = QADrawClient::instance();

	m_title_pad->cd();
	TText title_text;
	title_text.SetTextAlign(22);
	title_text.SetTextSize(0.5);
	title_text.DrawText(0.5, 0.5, (boost::format("INTT Timing, Run %08d") % cl->RunNumber()).str().c_str());

	// For each felix server, get the distribution of peaks over felix channels
	std::array<std::map<int, int>, 8> server_peak_counts;
	for (int felix_server = 0; felix_server < 8; ++felix_server) {
		for (int felix_channel = 0; felix_channel < 14; ++felix_channel) {
			std::string hist_name = (boost::format("%s_bco_distribution_server%01d_channel%02d") % m_prefix % felix_server % felix_channel).str();
			auto* hist = dynamic_cast<TH1*>(cl->getHisto(hist_name.c_str()));
			if (!hist) {
				std::cout
					<< PHWHERE
					<< "Could not get hist '" << hist_name << "' from client"
					<< std::endl;
				return 1;
			}
			double peak_bin = hist->GetMaximumBin();
			++server_peak_counts[felix_server][hist->GetBinCenter(peak_bin)];
		}
	}

	// They are sorted by peak position--sort them by count
	// (useful for selected the mode of the distribution, and printing)
	std::array<std::map<int, int>, 8> sorted_server_peak_counts;
	for (int felix_server = 0; felix_server < 8; ++felix_server) {
		for (auto const& [peak, count] : server_peak_counts[felix_server]) {
			sorted_server_peak_counts[felix_server][count] = peak;
		}
	}

	// Now for the whole of the intt, get the distribution of peaks over felix servers
	// For each felix server, choose the mode of felix channel peaks as the server peak
	// Because they're sorted by count now, we can get a reverse iterator to find the mode
	std::set<int> intt_peaks;
	for (int felix_server = 0; felix_server < 8; ++felix_server) {
		intt_peaks.insert(sorted_server_peak_counts[felix_server].rbegin()->second);
	}

	bool timing_okay = intt_peaks.size() == 1;
	for (auto const& peak_map : sorted_server_peak_counts) {
		if (peak_map.rbegin()->first < m_min_timed_channels) timing_okay = false;
	}

	m_text_pad->cd();
	TText okay_text;
	okay_text.SetTextAlign(22);
	okay_text.SetTextSize(0.1);
	if (timing_okay) {
		okay_text.SetTextColor(kGreen+1);
		okay_text.DrawText(0.5, 0.9, "Timing Okay");
	} else {
		okay_text.SetTextColor(kRed+1);
		okay_text.DrawText(0.5, 0.9, "Timing Not Okay");
	}

	okay_text.SetTextAlign(12);
	okay_text.SetTextSize(0.05);
	okay_text.SetTextColor(kBlack);
	for (int felix_server = 0; felix_server < 8; ++felix_server) {
		auto const& peak_map = sorted_server_peak_counts[felix_server];
		std::stringstream peak_stream;
		peak_stream
			<< (boost::format("intt%01d peak (counts): ") % felix_server).str();
		for (auto itr = peak_map.rbegin();;) {
			peak_stream
				<< itr->second
				<< " (" << itr->first << ")";
			if (++itr == peak_map.rend()) break;
			peak_stream
				<< ", ";
		}
		if (peak_map.rbegin()->first < m_min_timed_channels) {
			okay_text.SetTextColor(kRed+1);
		} else {
			okay_text.SetTextColor(kGreen+1);
		}
		okay_text.DrawText(0.25, 0.75 - (felix_server + 0.5) / 16, peak_stream.str().c_str());
	}

	m_canvas->Update();
	m_canvas->Show();
	m_canvas->SetEditable(false);

	return 0;
}
