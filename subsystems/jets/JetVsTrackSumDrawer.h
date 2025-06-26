// Jennifer James <jennifer.l.james@vanderbilt.edu>, McKenna Sleeth, Derek Anderson, Mariia Mitrankova

#ifndef JET_VS_TRACK_SUM_DRAWER_H
#define JET_VS_TRACK_SUM_DRAWER_H

#include "BaseJetDrawer.h"

// ============================================================================
//! Drawer for Track Sum vs. Jets
// ============================================================================
/*! Class to draw histograms produced by the StructureinJets module
 */
class JetVsTrackSumDrawer : public BaseJetDrawer
{
  public:

    // ctor/dtor
    JetVsTrackSumDrawer(const std::string& name = "JetVsTrackSum",
                        const std::string& modu = "JetDraw",
                        const std::string& type = "track_antikt",
                        const std::string& pref = "h_structureinjets",
                        const bool debug = false);
    ~JetVsTrackSumDrawer();

    // inherited public methods
    int Draw(const std::vector<uint32_t> vecTrigToDraw,
             const std::vector<uint32_t> vecResToDraw) override;
    int MakeHtml(const std::vector<uint32_t> vecTrigToDraw,
                 const std::vector<uint32_t> vecResToDraw,
                 const QADraw& subsystem) override;

  private:

    // inherited private methods
    void DoDrawing(const uint32_t trig, const uint32_t res) override;

};  // end JetVsTrackSumDrawer

#endif
