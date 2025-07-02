#include "TrackingDraw.h"

#include "Tracking1dPmDrawer.h"
#include "Tracking2dPmDrawer.h"
//...

#include <qahtml/QADrawClient.h>
#include <qahtml/QADrawDB.h>
#include <qahtml/SingleCanvasDrawer.h>

#include <iostream>

TrackingDraw::TrackingDraw(const std::string &name)
	: QADraw(name)
{
	m_options["p"] = new Tracking1dPmDrawer("p");
	m_options["pt"] = new Tracking1dPmDrawer("pt");
	m_options["eta"] = new Tracking1dPmDrawer("eta");
	m_options["mvtx_states"] = new Tracking1dPmDrawer("mvtx_states");
	m_options["intt_states"] = new Tracking1dPmDrawer("intt_states");
	m_options["tpc_states"] = new Tracking1dPmDrawer("tpc_states");
	m_options["tpot_states"] = new Tracking1dPmDrawer("tpot_states");
	m_options["quality"] = new Tracking1dPmDrawer("quality");

	m_options["phi_eta"] = new Tracking2dPmDrawer("phi_eta");

	DBVarInit();
}

TrackingDraw::~TrackingDraw()
{
	// delete db;
	for(auto& [name, option] : m_options)
	{
		delete option;
	}
}

int
TrackingDraw::Draw (
	std::string const& what
) {
	// SetsPhenixStyle();

	int iret = 0;
	int idraw = 0;
	for(auto const& [name, option] : m_options)
	{
		if(what != "ALL" && what != name) continue;
		// I've seen people returning -1 on error instead of 1
		// Increment if the return value is nonzero
		iret += (option->DrawCanvas() != 0);
		++idraw;
	}

	if(!idraw)
	{
		std::cerr
			<< "Unimplemented drawing option:\n"
			<< "\t" << what << "\n"
			<< "Implemented options:\n"
			<< "\tALL" << std::endl;
		for(auto const& [name, option] : m_options)
		{
			std::cerr << "\t" << name << std::endl;
		}
		++iret;
	}

	return iret;
}
 
int
TrackingDraw::MakeHtml (
	std::string const& what
) {
	QADrawClient *cl = QADrawClient::instance();

	int iret = 0;
	int idraw = 0;
	for(auto const& [name, option] : m_options)
	{
		++idraw;
		if(what != "ALL" && what != name)continue;

		// I've seen people returning -1 on error instead of 1
		// Increment if the return value is nonzero
		int rv = option->DrawCanvas() != 0;
		iret += rv;

		// on error no html output please
		if(rv || !option->GetCanvas())continue;

		// Registers the canvas png file to the menu and produces the png file
		std::string pngfile = cl->htmlRegisterPage(*this, name, std::to_string(idraw), "png");
		cl->CanvasToPng(option->GetCanvas(), pngfile);
	}

	if(!idraw)
	{
		std::cerr
			<< "Unimplemented drawing option:\n"
			<< "\t" << what << "\n"
			<< "Implemented options:\n"
			<< "\tALL" << std::endl;
		for(auto const& [name, option] : m_options)
		{
			std::cerr << "\t" << name << std::endl;
		}
		++iret;
	}

	return iret;
}

int
TrackingDraw::DBVarInit (
) {
	/* db = new QADrawDB(this); */
	/* db->DBInit(); */
	return 0;
}
