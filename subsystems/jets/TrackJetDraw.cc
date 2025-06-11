// Jennifer James <jennifer.l.james@vanderbilt.edu>, McKenna Sleeth, Derek Anderson, Mariia Mitrankova

#include "TrackJetDraw.h"
#include <TFile.h>
#include <TCanvas.h>
#include <iostream>

// ctor/dtor ==================================================================

// ----------------------------------------------------------------------------
//! Subsystem constructor
// ----------------------------------------------------------------------------
TrackJetDraw::TrackJetDraw(const std::string& name,
                           const std::string& type,
                           const bool debug)
  : QADraw(name)
  , m_jet_type(type)
  , m_do_debug(debug)
{
  // initialize components
  m_drawers["KINEMATIC"] = std::make_unique<JetKinematicDrawer>();
  m_drawers["SEED"] = std::make_unique<JetSeedDrawer>();
}

// ----------------------------------------------------------------------------
//! Subsystem destructor
// ----------------------------------------------------------------------------
TrackJetDraw::~TrackJetDraw() {};

// inherited public methods ===================================================

// ----------------------------------------------------------------------------
//! Draw plots
// ----------------------------------------------------------------------------
/*! Draws plots based on options. Implemented options:
 *    - `"KINEMATIC"` = draw jet kinematic plots,
 *    - `"SEED"` = draw jet seed plots,
 *    - `"ALL"` = draw all of the above.
 *
 *  \param what drawing option
 */
int TrackJetDraw::Draw(const std::string& what)
{
  // emit debugging message
  if (m_do_debug)
  {
    std::cout << "  -- Drawing component: " << what << std::endl;
  }

  int nDraw = 0;
  if (what == "ALL")
  {
    for (auto& drawer : m_drawers)
    {
      nDraw += drawer.second->Draw(m_vecTrigToDraw, m_vecResToDraw);
    }
  }
  else
  {
    nDraw += m_drawers.at(what)->Draw(m_vecTrigToDraw, m_vecResToDraw);
  }

  // return error (-1) if nothing drawn, otherwise return 0
  if (nDraw == 0)
  {
    std::cout << "Warning: unimplemented drawing option, " << what << "!" << std::endl;
    return -1;
  }
  else
  {
    return 0;
  }
}

// ----------------------------------------------------------------------------
//! Draw plots and generate HTML pages
// ----------------------------------------------------------------------------
/*! Draws plots based on provided option (see `JetDraw::Draw(std::string&)`)
 *  and generates html pages for each.
 *
 *  \param what drawing option
 */
int TrackJetDraw::MakeHtml(const std::string& what)
{
  // emit debugging messages
  if (m_do_debug)
  {
    std::cout << "  -- Creating HTML pages for " << what << std::endl;
  }

  // draw relevant plots, return error if need be
  const int drawError = Draw(what);
  if (drawError)
  {
    return drawError;
  }

  // generate relevant html pages
  if (what == "ALL")
  {
    for (auto& drawer : m_drawers)
    {
      drawer.second->MakeHtml(m_vecTrigToDraw, m_vecResToDraw, *this);
    }
  }
  else
  {
    m_drawers.at(what)->MakeHtml(m_vecTrigToDraw, m_vecResToDraw, *this);
  }

  // reuturn w/o error
  return 0;
}

// other public methods =======================================================

// ----------------------------------------------------------------------------
//! Save canvases to file
// ----------------------------------------------------------------------------
/*! Helper method to save all canvases to a specified file.
 *  This is useful for debugging and quick testing when
 *  adjusting plotting details/etc.
 *
 *  \param[out] file file to write canvases to
 */
void TrackJetDraw::SaveCanvasesToFile(TFile* file)
{
  // emit debugging message
  if (m_do_debug)
  {
    std::cout << "  -- Saving plots to file:\n"
              << "       file: " << file->GetName()
              << std::endl;
  }

  // save canvases
  for (auto& drawer : m_drawers)
  {
    drawer.second->SaveToFile(file);
  }
}
