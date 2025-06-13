// Jennifer James <jennifer.l.james@vanderbilt.edu>, McKenna Sleeth, Derek Anderson, Mariia Mitrankova

#ifndef TRACK_JET_DRAW_H
#define TRACK_JET_DRAW_H

#include "BaseJetDraw.h"
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

    // ctor/dtor
    TrackJetDraw(const std::string& name = "TrackJetQA",
                 const std::string& type = "track_antikt",
                 const bool debug = false);
    ~TrackJetDraw();

};  // end TrackJetDraw

#endif
