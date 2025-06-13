// Jennifer James <jennifer.l.james@vanderbilt.edu>, McKenna Sleeth, Derek Anderson, Mariia Mitrankova

#include "TrackJetDraw.h"

#include "BaseJetDrawer.h"
#include "DijetQADrawer.h"
#include "JetKinematicDrawer.h"
#include "JetSeedDrawer.h"
#include <map>
#include <memory>

// ctor/dtor ==================================================================

// ----------------------------------------------------------------------------
//! Default ctor
// ----------------------------------------------------------------------------
/*! Which components to implement are set here. Currently
 *  implemented ones:
 *    - "KINEMATIC" = draw jet kinematic plots,
 *    - "DIJET" = draw dijet qa plots,
 *    - "SEED" = draw jet seed plots,
 *    - "ALL" = draw all of the above.
 *
 *  Note that "ALL" doesn't have an associated drawer class, since
 *  it just indicates to run all of the above drawers.
 */
TrackJetDraw::TrackJetDraw(const std::string& name,
                           const std::string& type,
                           const bool debug)
  : BaseJetDraw(name, type, debug)
{
  // for jet kinematic plots
  m_drawers["KINEMATIC"] = std::make_unique<JetKinematicDrawer>("JetKinematic",
                                                                name,
                                                                type,
                                                                "h_jetkinematiccheck",
                                                                debug);

  // for dijet plots
  m_drawers["DIJET"] = std::make_unique<DijetQADrawer>("DijetQA",
                                                       name,
                                                       type,
                                                       "h_dijetqa",
                                                       debug);

  // for jet seed plots
  m_drawers["SEED"] = std::make_unique<JetSeedDrawer>("JetSeed",
                                                      name,
                                                      type,
                                                      "h_jetseedcount",
                                                      debug);
}

// ----------------------------------------------------------------------------
//! Default dtor
// ----------------------------------------------------------------------------
TrackJetDraw::~TrackJetDraw() {};
