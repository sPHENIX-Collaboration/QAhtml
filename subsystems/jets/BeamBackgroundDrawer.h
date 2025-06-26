// Jennifer James <jennifer.l.james@vanderbilt.edu>, McKenna Sleeth, Derek Anderson, Mariia Mitrankova

#ifndef BEAM_BACKGROUND_DRAWER_H
#define BEAM_BACKGROUND_DRAWER_H

#include "BaseJetDrawer.h"

// ============================================================================
//! Drawer for Beam-Background QA
// ============================================================================
/*! Class to draw histograms produced by the
 *  BeamBackgroundFilterAndQA module
 */
class BeamBackgroundDrawer : public BaseJetDrawer
{
  public:

    // ctor/dtor
    BeamBackgroundDrawer(const std::string& name = "BeamBackground",
                         const std::string& modu = "JetDraw",
                         const std::string& type = "",
                         const std::string& pref = "h_beambackgroundfilterandqa",
                         const bool debug = false);
    ~BeamBackgroundDrawer();

    // inherited public methods
    int Draw(const std::vector<uint32_t> vecTrigToDraw,
             const std::vector<uint32_t> vecResToDraw) override;
    int MakeHtml(const std::vector<uint32_t> vecTrigToDraw,
                 const std::vector<uint32_t> vecResToDraw,
                 const QADraw& subsystem) override;

  private:

    //inherited private methods
    void DoDrawing(const uint32_t trig, const uint32_t res) override;

};  // end BeamBackgroundDrawer

#endif
