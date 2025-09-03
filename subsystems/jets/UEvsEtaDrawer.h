// Jennifer James <jennifer.l.james@vanderbilt.edu>, McKenna Sleeth, Derek Anderson, Mariia Mitrankova, Jinglin Liu

#ifndef UE_VS_ETA_DRAWER_H
#define UE_VS_ETA_DRAWER_H

#include "BaseJetDrawer.h"

// ============================================================================
//! Drawer for Underlying Event vs Eta
// ============================================================================
/*! Class to draw histograms produced by the UEvsEtaCentrality module
 */
class UEvsEtaDrawer : public BaseJetDrawer
{
  public:

    // ctor/dtor
    UEvsEtaDrawer(const std::string& name = "UEvsEta",
                   const std::string& modu = "JetDraw",
                   const std::string& type = "towersub1_antikt",
                   const std::string& pref = "h_uevsetacent",
                   const bool debug = true);
    ~UEvsEtaDrawer();

    // public inherited methods
    int Draw(const std::vector<uint32_t> vecTrigToDraw,
             const std::vector<uint32_t> vecResToDraw) override;
    int MakeHtml(const std::vector<uint32_t> vecTrigToDraw,
                 const std::vector<uint32_t> vecResToDraw,
                 const QADraw& subsystem) override;

  private:

    // private inherited methods 
    void DoDrawing(const uint32_t trig, const uint32_t /*res*/) override;

};  // end UEvsEtaDrawer

#endif
