#ifndef EMCAL_SHOWER_SHAPES_DRAWER_H
#define EMCAL_SHOWER_SHAPES_DRAWER_H

#include "BaseJetDrawer.h"

// ============================================================================
//! Drawer for EMCal shower-shapes QA
// ============================================================================
/*! Class to draw histograms produced by the EMCalShowerShapes module. */
class EMCalShowerShapesDrawer : public BaseJetDrawer
{
  public:

    // ctor/dtor
    EMCalShowerShapesDrawer(const std::string& name = "EMCalShowerShapes",
                            const std::string& modu = "JetDraw",
                            const std::string& type = "",
                            const std::string& pref = "h_emcalshowershapes",
                            const bool debug = false);
    ~EMCalShowerShapesDrawer();

    // inherited public methods
    int Draw(const std::vector<uint32_t> vecTrigToDraw,
             const std::vector<uint32_t> vecResToDraw) override;
    int MakeHtml(const std::vector<uint32_t> vecTrigToDraw,
                 const std::vector<uint32_t> vecResToDraw,
                 const QADraw& subsystem) override;

  private:

    // inherited private methods
    void DoDrawing(const uint32_t trig, const uint32_t res) override;

};  // end EMCalShowerShapesDrawer

#endif

