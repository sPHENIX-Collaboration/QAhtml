// Jennifer James <jennifer.l.james@vanderbilt.edu>, McKenna Sleeth, Derek Anderson, Mariia Mitrankova

#include "TrackJetDraw.h"

#include "BaseJetDrawer.h"
#include "BeamBackgroundDrawer.h"
#include "DijetQADrawer.h"
#include "EventRhoDrawer.h"
#include "JetKinematicDrawer.h"
#include "JetSeedDrawer.h"
#include "JetVsTrackSumDrawer.h"
#include <map>
#include <memory>

// ctor/dtor ==================================================================

// ----------------------------------------------------------------------------
//! Default ctor
// ----------------------------------------------------------------------------
/*! Which components to implement are set here. Currently
 *  implemented ones:
 *    - "BEAM" = draw beam background plots,
 *    - "RHO" = draw event-wise rho plots,
 *    - "KINEMATIC" = draw jet kinematic plots,
 *    - "DIJET" = draw dijet qa plots,
 *    - "SEED" = draw jet seed plots,
 *    - "SUM" = draw track sum plots,
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
  // for beam background plots
  m_drawers["BEAM"] = std::make_unique<BeamBackgroundDrawer>("BeamBackground",
                                                             name,
                                                             "",
                                                             "h_beambackgroundfilterandqa",
                                                             debug);

  // for event-wise rho plots
  m_drawers["RHO"] = std::make_unique<EventRhoDrawer>("EventRho",
                                                      name,
                                                      type,
                                                      "h_eventwisetrackrho",
                                                      debug);

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

  // for track sum vs. jet plots
  m_drawers["SUM"] = std::make_unique<JetVsTrackSumDrawer>("JetVsTrackSum",
                                                           name,
                                                           type,
                                                           "h_structureinjets",
                                                           debug);
}

// ----------------------------------------------------------------------------
//! Default dtor
// ----------------------------------------------------------------------------
TrackJetDraw::~TrackJetDraw() {};
