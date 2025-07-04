// Jennifer James <jennifer.l.james@vanderbilt.edu>, McKenna Sleeth, Derek Anderson, Mariia Mitrankova

#ifndef JET_CST_DRAWER_H
#define JET_CST_DRAWER_H

#include "BaseJetDrawer.h"

// ============================================================================
//! Drawer for Calorimeter Jet Contituents
// ============================================================================
/*! Class to draw histograms produced by the ConstituentsinJets module
 */
class JetCstDrawer : public BaseJetDrawer
{
  public:

    // ctor/dtor
    JetCstDrawer(const std::string& name = "JetCst",
                 const std::string& modu = "JetDraw",
                 const std::string& type = "towersub1_antikt",
                 const std::string& pref = "h_constituentsinjets",
                 const bool debug = false);
    ~JetCstDrawer();

    // inherited public mehods
    int Draw(const std::vector<uint32_t> vecTrigToDraw,
             const std::vector<uint32_t> vecResToDraw) override;
    int MakeHtml(const std::vector<uint32_t> vecTrigToDraw,
                 const std::vector<uint32_t> vecResToDraw,
                 const QADraw& subsystem) override;

  private:

    // inherited private methods
    void DoDrawing(const uint32_t trig, const uint32_t res) override;

};  // end JetCstDrawer

#endif
