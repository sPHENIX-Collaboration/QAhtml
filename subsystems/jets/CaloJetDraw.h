// Jennifer James <jennifer.l.james@vanderbilt.edu>, McKenna Sleeth, Derek Anderson, Mariia Mitrankova

#ifndef CALO_JET_DRAW_H
#define CALO_JET_DRAW_H

#include "BaseJetDraw.h"
#include "BaseJetDrawer.h"
#include "DijetQADrawer.h"
#include "EventRhoDrawer.h"
#include "JetCstDrawer.h"
#include "JetKinematicDrawer.h"
#include "JetSeedDrawer.h"
#include <map>
#include <memory>
#include <string>

// ============================================================================
//! Draw calorimeter jet QA histograms
// ============================================================================
/*! A QAhtml subsystem to draw calorimeter jet QA
 *  histograms and generate relevant HTML pages.
 */
class CaloJetDraw : public BaseJetDraw
{
  public:

    // ------------------------------------------------------------------------
    //! Default ctor
    // ------------------------------------------------------------------------
    /*! Which components to implement are set here. Currently
     *  implemented ones:
     *    - "RHO" = draw event-wise rho plots,
     *    - "CONSTITUENTS" = draw jet calorimeter constituent plots,
     *    - "KINEMATIC" = draw jet kinematic plots,
     *    - "SEED" = draw jet seed plots,
     *    - "DIJET" = draw dijet qa plots,
     *    - "ALL" = draw all of the above.
     *
     *  Note that "ALL" doesn't have an associated drawer class, since
     *  it just indicates to run all of the above drawers.
     */
    CaloJetDraw(const std::string& name = "CaloJetQA",
                const std::string& type = "towersub1_antikt",
                const bool debug = false)
      : BaseJetDraw(name, type, debug)
    {
      // initialize components
      m_drawers["RHO"] = std::make_unique<EventRhoDrawer>("EventRho", name, type, "h_eventwiserho", debug);
      m_drawers["CONSTITUENTS"] = std::make_unique<JetCstDrawer>("JetCst", name, type, "h_constituentsinjets", debug);
      m_drawers["KINEMATIC"] = std::make_unique<JetKinematicDrawer>("JetKinematic", name, type, "h_jetkinematiccheck", debug);
      m_drawers["SEED"] = std::make_unique<JetSeedDrawer>("JetSeed", name, type, "h_jetseedcount", debug);
      m_drawers["DIJET"] = std::make_unique<DijetQADrawer>("DijetQA", name, type, "h_dijetqa", debug);
    }

    // ------------------------------------------------------------------------
    //! Default dtor
    // ------------------------------------------------------------------------
    ~CaloJetDraw() {};

};  // end CaloJetDraw

#endif
