// Jennifer James <jennifer.l.james@vanderbilt.edu>, McKenna Sleeth, Derek Anderson, Mariia Mitrankova

#ifndef BASE_JET_DRAW_H
#define BASE_JET_DRAW_H

#include "BaseJetDrawer.h"
#include <qahtml/QADrawClient.h>
#include <qahtml/QADraw.h>
#include <map>
#include <memory>
#include <string>

// ============================================================================
//! Base class for jet QA
// ============================================================================
/*! Base class to define common functionality
 *  across the QAhtml jet subsystems (e.g.
 *  CaloJetDraw and TrackJetDraw). Each type
 *  of jet to QA should have a corresponding
 *  "draw" (NOT a "drawer") class inheriting
 *  from this one.
 */
class BaseJetDraw : public QADraw
{
  public:

    // ctor/dtor
    BaseJetDraw(const std::string& name = "JetQA",
                const std::string& type = "towersub1_antikt",
                const bool debug = false,
                const bool local = false);
    ~BaseJetDraw();

    // getters 
    bool GetDoDebug() const {return m_do_debug;}
    bool GetDoLocal() const {return m_do_local;}
    std::string GetJetType() const {return m_jet_type;}
    std::string GetName() const {return m_name;}

    // inherited public methods
    int Draw(const std::string& what = "ALL") override;
    int MakeHtml(const std::string& what = "ALL") override;

    // other public methods
    void SetDoDebug(const bool debug);
    void SetDoLocal(const bool local);
    void SetJetType(const std::string& type);
    void SaveCanvasesToFile(TFile* file);

  protected:

    ///! name of subsystem
    std::string m_name;

    ///! type of jet input, used in histogram names
    std::string m_jet_type;

    ///! turn debugging statements on/off
    bool m_do_debug;

    ///! turn on/off local drawing mode
    bool m_do_local;

    ///! components to do actual histogram drawing
    std::map<std::string, std::unique_ptr<BaseJetDrawer>> m_drawers;

    ///! triggers we want to draw
    ///!   - FIXME dynamically allocate based on pp vs. pau
    std::vector<uint32_t> m_vecTrigToDraw = {
      JetQADefs::GL1::MBDNSJet1,
      JetQADefs::GL1::MBDNSJet2,
      JetQADefs::GL1::MBDNSJet3,
      JetQADefs::GL1::MBDNSJet4,
      999  // FIXME swap out for official tag when ready
    };

    ///! resolutions we want to draw
    ///!   - FIXME initialize in ctor
    std::vector<uint32_t> m_vecResToDraw = {
      JetDrawDefs::JetRes::R02,
      JetDrawDefs::JetRes::R03,
      JetDrawDefs::JetRes::R04,
      JetDrawDefs::JetRes::R05
    };


};  // end BaseJetDraw

#endif
