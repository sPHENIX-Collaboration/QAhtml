// Jennifer James <jennifer.l.james@vanderbilt.edu>, McKenna Sleeth, Derek Anderson, Mariia Mitrankova

#ifndef TRACK_IN_JET_DRAWER_H
#define TRACK_IN_JET_DRAWER_H

#include "BaseJetDrawer.h"

// ============================================================================
//! Drawer for Tracks in Jets
// ============================================================================
/*! Class to draw histograms produced by the TrksInJetsQA module
 */
class TrackInJetDrawer : public BaseJetDrawer
{
  public:

    // ctor/dtor
    TrackInJetDrawer(const std::string& name = "TrackInJet",
                     const std::string& modu = "JetDraw",
                     const std::string& type = "track_antikt",
                     const std::string& pref = "h_trksinjet",
                     const bool debug = false);
    ~TrackInJetDrawer();

    // inherited public methods
    int Draw(const std::vector<uint32_t> vecTrigToDraw,
             const std::vector<uint32_t> vecResToDraw) override;
    int MakeHtml(const std::vector<uint32_t> vecTrigToDraw,
                 const std::vector<uint32_t> vecResToDraw,
                 const QADraw& subsystem) override;

  private:

    // inherited private methods
    void DoDrawing(const uint32_t trig, const uint32_t res) override;

};  // end TrackInJetDrawer

#endif
