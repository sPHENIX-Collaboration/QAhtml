// Jennifer James <jennifer.l.james@vanderbilt.edu>, McKenna Sleeth, Derek Anderson, Mariia Mitrankova

#ifndef EMCLUSTER_KINEMATIC_DRAWER_H
#define EMCLUSTER_KINEMATIC_DRAWER_H

#include "BaseJetDrawer.h"

// ============================================================================
//! Drawer for EMCluster Kinematics
// ============================================================================
/*! Class to draw histograms produced by the EMClusterJetsKinematics module
 */
class EMClusterKinematicDrawer : public BaseJetDrawer
{
  public:

    // ctor/dtor
    EMClusterKinematicDrawer(const std::string& name = "EMClusterKinematics",
                          const std::string& modu = "JetDraw",
                          const std::string& type = "emcal_cluster",
                          const std::string& pref = "h_emclusterkinematics",
                          const bool debug = false);
    ~EMClusterKinematicDrawer();

    // inherited public methods
    int Draw(const std::vector<uint32_t> vecTrigToDraw,
             const std::vector<uint32_t> vecResToDraw) override;
    int MakeHtml(const std::vector<uint32_t> vecTrigToDraw,
                 const std::vector<uint32_t> vecResToDraw,
                 const QADraw& subsystem) override;

  private:

    // inherited private methods
    void DoDrawing(const uint32_t trig, const uint32_t res) override;

};  // end EMClusterKinematicDrawer

#endif
