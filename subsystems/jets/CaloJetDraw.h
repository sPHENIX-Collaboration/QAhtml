// Jennifer James <jennifer.l.james@vanderbilt.edu>, McKenna Sleeth, Derek Anderson, Mariia Mitrankova

#ifndef CALO_JET_DRAW_H
#define CALO_JET_DRAW_H

#include "BaseJetDraw.h"
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

    CaloJetDraw(const std::string& name = "CaloJetQA",
                const std::string& type = "towersub1_antikt",
                const bool debug = false);
    ~CaloJetDraw();

};  // end CaloJetDraw

#endif
