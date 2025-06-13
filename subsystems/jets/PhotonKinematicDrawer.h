// Jennifer James <jennifer.l.james@vanderbilt.edu>, McKenna Sleeth, Derek Anderson, Mariia Mitrankova

#ifndef PHOTON_KINEMATIC_DRAWER_H
#define PHOTON_KINEMATIC_DRAWER_H

#include "BaseJetDrawer.h"

// ============================================================================
//! Drawer for Photon Kinematics
// ============================================================================
/*! Class to draw histograms produced by the PhotonJetsKinematics module
 */
class PhotonKinematicDrawer : public BaseJetDrawer
{
  public:

    // ctor/dtor
    PhotonKinematicDrawer(const std::string& name = "PhotonKinematics",
                          const std::string& modu = "JetDraw",
                          const std::string& type = "emcal_cluster",
                          const std::string& pref = "h_photonjetskinematics",
                          const bool debug = false);
    ~PhotonKinematicDrawer();

    // inherited public methods
    int Draw(const std::vector<uint32_t> vecTrigToDraw,
             const std::vector<uint32_t> vecResToDraw) override;
    int MakeHtml(const std::vector<uint32_t> vecTrigToDraw,
                 const std::vector<uint32_t> vecResToDraw,
                 const QADraw& subsystem) override;

  private:

    // inherited private methods
    void DoDrawing(const uint32_t trig, const uint32_t res) override;

};  // end PhotonKinematicDrawer

#endif
