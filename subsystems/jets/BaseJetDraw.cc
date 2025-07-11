// Jennifer James <jennifer.l.james@vanderbilt.edu>, McKenna Sleeth, Derek Anderson, Mariia Mitrankova

#include "BaseJetDraw.h"
#include "JetDrawDefs.h"

#include <qahtml/QADrawClient.h>
#include <TFile.h>
#include <iostream>

// ctor/dtor ==================================================================

// ----------------------------------------------------------------------------
//! Subsystem constructor
// ----------------------------------------------------------------------------
/*! Default constructor to initialize base
 *  jet QAhtml module. All arguments are
 *  technically optional.
 *
 *    \param name  name of the QAhtml module (e.g. "JetDraw")
 *    \param type  type of jet being drawn, part of the histogram names
 *    \param debug turn debugging mode on/off
 *    \param local turn local mode on/off
 */
BaseJetDraw::BaseJetDraw(const std::string& name,
                         const std::string& type,
                         const bool debug,
                         const bool local)
  : QADraw(name)
  , m_jet_type(type)
  , m_do_debug(debug)
  , m_do_local(local)
  , m_is_pp(true)
{
  Initialize();
}

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
//! Turn on/off local mode
// ----------------------------------------------------------------------------
/*! Setter to turn on/off local mode for the QA subsystem
 *  and all of its associated components.
 */
void BaseJetDraw::SetDoLocal(const bool local)
{
  m_do_local = local;
  for (auto& drawer : m_drawers)
  {
    drawer.second->SetDoLocal(m_do_local);
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

// inherited private methods ==================================================

// ----------------------------------------------------------------------------
//! Initialize a module
// ----------------------------------------------------------------------------
/*! This method determines if the run being drawn is p+p
 *  or Au+Au and loads the appropriate list of triggers
 *  and resolutions to draw.
 */
void BaseJetDraw::Initialize()
{
  // emit debugging message
  if (m_do_debug)
  {
    std::cout << "  -- Initializing module: " << m_name << std::endl;
  }

  // connect to draw client
  QADrawClient* cl = QADrawClient::instance();

  // grab run number & set pp/AuAu mode accordingly
  m_is_pp = JetDrawDefs::IsPP(cl->RunNumber());
  if (m_do_debug)
  {
    std::cout << "  -- Is Run " << cl->RunNumber() << " p+p? " << m_is_pp << std::endl;
  }

  // now pick out appropriate trigger list
  if (m_is_pp)
  {
    auto triggers = JetDrawDefs::VecTrigToDrawPP();
    m_vecTrigToDraw.clear();
    m_vecTrigToDraw.insert(m_vecTrigToDraw.end(),
                           triggers.begin(),
                           triggers.end());
  }
  else
  {
    auto triggers = JetDrawDefs::VecTrigToDrawAuAu();
    m_vecTrigToDraw.clear();
    m_vecTrigToDraw.insert(m_vecTrigToDraw.end(),
                           triggers.begin(),
                           triggers.end());
  }

  // finally, load resolutions to draw
  auto resos = JetDrawDefs::VecResToDraw();
  m_vecResToDraw.clear();
  m_vecResToDraw.insert(m_vecResToDraw.end(),
                        resos.begin(),
                        resos.end());
}
