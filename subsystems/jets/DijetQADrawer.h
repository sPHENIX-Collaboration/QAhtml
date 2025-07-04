// Jennifer James <jennifer.l.james@vanderbilt.edu>, McKenna Sleeth, Derek Anderson, Mariia Mitrankova

#ifndef DIJET_QA_DRAWER_H
#define DIJET_QA_DRAWER_H

#include "BaseJetDrawer.h"

// ============================================================================
//! Drawer for Dijet QA
// ============================================================================
/*! Class to draw histograms produced by the DijetQA module
 */
class DijetQADrawer : public BaseJetDrawer
{
  public:

    // ctor/dtor
    DijetQADrawer(const std::string& name = "DijetQA",
                  const std::string& modu = "JetDraw",
                  const std::string& type = "towersub1_antikt",
                  const std::string& pref = "h_dijetqa",
                  const bool debug = false);
    ~DijetQADrawer();

    // inherited public methods
    int Draw(const std::vector<uint32_t> vecTrigToDraw,
             const std::vector<uint32_t> vecResToDraw) override;
    int MakeHtml(const std::vector<uint32_t> vecTrigToDraw,
                 const std::vector<uint32_t> vecResToDraw,
                 const QADraw& subsystem) override;

  private:

    // inherited private methods
    void DoDrawing(const uint32_t trig, const uint32_t res) override;

};  // end DijetQADrawer

#endif
