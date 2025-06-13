// Jennifer James <jennifer.l.james@vanderbilt.edu>, McKenna Sleeth, Derek Anderson, Mariia Mitrankova

#include "CaloJetDraw.h"

#include "BaseJetDrawer.h"
#include "DijetQADrawer.h"
#include "EventRhoDrawer.h"
#include "JetCstDrawer.h"
#include "JetKinematicDrawer.h"
#include "JetSeedDrawer.h"
#include "PhotonKinematicDrawer.h"
#include "StatusMapDrawer.h"
#include <map>
#include <memory>

// ctor/dtor ==================================================================

// ----------------------------------------------------------------------------
//! Default ctor
// ----------------------------------------------------------------------------
/*! Which components to implement are set here. Currently
 *  implemented ones:
 *    - "STATUS" = draw calo status map plots,
 *    - "PHOTON" = draw photon kinematic plots,
 *    - "RHO" = draw event-wise rho plots,
 *    - "KINEMATIC" = draw jet kinematic plots,
 *    - "CONSTITUENTS" = draw jet calorimeter constituent plots,
 *    - "SEED" = draw jet seed plots,
 *    - "DIJET" = draw dijet qa plots,
 *    - "ALL" = draw all of the above.
 *
 *  Note that "ALL" doesn't have an associated drawer class, since
 *  it just indicates to run all of the above drawers.
 */
CaloJetDraw::CaloJetDraw(const std::string& name,
                         const std::string& type,
                         const bool debug)
  : BaseJetDraw(name, type, debug)
{
  // for status map plots
  m_drawers["STATUS"] = std::make_unique<StatusMapDrawer>("CaloStatusMap",
                                                          name,
                                                          "towerinfo_calib",
                                                          "h_calostatusmapper",
                                                          debug);

  // for photon kinematic plots
  m_drawers["PHOTON"] = std::make_unique<PhotonKinematicDrawer>("PhotonKinematics",
                                                                name,
                                                                "emcal_cluster",
                                                                "h_photonjetskinematics",
                                                                debug);

  // for event-wise rho plots
  m_drawers["RHO"] = std::make_unique<EventRhoDrawer>("EventRho",
                                                      name,
                                                      type,
                                                      "h_eventwiserho",
                                                      debug);

  // for jet kinematic plots
  m_drawers["KINEMATIC"] = std::make_unique<JetKinematicDrawer>("JetKinematics",
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

  // for calo jet constituent plots
  m_drawers["CONSTITUENTS"] = std::make_unique<JetCstDrawer>("JetCst",
                                                             name,
                                                             type,
                                                             "h_constituentsinjets",
                                                             debug);
}

// ----------------------------------------------------------------------------
//! Default dtor
// ----------------------------------------------------------------------------
CaloJetDraw::~CaloJetDraw() {};
