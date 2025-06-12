// Jennifer James <jennifer.l.james@vanderbilt.edu>, McKenna Sleeth, Derek Anderson, Mariia Mitrankova

#ifndef TRACK_JET_DRAW_H
#define TRACK_JET_DRAW_H

#include "BaseJetDraw.h"
#include "BaseJetDrawer.h"
#include "JetKinematicDrawer.h"
#include "JetSeedDrawer.h"
#include <map>
#include <memory>
#include <string>

// ============================================================================
//! Draw track jet QA histograms
// ============================================================================
/*! A QAhtml subsystem to draw track jet QA histograms
 *  and generate relevant HTML pages.
 */
class TrackJetDraw : public BaseJetDraw
{
  public:

    // ------------------------------------------------------------------------
    //! Default ctor
    // ------------------------------------------------------------------------
    /*! Which components to implement are set here. Currently
     *  implemented ones:
     *    - `"KINEMATIC"` = draw jet kinematic plots,
     *    - `"SEED"` = draw jet seed plots,
     *    - `"ALL"` = draw all of the above.
     *
     *  Note that "ALL" doesn't have an associated drawer class, since
     *  it just indicates to run all of the above drawers.
     */
    TrackJetDraw(const std::string& name = "TrackJetQA",
                 const std::string& type = "track_antikt",
                 const bool debug = false)
      : BaseJetDraw(name, type, debug)
    {
      // initialize components
      m_drawers["KINEMATIC"] = std::make_unique<JetKinematicDrawer>("JetKinematic", name, type, "h_jetkinematiccheck", debug);
      m_drawers["SEED"] = std::make_unique<JetSeedDrawer>("JetSeed", name, type, "h_jetseedcount", debug);
    }

    // ------------------------------------------------------------------------
    //! Default dtor
    // ------------------------------------------------------------------------
    ~TrackJetDraw() {};

};  // end TrackJetDraw

#endif
