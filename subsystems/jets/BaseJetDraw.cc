// Jennifer James <jennifer.l.james@vanderbilt.edu>, McKenna Sleeth, Derek Anderson, Mariia Mitrankova

#include "BaseJetDraw.h"
#include <TFile.h>
#include <iostream>

// ctor/dtor ==================================================================

// ----------------------------------------------------------------------------
//! Subsystem constructor
// ----------------------------------------------------------------------------
BaseJetDraw::BaseJetDraw(const std::string& name,
                         const std::string& type,
                         const bool debug)
  : QADraw(name)
  , m_jet_type(type)
  , m_do_debug(debug)
{
  /* TODO picking out triggers goes here */
};

// ----------------------------------------------------------------------------
//! Subsystem destructor
// ----------------------------------------------------------------------------
BaseJetDraw::~BaseJetDraw() {};

// inherited public methods ===================================================

// ----------------------------------------------------------------------------
//! Draw plots
// ----------------------------------------------------------------------------
/*! Draws plots based on options. Specific options need to be
 *  implemented in derived classes such as `CaloJetDraw`.
 *
 *  \param what drawing option
 */
int BaseJetDraw::Draw(const std::string& what)
{
  // emit debugging message
  if (m_do_debug)
  {
    std::cout << "  -- Drawing component: " << what << std::endl;
  }

  int nDraw = 0;
  int index = 0;
  if (what == "ALL")
  {
    for (auto& drawer : m_drawers)
    {
      nDraw += drawer.second->Draw(m_vecTrigToDraw, m_vecResToDraw);
      ++index;
    }
  }
  else
  {
    nDraw += m_drawers.at(what)->Draw(m_vecTrigToDraw, m_vecResToDraw);
  }

  // return error (-1) if nothing drawn, otherwise return 0
  if (nDraw == 0)
  {
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
/*! Draws plots based on provided option -- see `BaseJetDraw::
 *  Draw(std::string&)` -- and generates html pages for each.
 *
 *  \param what drawing option
 */
int BaseJetDraw::MakeHtml(const std::string& what)
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
//! Turn on/off debugging
// ----------------------------------------------------------------------------
/*! Setter to turn on/off debugging for the QA subsystem
 *  and all of its associated components.
 */
void BaseJetDraw::SetDoDebug(const bool debug)
{
  m_do_debug = debug;
  for (auto& drawer : m_drawers)
  {
    drawer.second->SetDoDebug(m_do_debug);
  }
}

// ----------------------------------------------------------------------------
//! Set jet type
// ----------------------------------------------------------------------------
/*! Setter to update jet type for the QA subsystem
 *  and all of its associated components.
 */
void BaseJetDraw::SetJetType(const std::string& type)
{
  m_jet_type = type;
  for (auto& drawer : m_drawers)
  {
    drawer.second->SetJetType(type);
  }
}

// ----------------------------------------------------------------------------
//! Save canvases to file
// ----------------------------------------------------------------------------
/*! Helper method to save all canvases to a specified file.
 *  This is useful for debugging and quick testing when
 *  adjusting plotting details/etc.
 *
 *  \param[out] file file to write canvases to
 */
void BaseJetDraw::SaveCanvasesToFile(TFile* file)
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
