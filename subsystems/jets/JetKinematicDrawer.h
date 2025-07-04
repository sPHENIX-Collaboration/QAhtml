// Jennifer James <jennifer.l.james@vanderbilt.edu>, McKenna Sleeth, Derek Anderson, Mariia Mitrankova

#ifndef JET_KINEMATIC_DRAWER_H
#define JET_KINEMATIC_DRAWER_H

#include "BaseJetDrawer.h"

// ============================================================================
//! Drawer for Jet Kinematics
// ============================================================================
/*! Class to draw histograms produced by the JetKinematicCheck module
 */
class JetKinematicDrawer : public BaseJetDrawer
{
  public:

    // ctor/dtor
    JetKinematicDrawer(const std::string& name = "JetKinematic",
                 const std::string& modu = "JetDraw",
                 const std::string& type = "towersub1_antikt",
                 const std::string& pref = "h_jetkinematiccheck",
                 const bool debug = false);
    ~JetKinematicDrawer();

    // inherited public methods
    int Draw(const std::vector<uint32_t> vecTrigToDraw,
             const std::vector<uint32_t> vecResToDraw) override;
    int MakeHtml(const std::vector<uint32_t> vecTrigToDraw,
                 const std::vector<uint32_t> vecResToDraw,
                 const QADraw& subsystem) override;

  private:

    // inherited private methods
    void DoDrawing(const uint32_t trig, const uint32_t res) override;

};  // end JetKinematicDrawer

#endif
