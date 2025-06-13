// Jennifer James <jennifer.l.james@vanderbilt.edu>, McKenna Sleeth, Derek Anderson, Mariia Mitrankova

#ifndef STATUS_MAP_DRAWER_H
#define STATUS_MAP_DRAWER_H

#include "BaseJetDrawer.h"

// ============================================================================
//! Drawer for Event-Wise Rho
// ============================================================================
/*! Class to draw histograms produced by the RhosinEvent module
 */
class StatusMapDrawer : public BaseJetDrawer
{
  public:

    // ctor/dtor
    StatusMapDrawer(const std::string& name = "CaloStatusMap",
                    const std::string& modu = "JetDraw",
                    const std::string& type = "towerinfo_calib",
                    const std::string& pref = "h_calostatusmapper",
                    const bool debug = false);
    ~StatusMapDrawer();

    // inherited public methods
    int Draw(const std::vector<uint32_t> vecTrigToDraw,
             const std::vector<uint32_t> vecResToDraw) override;
    int MakeHtml(const std::vector<uint32_t> vecTrigToDraw,
                 const std::vector<uint32_t> vecResToDraw,
                 const QADraw& subsystem) override;

  private:

    //inherited private methods
    void DoDrawing(const uint32_t trig, const uint32_t res) override;

};  // end StatusMapDrawer

#endif
